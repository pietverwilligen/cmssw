# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 --filein dbs:/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/TP2023HGCALGS-newsplit_DES23_62_V1-v1/GEN-SIM --fileout file:JME-TP2023SHCALDR-00001_step1.root --mc --eventcontent FEVTDEBUG --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2023HGCalMuon,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI-RAW --conditions PH2_1K_FB_V6::All --step DIGI:pdigi_valid,L1,DIGI2RAW --geometry Extended2023HGCalMuon,Extended2023HGCalMuonReco --python_filename HGCal_config_DIGI_noPU_test.py --no_exec -n 5
import FWCore.ParameterSet.Config as cms

process = cms.Process('DIGI2RAW')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
      # '/store/mc/TP2023HGCALGS/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/GEN-SIM/newsplit_DES23_62_V1-v1/00000/00080254-D2ED-E411-9B24-E03F49D6226B.root', 
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_1.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_2.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_3.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_4.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_5.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_6.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_7.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_8.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_9.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_10.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_11.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_12.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_13.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_14.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_15.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_16.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_17.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_18.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_19.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_20.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_21.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_22.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_23.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_24.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_25.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_26.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_27.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_28.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_29.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_30.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_31.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_32.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_33.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_34.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_35.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_36.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_37.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_38.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_39.root',
      '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/crab_DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM/151112_174917/0000/out_40.root',
    )
)

process.options = cms.untracked.PSet()

### TO ACTIVATE LogDebug NEED TO COMPILE IT WITH:
### --------------------------------------------------------------
### --> scram b -j8 USER_CXXFLAGS="-DEDM_ML_DEBUG"                
### Make sure that you first cleaned your CMSSW version:          
### --> scram b clean                                             
### before issuing the scram command above                        
### --------------------------------------------------------------
### LogDebug output goes to cout; all other output to "junk.log"  
### Code/Configuration with thanks to Tim Cox                     
##################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.categories.append("ME0PreRecoGaussianModel")
process.MessageLogger.categories.append("ME0PreRecoGaussianModelNoise")
process.MessageLogger.debugModules = cms.untracked.vstring("*")
process.MessageLogger.destinations = cms.untracked.vstring("cout","junk")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("DEBUG"),
    default = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
    FwkReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0PreRecoGaussianModel      = cms.untracked.PSet( limit = cms.untracked.int32(-1) ), 
    ME0PreRecoGaussianModelNoise = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
)


# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('step1 nevts:5'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.FEVTDEBUGoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    fileName = cms.untracked.string('file:JME-TP2023SHCALDR-00001_step1.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW')
    )
)

# Additional output definition

# Other statements
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PH2_1K_FB_V6::All', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGoutput_step = cms.EndPath(process.FEVTDEBUGoutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.endjob_step,process.FEVTDEBUGoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2023HGCalMuon 

#call to customisation function cust_2023HGCalMuon imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2023HGCalMuon(process)

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions
process.load('SimMuon.GEMDigitizer.muonME0DigisPreReco_cfi')
process.simMuonME0Digis.timeResolution = cms.double(1.00) # ns  -- default 0.10 = 100ps
process.simMuonME0Digis.phiResolution  = cms.double(1.00) # cm  -- default 0.05 = 500um
process.simMuonME0Digis.etaResolution  = cms.double(1.00) # cm  -- default 1.00 = 1cm
