import FWCore.ParameterSet.Config as cms

process = cms.Process("STARECO")

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2019_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

#process.load('RecoLocalMuon.GEMRecHit.gemRecHits_cfi')
process.load('RecoMuon.StandAloneMuonProducer.standAloneMuons_cfi')
process.load('RecoMuon.GlobalMuonProducer.GlobalMuonProducer_cfi')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations   = cms.untracked.vstring(
                                                                           'detailedInfo'
                                                                           ,'critical'
                                                                           ,'cerr'
                                                                           ),
                                    critical       = cms.untracked.PSet(
                                                                          threshold = cms.untracked.string('ERROR')
                                                                         ),
                                    detailedInfo   = cms.untracked.PSet(
                                                                        threshold = cms.untracked.string('INFO')
                                                                        ),
                                    cerr           = cms.untracked.PSet(
                                                                        threshold = cms.untracked.string('WARNING')
                                                                        )
                                    )

process.standAloneMuons.STATrajBuilderParameters.FilterParameters.EnableGEMCSCMeasurement = cms.bool(False)
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.GEMCSCRecSegmentLabel = cms.InputTag("gemcscSegments","","GEMCSCREC")
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.EnableCSCMeasurement = cms.bool(True)
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.CSCRecSegmentLabel = cms.InputTag("cscSegments","","RECO")
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.GEMRecSegmentLabel = cms.InputTag("gemRecHits","","RECO")

process.standAloneMuons.STATrajBuilderParameters.DoBackwardFilter.EnableGEMCSCMeasurement = cms.bool(False)
process.standAloneMuons.STATrajBuilderParameters.DoBackwardFilter.GEMCSCRecSegmentLabel = cms.InputTag("gemcscSegments","","GEMCSCREC")
process.standAloneMuons.STATrajBuilderParameters.DoBackwardFilter.EnableCSCMeasurement = cms.bool(True)
process.standAloneMuons.STATrajBuilderParameters.DoBackwardFilter.CSCRecSegmentLabel = cms.InputTag("cscSegments","","RECO")
process.standAloneMuons.STATrajBuilderParameters.DoBackwardFilter.GEMRecSegmentLabel = cms.InputTag("gemRecHits","","RECO")

process.globalMuons.MuonCollectionLabel = cms.InputTag("standAloneMuons","UpdatedAtVtx","STARECO")
#process.standAloneMuons.STATrajBuilderParameters.FilterParameters.GEMRecSegmentLabel.

### Input and Output Files
##########################
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
                 'file:/lustre/cms/store/user/radogna/GEMCSCSegment/FullDigi_withNewSSegm/out_rec_gemcsc.test10_xcommit.root',
    )
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string( 
                 'file:/lustre/cms/store/user/radogna/GEMCSCSegment/FullDigi_withNewSSegm/out_rec_gemcsc_Xdof_STAGLB_CSCSeg.test10_xcommit.root',
    )
)

### Paths and Schedules
#######################
#process.contentAna = cms.EDAnalyzer("EventContentAnalyzer")
#process.p    = cms.Path(process.standAloneMuons * process.globalMuons * process.contentAna)
process.p    = cms.Path(process.standAloneMuons * process.globalMuons)
#process.this_is_the_end = cms.EndPath(process.out)

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2019WithGem 

#call to customisation function cust_2019WithGem imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2019WithGem(process)

# End of customisation functions
process.outpath = cms.EndPath(process.out)

