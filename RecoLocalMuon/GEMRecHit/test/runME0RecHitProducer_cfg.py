
import FWCore.ParameterSet.Config as cms

process = cms.Process("ME0REC")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
# process.load('Configuration.Geometry.GeometryExtended2023D1Reco_cff')
# process.load('Configuration.Geometry.GeometryExtended2023D1_cff')
process.load('Configuration.Geometry.GeometryExtended2023D1ME0DevReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D1ME0Dev_cff')
# process.load('Configuration.Geometry.GeometryExtended2023D5Reco_cff')
# process.load('Configuration.Geometry.GeometryExtended2023D5_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Manual Customisation
process.ME0GeometryESModule.use10EtaPart = cms.bool(True) # build geometry with 10 eta partitions

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")

process.load('RecoLocalMuon.GEMRecHit.me0RecHits_cfi')
process.load('RecoLocalMuon.GEMSegment.me0Segments_cfi')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        # 'file:stepME0digiGauss.root'
        'file:/lustre/home/piet/cmshome/SLC6/ME0_Studies/810_ME0Granularity/CMSSW_8_1_0_pre11/src/MyCmsDriverCommands/step2.root'
    )
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string(
        'file:out_rec_me0.test2.root'
    ),
    outputCommands = cms.untracked.vstring(
        'keep  *_*_*_*',
    )
)


process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")
# process.reco_step    = cms.Path(process.me0RecHits*process.me0Segments)
process.reco_step    = cms.Path(process.me0RecHits)
process.endjob_step  = cms.Path(process.endOfProcess)
process.out_step     = cms.EndPath(process.output)

process.schedule = cms.Schedule(
    process.reco_step,
    process.endjob_step,
    process.out_step
)

### -----------------------------------------------------------                                                                                                                                                                                                                
### --> scram b -j8 USER_CXXFLAGS="-DEDM_ML_DEBUG"                                                                                                                                                                                                                             
### Make sure that you first cleaned your CMSSW version:        
### --> scram b clean                                           
### before issuing the scram command above                      
############################################################### 
process.load("FWCore.MessageLogger.MessageLogger_cfi")
# process.MessageLogger.categories.append("ME0NumberingScheme")       
# process.MessageLogger.categories.append("ME0GeometryBuilderFromDDD")
process.MessageLogger.categories.append("ME0RecHitProducer")
process.MessageLogger.categories.append("ME0RecHitBaseAlgo")
process.MessageLogger.categories.append("ME0RecHitStandardAlgo")
process.MessageLogger.categories.append("ME0SegmentProducer")
# process.MessageLogger.categories.append("Muon|RecoMuon|RecoMuonDetLayers|MuonME0DetLayerGeometryBuilder")
process.MessageLogger.categories.append("Muon")
process.MessageLogger.categories.append("RecoMuon")
process.MessageLogger.categories.append("RecoMuonDetLayers")
process.MessageLogger.categories.append("MuonME0DetLayerGeometryBuilder")
process.MessageLogger.debugModules = cms.untracked.vstring("*")
process.MessageLogger.destinations = cms.untracked.vstring("cout","junk")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("DEBUG"),
    default = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
    FwkReport = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    # ME0GeometryBuilderFromDDD   = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),                 
    # ME0NumberingScheme = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),                          
    ME0RecHitProducer              = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0RecHitBaseAlgo              = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0RecHitStandardAlgo          = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    ME0SegmentProducer             = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    Muon                           = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    RecoMuon                       = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    RecoMuonDetLayers              = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
    MuonME0DetLayerGeometryBuilder = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
)
