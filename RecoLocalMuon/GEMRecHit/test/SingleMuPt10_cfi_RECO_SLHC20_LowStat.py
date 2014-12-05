# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 -s RAW2DIGI,L1Reco,RECO --conditions auto:upgrade2019 --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2019WithGem --magField 38T_PostLS1 --datatier GEN-SIM-DIGI-RECO --geometry Extended2019 --eventcontent FEVTDEBUGHLT -n -1 --no_exec --filein=file:SingleMuPt10_DIGI-RAW_upgrade2019.root --fileout=file:SingleMuPt10_RECO_upgrade2019.root --python_filename=SingleMuPt10_cfi_RECO_SLHC20.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:SingleMuPt10_DIGI-RAW_upgrade2019_lowstat.root')
)

process.options = cms.untracked.PSet(

)

# # Message Logger
# process.MessageLogger = cms.Service("MessageLogger",
#     # --- get more debug printout ---
#     # activate LogDebug messages     
#     # more info: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMessageLoggerDebug
#     destinations = cms.untracked.vstring(
#         'detailedInfo',
#         'critical'
#     ),
#     detailedInfo = cms.untracked.PSet(
#     threshold = cms.untracked.string('DEBUG')
#     ),
#     debugModules = cms.untracked.vstring(
#         # 'GEMCSCSegmentBuilder',
#         # 'GEMCSCSegmentProducer',
#         # 'GEMCSCSegmentBuilderPluginFactory',
#         # 'GEMCSCSegAlgoRR',
#         # 'GEMCSCSegmentAlgorithm',
#         # 'TevMuonProducer',
#         '*'
#     )
# )



# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('step3 nevts:-1'),
    name = cms.untracked.string('Applications')
)

# Output definition
process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('file:SingleMuPt10_RECO_upgrade2019_lowstat.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RECO')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2019WithGem 

# call to customisation function cust_2019WithGem imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2019WithGem(process)

# End of customisation functions
