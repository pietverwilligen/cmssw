#include "SimMuon/GEMDigitizer/interface/ME0DigiGaussianModel.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "DataFormats/GEMDigi/interface/ME0Digi.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandPoissonQ.h"
#include "CLHEP/Random/RandExponential.h"
#include <cmath>
#include <math.h>
#include <utility>
#include <map>


const int bxwidth   = 25;         // [ns]

ME0DigiGaussianModel::ME0DigiGaussianModel(const edm::ParameterSet& config) :
    ME0DigiModel(config), 
    sigma_t(config.getParameter<double>("timeResolution")), 
    sigma_u(config.getParameter<double>("phiResolution")), 
    sigma_v(config.getParameter<double>("etaResolution")), 
    gaussianSmearing_(config.getParameter<bool>("gaussianSmearing")),
    constPhiSmearing_(config.getParameter<bool>("constantPhiSpatialResolution")),
    digitizeOnlyMuons_(config.getParameter<bool>("digitizeOnlyMuons")),
    averageEfficiency_(config.getParameter<double>("averageEfficiency")),
    // simulateIntrinsicNoise_(config.getParameter<bool>("simulateIntrinsicNoise")),
    // averageNoiseRate_(config.getParameter<double>("averageNoiseRate")), 
    simulateElectronBkg_(config.getParameter<bool>("simulateElectronBkg")), 
    simulateNeutralBkg_(config.getParameter<bool>("simulateNeutralBkg")), 
    tDeadTime_(config.getParameter<double>("tDeadTime")),
    meanCls_(config.getParameter<double>("meanCls")),
    minBunch_(config.getParameter<int>("minBunch")), 
    maxBunch_(config.getParameter<int>("maxBunch"))
{
  // polynomial parametrisation of neutral (n+g) and electron background
  neuBkg.push_back(899644.0);     neuBkg.push_back(-30841.0);     neuBkg.push_back(441.28); 
  neuBkg.push_back(-3.3405);      neuBkg.push_back(0.0140588);    neuBkg.push_back(-3.11473e-05); neuBkg.push_back(2.83736e-08);
  eleBkg.push_back(4.68590e+05);  eleBkg.push_back(-1.63834e+04); eleBkg.push_back(2.35700e+02);
  eleBkg.push_back(-1.77706e+00); eleBkg.push_back(7.39960e-03);  eleBkg.push_back(-1.61448e-05); eleBkg.push_back(1.44368e-08);
}

ME0DigiGaussianModel::~ME0DigiGaussianModel()
{
}

void ME0DigiGaussianModel::simulateSignal(const ME0EtaPartition* roll, const edm::PSimHitContainer& simHits, CLHEP::HepRandomEngine* engine)
{

  /* stripDigiSimLinks_.clear();
  detectorHitMap_.clear();
  stripDigiSimLinks_ = StripDigiSimLinks(roll->id().rawId());
  theME0DigiSimLinks_.clear();
  theME0DigiSimLinks_ = ME0DigiSimLinks(roll->id().rawId());
  */
  for (const auto & hit : simHits)
  {
    // Digitize only Muons?
    if (std::abs(hit.particleType()) != 13 && digitizeOnlyMuons_) continue;
    // Digitize only in [minBunch,maxBunch] window
    // window is: [(2n-1)*bxw/2, (2n+1)*bxw/2], n = [minBunch, maxBunch]
    if(hit.timeOfFlight() < (2*minBunch_-1)*bxwidth*1.0/2 || hit.timeOfFlight() > (2*maxBunch_+1)*bxwidth*1.0/2) continue;
    // is GEM efficient?
    if (CLHEP::RandFlat::shoot(engine) > averageEfficiency_) continue;

    // extract info
    LocalPoint hitpnt  = hit.localPosition();
    double tof         = CLHEP::RandGaussQ::shoot(engine, hit.timeOfFlight(), sigma_t);
    int pdgid          = hit.particleType();

    // create digis 
    const std::vector<ME0Digi> cluster(simulateClustering(roll,hitpnt,tof,pdgid,1, engine));

    // insert digis
    for(auto& digi:cluster)
      {
	edm::LogVerbatim("ME0DigiGaussianModel")<<"[simulateSignal] ME0Digi inserted: "<<digi<<std::endl;
	digi_.insert(digi);
	// edm::LogVerbatim("ME0DigiGaussianModel")<<"[simulateSignal] ME0DigiSimLink inserted ";
	// detectorHitMap_.insert(DetectorHitMap::value_type(digi,&*(hit)));
      }

  }
}



