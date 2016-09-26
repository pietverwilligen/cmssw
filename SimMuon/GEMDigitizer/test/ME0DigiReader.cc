#ifndef SimMuon_ME0DigiReader_h
#define SimMuon_ME0DigiReader_h

/** \class ME0DigiReader
 *
 *  Dumps ME0 2D digis 
 *  
 *  \authors: Vadim Khotilovich
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/GEMDigi/interface/ME0DigiCollection.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include <map>
#include <set>

#include "DataFormats/Common/interface/DetSet.h"

#include <iostream>

using namespace std;


class ME0DigiReader: public edm::EDAnalyzer
{
public:

  explicit ME0DigiReader(const edm::ParameterSet& pset);
  
  virtual ~ME0DigiReader(){}
  
  void analyze(const edm::Event &, const edm::EventSetup&); 
  
private:

  edm::EDGetTokenT<edm::PSimHitContainer> simhitToken_;
  edm::EDGetTokenT<ME0DigiCollection> me0DigiToken_;
};



ME0DigiReader::ME0DigiReader(const edm::ParameterSet& pset):
  simhitToken_(consumes<edm::PSimHitContainer>(pset.getParameter<edm::InputTag>("simhitToken"))),
  me0DigiToken_(consumes<ME0DigiCollection>(pset.getParameter<edm::InputTag>("me0DigiToken")))
{
}


void ME0DigiReader::analyze(const edm::Event & event, const edm::EventSetup& eventSetup)
{
  cout << "--- Run: " << event.id().run() << " Event: " << event.id().event() << endl;

  edm::ESHandle<ME0Geometry> pDD;
  eventSetup.get<MuonGeometryRecord>().get( pDD );

  edm::Handle<edm::PSimHitContainer> simHits;
  event.getByToken(simhitToken_, simHits);

  edm::Handle<ME0DigiCollection> digis;
  event.getByToken(me0DigiToken_, digis);

  ME0DigiCollection::DigiRangeIterator detUnitIt;
  for (detUnitIt = digis->begin();	detUnitIt != digis->end(); ++detUnitIt)
  {
    const ME0DetId& id = (*detUnitIt).first;
    const ME0EtaPartition* roll = pDD->etaPartition(id);

    //     if(id.rawId() != 637567293) continue;

    // ME0DetId print-out
    cout<<"--------------"<<endl;
    cout<<"id: "<<id.rawId()<<" number of strips "<<roll->nstrips()<<endl;

    // Loop over the digis of this DetUnit
    const ME0DigiCollection::Range& range = (*detUnitIt).second;
    for (ME0DigiCollection::const_iterator digiIt = range.first; digiIt!=range.second; ++digiIt)
    {
      cout<<" digi "<<*digiIt<<endl;
      if (digiIt->strip() < 1 || digiIt->strip() > roll->nstrips() )
      {
        cout <<" XXXXXXXXXXXXX Problem with "<<id<<"  a digi has strip# = "<<digiIt->strip()<<endl;
      } 
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
DEFINE_FWK_MODULE(ME0DigiReader);

