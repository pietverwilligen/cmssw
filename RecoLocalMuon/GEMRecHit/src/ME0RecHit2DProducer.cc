/** \file
 *
 *  $Date: 2013/04/24 17:16:35 $
 *  $Revision: 1.1 $
 *  \author M. Maggi -- INFN Bari
*/

#include "ME0RecHit2DProducer.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/GEMDigi/interface/ME0Digi2DCollection.h"

#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHit.h"

#include "RecoLocalMuon/GEMRecHit/interface/ME0RecHit2DBaseAlgo.h"
#include "RecoLocalMuon/GEMRecHit/interface/ME0RecHit2DAlgoFactory.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHit2DCollection.h"

#include <string>


using namespace edm;
using namespace std;


ME0RecHit2DProducer::ME0RecHit2DProducer(const ParameterSet& config){

  produces<ME0RecHit2DCollection>();
  theME0DigiLabel = config.getParameter<InputTag>("me0DigiLabel");
  
  // Get the concrete reconstruction algo from the factory

  std::string theAlgoName = config.getParameter<string>("recAlgo");
  theTimeRes = config.getParameter<double>("timeresolution");
  theAlgo = ME0RecHit2DAlgoFactory::get()->create(theAlgoName,
						config.getParameter<ParameterSet>("recAlgoConfig"));
}


ME0RecHit2DProducer::~ME0RecHit2DProducer(){
  delete theAlgo;
}



void ME0RecHit2DProducer::beginRun(const edm::Run& r, const edm::EventSetup& setup){
}



void ME0RecHit2DProducer::produce(Event& event, const EventSetup& setup) {

  // Get the ME0 Geometry
  ESHandle<ME0Geometry> me0Geom;
  setup.get<MuonGeometryRecord>().get(me0Geom);

  // Get the digis from the event

  Handle<ME0Digi2DCollection> digis; 
  event.getByLabel(theME0DigiLabel,digis);

  // Pass the EventSetup to the algo

  theAlgo->setES(setup);
  // Create the pointer to the collection which will store the rechits

  auto_ptr<ME0RecHit2DCollection> recHitCollection(new ME0RecHit2DCollection());

  // Iterate through all digi collections ordered by LayerId   

  ME0Digi2DCollection::DigiRangeIterator me0dgIt;
  for (me0dgIt = digis->begin(); me0dgIt != digis->end();
       ++me0dgIt){
       
    // The layerId
    const ME0DetId& me0Id = (*me0dgIt).first;

    // Get the GeomDet from the setup
    const ME0EtaPartition* roll = me0Geom->etaPartition(me0Id);

    // Get the iterators over the digis associated with this LayerId
    const ME0Digi2DCollection::Range& range = (*me0dgIt).second;

    // Call the reconstruction algorithm    
    theAlgo->setupEtaPartition(*roll);
    OwnVector<ME0RecHit2D> recHits =
      theAlgo->reconstruct(*roll, me0Id, range, theTimeRes);
    
    if(recHits.size() > 0) //FIXME: is it really needed?
      recHitCollection->put(me0Id, recHits.begin(), recHits.end());
  }

  event.put(recHitCollection);

}

