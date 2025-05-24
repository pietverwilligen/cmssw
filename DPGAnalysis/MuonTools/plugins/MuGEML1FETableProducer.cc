/** \class MuGEML1FETableProducer MuGEML1FETableProducer.cc DPGAnalysis/MuonTools/src/MuGEML1FETableProducer.cc
 *  
 * Helper class : FlatTableProducer for GEM Flower Event (reading FED RAW Data)
 *
 * \author Jeewon Heo
 * based on code written by C.Battilana (INFN BO)
 *
 *
 */

#include "FWCore/ParameterSet/interface/allowedValues.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <iostream>
#include <vector>

#include "DPGAnalysis/MuonTools/interface/MuBaseFlatTableProducer.h"
#include "DPGAnalysis/MuonTools/interface/MuNtupleUtils.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
// #include "DataFormats/OnlineMetaData/interface/OnlineLuminosityRecord.h"
#include "DataFormats/TCDS/interface/TCDSRecord.h"


class MuGEML1FETableProducer : public MuBaseFlatTableProducer {
public:

  /// Constructor
  MuGEML1FETableProducer(const edm::ParameterSet&);

  /// Fill descriptors
  static void fillDescriptions(edm::ConfigurationDescriptions&);

protected:
  /// Fill tree branches for a given event
  void fillTable(edm::Event&) final;

  /// Get info from the ES by run
  void getFromES(const edm::Run&, const edm::EventSetup&) final;

private:
  nano_mu::EDTokenHandle<TCDSRecord> m_token;
  // edm::EDGetTokenT<OnlineLuminosityRecord> m_lumiScalerToken;
};

MuGEML1FETableProducer::MuGEML1FETableProducer(const edm::ParameterSet& config)
  : MuBaseFlatTableProducer{config},
  m_token{config, consumesCollector(), "src"} {
    produces<nanoaod::FlatTable>();
  }

void MuGEML1FETableProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;

  desc.add<std::string>("name", "L1A");
  desc.add<edm::InputTag>("src", edm::InputTag{"tcdsDigis"});

  descriptions.addWithDefaultLabel(desc);
}

void MuGEML1FETableProducer::getFromES(const edm::Run& run, const edm::EventSetup& environment) {
}

void MuGEML1FETableProducer::fillTable(edm::Event& ev) {
  std::vector<int> l1aDiff;

  auto record = m_token.conditionalGet(ev);
  constexpr size_t max_trigger = 16;

  // in Heavy Ion Physics the getL1aHistoryEntry is not saved ... 
  // comment out and use this as proxy to inquire BX,Orbit and Lumi
  for (size_t i = 0; i < max_trigger; ++i) {
    int l1a_diff = 3564 * (record->getOrbitNr() - record->getL1aHistoryEntry(i).getOrbitNr())
      + record->getBXID() - record->getL1aHistoryEntry(i).getBXID();
    l1aDiff.push_back(l1a_diff);
  }
  // int m_bunchCrossing = ev.eventAuxiliary().bunchCrossing();
  // int m_orbitNumber   = ev.eventAuxiliary().orbitNumber();
  // int m_instLumi_int  = 0;
  // double m_instLumi_double = 0.0;
  // auto lumiScalers = conditionalGet<OnlineLuminosityRecord>(ev, m_lumiScalerToken, "OnlineLuminosityRecord");
  // auto lumiScalers = m_lumiScalerToken.conditionalGet(ev);
  // if (lumiScalers.isValid()) 
  //   {
  //    m_instLumi_double = lumiScalers->instLumi();
  //  }
  // m_instLumi_int = m_instLumi_double;
  // std::cout<<"BX-id = "<<m_bunchCrossing<<" Orbit Number = "<<m_orbitNumber<<" Inst Lumi = "<<m_instLumi_double<<std::endl;
  // l1aDiff.push_back(m_bunchCrossing);
  // l1aDiff.push_back(m_orbitNumber);
  // l1aDiff.push_back(m_instLumi_int);

  auto table = std::make_unique<nanoaod::FlatTable>(max_trigger, m_name, false, false);
  addColumn(table, "L1Adiff", l1aDiff, "BX differences between event and L1As");
  // addColumn(table, "MuInfo", l1aDiff, "vector with BX-id, Orbit Number and Inst Lumi calculated in MuonTools");

  ev.put(std::move(table));
}

DEFINE_FWK_MODULE(MuGEML1FETableProducer);
