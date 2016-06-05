#include "SimMuon/GEMDigitizer/interface/ME0Digi2DModel.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"

void 
ME0Digi2DModel::fillDigis(int rollDetId, ME0Digi2DCollection& digis)
{
  for (auto d: digi_)
  {
    digis.insertDigi(ME0DetId(rollDetId), d);
  }
  digi_.clear();
}

