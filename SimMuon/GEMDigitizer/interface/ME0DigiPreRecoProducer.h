#ifndef SimMuon_GEMDigitizer_ME0DigiPreRecoProducer_h
#define SimMuon_GEMDigitizer_ME0DigiPreRecoProducer_h

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupMixingContent.h"
#include "SimGeneral/MixingModule/interface/DigiAccumulatorMixMod.h"
#include <string>

class ME0Geometry;
class ME0DigiPreRecoModel;

class ME0DigiPreRecoProducer : public edm::stream::EDProducer<>
// class ME0DigiPreRecoProducer : public DigiAccumulatorMixMod
			       // class ME0DigiPreRecoProducer : public edm::stream::EDProducer<>, public DigiAccumulatorMixMod
{
public:

  explicit ME0DigiPreRecoProducer(const edm::ParameterSet& ps);
  virtual ~ME0DigiPreRecoProducer();

  // virtual void initializeEvent(edm::Event const& e, edm::EventSetup const& c) override;
  // virtual void accumulate(edm::Event const& e, edm::EventSetup const& c) override;
  // virtual void accumulate(PileUpEventPrincipal const& e, edm::EventSetup const& c, edm::StreamID const&) override;
  // virtual void finalizeEvent(edm::Event& e, edm::EventSetup const& c) override;


  virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;

  double getLumi(int, int, int);

  virtual void StorePileupInformation( std::vector<int> &numInteractionList,
				       std::vector<int> &bunchCrossingList,
				       std::vector<float> &TrueInteractionList, 
				       std::vector<edm::EventID> &eventInfoList, int bunchSpacing){
    PileupInfo_ = new PileupMixingContent(numInteractionList, bunchCrossingList, TrueInteractionList, eventInfoList, bunchSpacing);
  }

  virtual PileupMixingContent* getEventPileupInfo() { return PileupInfo_; }

  void calculateInstlumiFactor(PileupMixingContent*);

private:

  //Name of Collection used for create the XF 
  edm::EDGetTokenT<CrossingFrame<PSimHit> > cf_token; 
  // edm::EDGetTokenT<std::vector<PileupSummaryInfo>> pu_token; // does not work ... probably because collection is created during digi step and is not accessible this way

  std::string digiPreRecoModelString_;
  ME0DigiPreRecoModel* me0DigiPreRecoModel_;

  int actualInt_;
  double instLumi_;
  PileupMixingContent* PileupInfo_;

};

#endif

