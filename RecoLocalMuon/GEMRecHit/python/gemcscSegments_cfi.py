import FWCore.ParameterSet.Config as cms

gemcscSegments = cms.EDProducer("GEMCSCSegmentProducer",
# define input
                                inputObjectsGEM = cms.InputTag("gemRecHits","","RECO"),
                                #inputObjectsGEM = cms.InputTag("gemRecHits","","NEWRECO"),
                                inputObjectsCSC = cms.InputTag("cscSegments","","RECO"),    #2 input gem rec hits and csc segments
                                #inputObjectsCSC = cms.InputTag("cscSegments","","NEWRECO"),#2 input gem rec hits and csc segments
 algo_name = cms.string("GEMCSCSegAlgoRR"),                             
 algo_psets = cms.PSet(
     GEMCSCDebug = cms.untracked.bool(True),
     minHitsPerSegment = cms.uint32(2),
     preClustering = cms.bool(True),
     dXclusBoxMax = cms.double(1.),
     dYclusBoxMax = cms.double(5.),
     preClusteringUseChaining = cms.bool(True),
                       dPhiChainBoxMax = cms.double(1.0),
                       # dPhiChainBoxMax = cms.double(0.02),
                       # dPhiChainBoxMax = cms.double(0.01),
                       # dPhiChainBoxMax = cms.double(0.005),
                       # dPhiChainBoxMax = cms.double(0.0025),
                       # dPhiChainBoxMax = cms.double(0.001),
                       # dPhiChainBoxMax = cms.double(0.),
     # dThetaChainBoxMax = cms.double(1.),
     dThetaChainBoxMax = cms.double(0.02),
     dRChainBoxMax = cms.double(0.5),
     maxRecHitsInCluster = cms.int32(6)
     )
)
