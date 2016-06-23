#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"

#include "SimMuon/GEMDigitizer/interface/ME0Digi2DProducer.h"
#include "SimMuon/GEMDigitizer/interface/ME0Digi2DModel.h"
#include "SimMuon/GEMDigitizer/interface/ME0Digi2DModelFactory.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"

#include "CLHEP/Random/RandomEngine.h"

#include <sstream>
#include <string>
#include <map>
#include <vector>


ME0Digi2DProducer::ME0Digi2DProducer(const edm::ParameterSet& ps)
  : collectionXF_(ps.getParameter<std::string>("inputCollection"))
  , digi2DModelString_(ps.getParameter<std::string>("digi2DModelString"))
{
  produces<ME0Digi2DCollection>();

  edm::Service<edm::RandomNumberGenerator> rng;
  if (!rng.isAvailable()){
    throw cms::Exception("Configuration")
      << "ME0Digi2DProducer::ME0Digi2DProducer() - RandomNumberGeneratorService is not present in configuration file.\n"
      << "Add the service in the configuration file or remove the modules that require it.";
  }
  CLHEP::HepRandomEngine& engine = rng->getEngine();
  me0Digi2DModel_ = ME0Digi2DModelFactory::get()->create("ME0" + digi2DModelString_ + "Model", ps);
  LogDebug("ME0Digi2DProducer") << "Using ME0" + digi2DModelString_ + "Model";
  me0Digi2DModel_->setRandomEngine(engine);
}


ME0Digi2DProducer::~ME0Digi2DProducer()
{
  delete me0Digi2DModel_;
}


void ME0Digi2DProducer::produce(edm::Event& e, const edm::EventSetup& eventSetup)
{
  // set geometry
  edm::ESHandle<ME0Geometry> hGeom;
  eventSetup.get<MuonGeometryRecord>().get(hGeom);
  me0Digi2DModel_->setGeometry(&*hGeom);
  me0Digi2DModel_->setup();

  edm::Handle<CrossingFrame<PSimHit> > cf;
  e.getByLabel("mix", collectionXF_, cf);

  std::auto_ptr<MixCollection<PSimHit> > hits( new MixCollection<PSimHit>(cf.product()) );

  // Create empty output
  std::auto_ptr<ME0Digi2DCollection> digis(new ME0Digi2DCollection());

  // arrange the hits by eta partition
  std::map<uint32_t, edm::PSimHitContainer> hitMap;
  for(auto &hit: *hits){
    hitMap[hit.detUnitId()].push_back(hit);
   }
  
  // simulate signal and noise for each eta partition
  const auto & etaPartitions(me0Digi2DModel_->getGeometry()->etaPartitions());
  
  for(auto &roll: etaPartitions){
    const ME0DetId detId(roll->id());
    const uint32_t rawId(detId.rawId());
    const auto & simHits(hitMap[rawId]);
    
    LogDebug("ME0Digi2DProducer") 
      << "ME0Digi2DProducer: found " << simHits.size() << " hit(s) in eta partition" << rawId;

    me0Digi2DModel_->setupEtaPartition(roll);    
    me0Digi2DModel_->simulateSignal(roll, simHits);
    me0Digi2DModel_->simulateNoise(roll);
    me0Digi2DModel_->fillDigis(rawId, *digis);
  }

  // print them
  // me0Digi2DModel_->printDigis(*digis);  
  // store them in the event
  e.put(digis);
}

