import FWCore.ParameterSet.Config as cms

me0RecHits2D = cms.EDProducer("ME0RecHit2DProducer",
    recAlgoConfig = cms.PSet(
        recOnlyMuons = cms.bool(False),
        recOnlyPrompt = cms.bool(False),
        nEtaPart = cms.int32(8),
        dEtaFixed = cms.bool(True),
    ),
    timeresolution = cms.double(1.0), 
    recAlgo = cms.string('ME0RecHit2DStandardAlgo'),
    me0DigiLabel = cms.InputTag("simMuonME0Digis2D"),
)
