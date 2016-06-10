#include "SimMuon/GEMDigitizer/interface/ME0Digi2DModel.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"

void 
ME0Digi2DModel::fillDigis(int rollDetId, ME0Digi2DCollection& digis)
{
  for (auto d: digi_)
  {
    digis.insertDigi(ME0DetId(rollDetId), d);

    std::cout<<"[ME0Digi2DModel] DetId "<<ME0DetId(rollDetId)<<" set of Digis inserted: "<<d<<std::endl;
    // does not work
    /*
    for(auto& digi:d)
      {
	std::cout<<"[ME0Digi2DModel] DetId "<<ME0DetId(rollDetId)<<" ME02DDigi inserted: "<<digi<<std::endl;
      }
    */


  }
  digi_.clear();
}

