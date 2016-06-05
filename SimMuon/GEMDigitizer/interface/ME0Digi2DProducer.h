#ifndef GEMDigitizer_ME0Digi2DProducer_h
#define GEMDigitizer_ME0Digi2DProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimMuon/GEMDigitizer/interface/ME0Digi2DModel.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "string"

class ME0Geometry;

class ME0Digi2DProducer : public edm::EDProducer
{
public:

  explicit ME0Digi2DProducer(const edm::ParameterSet& ps);

  virtual ~ME0Digi2DProducer();

  virtual void beginRun( edm::Run&, const edm::EventSetup& ) {};

  virtual void produce(edm::Event& e, const edm::EventSetup& c);

  virtual void endRun( edm::Run&, const edm::EventSetup& ) {}

private:

  //Name of Collection used for create the XF 
  std::string collectionXF_;
  std::string digi2DModelString_;
  
  ME0Digi2DModel* me0Digi2DModel_;

};

#endif

