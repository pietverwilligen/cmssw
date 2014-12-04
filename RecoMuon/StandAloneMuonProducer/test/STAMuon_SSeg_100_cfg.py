import FWCore.ParameterSet.Config as cms

process = cms.Process("AnalyzerGLB2")

#process.load("RecoMuon.Configuration.RecoMuon_cff")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2019_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff') #!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')


process.maxEvents = cms.untracked.PSet(
                                       input = cms.untracked.int32(-1)
                                       )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( (
        
                   'file:/lustre/cms/store/user/radogna/GEMCSCSegment/NoNoise_withNewSSegm/out_rec_gemcsc_Xdof_AllChambersSTAGLB_SSeg.test100.root',
                   
                   ))

secFiles.extend((
                
                
                ))

#process.out = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('RecoGLBMuons.root')
#)

process.TFileService = cms.Service("TFileService", fileName = cms.string("file:/lustre/cms/store/user/radogna/GEMCSCSegment/histo_SSeg_100GeV.root") )

minEtaTot = 1.64
maxEtaTot = 2.43

minEtaR1 = 1.64
maxEtaR1 = 2.06

minEtaR2 = 2.13
maxEtaR2 = 2.43

process.STAMuonAnalyzerWithGEMs = cms.EDAnalyzer("STAMuonAnalyzer",
                                                 DataType = cms.untracked.string('SimData'),
                                                 StandAloneTrackCollectionLabel = cms.untracked.InputTag('standAloneMuons','UpdatedAtVtx','NEWRECO'),
                                                 MuonCollectionLabel = cms.untracked.InputTag('muons','','NEWRECO'),
                                                 NoGEMCase = cms.untracked.bool(False),
                                                 isGlobalMuon = cms.untracked.bool(False),
                                                 minEta = cms.untracked.double(minEtaTot),
                                                 maxEta = cms.untracked.double(maxEtaTot),
                                                 )

process.GLBMuonAnalyzerWithGEMs = cms.EDAnalyzer("STAMuonAnalyzer",
                                                 DataType = cms.untracked.string('SimData'),
                                                 StandAloneTrackCollectionLabel = cms.untracked.InputTag('globalMuons','','NEWRECO'),
                                                 MuonCollectionLabel = cms.untracked.InputTag('muons','','NEWRECO'),
                                                 NoGEMCase = cms.untracked.bool(False),
                                                 isGlobalMuon = cms.untracked.bool(True)
                                                 )

process.STAMuonAnalyzerSSegm = cms.EDAnalyzer("STAMuonAnalyzer",
                                                 DataType = cms.untracked.string('SimData'),
                                                 StandAloneTrackCollectionLabel = cms.untracked.InputTag('standAloneMuons','UpdatedAtVtx','STARECO'),
                                                 MuonCollectionLabel = cms.untracked.InputTag('muons','','NEWRECO'),
                                                 NoGEMCase = cms.untracked.bool(True),
                                                 isGlobalMuon = cms.untracked.bool(False),
                                                 minEta = cms.untracked.double(minEtaTot),
                                                 maxEta = cms.untracked.double(maxEtaTot),
                                                 )

process.GLBMuonAnalyzerSSegm = cms.EDAnalyzer("STAMuonAnalyzer",
                                                 DataType = cms.untracked.string('SimData'),
                                                 StandAloneTrackCollectionLabel = cms.untracked.InputTag('globalMuons','','STARECO'),
                                                 MuonCollectionLabel = cms.untracked.InputTag('muons','','NEWRECO'),
                                                 NoGEMCase = cms.untracked.bool(True),
                                                 isGlobalMuon = cms.untracked.bool(True)
                                                 )

    #process.staMuonSequence = cms.Sequence(  process.STAMuonAnalyzerWithGEMs    )



#process.p = cms.Path(process.staMuonSequence)
process.p = cms.Path(process.STAMuonAnalyzerWithGEMs * process.GLBMuonAnalyzerWithGEMs * process.STAMuonAnalyzerSSegm * process.GLBMuonAnalyzerSSegm)
#process.this_is_the_end = cms.EndPath(process.out)

#Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2019WithGem

#call to customisation function cust_2019 imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2019WithGem(process)

# End of customisation functions
