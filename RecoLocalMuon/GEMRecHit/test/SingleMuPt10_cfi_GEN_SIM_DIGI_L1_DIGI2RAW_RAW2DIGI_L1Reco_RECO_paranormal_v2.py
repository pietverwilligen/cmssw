# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: SingleMuPt100_cfi -s GEN,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,L1Reco,RECO --conditions auto:upgrade2019 --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2019WithGem --magField 38T_PostLS1 --datatier GEN-SIM-DIGI-RECO --geometry Extended2019 --eventcontent FEVTDEBUGHLT -n 1000 --no_exec --fileout out_reco.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2019_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Message Logger
process.MessageLogger = cms.Service("MessageLogger",
    # --- get more debug printout ---
    # activate LogDebug messages
    # more info: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMessageLoggerDebug
    destinations = cms.untracked.vstring(
        'detailedInfo',
        'critical'
    ),
    detailedInfo = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG')
    ),
    debugModules = cms.untracked.vstring(
        'GEMCSCSegmentBuilder',
        'GEMCSCSegmentProducer',
        'GEMCSCSegmentBuilderPluginFactory',
	'GEMCSCSegAlgoRR',
        'GEMCSCSegmentAlgorithm',
        'CSCSegtoGEM',
    )
)

# Input source
process.source = cms.Source("EmptySource")
process.options = cms.untracked.PSet()

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('SingleMuPt100_cfi nevts:1000'),
    name = cms.untracked.string('Applications')
)

# Output definition
process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    # fileName = cms.untracked.string('/lustre/cms/store/user/radogna/GEMCSCSegment/out_reco10_paranormal.root'),
    fileName = cms.untracked.string('out_reco_overlap.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RECO')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_simMuonCSC*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_simMuonDT*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_simMuonGEM*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_simMuonRPC*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_muonDT*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_muonCSC*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_muonGEM*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_muonRPC*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_*GEM*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_*Segment*_*_*')
# process.FEVTDEBUGHLToutput.outputCommands.append('keep  *_*_*Segment*_*')

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(10.01),
        MinPt = cms.double(9.99),
        PartID = cms.vint32(13),
        MaxEta = cms.double(2.5),
        MaxPhi = cms.double(3.14159265359/180*7), # 7 degrees
        MinEta = cms.double(1.5),
        MinPhi = cms.double(3.14159265359/180*3)  # 3 degrees
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single mu pt 5'),
    AddAntiParticle = cms.bool(True),
    firstRun = cms.untracked.uint32(1)
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step,process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2019WithGem 

#call to customisation function cust_2019WithGem imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2019WithGem(process)

# End of customisation functions
