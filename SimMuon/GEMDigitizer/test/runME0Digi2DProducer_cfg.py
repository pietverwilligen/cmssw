
import FWCore.ParameterSet.Config as cms

process = cms.Process("ME0DIGI2D")

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
from Configuration.StandardSequences.Services_cff import RandomNumberGeneratorService
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    # 'file:step1.root'
    # 'file:/cmshome/piet/SLC6/ME0_Studies/SLHC28_InTimePU/CMSSW_6_2_0_SLHC28_InTimePU/src/MyCMSDriverCommands/step3.root'
    # 'file:/cmshome/piet/SLC6/ME0_Studies/SLHC28_InTimePU/CMSSW_6_2_0_SLHC28_InTimePU/src/MyCMSDriverCommands/singlemupt10_step2.root'
    'file:/tmp/piet/formarcello/singlemupt10_step2.root'
    )
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
process.simMuonME0Digis2D.tDeadTime = cms.double(1.0)
process.simMuonME0Digis2D.meanCls  = cms.double(1.0)           # meanCls = 0.0 ==> clustersize exactly 1




# customization of the process.pdigi sequence to add the GEM digitizer
from SimMuon.GEMDigitizer.customizeME0Digi import *
#process = customize_digi_addGEM(process)  # run all detectors digi
process = customize_digi_addME0_me0_only(process) # only ME0 digi

### TO ACTIVATE LogTrace IN GEMSegment NEED TO COMPILE IT WITH:
### -----------------------------------------------------------
### --> scram b -j8 USER_CXXFLAGS="-DEDM_ML_DEBUG"             
### Make sure that you first cleaned your CMSSW version:       
### --> scram b clean                                          
### before issuing the scram command above                     
### -----------------------------------------------------------
### LogTrace output goes to cout; all other output to "junk.log"
### Code/Configuration with thanks to Tim Cox                   
### -----------------------------------------------------------
### to have a handle on the loops inside RPCSimSetup           
### I have split the LogDebug stream in several streams        
### that can be activated independentl                         
###############################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
# process.MessageLogger.categories.append("GEMGeometryBuilderFromDDD")
# process.MessageLogger.categories.append("ME0GeometryBuilderFromDDD")
# process.MessageLogger.categories.append("ME0Digi2DProducer")
# process.MessageLogger.categories.append("ME0Digi2DModelFactory")
# process.MessageLogger.categories.append("ME0Digi2DModel")
process.MessageLogger.categories.append("ME0Digi2DGaussianModel")
process.MessageLogger.categories.append("ME0Digi2DGaussianModelNoise")
process.MessageLogger.debugModules = cms.untracked.vstring("*")
process.MessageLogger.destinations = cms.untracked.vstring("cout","junk")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("DEBUG"),
    default = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
    FwkReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # GEMGeometryBuilderFromDDD   = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0GeometryBuilderFromDDD   = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0Digi2DProducer           = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0Digi2DModelFactory       = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0Digi2DModel              = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0Digi2DGaussianModel      = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0Digi2DGaussianModelNoise = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
)


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
