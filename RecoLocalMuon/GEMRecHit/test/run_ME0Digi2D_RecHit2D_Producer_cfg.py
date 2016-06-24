
import FWCore.ParameterSet.Config as cms

process = cms.Process("ME0REC")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuon_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")


# GEM digitizer
process.load('SimMuon.GEMDigitizer.muonME0Digis2D_cfi')
# testing some different settings ... overwritting defaults
process.simMuonME0Digis2D.timeResolution = cms.double(0.001)
process.simMuonME0Digis2D.phiResolution = cms.double(0.05)
process.simMuonME0Digis2D.etaResolution = cms.double(1.)
process.simMuonME0Digis2D.digitizeOnlyMuons = cms.bool(True)
process.simMuonME0Digis2D.gaussianSmearing = cms.bool(True)
process.simMuonME0Digis2D.simulateElectronBkg = cms.bool(True)
process.simMuonME0Digis2D.simulateNeutralBkg  = cms.bool(True)
process.simMuonME0Digis2D.nEtaPart = cms.int32(8)
process.simMuonME0Digis2D.nEtaFixed = cms.bool(True)
process.simMuonME0Digis2D.tDeadTime = cms.double(1.0)
process.simMuonME0Digis2D.meanCls  = cms.double(1.0)           # meanCls = 0.0 ==> clustersize exactly 1

# customization of the process.pdigi sequence to add the GEM digitizer                                                                                                                                                                                                        
from SimMuon.GEMDigitizer.customizeME0Digi import *
# process = customize_digi_addGEM(process)  # run all detectors digi
process = customize_digi_addME0_me0_only(process) # only ME0 digi                                             

process.load('RecoLocalMuon.GEMRecHit.me0RecHits2D_cfi')
# process.load('RecoLocalMuon.GEMRecHit.me0Segments_cfi')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/tmp/piet/formarcello/singlemupt10_step3.root'
    )
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(
        'file:out_reco_me0_rechits.root'
    ),
    outputCommands = cms.untracked.vstring(
        'keep  *_*_*_*',
    )
)


process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")
process.reco_step    = cms.Path(process.pdigi*process.me0RecHits2D)
# process.reco_step    = cms.Path(process.me0RecHits2D*process.me0Segments)
process.endjob_step  = cms.Path(process.endOfProcess)
process.out_step     = cms.EndPath(process.output)

process.schedule = cms.Schedule(
    process.reco_step,
    process.endjob_step,
    process.out_step
)
