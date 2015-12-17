# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/Generator/python/DYtoMuMu_M_20_TuneZ2star_14TeV_pythia6_tauola_cff.py --fileout file:out.root --mc --eventcontent RAWSIM --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2023HGCalMuon,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --conditions DES23_62_V1::All --beamspot HLLHC --step GEN,SIM --magField 38T_PostLS1 --geometry Extended2023HGCalMuon,Extended2023HGCalMuonReco --python_filename DYToMuMu_M_20_TuneZ2star_14TeV_pythia6_GEN-SIM_cfg.py --no_exec -n 10
import FWCore.ParameterSet.Config as cms

process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuon_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('Configuration/Generator/python/DYtoMuMu_M_20_TuneZ2star_7TeV_pythia6_tauola_cff.py nevts:10'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:out.root'),
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
process.GlobalTag = GlobalTag(process.GlobalTag, 'DES23_62_V1::All', '')

# from Configuration.Generator.PythiaUEZ2starSettings import pythiaUESettingsBlock
process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    crossSection = cms.untracked.double(2725),
    comEnergy = cms.double(14000.0),
    PythiaParameters = cms.PSet(
        # pythiaUESettingsBlock, # manual copy below
		pythiaUESettings = cms.vstring(
			'MSTU(21)=1     ! Check on possible errors during program execution', 
			'MSTJ(22)=2     ! Decay those unstable particles', 
			'PARJ(71)=10 .  ! for which ctau  10 mm', 
			'MSTP(33)=0     ! no K factors in hard cross sections', 
			'MSTP(2)=1      ! which order running alphaS', 
			'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)',
			'MSTP(52)=2     ! work with LHAPDF',
			
			'PARP(82)=1.921 ! pt cutoff for multiparton interactions', 
			'PARP(89)=1800. ! sqrts for which PARP82 is set', 
			'PARP(90)=0.227 ! Multiple interactions: rescaling power', 
			
			'MSTP(95)=6     ! CR (color reconnection parameters)',
			'PARP(77)=1.016 ! CR',
			'PARP(78)=0.538 ! CR',
			
			'PARP(80)=0.1   ! Prob. colored parton from BBR',
			
			'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
			'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
			
			'PARP(62)=1.025 ! ISR cutoff', 
			
			'MSTP(91)=1     ! Gaussian primordial kT', 
			'PARP(93)=10.0  ! primordial kT-max', 
			
			'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
			'MSTP(82)=4     ! Defines the multi-parton model', 
			),
		processParameters = cms.vstring('MSEL=0            !User defined processes',
                                        'MSUB(1)=1         !Incl Z0/gamma* production',
                                        'MSTP(43)=3        !Both Z0 and gamma*',
                                        'MDME(174,1)=0     !Z decay into d dbar',
                                        'MDME(175,1)=0     !Z decay into u ubar',
                                        'MDME(176,1)=0     !Z decay into s sbar',
                                        'MDME(177,1)=0     !Z decay into c cbar',
                                        'MDME(178,1)=0     !Z decay into b bbar',
                                        'MDME(179,1)=0     !Z decay into t tbar',
                                        'MDME(182,1)=0     !Z decay into e- e+',
                                        'MDME(183,1)=0     !Z decay into nu_e nu_ebar',
                                        'MDME(184,1)=1     !Z decay into mu- mu+',
                                        'MDME(185,1)=0     !Z decay into nu_mu nu_mubar',
                                        'MDME(186,1)=0     !Z decay into tau- tau+',
                                        'MDME(187,1)=0     !Z decay into nu_tau nu_taubar',
                                        'CKIN(1)=20.       !Minimum sqrt(s_hat) value (=Z mass)'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings',
            'processParameters')
    )
)
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/GenProduction/python/EightTeV/DYToMuMu_M_20_TuneZ2star_14TeV_pythia6_cff.py,v $'),
    annotation = cms.untracked.string('PYTHIA6 Z/gamma* to mumu, M(mu+mu-) > 20 GeV at sqrt(s) = 14 TeV, Tune Z2*')
)

process.ProductionFilterSequence = cms.Sequence(process.generator)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 

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
