#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "SimMuon/GEMDigitizer/interface/ME0DigiPreRecoProducer.h"
#include "SimMuon/GEMDigitizer/interface/ME0DigiPreRecoModelFactory.h"
#include "SimMuon/GEMDigitizer/interface/ME0DigiPreRecoModel.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"

#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace CLHEP {
  class HepRandomEngine;
}

ME0DigiPreRecoProducer::ME0DigiPreRecoProducer(const edm::ParameterSet& ps)
  : digiPreRecoModelString_(ps.getParameter<std::string>("digiPreRecoModelString"))
{
  produces<ME0DigiPreRecoCollection>();

  edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable()){
    throw cms::Exception("Configuration")
      << "ME0DigiPreRecoProducer::ME0PreRecoDigiProducer() - RandomNumberGeneratorService is not present in configuration file.\n"
      << "Add the service in the configuration file or remove the modules that require it.";
  }
  me0DigiPreRecoModel_ = ME0DigiPreRecoModelFactory::get()->create("ME0" + digiPreRecoModelString_ + "Model", ps);
  LogDebug("ME0DigiPreRecoProducer") << "Using ME0" + digiPreRecoModelString_ + "Model";

  std::string mix_(ps.getParameter<std::string>("mixLabel"));
  std::string collection_(ps.getParameter<std::string>("inputCollection"));

  cf_token = consumes<CrossingFrame<PSimHit> >(edm::InputTag(mix_, collection_));
  // pu_token = consumes<std::vector<PileupSummaryInfo> >(edm::InputTag("PileupSummaryInfoInputTag"));
}


ME0DigiPreRecoProducer::~ME0DigiPreRecoProducer()
{
  delete me0DigiPreRecoModel_;
}


void ME0DigiPreRecoProducer::beginRun(const edm::Run&, const edm::EventSetup& eventSetup)
{
  // set geometry
  edm::ESHandle<ME0Geometry> hGeom;
  eventSetup.get<MuonGeometryRecord>().get(hGeom);
  me0DigiPreRecoModel_->setGeometry(&*hGeom);
  me0DigiPreRecoModel_->setup();
}


void ME0DigiPreRecoProducer::produce(edm::Event& e, const edm::EventSetup& eventSetup)
{
  std::cout<<"ME0DigiPreRecoProducer::produce"<<std::endl;
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine* engine = &rng->getEngine(e.streamID());

  edm::Handle<CrossingFrame<PSimHit> > cf;
  e.getByToken(cf_token, cf);

  std::unique_ptr<MixCollection<PSimHit> > hits( new MixCollection<PSimHit>(cf.product()) );

  // Grab the pileup information for this event
  // unfortunately this is only available after digi step ???
  /*
  edm::Handle<std::vector< PileupSummaryInfo > >  puInfo;
  e.getByToken(pu_token, puInfo);
  // e.getByLabel(edm::InputTag("addPileupInfo"), puInfo);
  if (puInfo.isValid()) {
    std::vector<PileupSummaryInfo>::const_iterator pvi;   
    for(pvi = puInfo->begin(); pvi != puInfo->end(); ++pvi) {
      int bx = pvi->getBunchCrossing();
      if(bx == 0) { 
	// meanInt_   = pvi->getTrueNumInteractions();
	actualInt_ = pvi->getPU_NumInteractions();
	continue;
      }
    }      
  }
  else {
    // meanInt_   = -1.;
    actualInt_ = -1;
  } 
  if(actualInt_ != -1) { instLumi_= getLumi(actualInt_,25,14);}
  else instLumi_ = 0.0;
  std::cout<<"ME0Digitizer :: puInfo :: number of PU interactions = "<<actualInt_<<" :: calculated instantaneous luminosity = "<<instLumi_<<std::endl;
  */
  // get pileup info
  // Si Pixel Method

  std::cout<<"ME0DigiPreRecoProducer::produce :: about to ask for the PileUpInfo"<<std::endl;
  PileupInfo_ = getEventPileupInfo();
  std::cout<<"ME0DigiPreRecoProducer::produce :: got PileupInfo, about to calculate Inst Lumi"<<std::endl;
  calculateInstlumiFactor(PileupInfo_);
  // calculate instantaneous luminosity for this event  
  // instLumi_ = calculateInstlumiFactor(puInfo);




  // Create empty output
  std::unique_ptr<ME0DigiPreRecoCollection> digis(new ME0DigiPreRecoCollection());

  // arrange the hits by eta partition
  std::map<uint32_t, edm::PSimHitContainer> hitMap;
  for(auto &hit: *hits){
    hitMap[hit.detUnitId()].push_back(hit);
  }

  // simulate signal and noise for each eta partition:
  // get the eta partitions
  const auto & etaPartitions(me0DigiPreRecoModel_->getGeometry()->etaPartitions());
  // loop over the eta partitions and simulate signal and noise
  for(auto &roll: etaPartitions){
    const ME0DetId detId(roll->id());
    const uint32_t rawId(detId.rawId());
    const auto & simHits(hitMap[rawId]);
    
    LogDebug("ME0DigiPreRecoProducer") 
      << "ME0DigiPreRecoProducer: found " << simHits.size() << " hit(s) in eta partition" << rawId;
    
    me0DigiPreRecoModel_->simulateSignal(roll, simHits, engine);
    me0DigiPreRecoModel_->simulateNoise(roll, instLumi_, engine);
    me0DigiPreRecoModel_->fillDigis(rawId, *digis);
  }
  
  // store them in the event
  e.put(std::move(digis));
}

