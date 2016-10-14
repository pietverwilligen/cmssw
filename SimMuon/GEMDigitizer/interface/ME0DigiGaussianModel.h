#ifndef GEMDigitizer_ME0DigiGaussianModel_h
#define GEMDigitizer_ME0DigiGaussianModel_h

/**
 * \class ME0DigiGaussianModel
 *
 * Class for the ME0 Realistic Digitization with Gaussian smearing 
 * to emulate spatial and time response of the detector
 */

#include "SimMuon/GEMDigitizer/interface/ME0DigiModel.h"
class ME0Geometry;
namespace CLHEP
{
  class HepRandomEngine;
}
class ME0DigiGaussianModel: public ME0DigiModel
{
public:
  ME0DigiGaussianModel(const edm::ParameterSet&);
  ~ME0DigiGaussianModel();
  // void   setupEtaPartition(const ME0EtaPartition*);
  void   simulateSignal(const ME0EtaPartition*, const edm::PSimHitContainer&, CLHEP::HepRandomEngine*);
  void   simulateNoise(const ME0EtaPartition*, CLHEP::HepRandomEngine*);
  // void   implementDeadTime();
  std::vector<ME0Digi> simulateClustering(const ME0EtaPartition*, LocalPoint, double, int, bool, CLHEP::HepRandomEngine*);
  double correctSigmaU(const ME0EtaPartition*, double);
  void   setup() {}
private:
  double sigma_t;
  double sigma_u;
  double sigma_v;
  double gaussianSmearing_;
  double constPhiSmearing_;
  bool   digitizeOnlyMuons_;
  double averageEfficiency_;
  // bool simulateIntrinsicNoise_; // not implemented
  // double averageNoiseRate_;     // not implemented
  bool simulateElectronBkg_;
  bool simulateNeutralBkg_;
  double tDeadTime_;   // Electronics Dead Time ... in ns 
  double meanCls_;
  int minBunch_;
  int maxBunch_;

  // params for the simple pol6 model of neutral bkg for ME0:
  std::vector<double> neuBkg, eleBkg;
};
#endif