std::vector<ME0Digi> ME0DigiGaussianModel::simulateClustering(const ME0EtaPartition* roll, LocalPoint hitpnt, double tof, int pdgid, bool prompt, CLHEP::HepRandomEngine* engine)
{
  
  // Detector Resolution smearing
  // ----------------------------
  double x=0.0, y=0.0;  
  double sigma_u_new = sigma_u;
  if(constPhiSmearing_) sigma_u_new = correctSigmaU(roll, hitpnt.y());
  if(gaussianSmearing_) {  // Gaussian Smearing
    x=CLHEP::RandGaussQ::shoot(engine, hitpnt.x(), sigma_u_new);
    // y=CLHEP::RandGaussQ::shoot(engine, hitpnt.y(), sigma_v);
  }
  else {                   // Uniform Smearing 
    // ... use the sqrt(12)*sigmas as boundaries
    x=hitpnt.x()+(CLHEP::RandFlat::shoot(engine)-0.5)*sqrt(12)*sigma_u_new;
    // y=hitpnt.y()+(CLHEP::RandFlat::shoot(engine)-0.5)*sqrt(12)*sigma_v;
  }
  y = hitpnt.y(); // no additional smearing for now in y-direction. we have partitions
  LocalPoint smeared(x,y,0.);  

  edm::LogVerbatim("ME0DigiGaussianModel") << "---------------------------------------------------------" ;
  edm::LogVerbatim("ME0DigiGaussianModel") << "[simulateClustering] :: hitpoint (x,y) = ("<<hitpnt.x()<<","<<hitpnt.y()<<") smearedpoint (x,y) = ("<<x<<","<<y<<") --> dx = "<<hitpnt.x()-x<<" cm and dy = "<<hitpnt.y()-y;

  // const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology())));
  const Topology&                 topology=roll->specs()->topology();

  const int nstrips(roll->nstrips());
  int centralStrip = 0;
  if (!(topology.channel(smeared) + 1 > nstrips))
    centralStrip = topology.channel(smeared) + 1;
  else
    centralStrip = topology.channel(smeared);

  edm::LogVerbatim("ME0DigiGaussianModel") << "[simulateClustering] :: topology.channel(simhit) = "<<topology.channel(hitpnt)<<std::endl;
  edm::LogVerbatim("ME0DigiGaussianModel") << "[simulateClustering] :: topology.channel(smeared) = "<<topology.channel(smeared)+1*roll->localPitch(smeared)<<std::endl;
  edm::LogVerbatim("ME0DigiGaussianModel") << "[simulateClustering] :: central strip = "<<centralStrip;
  /*
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: nstrips       = "<<roll->nstrips();
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: centralstrip  = "<<roll->strip(smeared);
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: local pitch s = "<<roll->localPitch(smeared);
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: pitch strip   = "<<roll->pitch();
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: pads          = "<<roll->npads();
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: centralpad    = "<<roll->pad(smeared);
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: local pitch p = "<<roll->localPadPitch(smeared);
  edm::LogVerbatim("ME0DigiGaussianModel") << "[test] :: pitch pad     = "<<roll->padPitch();
  */

  // Make Digi
  // ---------
  // make digi for central strip
  ME0Digi centralDigi(centralStrip, tof, pdgid, prompt);

  // add central digi to cluster
  std::vector < ME0Digi > cluster_;
  cluster_.clear();
  cluster_.push_back(centralDigi);


  // calculate clustersize 
  // meanCls_ is the mean of an exponential distribution: lamda*exp(-lambda*x), meanCls_ = 1/lambda
  // setting meanCls_ to 0. (lambda -> inf) will result in CLS==1 (only the signal strip, no additional strips fired)
  if(meanCls_>1.e-5) 
    {
      // draw a number from the exponential distribution
      CLHEP::RandExponential expDist(engine);
      int clustersize = expDist.fire(meanCls_); 
      if(clustersize > 1) 
	{
	  // find out whether we need to add strip on the left or on the right of central strip
	  // LocalPoint pointSimHit  = simHit->localPosition();
	  LocalPoint digpnt = roll->centreOfStrip(centralStrip);
	  double deltaX     = hitpnt.x() - digpnt.x();

	  GlobalPoint pointSimHit = roll->toGlobal(smeared);
	  GlobalPoint pointDigiHit = roll->toGlobal(roll->centreOfStrip(centralStrip));
	  double DeltaX = pointSimHit.x() - pointDigiHit.x();
	  edm::LogVerbatim("ME0DigiGaussianModel") << " Any difference between LocalPoint Delta X and GlobalPoint Delta X? LocalP DX = "<<deltaX<<" GlobalP DX = "<<DeltaX;

	  int fstrip = centralStrip, lstrip = centralStrip;
	  // For CLS > 3 insert both left and right adjacent strips
	  for(int cl=0; cl<(clustersize-1)/2; ++cl) 
	    {
	      if (centralStrip - cl - 1 >= 1  )
		{
		  fstrip = centralStrip-cl-1;
		  ME0Digi clsDigi(fstrip, tof, pdgid, 1);
		  cluster_.push_back(clsDigi);
		}
	      if (centralStrip + cl + 1 <= roll->nstrips() )
		{
		  lstrip = centralStrip+cl+1;
		  ME0Digi clsDigi(lstrip, tof, pdgid, 1);
		  cluster_.push_back(clsDigi);
		}
	    }
	  // for even clustersize (2,4,...) 
	  // insert the last strip according to the 
	  // simhit position in the central strip 
	  if (clustersize%2 == 0 ){
	    if (deltaX < 0. && lstrip < roll->nstrips() ) 
	      {
		lstrip++;
		ME0Digi clsDigi(lstrip, tof, pdgid, 1);
		cluster_.push_back(clsDigi);
	      }
	    else if (deltaX > 0. && fstrip > 1 ) 
	      {
		fstrip--;
		ME0Digi clsDigi(fstrip, tof, pdgid, 1);
		cluster_.push_back(clsDigi);
	      }
	  } 
	}
    } // end condition meanCls_ > 1.e-5

  // ORIGINAL CODE FROM GEMSIMPLE MODEL ... to be checked
  /* 
  std::vector < std::pair<int, int> > cluster_;
  cluster_.clear();
  cluster_.push_back(std::pair<int, int>(centralStrip, bx));
  int clusterSize((CLHEP::RandFlat::shoot(engine)) <= 0.53 ? 1 : 2);
  if (clusterSize == 2)    {
      if (deltaX <= 0)	{
	  if (CLHEP::RandFlat::shoot(engine) < averageEfficiency_ && (centralStrip - 1 > 0))
	    cluster_.push_back(std::pair<int, int>(centralStrip - 1, bx));	}
      else	{
	  if (CLHEP::RandFlat::shoot(engine) < averageEfficiency_ && (centralStrip + 1 <= nstrips))
	    cluster_.push_back(std::pair<int, int>(centralStrip + 1, bx));	}
  }
  return cluster_;
  */

  // Some Debug Printout
  edm::LogVerbatim("ME0DigiGaussianModel") << "[simulateClustering] :: simhit  in "<<roll->id()<<" at loc x = "<<std::setw(8)<<hitpnt.x()<<" [cm]"
					   << " loc y = "<<std::setw(8)<<hitpnt.y()<<" [cm] time = "<<std::setw(8)<<tof<<" [ns] pdgid = "<<std::showpos<<std::setw(4)<<pdgid;

  edm::LogVerbatim("ME0DigiGaussianModel") << "[simulateClustering] :: cluster in "<<roll->id()<<" at loc x = "<<std::setw(8)<<roll->centreOfStrip(centralStrip).x()
					   <<" [cm] loc y = "<<std::setw(8)<<roll->centreOfStrip(centralStrip).y()
					   <<" time = "<<std::setw(8)<<tof<<" [ns]"<<" with clustersize "<<cluster_.size();
  for(unsigned int cl=0; cl<cluster_.size(); ++cl) { 
      edm::LogVerbatim("ME0DigiGaussianModel") << "[simulateClustering] :: digi   in "<<roll->id()<<" with strip x"<<std::setw(8)<<cluster_[cl].strip()<<" time = "<<std::setw(8)<<tof<<" [ns]";
  }

  // Return Cluster
  return cluster_;
}


