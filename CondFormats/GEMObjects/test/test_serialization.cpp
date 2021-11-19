#include "CondFormats/Serialization/interface/Test.h"
#include "CondFormats/GEMObjects/src/headers.h"

int main() {
  testSerialization<GEMELMap>();
  testSerialization<GEMELMap::GEMChamberMap>();
  testSerialization<std::vector<GEMELMap::GEMChamberMap>>();
  testSerialization<GEMELMap::GEMStripMap>();
  testSerialization<std::vector<GEMELMap::GEMStripMap>>();

  testSerialization<GEMeMap>();
  testSerialization<GEMeMap::GEMChamberMap>();
  testSerialization<std::vector<GEMeMap::GEMChamberMap>>();
  testSerialization<GEMeMap::GEMStripMap>();
  testSerialization<std::vector<GEMeMap::GEMStripMap>>();

  testSerialization<GEMDeadStrips>();
  testSerialization<GEMDeadStrips::DeadItem>();
  testSerialization<GEMMaskedStrips>();
  testSerialization<GEMMaskedStrips::MaskItem>();
}
