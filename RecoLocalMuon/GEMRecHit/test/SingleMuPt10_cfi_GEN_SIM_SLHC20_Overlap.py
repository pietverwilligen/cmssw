# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: SingleMuPt100_cfi -s GEN,SIM --conditions auto:upgrade2019 --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2019WithGem --magField 38T_PostLS1 --datatier GEN-SIM --geometry Extended2019 --eventcontent FEVTDEBUGHLT -n 100 --no_exec --fileout=SingleMuPt10_GEN-SIM_upgrade2019.root --python_filename=SingleMuPt10_cfi_GEN_SIM_SLHC20.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

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
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(500)
)

# Input source
process.source = cms.Source("EmptySource")

# modify FlatRandomPtGunProducer
process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(10.01),
        MinPt = cms.double(9.99),
        PartID = cms.vint32(13),
        MaxEta = cms.double(1.99),
        MaxPhi = cms.double(0.17451), # 10 degrees
        MinEta = cms.double(2.01),
        MinPhi = cms.double(0.17449)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single mu pt 5'),
    AddAntiParticle = cms.bool(True),
    firstRun = cms.untracked.uint32(1)
)
# select generated muons and antimuons
process.genMuons = cms.EDFilter("PdgIdCandViewSelector",
src = cms.InputTag("genParticles"),
pdgId = cms.vint32( 13, -13 )
)
# filter by applying cuts to these generated muons
process.genMuonsFltr = cms.EDFilter("CandViewSelector",
src = cms.InputTag("genMuons"),
cut = cms.string( "pt > 15. & abs(eta)<2.2 & abs(eta)>1.5" ), # or whatever cut expression is deemed necessary
filter = cms.bool(True)
)


process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('SingleMuPt100_cfi nevts:100'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('SingleMuPt10_GEN-SIM_upgrade2019_overlap.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(100.01),
        MinPt = cms.double(99.99),
        PartID = cms.vint32(-13),
        MaxEta = cms.double(2.5),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-2.5),
        MinPhi = cms.double(-3.14159265359)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single mu pt 100'),
    AddAntiParticle = cms.bool(True),
    firstRun = cms.untracked.uint32(1)
)


# Path and EndPath definitions
#process.generation_step = cms.Path(process.pgen)
process.gen_mu_select = cms.Sequence(process.genMuons * process.genMuonsFltr)
process.generation_step = cms.Path(process.pgen * process.gen_mu_select)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition

process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2019WithGem 

#call to customisation function cust_2019WithGem imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2019WithGem(process)

# End of customisation functions
