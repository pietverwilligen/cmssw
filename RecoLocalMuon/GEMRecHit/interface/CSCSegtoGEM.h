#ifndef  CSCSEGTOGEM_H
#define  CSCSEGTOGEM_H


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHit.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"


class CSCStationIndex{
public:
  CSCStationIndex():_region(0),_station(0),_ring(0),_chamber(0),_layer(0){}
  CSCStationIndex(int region, int station, int ring, int chamber, int layer):
    _region(region),
    _station(station),
    _ring(ring),
    _chamber(chamber),
    _layer(layer){}
  ~CSCStationIndex(){}
  int region() const {return _region;}
  int station() const {return _station;}
  int ring() const {return _ring;}
  int chamber() const {return _chamber;}
  int layer() const {return _layer;}
  bool operator<(const CSCStationIndex& cscind) const{
    if(cscind.region()!=this->region())
      return cscind.region()<this->region();
    else if(cscind.station()!=this->station())
      return cscind.station()<this->station();
    else if(cscind.ring()!=this->ring())
      return cscind.ring()<this->ring();
    else if(cscind.chamber()!=this->chamber())
      return cscind.chamber()<this->chamber();
    else if(cscind.layer()!=this->layer())
        return cscind.layer()<this->layer();
    return false;
  }

private:
  int _region;
  int _station;
  int _ring;  
  int _chamber;
  int _layer;
};

class ObjectMapCSC{
public:
  static ObjectMapCSC* GetInstance(const edm::EventSetup& iSetup);
  std::set<GEMDetId> GetRolls(CSCStationIndex cscstationindex){return mapInstance->rollstoreCSC[cscstationindex];}
//protected:
  std::map<CSCStationIndex,std::set<GEMDetId> > rollstoreCSC;
  ObjectMapCSC(const edm::EventSetup& iSetup);
private:
  static ObjectMapCSC* mapInstance;
}; 

#endif
