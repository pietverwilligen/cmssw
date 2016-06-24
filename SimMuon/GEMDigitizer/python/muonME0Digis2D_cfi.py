import FWCore.ParameterSet.Config as cms

# Module to create simulated ME0 Pre Reco digis.
simMuonME0Digis2D = cms.EDProducer("ME0Digi2DProducer",
    # initialSeed = cms.untracked.uint32(1234567890),
    # engineName = cms.untracked.string('TRandom3'),
    inputCollection = cms.string('g4SimHitsMuonME0Hits'),
    digi2DModelString = cms.string('Digi2DGaussian'),
    timeResolution = cms.double(0.001), # in ns
    phiResolution = cms.double(0.05),   # in cm average resolution along local x in case of no correlation
    etaResolution = cms.double(1.),     # in cm average resolution along local y in case of no correlation
    constantPhiSpatialResolution = cms.bool(False),
    averageEfficiency = cms.double(0.98),
    digitizeOnlyMuons = cms.bool(True),
    gaussianSmearing = cms.bool(True),
    # simulateIntrinsicNoise = cms.bool(False), # intrinsic noise --> not implemented
    # averageNoiseRate = cms.double(0.001),     # intrinsic noise --> not implemented
    simulateElectronBkg = cms.bool(True),       # True - will simulate electron background
    simulateNeutralBkg  = cms.bool(True),       # True - will simulate neutral (n+g)  background
    nEtaPart = cms.int32(8),
    dEtaFixed = cms.bool(True),                 # True - fixed dEta spacing of the partitions
    tDeadTime = cms.double(1.0),
    meanCls  = cms.double(0.0),                             
    minBunch = cms.int32(-5),                   # [x 25 ns], forms the readout window together with maxBunch,
    maxBunch = cms.int32(3),                    # we should think of shrinking this window ...
)
