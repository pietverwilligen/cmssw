// -*- C++ -*-
//
// Package:    GEMRecHit
// Class:      ME0RecHit2Dto1DProducer
// 
/**\class ME0RecHit2Dto1DProducer ME0RecHit2Dto1DProducer.cc RecoLocalMuon/GEMRecHit/plugins/ME0RecHit2Dto1DProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Mon, 27 Jun 2016 08:57:24 GMT
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include <DataFormats/GEMRecHit/interface/ME0RecHitCollection.h>
#include <DataFormats/GEMRecHit/interface/ME0RecHit2DCollection.h>
#include <DataFormats/GEMRecHit/interface/ME0RecHit.h>
#include <DataFormats/GEMRecHit/interface/ME0RecHit2D.h>

//
// class declaration
//

class ME0RecHit2Dto1DProducer : public edm::EDProducer {
   public:
      explicit ME0RecHit2Dto1DProducer(const edm::ParameterSet&);
      ~ME0RecHit2Dto1DProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
  edm::InputTag inputObjectsTag; 
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
ME0RecHit2Dto1DProducer::ME0RecHit2Dto1DProducer(const edm::ParameterSet& iConfig)
{

  // std::cout<<"ME0RecHit2Dto1DProducer::constructor"<<std::endl;

  inputObjectsTag  = iConfig.getParameter<edm::InputTag>("me0RecHit2DLabel");

  //register your products
  produces<ME0RecHitCollection>();

  //now do what ever other initialization is needed
  
}


ME0RecHit2Dto1DProducer::~ME0RecHit2Dto1DProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
ME0RecHit2Dto1DProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // std::cout<<"ME0RecHit2Dto1DProducer::produce"<<std::endl;

  // get collection of ME0RecHits (2D)
  edm::Handle<ME0RecHit2DCollection> me0RecHits2D;
  iEvent.getByLabel(inputObjectsTag, me0RecHits2D);

  // create empty collection of ME0RecHits (1D)
  std::auto_ptr<ME0RecHitCollection> oc( new ME0RecHitCollection() );

  // collect all RH by ME0DetId
  std::map<uint32_t, std::vector<ME0RecHit> > collectRHbyID;
  for(ME0RecHit2DCollection::const_iterator it = me0RecHits2D.product()->begin(); it != me0RecHits2D.product()->end(); ++it) {
    ME0DetId id(it->me0Id());
    ME0RecHit rh(it->me0Id(), it->tof(), it->localPosition(), it->localPositionError());
    collectRHbyID[id.rawId()].push_back(rh);
  }

  // Loop over map and put in collection
  for(auto enIt=collectRHbyID.begin(); enIt != collectRHbyID.end(); ++enIt) {
    // std::cout<<"Map collectRHbyID :: detId = "<<ME0DetId(enIt->first)<<" = "<<enIt->first<<" has "<<enIt->second.size()<<" me0rechits"<<std::endl;
    oc->put(ME0DetId(enIt->first), enIt->second.begin(), enIt->second.end()); 
  }

  // put collection in event                                                                                                                                                                                                                                                 
  iEvent.put(oc);
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
ME0RecHit2Dto1DProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ME0RecHit2Dto1DProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
ME0RecHit2Dto1DProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
ME0RecHit2Dto1DProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
ME0RecHit2Dto1DProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
ME0RecHit2Dto1DProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ME0RecHit2Dto1DProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ME0RecHit2Dto1DProducer);
