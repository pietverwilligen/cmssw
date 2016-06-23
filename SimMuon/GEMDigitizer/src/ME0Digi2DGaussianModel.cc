#include "SimMuon/GEMDigitizer/interface/ME0Digi2DGaussianModel.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "DataFormats/GEMDigi/interface/ME0Digi2D.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandPoissonQ.h"
#include "CLHEP/Random/RandExponential.h"
#include <cmath>
#include <utility>
#include <map>


const int bxwidth   = 25;         // [ns]

ME0Digi2DGaussianModel::ME0Digi2DGaussianModel(const edm::ParameterSet& config) :
    ME0Digi2DModel(config), 
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
    nEtaPart_(config.getParameter<int>("nEtaPart")),
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

ME0Digi2DGaussianModel::~ME0Digi2DGaussianModel()
{
  if (flat1_)   delete flat1_;
  if (flat2_)   delete flat2_;
  if (gauss_)   delete gauss_;
  if (poisson_) delete poisson_;
  if (exp_)     delete exp_;
}

void ME0Digi2DGaussianModel::setRandomEngine(CLHEP::HepRandomEngine& eng)
{
  flat1_   = new CLHEP::RandFlat(eng);
  flat2_   = new CLHEP::RandFlat(eng);
  gauss_   = new CLHEP::RandGaussQ(eng);
  poisson_ = new CLHEP::RandPoissonQ(eng);
  exp_     = new CLHEP::RandExponential(eng);
}

void ME0Digi2DGaussianModel::setupEtaPartition(const ME0EtaPartition* roll)
{
  // --- DISCLAIMER ------------------------------------------------------------
  // This method should be abandonned at the point that a real eta partition
  // description becomes available in the ME0 Geometry description in XML
  // for now we will artificially (and not very precise) introduce rolls in C++
  // ---------------------------------------------------------------------------

  // Extract detailed information from the Strip Topology
  // base_bottom, base_top, height, strips, pads 
  // note that (0,0) is in the middle of the roll ==> all param are at all half length

  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology())));

  auto& parameters(roll->specs()->parameters());
  double bottomLength(parameters[0]); bottomLength = 2*bottomLength; // bottom is largest length, so furtest away from beamline 
  double topLength(parameters[1]);    topLength    = 2*topLength;    // top is shortest length, so closest to beamline
  double height(parameters[2]);       height       = 2*height;
  double rollRadius = top_->radius();
 
  double me0top = rollRadius + height/2;      //             /|  
  double me0bot = rollRadius - height/2;      //            / |
                                              //           /  |
                                              //        C /   |
  // conversion from spherical coords to eta,phi         /   B| 
  // eta = - ln ( tan (theta/2) )                       /     |  
  // tan theta/2 = sin theta / (1 + cos theta)         /__A___|
  // sin theta = A/C
  // cos theta = B/C
  // ==> eta = - ln ( A / (sqrt(A^2 + B^2) + B) 

  // For B take middle of layer 3 anad 4 :: 540 cm
  // ==> eta becomes function of height A only

  for(int i=0; i<=nEtaPart_; ++i) {
    double A = me0bot+i*1.0/nEtaPart_*(me0top-me0bot);
    double B = 540.;
    etaPartsE_.push_back(-log(A/(sqrt(A*A+B*B)+B))); // save here eta value
    etaPartsY_.push_back(A);                         // save here the height
  }

  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateSignal] :: vector with y-coordinates bottom/top Eta Partitions: ";
  for(int i=0; i<=nEtaPart_; ++i){
    edm::LogVerbatim("ME0Digi2DGaussianModel") << etaPartsY_[i]; 
  }
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateSignal] :: vector with eta-coordinates bottom/top Eta Partitions: ";
  for(int i=0; i<=nEtaPart_; ++i){
    edm::LogVerbatim("ME0Digi2DGaussianModel") << etaPartsE_[i]; 
  }

}



