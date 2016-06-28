import FWCore.ParameterSet.Config as cms

me0RecHits2Dto1D = cms.EDProducer("ME0RecHit2Dto1DProducer",
    me0RecHit2DLabel = cms.InputTag("me0RecHits2D"),
)
