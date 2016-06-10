#ifndef GEMDigitizer_ME0Digi2DModel_h
#define GEMDigitizer_ME0Digi2DModel_h

/** 
 *  \class ME0Digi2DModel
 *
 *  Base Class for the ME0 strip response simulation 
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/GEMDigi/interface/ME0Digi2DCollection.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "CLHEP/Random/RandomEngine.h"

#include <map>
#include <set>

class ME0EtaPartition;
class ME0Geometry;
class PSimHit;

class ME0Digi2DModel
{
public:
  
  virtual ~ME0Digi2DModel() {}

  void setGeometry(const ME0Geometry *geom) {geometry_ = geom;}

  const ME0Geometry* getGeometry() {return geometry_;}

  virtual void setupEtaPartition(const ME0EtaPartition*) = 0;

  virtual void simulateSignal(const ME0EtaPartition*, const edm::PSimHitContainer&) = 0;

  virtual void simulateNoise(const ME0EtaPartition*) = 0;
  
  // virtual std::vector<std::pair<int,int> > 
  // simulateClustering(const ME0EtaPartition*, const PSimHit*, const int) = 0;

  virtual void setRandomEngine(CLHEP::HepRandomEngine&) = 0;

  void fillDigis(int rollDetId, ME0Digi2DCollection&);

  virtual void printDigis() = 0;

  virtual void setup() = 0;

protected:
  std::set< ME0Digi2D> digi_;

  ME0Digi2DModel(const edm::ParameterSet&) {}

  const ME0Geometry * geometry_;
  
  //  DetectorHitMap detectorHitMap_;
};
#endif