void ME0Digi2DGaussianModel::simulateSignal(const ME0EtaPartition* roll, const edm::PSimHitContainer& simHits)
{
  for (const auto & hit : simHits)
  {
    // Digitize only Muons?
    if (std::abs(hit.particleType()) != 13 && digitizeOnlyMuons_) continue;
    // Digitize only in [minBunch,maxBunch] window
    // window is: [(2n-1)*bxw/2, (2n+1)*bxw/2], n = [minBunch, maxBunch]
    if(hit.timeOfFlight() < (2*minBunch_-1)*bxwidth*1.0/2 || hit.timeOfFlight() > (2*maxBunch_+1)*bxwidth*1.0/2) continue;
    // is GEM efficient?
    if (flat1_->fire(1) > averageEfficiency_) continue;

    // extract info
    LocalPoint hitpnt  = hit.localPosition();
    double tof         = gauss_->fire(hit.timeOfFlight(), sigma_t);
    int pdgid          = hit.particleType();

    // create digis 
    const std::vector<ME0Digi2D> cluster(simulateClustering(roll,hitpnt,tof,pdgid,1));

    // insert digis
    for(auto& digi:cluster)
      {
	edm::LogVerbatim("ME0Digi2DGaussianModel")<<"[ME0Digi2DGaussianModel::simulateSignal] ME02DDigi inserted: "<<digi<<std::endl;
	digi_.insert(digi);
      }
  }
}



