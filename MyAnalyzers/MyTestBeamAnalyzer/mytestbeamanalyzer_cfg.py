import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

# process.load('Configuration.Geometry.GeometryExtended2015MuonGEMDevReco_cff')
# process.load('Configuration.Geometry.GeometryExtended2015MuonGEMDev_cff')
# process.load('Configuration.Geometry.GeometryExtended2026D87Reco_cff')
process.load('gemsw.Geometry.GeometryTestBeam_cff')

# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )
# process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(250000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        # 'file:out_local_reco.root'

        # old mapping file
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_100k_Run_20211028_1924/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_250k_20211029_1110/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_2p5M_20211029_1110/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_2p5M_20211029_1837/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_250k_20211030_1600/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_2p5M_20211030_1844/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/fix-II/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_ME0_250k_20211103_0159/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/fix-II/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_ME0_250k_20211103_0304/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/fix-IV/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_ME0_250k_20211103_0304/output_edm.root'
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV1_DeadStrips/Results_250k_20211030_1844/output_edm.root'
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

        # new mapping file
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_250k_20211030_1844_newMap/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV2_DeadStrips/Results_250k_20211030_1844/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV2_DeadStrips/Results_250k_20211111_0955/output_edm.root'
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_GE21_250k_20211111_1518/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_250k_20211111_1518/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_simple_eMap_250k_20211111_1518/output_edm.root'
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        # simple eMAP :: Pions / Muons
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_simple_eMap_1M_20211111_1518/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_simple_eMap_1M_20211111_0955/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_simple_eMap_1M_20211103_0304/output_edm.root'
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        # standard eMAP :: Pions / Muons
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_std_eMap_1M_20211111_1518/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_std_eMap_1M_20211111_0955/output_edm.root'
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_GE21_1M_20211111_1518/output_edm.root'
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_GE21_1M_20211111_0955/output_edm.root'
        # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
        # 'file:/lustre/home/piet/Experiment_CMS/GEMDPG/TestBeam/new-geo-try3a/CMSSW_12_1_0_pre4/src/gemsw/EventFilter/test/Results_eMapV3_DeadStrips/Results_ME0_new_eMap_1M_20211111_0955/output_edm.root'

    ),
    labelRawDataLikeMC = cms.untracked.bool(False)
)

process.demo = cms.EDAnalyzer('MyTestBeamAnalyzer',
                              MapVersion    = cms.untracked.uint32(2),
                              DebugInfo     = cms.untracked.bool(False),
                              DebugTest     = cms.untracked.bool(False),
                              DebugDead     = cms.untracked.bool(False),
                              DebugMatching = cms.untracked.bool(False),
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_250k_20211030_1844.root"),
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_250k_20211111_0955.root"),
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_250k_20211111_1518.root"),
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_GE21_250k_20211111_1518.root"),
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_250k_20211111_1518.root"),
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_test.root"),
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_std_eMap.root"),  
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_simple_eMap.root"), 
                              # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_simple_eMap_1M_pions.root"), 
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_simple_eMap_1M_muons.root"), 
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_simple_eMap_1M_muons_1stperiod.root"), 
                              # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_std_eMap_1M_pions.root"), 
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_std_eMap_1M_muons.root"), 
                              # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                              RootFileName  = cms.untracked.string("TestBeamHistograms_GE21_1M_pions.root"), 
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_GE21_1M_muons.root"), 
                              # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                              # RootFileName  = cms.untracked.string("TestBeamHistograms_ME0_new_eMap_1M_muons.root"), 

)


process.p = cms.Path(process.demo)
