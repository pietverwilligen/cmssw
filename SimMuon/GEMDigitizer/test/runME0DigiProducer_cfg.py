
import FWCore.ParameterSet.Config as cms

process = cms.Process("GEMDIGI")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D5Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D5_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    'file:step1.root'
    # 'file:/lustre/home/piet/cmshome/SLC6/ME0_Studies/810_ME0Granularity/CMSSW_8_1_0_pre11/src/MyCmsDriverCommands/step2.root'

    )
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")

# GEM digitizer
process.load('SimMuon.GEMDigitizer.muonME0Digis_cfi')
# testing some different settings ... overwritting defaults
process.simMuonME0Digis.timeResolution = cms.double(0.001)
process.simMuonME0Digis.phiResolution = cms.double(0.05)
process.simMuonME0Digis.etaResolution = cms.double(1.)
process.simMuonME0Digis.digitizeOnlyMuons = cms.bool(True)
process.simMuonME0Digis.gaussianSmearing = cms.bool(True)
process.simMuonME0Digis.constantPhiSpatialResolution = cms.bool(True)
process.simMuonME0Digis.simulateElectronBkg = cms.bool(False)
process.simMuonME0Digis.simulateNeutralBkg  = cms.bool(False)
process.simMuonME0Digis.nEtaPart = cms.int32(8)
process.simMuonME0Digis.dEtaFixed = cms.bool(True)
process.simMuonME0Digis.tDeadTime = cms.double(1.0)
process.simMuonME0Digis.meanCls  = cms.double(1.0)           # meanCls = 0.0 ==> clustersize exactly 1 

### TO ACTIVATE LogTrace IN ME0Digitizer NEED TO COMPILE IT WITH:
### -------------------------------------------------------------
### --> scram b clean                                          
### --> scram b -j8 USER_CXXFLAGS="-DEDM_ML_DEBUG"             
### -------------------------------------------------------------
#################################################################                                                                                                                                                                                                             
process.load("FWCore.MessageLogger.MessageLogger_cfi")
# process.MessageLogger.categories.append("GEMGeometryBuilderFromDDD")
process.MessageLogger.categories.append("ME0GeometryBuilderFromDDD")
process.MessageLogger.categories.append("ME0DigiProducer")
process.MessageLogger.categories.append("ME0DigiModelFactory")
process.MessageLogger.categories.append("ME0DigiModel")
process.MessageLogger.categories.append("ME0DigiGaussianModel")
# process.MessageLogger.categories.append("ME0DigiGaussianModelNoise")
process.MessageLogger.debugModules = cms.untracked.vstring("*")
process.MessageLogger.destinations = cms.untracked.vstring("cout","junk")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("DEBUG"),
    default = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
    FwkReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # GEMGeometryBuilderFromDDD   = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0GeometryBuilderFromDDD   = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0DigiProducer           = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0DigiModelFactory       = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0DigiModel              = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0DigiGaussianModel      = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0DigiGaussianModelNoise = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
)








# customization of the process.pdigi sequence to add the GEM digitizer
# from SimMuon.GEMDigitizer.customizeGEMDigi import *
# process = customize_digi_addGEM_addME0_muon_only(process) # only muon+GEM+ME0 digi

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(
        'file:out_digi_me0.test.root'
    ),
    outputCommands = cms.untracked.vstring(
        'keep  *_*_*_*',
        #'drop CastorDataFramesSorted_simCastorDigis_*_GEMDIGI'
        # drop all CF stuff
        ##'drop *_mix_*_*',
        # drop tracker simhits
        ##'drop PSimHits_*_Tracker*_*',
        # drop calorimetry stuff
        ##'drop PCaloHits_*_*_*',
        # clean up simhits from other detectors
        ##'drop PSimHits_*_Totem*_*',
        ##'drop PSimHits_*_FP420*_*',
        ##'drop PSimHits_*_BSC*_*',
        # drop some not useful muon digis and links
        ##'drop *_*_MuonCSCStripDigi_*',
        ##'drop *_*_MuonCSCStripDigiSimLinks_*',
        #'drop *SimLink*_*_*_*',
        ##'drop *RandomEngineStates_*_*_*',
        ##'drop *_randomEngineStateProducer_*_*'
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('digi_step')
    )
)


process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")
process.digi_step    = cms.Path(process.pdigi)
process.endjob_step  = cms.Path(process.endOfProcess)
process.out_step     = cms.EndPath(process.output)

process.schedule = cms.Schedule(
    process.digi_step,
    process.endjob_step,
    process.out_step
)
