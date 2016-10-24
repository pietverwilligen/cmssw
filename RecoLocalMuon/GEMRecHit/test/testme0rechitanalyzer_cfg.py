import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.Geometry.GeometryExtended2023D6Reco_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/afs/cern.ch/work/m/mmaggi/ME0/SEGMENTS/TEST/CMSSW_8_1_0_pre11/src/McProd/out_local_reco_me0segment.root'
    )
)

process.demo = cms.EDAnalyzer('TestME0RecHitAnalyzer',
                              RootFileName = cms.untracked.string("TestME0RecHitHistograms.root"),
)


process.p = cms.Path(process.demo)
