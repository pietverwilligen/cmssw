import FWCore.ParameterSet.Config as cms

process = cms.Process("Dump")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
# process.load('Configuration.Geometry.GeometryExtended2023D1Reco_cff')
# process.load('Configuration.Geometry.GeometryExtended2023D1_cff')
process.load('Configuration.Geometry.GeometryExtended2023D1ME0DevReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D1ME0Dev_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        # 'file:/lustre/cms/store/user/rosma/SingleMuPlusPt30_14TeV_GEN-SIM_2023HGCalME0Geomv2_AllEtaPhi/crab_SingleMuPt30_FullPt_PU0_NoBkg_8etaPart_768Strip_1ns_RECO_RealRO/160705_110743/0000/out_reco_me0_rechits_ME0Muon_PU0_1.root'
    )
)

process.dumper = cms.EDAnalyzer("ME0DigiReader",
    simhitToken = cms.InputTag("g4SimHits","MuonME0Hits"),
    me0DigiToken = cms.InputTag("simMuonME0Digis")
)

process.p = cms.Path(process.dumper)
