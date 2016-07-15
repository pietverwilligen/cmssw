import FWCore.ParameterSet.Config as cms

# Module to create simulated RPC digis.
simMuonRPCDigis = cms.EDProducer("RPCDigiProducer",
    Noise = cms.bool(True),
    digiModelConfig = cms.PSet(
        signalPropagationSpeed = cms.double(0.66),
        timingRPCOffset = cms.double(50.0),
        Frate = cms.double(1.0),
        printOutDigitizer = cms.bool(False),
        cosmics = cms.bool(False),
        deltatimeAdjacentStrip = cms.double(3.0),
        linkGateWidth = cms.double(20.0),
        Rate = cms.double(0.0),
        timeResolution = cms.double(2.5),
        averageClusterSize = cms.double(1.5),
        Gate = cms.double(25.0),
        averageEfficiency = cms.double(0.95),
        Nbxing = cms.int32(9),
        timeJitter = cms.double(1.0),
        digitizeElectrons = cms.bool(False), # False - do not digitize electron hits (they are included in bkg simulation configured with doBkgNoise)
    ),
    doBkgNoise = cms.bool(True), #False - no noise and bkg simulation
    Signal = cms.bool(True),
    mixLabel = cms.string('mix'),                                 
    InputCollection = cms.string('g4SimHitsMuonRPCHits'),
#    digiModel = cms.string('RPCSimAverageNoiseEffCls')
    digiModel = cms.string('RPCSimAsymmetricCls')
#the new digitizer is RPCSimAsymmetricCls
)

from Configuration.StandardSequences.Eras import eras
if eras.fastSim.isChosen():
    simMuonRPCDigis.InputCollection = 'MuonSimHitsMuonRPCHits'
eras.phase2_muon.toModify( simMuonRPCDigis, digiModel = cms.string('RPCSimAverageNoiseEffCls') )