void ME0DigiGaussianModel::implementDeadTime(){} // not implemented for now ... effect is 10^-4 ... maybe not even the right place to implement it here


void ME0DigiGaussianModel::simulateNoise(const ME0EtaPartition* roll, CLHEP::HepRandomEngine* engine)
{
  const ME0DetId me0Id(roll->id());
  const int nstrips(roll->nstrips());
  // double trArea(0.0);
  double trStripArea(0.0);

  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology())));
  const double striplength(top_->stripLength());
  const double rollRadius(top_->radius());

  trStripArea = (roll->pitch()) * striplength;
  // trArea = trStripArea * nstrips;

  // simulate intrinsic noise and background hits in all BX that are being read out
  // for(int bx=minBunch_; bx<maxBunch_+1; ++bx) {
  
  // 1) Intrinsic Noise ... Not implemented right now
  // ------------------------------------------------
  // if (simulateIntrinsicNoise_)
  // {
  // }
  
  // 2) Background Noise 
  // ----------------------------
  
  // 2a) electron background
  // -----------------------
  if (simulateElectronBkg_) {	
    // Extract / Calculate the Average Electron Rate 
    // for the given global Y coord from Parametrization
    double averageElectronRatePerRoll = 0.0;
    double yy_helper = 1.0;
    for(int j=0; j<7; ++j) { averageElectronRatePerRoll += eleBkg[j]*yy_helper; yy_helper *= rollRadius; }
    
    // Rate [Hz/cm^2] * Nbx * 25*10^-9 [s] * Area [cm] = # hits in this roll in this bx
    const double averageElecRate(averageElectronRatePerRoll * (maxBunch_-minBunch_+1)*(bxwidth*1.0e-9) * trStripArea); 
    
    edm::LogVerbatim("ME0DigiGaussianModelNoise") << "[elebkg]["<<roll->id().rawId()<<"]" /* "] :: BX = "<<std::showpos<<bx*/
						  << " evaluation of Background Hit Rate at this coord :: "<<std::setw(12)<<averageElectronRatePerRoll<<" [Hz/cm^2]"
						  << " x 9 x 25*10^-9 [s] x Area (of strip = "<<std::setw(12)<<trStripArea<<" [cm^2]) ==> "<<std::setw(12)<<averageElecRate<<" [hits]"; 
      
    CLHEP::RandPoissonQ randPoissonQ(*engine, averageElecRate);
    int n_elechits(randPoissonQ.fire());
    /* to be rewritten
       edm::LogVerbatim("ME0DigiGaussianModelNoise") << "[elebkg]["<<roll->id().rawId()<<"] :: myRandY = "<<std::setw(12)<<myRandY<<" => local y = "<<std::setw(12)<<yy_rand<<" [cm]"
       <<" => global y (global R) = "<<std::setw(12)<<yy_glob<<" [cm] || Probability = "<<std::setw(12)<<averageElecRate
       <<" => number of electron hits in this eta-partition "<<n_elechits<<std::endl;
    */ 
    // loop over amount of electron hits in this eta partition
    for (int i = 0; i < n_elechits; ++i) {
      
      int centralStrip(static_cast<int> (CLHEP::RandFlat::shoot(engine, 1, nstrips)));
      // extract random BX
      double myrandBX = CLHEP::RandFlat::shoot(engine, 0., 1.);
      int bx = int((maxBunch_-minBunch_+1)*myrandBX)+minBunch_;
      // extract random time in this BX
      double myrandT = CLHEP::RandFlat::shoot(engine, 0., 1.);
      double minBXtime = (bx-0.5)*bxwidth;      // double maxBXtime = (bx+0.5)*bxwidth;
      double time = myrandT*bxwidth+minBXtime;
      double myrandP = CLHEP::RandFlat::shoot(engine, 0., 1.);
      int pdgid = 0;
      if (myrandP <= 0.5) pdgid = -11; // electron
      else 	            pdgid = 11;  // positron
      edm::LogVerbatim("ME0DigiGaussianModelNoise") << "[elebkg]["<<roll->id().rawId()<<"] =====> electron hit in "<<roll->id()<<" pdgid = "<<pdgid<<" bx = "<<bx
						    <<" ==> digitized"
						    <<" at strip = "<<centralStrip<<" time = "<<time<<" [ns]"; 
      
      // conversion from centralstrip to Local Point (LP)...
      // actually this conversion is not necessary and the method "cluster"
      // should be rewritten to start from a central strip instead of LP
      LocalPoint hitpnt(roll->centreOfStrip(centralStrip));
      const std::vector<ME0Digi> cluster(simulateClustering(roll,hitpnt,time,pdgid,0,engine));
      for(auto& digi:cluster)
	{
	  edm::LogVerbatim("ME0DigiGaussianModelNoise")<<"[simulateNoise::e-bkg] ME02DDigi inserted: "<<digi<<std::endl;
	  digi_.insert(digi);
	}
    } // end loop n_elechits
  } // end if electron bkg
			  
			  
  // 2b) neutral (n+g) background
  // ----------------------------
  if (simulateNeutralBkg_) {
    // Extract / Calculate the Average Electron Rate 
    // for the given global Y coord from Parametrization
    double averageNeutralRatePerRoll = 0.0;
    double yy_helper = 1.0;
    for(int j=0; j<7; ++j) { averageNeutralRatePerRoll += neuBkg[j]*yy_helper; yy_helper *= rollRadius; }
    
    // Rate [Hz/cm^2] * Nbx * 25*10^-9 [s] * Area [cm] = # hits in this roll
    const double averageNeutrRate(averageNeutralRatePerRoll * (maxBunch_-minBunch_+1)*(bxwidth*1.0e-9) * trStripArea);
    
    edm::LogVerbatim("ME0DigiGaussianModelNoise") << "[neubkg]["<<roll->id().rawId()<<"]" /* "] :: BX = "<<std::showpos<<bx*/
						  << " evaluation of Background Hit Rate at this coord :: "<<std::setw(12)<<averageNeutralRatePerRoll<<" [Hz/cm^2]"
						  << " x 9 x 25*10^-9 [s] x Area (of strip = "<<std::setw(12)<<trStripArea<<" [cm^2]) ==> "<<std::setw(12)<<averageNeutrRate<<" [hits]"; 
    
    CLHEP::RandPoissonQ randPoissonQ(*engine, averageNeutrRate);
    int n_hits(randPoissonQ.fire());
    /* to be rewritten
       edm::LogVerbatim("ME0DigiGaussianModelNoise") << "[neubkg]["<<roll->id().rawId()<<"] :: myRandY = "<<std::setw(12)<<myRandY<<" => local y = "<<std::setw(12)<<yy_rand<<" [cm]"
       <<" => global y (global R) = "<<std::setw(12)<<yy_glob<<" [cm] || Probability = "<<std::setw(12)<<averageNeutrRate
       <<" => number of neutral hits in this eta-partition "<<n_hits<<std::endl;
    */
    // loop over amount of neutral hits in this roll
    for (int i = 0; i < n_hits; ++i) {
      int centralStrip(static_cast<int> (CLHEP::RandFlat::shoot(engine, 1, nstrips)));
      // extract random BX
      double myrandBX = CLHEP::RandFlat::shoot(engine, 0., 1.);
      int bx = int((maxBunch_-minBunch_+1)*myrandBX)+minBunch_;
      // extract random time in this BX
      double myrandT = CLHEP::RandFlat::shoot(engine, 0., 1.);
      double minBXtime = (bx-0.5)*bxwidth;
      double time = myrandT*bxwidth+minBXtime;
      int pdgid = 0;
      double myrandP = CLHEP::RandFlat::shoot(engine, 0., 1.);
      if (myrandP <= 0.08) pdgid = 2112; // neutrons: GEM sensitivity for neutrons: 0.08%
      else                 pdgid = 22;   // photons:  GEM sensitivity for photons:  1.04% ==> neutron fraction = (0.08 / 1.04) = 0.077 = 0.08
      
      edm::LogVerbatim("ME0DigiGaussianModelNoise") << "[neubkg]["<<roll->id().rawId()<<"] ======> neutral hit in "<<roll->id()<<" pdgid = "<<pdgid<<" bx = "<<bx
						    <<" ==> digitized"
						    <<" at strip = "<<centralStrip<<" time = "<<time<<" [ns]";
      // conversion from centralstrip to Local Point (LP)...
      // actually this conversion is not necessary and the method "cluster"
      // should be rewritten to start from a central strip instead of LP
      LocalPoint hitpnt(roll->centreOfStrip(centralStrip));
      const std::vector<ME0Digi> cluster(simulateClustering(roll,hitpnt,time,pdgid,0,engine));
      
      // insert digis
      for(auto& digi:cluster)
	{
	  edm::LogVerbatim("ME0DigiGaussianModelNoise")<<"[simulateNoise::n+g-bkg] ME0Digi inserted: "<<digi<<std::endl;
	  digi_.insert(digi);
	}
    } // end loop n_hits
  } // end if neutral bkg
			  
}

double ME0DigiGaussianModel::correctSigmaU(const ME0EtaPartition* roll, double y) {
  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology())));
  auto& parameters(roll->specs()->parameters());
  double height(parameters[2]);       // height     = height from Center of Roll
  double rollRadius = top_->radius(); // rollRadius = Radius at Center of Roll
  double Rmax = rollRadius+height;    // MaxRadius  = Radius at top of Roll
  double Rx = rollRadius+y;           // y in [-height,+height]
  double sigma_u_new = Rx/Rmax*sigma_u;
  return sigma_u_new;
}