std::vector<ME0Digi2D> ME0Digi2DGaussianModel::simulateClustering(const ME0EtaPartition* roll, LocalPoint hitpnt, double tof, int pdgid, int prompt)
{
  
  // Detector Resolution smearing
  // ----------------------------
  double x=0.0, y=0.0;  
  double sigma_u_new = sigma_u;
  if(constPhiSmearing_) sigma_u_new = correctSigmaU(roll, hitpnt.y());
  if(gaussianSmearing_) {  // Gaussian Smearing
    x=gauss_->fire(hitpnt.x(), sigma_u_new);
    y=gauss_->fire(hitpnt.y(), sigma_v);
  }
  else {                   // Uniform Smearing 
    // ... use the sqrt(12)*sigmas as boundaries
    x=hitpnt.x()+(flat1_->fire(0., 1.)-0.5)*sqrt(12)*sigma_u_new;
    y=hitpnt.y()+(flat1_->fire(0., 1.)-0.5)*sqrt(12)*sigma_v;
  }
  LocalPoint smeared(x,y,0.);  

  edm::LogVerbatim("ME0Digi2DGaussianModel") << "---------------------------------------------------------" ;
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateClustering] :: hitpoint (x,y) = ("<<hitpnt.x()<<","<<hitpnt.y()<<") smearedpoint (x,y) = ("<<x<<","<<y<<")";


  // calculate strip x and strip y position
  // --------------------------------------
  // 1) x-position
  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology())));
  const Topology&                 topology=roll->specs()->topology();

  double rollRadius = top_->radius();
  int centralStrip = 0, etapart = 0;
  centralStrip = topology.channel(smeared)+1;
  
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateClustering] :: topology.channel(pnt) = "<<topology.channel(smeared)<<std::endl;
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateClustering] :: central strip = "<<centralStrip;
  /*
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: nstrips       = "<<roll->nstrips();
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: centralstrip  = "<<roll->strip(smeared);
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: local pitch s = "<<roll->localPitch(smeared);
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: pitch strip   = "<<roll->pitch();
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: pads          = "<<roll->npads();
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: centralpad    = "<<roll->pad(smeared);
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: local pitch p = "<<roll->localPadPitch(smeared);
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: test] :: pitch pad     = "<<roll->padPitch();
  */

  // 2) y-position
  // since the upper and lower values of ME0 cannot be obtained with infinite precision
  // therefore treat first and last etapartition separately
  if(y+rollRadius < etaPartsY_[1])                {etapart=1;}          // identify 1st eta-partition       
  else if(y+rollRadius > etaPartsY_[nEtaPart_-1]) {etapart=nEtaPart_;}  // identify last eta-partition
  else {                                                                // identify other eta-partitions
    for(int i=2; i<nEtaPart_; ++i) {
      if(y+rollRadius > etaPartsY_[i-1] && y+rollRadius < etaPartsY_[i]) {etapart = i;}
    }  
  }
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "i = "<<etapart<<" lower border (i-1) = "<<etaPartsY_[etapart-1]<<" < "<<y+rollRadius<<" < higher border (i) = "<<etaPartsY_[etapart]; 
  

  // Make Digi
  // ---------
  // make digi for central strip
  ME0Digi2D centralDigi(centralStrip, etapart, tof, pdgid, prompt);

  // add central digi to cluster
  std::vector < ME0Digi2D > cluster_;
  cluster_.clear();
  cluster_.push_back(centralDigi);


  // calculate clustersize 
  // meanCls_ is the mean of an exponential distribution: lamda*exp(-lambda*x), meanCls_ = 1/lambda
  // setting meanCls_ to 0. (lambda -> inf) will result in CLS==1 (only the signal strip, no additional strips fired)
  if(meanCls_>1.e-5) 
    {
      // draw a number from the exponential distribution
      int clustersize = exp_->fire(meanCls_); 
      if(clustersize > 1) 
	{
	  // find out whether we need to add strip on the left or on the right of central strip
	  // LocalPoint pointSimHit  = simHit->localPosition();
	  LocalPoint digpnt = roll->centreOfStrip(centralStrip);
	  double deltaX     = hitpnt.x() - digpnt.x();

	  int fstrip = centralStrip, lstrip = centralStrip;
	  // For CLS > 3 insert both left and right adjacent strips
	  for(int cl=0; cl<(clustersize-1)/2; ++cl) 
	    {
	      if (centralStrip - cl - 1 >= 1  )
		{
		  fstrip = centralStrip-cl-1;
		  ME0Digi2D clsDigi(fstrip, etapart, tof, pdgid, 1);
		  cluster_.push_back(clsDigi);
		}
	      if (centralStrip + cl + 1 <= roll->nstrips() )
		{
		  lstrip = centralStrip+cl+1;
		  ME0Digi2D clsDigi(lstrip, etapart, tof, pdgid, 1);
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
		ME0Digi2D clsDigi(lstrip, etapart, tof, pdgid, 1);
		cluster_.push_back(clsDigi);
	      }
	    else if (deltaX > 0. && fstrip > 1 ) 
	      {
		fstrip--;
		ME0Digi2D clsDigi(fstrip, etapart, tof, pdgid, 1);
		cluster_.push_back(clsDigi);
	      }
	  } 
	}
    } // end condition meanCls_ > 1.e-5


  // Some Debug Printout
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateClustering] :: simhit  in "<<roll->id()<<" at loc x = "<<std::setw(8)<<hitpnt.x()<<" [cm]"
					     << " loc y = "<<std::setw(8)<<hitpnt.y()<<" [cm] time = "<<std::setw(8)<<tof<<" [ns] pdgid = "<<std::showpos<<std::setw(4)<<pdgid;
  double middleCurrentEtaPart = (etaPartsY_[etapart-1]+etaPartsY_[etapart])/2;
  double middleLayer          = (nEtaPart_%2==0)? etaPartsY_[nEtaPart_/2] : (etaPartsY_[nEtaPart_/2] + etaPartsY_[nEtaPart_/2+1])/2;
  edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateClustering] :: cluster in "<<roll->id()<<" at loc x = "<<std::setw(8)<<roll->centreOfStrip(centralStrip).x()
					     <<" [cm] loc y = "<<std::setw(8)<<middleCurrentEtaPart-middleLayer<<" [cm]"
					     <<" time = "<<std::setw(8)<<tof<<" [ns]"<<" with clustersize "<<cluster_.size();
  for(unsigned int cl=0; cl<cluster_.size(); ++cl) { 
      edm::LogVerbatim("ME0Digi2DGaussianModel") << "[ME0Digi2DDigi :: simulateClustering] :: digi   in "<<roll->id()<<" with strip x"<<std::setw(8)<<cluster_[cl].stripx()
						 <<" EtaPart = "<<cluster_[cl].stripy()<<" time = "<<std::setw(8)<<tof<<" [ns]";
  }
  
  // Return Cluster
  return cluster_;
}


