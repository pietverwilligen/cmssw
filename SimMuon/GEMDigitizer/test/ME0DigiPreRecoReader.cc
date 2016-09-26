#ifndef SimMuon_ME0DigiPreRecoReader_h
#define SimMuon_ME0DigiPreRecoReader_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/GEMDigi/interface/ME0DigiPreRecoCollection.h"
// #include "DataFormats/GEMDigi/interface/ME0Digi2DCollection.h"
// #include "DataFormats/GEMDigi/interface/ME0DigiCollection.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include <map>
#include <set>

#include "DataFormats/Common/interface/DetSet.h"

#include <iostream>

using namespace std;


class ME0DigiPreRecoReader: public edm::EDAnalyzer
{
public:

  explicit ME0DigiPreRecoReader(const edm::ParameterSet& pset);
  
  virtual ~ME0DigiPreRecoReader(){}
  
  void analyze(const edm::Event &, const edm::EventSetup&); 
  
private:

  string label;
};



ME0DigiPreRecoReader::ME0DigiPreRecoReader(const edm::ParameterSet& pset)
{
  label = pset.getUntrackedParameter<string>("label", "simMuonME0Digis");
  // label = pset.getUntrackedParameter<string>("label", "simMuonME0Digis2D");
  // label = pset.getUntrackedParameter<string>("label", "simMuonME0Digis");
}


void ME0DigiPreRecoReader::analyze(const edm::Event & event, const edm::EventSetup& eventSetup)
{
  cout << "--- Run: " << event.id().run() << " Event: " << event.id().event() << endl;

  edm::Handle<ME0DigiPreRecoCollection> digis;
  event.getByLabel(label, digis);

  edm::Handle<edm::PSimHitContainer> simHits;
  event.getByLabel("g4SimHits","MuonME0Hits",simHits);    

  edm::ESHandle<ME0Geometry> pDD;
  eventSetup.get<MuonGeometryRecord>().get( pDD );

   
  // edm::Handle< edm::DetSetVector<StripDigiSimLink> > thelinkDigis;
  // event.getByLabel(label, "ME0", thelinkDigis);

  ME0DigiPreRecoCollection::DigiRangeIterator detUnitIt;
  for (detUnitIt = digis->begin();	detUnitIt != digis->end(); ++detUnitIt)
  {
    const ME0DetId& id = (*detUnitIt).first;
    const ME0EtaPartition* roll = pDD->etaPartition(id);

    //     if(id.rawId() != 637567293) continue;

    // ME0DetId print-out
    cout<<"--------------"<<endl;
    cout<<"id: "<<id.rawId()<<" number of strips "<<roll->nstrips()<<endl;

    // Loop over the digis of this DetUnit
    const ME0DigiPreRecoCollection::Range& range = (*detUnitIt).second;
    for (ME0DigiPreRecoCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt)
    {
      cout<<" digi "<<*digiIt<<endl;
      for(const auto& simHit: *simHits)
      {
        ME0DetId rpcId(simHit.detUnitId());
        if (rpcId == id && abs(simHit.particleType()) == 13)
        {
          cout<<"entry: "<< simHit.entryPoint()<<endl
              <<"exit: "<< simHit.exitPoint()<<endl
              <<"TOF: "<< simHit.timeOfFlight()<<endl;
        }
      }
    }// for digis in layer
  }// for layers

  /* // Future implementation
  for (edm::DetSetVector<StripDigiSimLink>::const_iterator itlink = thelinkDigis->begin(); itlink != thelinkDigis->end(); itlink++)
  {
    for(edm::DetSet<StripDigiSimLink>::const_iterator link_iter=itlink->data.begin();link_iter != itlink->data.end();++link_iter)
    {
      int detid = itlink->detId();
      int ev = link_iter->eventId().event();
      float frac =  link_iter->fraction();
      int strip = link_iter->channel();
      int trkid = link_iter->SimTrackId();
      int bx = link_iter->eventId().bunchCrossing();
      cout<<"DetUnit: "<<ME0DetId(detid)<<"  Event ID: "<<ev<<"  trkId: "<<trkid<<"  Strip: "<<strip<<"  Bx: "<<bx<<"  frac: "<<frac<<endl;
    }
  }
  */

  cout<<"--------------"<<endl;
}



#endif
#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(ME0DigiPreRecoReader);

