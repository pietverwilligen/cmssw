import FWCore.ParameterSet.Config as cms

RU_ME0 = cms.PSet(
    doCollisions = cms.bool(True),
    chi2Norm_2D_ = cms.double(35),
    chi2_str = cms.double(50.0),
    chi2Max = cms.double(100.0),
    dPhiIntMax = cms.double(0.005),
    dPhiMax = cms.double(0.006),
    wideSeg = cms.double(3.0),
    minLayersApart = cms.int32(1),
    dRIntMax = cms.double(2.0),
    dRMax = cms.double(1.5)
)

ME0SegAlgoRU = cms.PSet(
    algo_name = cms.string('ME0SegAlgoRU'),
    algo_pset = cms.PSet(cms.PSet(RU_ME0))
)

