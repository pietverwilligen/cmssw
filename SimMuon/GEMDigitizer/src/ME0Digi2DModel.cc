#include "SimMuon/GEMDigitizer/interface/ME0Digi2DModel.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"

void 
ME0Digi2DModel::fillDigis(int rollDetId, ME0Digi2DCollection& digis)
{
  for (auto d: digi_)
  {
    digis.insertDigi(ME0DetId(rollDetId), d);
    // std::cout<<"[ME0Digi2DModel] DetId "<<ME0DetId(rollDetId)<<" Digi inserted: "<<d<<std::endl;
  }
  digi_.clear();
}

void 
ME0Digi2DModel::printDigis(ME0Digi2DCollection& digis)
{
  ME0Digi2DCollection::DigiRangeIterator me0dgIt;
  for (me0dgIt = digis.begin(); me0dgIt != digis.end(); ++me0dgIt){
    const ME0DetId& me0Id = (*me0dgIt).first;
    const ME0Digi2DCollection::Range& digiRange = (*me0dgIt).second;
    for (ME0Digi2DCollection::const_iterator digi = digiRange.first; digi != digiRange.second;digi++) {
      std::cout<<"[ME0Digi2DModel] :: print Digi :: in DetId "<<me0Id<<" digi "<<*digi<<std::endl;
    }
  }
}

