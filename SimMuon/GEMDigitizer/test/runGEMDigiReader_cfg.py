import FWCore.ParameterSet.Config as cms

process = cms.Process("Dump")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
# process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
# process.load('Configuration.Geometry.GeometryExtended2019_cff')
process.load('Configuration.Geometry.GeometryExtended2023D1Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D1_cff')
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
        # 'file:out_digi.root'
        'file:/lustre/home/piet/cmshome/SLC6/ME0_Studies/810_ME0Granularity/CMSSW_8_1_0_pre11/src/MyCmsDriverCommands/TDR_baseline/step2.root'
    )
)

process.dumper = cms.EDAnalyzer("GEMDigiReader",
    simhitToken = cms.InputTag("g4SimHits","MuonGEMHits"), 
    gemDigiToken = cms.InputTag("simMuonGEMDigis"), 
    gemDigiSimLinkToken = cms.InputTag("simMuonGEMDigis","GEM") 
)

process.p    = cms.Path(process.dumper)

