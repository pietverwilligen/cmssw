#ifndef GEMDigitizer_ME0Digi2DGaussianModel_h
#define GEMDigitizer_ME0Digi2DGaussianModel_h

/**
 * \class ME0Digi2DGaussianModel
 *
 * Class for the ME0 Gaussian response simulation as pre-reco step
 */

#include "SimMuon/GEMDigitizer/interface/ME0Digi2DModel.h"
class ME0Geometry;
namespace CLHEP
{
  class HepRandomEngine;
  class RandFlat;
  class RandGaussQ;
  class RandPoissonQ;
  class RandExponential;
}
class ME0Digi2DGaussianModel: public ME0Digi2DModel
{
public:
  ME0Digi2DGaussianModel(const edm::ParameterSet&);
  ~ME0Digi2DGaussianModel();
  void   setupEtaPartition(const ME0EtaPartition*);
  void   simulateSignal(const ME0EtaPartition*, const edm::PSimHitContainer&);
  void   setRandomEngine(CLHEP::HepRandomEngine&);
  void   simulateNoise(const ME0EtaPartition*);
  void   implementDeadTime();
  // std::vector<ME0Digi2D> simulateClustering(const ME0EtaPartition*, const PSimHit*);
  std::vector<ME0Digi2D> simulateClustering(const ME0EtaPartition*, LocalPoint, double, int, int);
  double correctSigmaU(const ME0EtaPartition*, double);
  void   setup()
  {
  }
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
  int nEtaPart_;
  bool dEtaFixed_;
  double tDeadTime_;   // Electronics Deat Time ... in ns 
  double meanCls_;
  int minBunch_;
  int maxBunch_;



  std::vector<double> etaParts;
  CLHEP::RandGaussQ*      gauss_;
  CLHEP::RandFlat*        flat1_;
  CLHEP::RandFlat*        flat2_;
  CLHEP::RandPoissonQ*    poisson_;
  CLHEP::RandExponential* exp_;
  // params for the simple pol6 model of neutral bkg for ME0:
  std::vector<double> neuBkg, eleBkg;
  std::vector<double> etaPartsE_, etaPartsY_;
};
#endif

