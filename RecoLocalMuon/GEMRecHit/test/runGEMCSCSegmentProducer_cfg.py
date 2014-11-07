import FWCore.ParameterSet.Config as cms

process = cms.Process("GEMCSCREC")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2019_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")


process.load('RecoLocalMuon.GEMRecHit.gemcscSegments_cfi')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
                                      #'file:/lustre/cms/store/user/radogna/GEMCSCSegment/NoNoise/out_digireco100_10000.root',
                                      'file:/lustre/cms/store/user/radogna/GEMCSCSegment/out_reco10_paranormal.root',
                                      #'file:/lustre/cms/store/user/radogna/GEMCSCSegment/NoNoise/out_digireco10_xcommit.root',
                                      #'file:/lustre/cms/store/user/calabria/calabria_SingleMuPt10_GEN-SIM-DIGI-RECO_CMSSW_6_2_0_SLHC9_8EtaPar_LXPLUS_DIGIv7_GeomV5_TeVMuon_v2/calabria_SingleMuPt10_GEN-SIM-DIGI-RECO_CMSSW_6_2_0_SLHC9_8EtaPar_LXPLUS_DIGIv7_GeomV5_TeVMuon_v2/387588dc8e1633241b2179741cba1455/out_reco_100_2_ztF.root',
                                    
   

    )
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(
        'file:/lustre/cms/store/user/radogna/GEMCSCSegment/FullDigi_withNewSSegm/out_rec_gemcsc.test10_xcommit.root'
    ),
    outputCommands = cms.untracked.vstring(
        'keep  *_*_*_*',
    )
)


process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")
process.reco_step    = cms.Path(process.gemcscSegments)
process.endjob_step  = cms.Path(process.endOfProcess)
process.out_step     = cms.EndPath(process.output)

process.schedule = cms.Schedule(
    process.reco_step,
    process.endjob_step,
    process.out_step
)