void ME0Digi2DGaussianModel::implementDeadTime(){} // not implemented for now ... effect is 10^-4 ... maybe not even the right place to implement it here


void ME0Digi2DGaussianModel::simulateNoise(const ME0EtaPartition* roll)
{
  double trArea(0.0);

  // Extract detailed information from the Strip Topology:
  // base_bottom, base_top, height, strips, pads 
  // note that (0,0) is in the middle of the roll ==> all param are at all half length
  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology())));


  auto& parameters(roll->specs()->parameters());
  double bottomLength(parameters[0]); bottomLength = 2*bottomLength; // bottom is largest length, so furtest away from beamline
  double topLength(parameters[1]);    topLength    = 2*topLength;    // top is shortest length, so closest to beamline
  double height(parameters[2]);       height       = 2*height;
  double myTanPhi    = (topLength - bottomLength) / (height * 2);
  double rollRadius = top_->radius();
  trArea = height * (topLength + bottomLength) / 2.0;

  // Divide the detector area in different strips
  // take smearing in y-coord as height for each strip
  // double heightIt = sigma_v;
  // int heightbins  = height/heightIt; // round down
  // double heightIt = etaPartsY_;
  int heightbins = nEtaPart_;

  edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: sNoise]["<<roll->id().rawId()<<"] :: roll with id = "<<roll->id();
  edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: sNoise]["<<roll->id().rawId()<<"] :: extracting parameters from the TrapezoidalStripTopology";
  edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: sNoise]["<<roll->id().rawId()<<"] :: bottom = "<<bottomLength<<" [cm] top  = "<<topLength<<" [cm] height = "<<height<<" [cm]"
						   <<" area  = "<<trArea<<" [cm^2] Rmid = "<<rollRadius<<" [cm] => Rmin = "<<rollRadius-height*1.0/2.0<<" [cm] Rmax = "<<rollRadius+height*1.0/2.0<<" [cm]";
  edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: sNoise]["<<roll->id().rawId()<<"] :: heightbins = "<<heightbins;


  for(int hx=0; hx<heightbins; ++hx) {
    double heightIt = etaPartsY_[hx+1]-etaPartsY_[hx]; // height of the pseudo eta partition
    double bottomIt = bottomLength +  hx  *2*tan(10./180*3.14)*heightIt;
    double topIt    = bottomLength + (hx+1)*2*tan(10./180*3.14)*heightIt;
    /* 
    if(hx==heightbins-1) {
      topIt = topLength;              // last bin ... make etapartition a bit larger to cover entire roll ... inheritted code but not necessary i think
      heightIt = height-hx*heightIt;  // i will remove this code after we know it is working fine 
    }
    */
    double areaIt   = heightIt*(bottomIt+topIt)*1.0/2;

    edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: sNoise]["<<roll->id().rawId()<<"] :: height = "<<std::setw(12)<<heightIt<<" [cm] bottom = "<<std::setw(12)<<bottomIt<<" [cm]"
						     << " top = "<<std::setw(12)<<topIt<<" [cm] area = "<<std::setw(12)<<areaIt<<" [cm^2] || sin(10) = "<<sin(10./180*3.14);

    double myRandY = flat1_->fire(0., 1.);
    double y0_rand = (hx+myRandY)*heightIt;  // Y coord, measured from the bottom of the roll
    if(hx==heightbins-1) y0_rand = hx*sigma_v + myRandY*heightIt;  // bugfix cesare
    double yy_rand = (y0_rand-height*1.0/2); // Y coord, measured from the middle of the roll, which is the Y coord in Local Coords
    double yy_glob = rollRadius + yy_rand;   // R coord in Global Coords
    // max length in x for given y coordinate (cfr trapezoidal eta partition)
    double xMax = topLength/2.0 - (height/2.0 - yy_rand) * myTanPhi;

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
      for(int j=0; j<7; ++j) { averageElectronRatePerRoll += eleBkg[j]*yy_helper; yy_helper *= yy_glob; }
      
      // Rate [Hz/cm^2] * Nbx * 25*10^-9 [s] * Area [cm] = # hits in this roll in this bx
      const double averageElecRate(averageElectronRatePerRoll * (maxBunch_-minBunch_+1)*(bxwidth*1.0e-9) * areaIt); 
      
      edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: elebkg]["<<roll->id().rawId()<<"]" /* "] :: BX = "<<std::showpos<<bx*/
						       << " evaluation of Background Hit Rate at this coord :: "<<std::setw(12)<<averageElectronRatePerRoll<<" [Hz/cm^2]"
						       << " x 9 x 25*10^-9 [s] x Area (of strip = "<<std::setw(12)<<areaIt<<" [cm^2]) ==> "<<std::setw(12)<<averageElecRate<<" [hits]"; 
      
      int n_elechits(poisson_->fire(averageElecRate));
      
      edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: elebkg]["<<roll->id().rawId()<<"] :: myRandY = "<<std::setw(12)<<myRandY<<" => local y = "<<std::setw(12)<<yy_rand<<" [cm]"
						       <<" => global y (global R) = "<<std::setw(12)<<yy_glob<<" [cm] || Probability = "<<std::setw(12)<<averageElecRate
						       <<" => number of electron hits in this eta-partition "<<n_elechits<<std::endl;
      
      // loop over amount of electron hits in this pseudo-eta partition
      for (int i = 0; i < n_elechits; ++i) {
	//calculate xx_rand at a given yy_rand
	double myRandX = flat1_->fire(0., 1.);
	double xx_rand = 2 * xMax * (myRandX - 0.5);
	// extract random BX
	double myrandBX = flat1_->fire(0., 1.);
	int bx = int((maxBunch_-minBunch_+1)*myrandBX)+minBunch_;
	// extract random time in this BX
	double myrandT = flat1_->fire(0., 1.);
	double minBXtime = (bx-0.5)*bxwidth;      // double maxBXtime = (bx+0.5)*bxwidth;
	double time = myrandT*bxwidth+minBXtime;
	double myrandP = flat1_->fire(0., 1.);
	int pdgid = 0;
	if (myrandP <= 0.5) pdgid = -11; // electron
	else 	            pdgid = 11;  // positron
	edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: elebkg]["<<roll->id().rawId()<<"] =====> electron hit in "<<roll->id()<<" pdgid = "<<pdgid<<" bx = "<<bx
							 <<" ==> digitized"
							 <<" at loc x = "<<xx_rand<<" loc y = "<<yy_rand<<" time = "<<time<<" [ns]"; 

	// conversion from xx_rand and yy_rand to stripx and etaPart ... implement also clustersize ...
	LocalPoint hitpnt(xx_rand,yy_rand,0.);
	const std::vector<ME0Digi2D> cluster(simulateClustering(roll,hitpnt,time,pdgid,0));
	for(auto& digi:cluster)
	  {
	    edm::LogVerbatim("ME0Digi2DGaussianModelNoise")<<"[ME0Digi2DGaussianModel::simulateNoise::e-bkg] ME02DDigi inserted: "<<digi<<std::endl;
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
      for(int j=0; j<7; ++j) { averageNeutralRatePerRoll += neuBkg[j]*yy_helper; yy_helper *= yy_glob; }
      
      // Rate [Hz/cm^2] * Nbx * 25*10^-9 [s] * Area [cm] = # hits in this roll
      const double averageNeutrRate(averageNeutralRatePerRoll * (maxBunch_-minBunch_+1)*(bxwidth*1.0e-9) * areaIt);

      edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: neubkg]["<<roll->id().rawId()<<"]" /* "] :: BX = "<<std::showpos<<bx*/
						       << " evaluation of Background Hit Rate at this coord :: "<<std::setw(12)<<averageNeutralRatePerRoll<<" [Hz/cm^2]"
						       << " x 9 x 25*10^-9 [s] x Area (of strip = "<<std::setw(12)<<areaIt<<" [cm^2]) ==> "<<std::setw(12)<<averageNeutrRate<<" [hits]"; 

      int n_hits(poisson_->fire(averageNeutrRate));
      
      edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: neubkg]["<<roll->id().rawId()<<"] :: myRandY = "<<std::setw(12)<<myRandY<<" => local y = "<<std::setw(12)<<yy_rand<<" [cm]"
						       <<" => global y (global R) = "<<std::setw(12)<<yy_glob<<" [cm] || Probability = "<<std::setw(12)<<averageNeutrRate
						      <<" => number of neutral hits in this eta-partition "<<n_hits<<std::endl;
      
      // loop over amount of neutral hits in this roll
      for (int i = 0; i < n_hits; ++i) {
	//calculate xx_rand at a given yy_rand
	double myRandX = flat1_->fire(0., 1.);
	double xx_rand = 2 * xMax * (myRandX - 0.5);
	// extract random BX
        double myrandBX= flat1_->fire(0., 1.);
	int bx = int((maxBunch_-minBunch_+1)*myrandBX)+minBunch_;
	// extract random time in this BX
	double myrandT = flat1_->fire(0., 1.);
	double minBXtime = (bx-0.5)*bxwidth;
	double time = myrandT*bxwidth+minBXtime;
	int pdgid = 0;
	double myrandP = flat1_->fire(0., 1.);
	if (myrandP <= 0.08) pdgid = 2112; // neutrons: GEM sensitivity for neutrons: 0.08%
	else                 pdgid = 22;   // photons:  GEM sensitivity for photons:  1.04% ==> neutron fraction = (0.08 / 1.04) = 0.077 = 0.08

	edm::LogVerbatim("ME0Digi2DGaussianModelNoise") << "[ME0Digi2DDigi :: neubkg]["<<roll->id().rawId()<<"] ======> neutral hit in "<<roll->id()<<" pdgid = "<<pdgid<<" bx = "<<bx
							 <<" ==> digitized"
							 <<" at loc x = "<<xx_rand<<" loc y = "<<yy_rand<<" time = "<<time<<" [ns]"; 
	LocalPoint hitpnt(xx_rand,yy_rand,0.);
	const std::vector<ME0Digi2D> cluster(simulateClustering(roll,hitpnt,time,pdgid,0));

	// insert digis
	for(auto& digi:cluster)
	  {
	    edm::LogVerbatim("ME0Digi2DGaussianModelNoise")<<"[ME0Digi2DGaussianModel::simulateNoise::n+g-bkg] ME02DDigi inserted: "<<digi<<std::endl;
	    digi_.insert(digi);
	  }
      } // end loop n_hits
    } // end if neutral bkg

  } // end loop over strips (= pseudo rolls)
}

double ME0Digi2DGaussianModel::correctSigmaU(const ME0EtaPartition* roll, double y) {
  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology())));
  auto& parameters(roll->specs()->parameters());
  double height(parameters[2]);       // height     = height from Center of Roll
  double rollRadius = top_->radius(); // rollRadius = Radius at Center of Roll
  double Rmax = rollRadius+height;    // MaxRadius  = Radius at top of Roll
  double Rx = rollRadius+y;           // y in [-height,+height]
  double sigma_u_new = Rx/Rmax*sigma_u;
  return sigma_u_new;
}
