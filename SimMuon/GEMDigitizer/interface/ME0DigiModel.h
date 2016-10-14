#ifndef SimMuon_GEMDigitizer_ME0DigiModel_h
#define SimMuon_GEMDigitizer_ME0DigiModel_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/GEMDigi/interface/ME0DigiCollection.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/StripDigiSimLink.h"
#include "SimDataFormats/GEMDigiSimLink/interface/ME0DigiSimLink.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"

#include <map>
#include <set>

namespace CLHEP {
  class HepRandomEngine;
}

class ME0EtaPartition;
class ME0Geometry;
class PSimHit;

class ME0DigiModel
{
public:

  typedef edm::DetSet<StripDigiSimLink> StripDigiSimLinks;
  typedef edm::DetSet<ME0DigiSimLink> ME0DigiSimLinks;

  virtual ~ME0DigiModel() {}

  void setGeometry(const ME0Geometry *geom) {geometry_ = geom;}

  const ME0Geometry* getGeometry() {return geometry_;}

  virtual void simulateSignal(const ME0EtaPartition*, const edm::PSimHitContainer&, CLHEP::HepRandomEngine* engine) = 0;

  virtual void simulateNoise(const ME0EtaPartition*, CLHEP::HepRandomEngine* engine) = 0;
  
  virtual std::vector<ME0Digi>
    simulateClustering(const ME0EtaPartition*, LocalPoint, double, int, bool, CLHEP::HepRandomEngine* engine) = 0;

  void fillDigis(int rollDetId, ME0DigiCollection&);

  virtual void setup() = 0;

  // const StripDigiSimLinks & stripDigiSimLinks() const {return stripDigiSimLinks_;}
  // const ME0DigiSimLinks & me0DigiSimLinks() const {return theMe0DigiSimLinks_;}

protected:

  ME0DigiModel(const edm::ParameterSet&) {}

  const ME0Geometry * geometry_;
  
  // std::set< std::pair<int, int> > strips_;
  std::set< ME0Digi> digi_;


  /// creates links from Digi to SimTrack
  /*
  // to be checked and implemented correctly
  void addLinks(unsigned int strip,int bx);
  void addLinksWithPartId(unsigned int strip,int bx);



  // keeps track of which hits contribute to which channels
  typedef std::multimap<
      std::pair<unsigned int, int>,
      const PSimHit*,
      std::less<std::pair<unsigned int, int> >
    >  DetectorHitMap;

  DetectorHitMap detectorHitMap_;
  StripDigiSimLinks stripDigiSimLinks_;
  ME0DigiSimLinks theMe0DigiSimLinks_;
  */

};
#endif