void ME0DigiPreRecoProducer::calculateInstlumiFactor(PileupMixingContent* puInfo){
  // Instlumi scalefactor calculating for neutron induced background
  if (puInfo) {
    std::cout<<"PU Info :: is not a null pointer ... getting the BX and True Interaction List"<<std::endl;
    const std::vector<int> bunchCrossing = puInfo->getMix_bunchCrossing();
    std::cout<<"PU Info :: is not a null pointer ... OK 1"<<std::endl;
    const std::vector<float> TrueInteractionList = puInfo->getMix_TrueInteractions();
    std::cout<<"PU Info :: is not a null pointer ... OK 2"<<std::endl;
    //const int bunchSpacing = puInfo->getMix_bunchSpacing();
    int pui = 0, p = 0;
    std::vector<int>::const_iterator pu;
    std::vector<int>::const_iterator pu0 = bunchCrossing.end();

    for (pu=bunchCrossing.begin(); pu!=bunchCrossing.end(); ++pu) {
      std::cout<<"PU Info :: In Loop and bx = "<<*pu<<std::endl;
      if (*pu==0) {
        pu0 = pu;
        p = pui;
	std::cout<<"PU Info :: In Loop and *pu==0 ==> pu0 points now to pu (iterators) pu0 = "<<*pu0<<" and p = pui = "<<p<<std::endl;
      }
      std::cout<<"PU Info :: In Loop :: iterator pu points to "<<*pu<<" counter pui = "<<pui<<" number of interactions = "<<TrueInteractionList.at(pui)<<std::endl;
      pui++;
    }
    if (pu0!=bunchCrossing.end()) {
      double instlumi1 = TrueInteractionList.at(p)*221.95;
      double instlumi2 = TrueInteractionList.at(p)*364;
      double instlumi3 = TrueInteractionList.at(p)*246.4;
      std::cout<<"PU Info :: Out Loop :: Number Of Interactions for pu0 = "<<TrueInteractionList.at(p)<<" => Inst Lumi (scale factor 221.95) = "<<instlumi1<<" (scale factor 364) "<<instlumi2<<" (scale factor 246.4) "<<instlumi3<<std::endl;
      actualInt_ = TrueInteractionList.at(p);
      std::cout<<"PU Info :: Out Loop :: Number Of Interactions for pu0 = "<<TrueInteractionList.at(p)<<" => Inst Lumi with method getLumi("<<actualInt_<<",25,14) = "<<getLumi(actualInt_,25,14)<<std::endl;
      instLumi_= getLumi(actualInt_,25,14); 
    }
  }
  else {
    std::cout<<"No PU Info ... what to do now?"<<std::endl;
    actualInt_ = -1;
    instLumi_  = 0.0;
  }
}



double ME0DigiPreRecoProducer::getLumi(int pu, int space, int com)
{
  double rev   = 11245.0;
  double scale = 1.0e34;
  std::map<int,double> cross;
  std::map<int,double> bunches;

  cross[7]=7.31e-26;  // TOTEM Measurement
  cross[8]=7.47e-26;  // TOTEM Measurement
  cross[13]=8.0e-26;
  cross[14]=8.0e-26;  // wild guess .. cant find number

  bunches[25]=2808.0; // number of coliding bunches
  bunches[50]=1380.0;
 
  double instlumi = pu*bunches[space]*rev/cross[com]/scale; 
  return instlumi;
}
