#include <Geometry/GEMGeometry/interface/GEMGeometry.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "FWCore/Framework/interface/ESHandle.h"

#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
#include <RecoLocalMuon/GEMRecHit/interface/CSCSegtoGEM.h>

ObjectMapCSC* ObjectMapCSC::mapInstance = NULL;

ObjectMapCSC* ObjectMapCSC::GetInstance(const edm::EventSetup& iSetup){
  if (mapInstance == NULL){
    mapInstance = new ObjectMapCSC(iSetup);
  }
  return mapInstance;
}

ObjectMapCSC::ObjectMapCSC(const edm::EventSetup& iSetup){
  edm::ESHandle<GEMGeometry> gemGeo;
  edm::ESHandle<CSCGeometry> cscGeo;
  
  iSetup.get<MuonGeometryRecord>().get(gemGeo);
  iSetup.get<MuonGeometryRecord>().get(cscGeo);
  for (TrackingGeometry::DetContainer::const_iterator it=gemGeo->dets().begin();it<gemGeo->dets().end();it++){
    if(dynamic_cast< GEMChamber* >( *it ) != 0 ){
      GEMChamber* ch = dynamic_cast< GEMChamber* >( *it );
      std::vector< const GEMEtaPartition*> roles = (ch->etaPartitions());
      for(std::vector<const GEMEtaPartition*>::const_iterator r = roles.begin();r != roles.end(); ++r){
          GEMDetId gemId = (*r)->id();
          int region=gemId.region();
          if(region!=0){
              int station=gemId.station();
              int ring=gemId.ring();
              int gemchamber=gemId.chamber();
              int layer=gemId.layer();
              int cscring=ring;
              int cscstation=station;
              int cscchamber = gemchamber;
              int csclayer = layer;
              CSCStationIndex ind(region,cscstation,cscring,cscchamber,csclayer);
              std::set<GEMDetId> myrolls;
              if (rollstoreCSC.find(ind)!=rollstoreCSC.end()) myrolls=rollstoreCSC[ind];
                myrolls.insert(gemId);
                rollstoreCSC[ind]=myrolls;
                }
            }
        }
    }
  }
  

