import FWCore.ParameterSet.Config as cms

# masking is off by default - turn on with 'applyMasking'
# default masking uses DB, to use txt file, set maskFile or deadFile to the relative path of file
 
from RecoLocalMuon.GEMRecHit.gemRecHitsDef_cfi import *
gemRecHits = gemRecHitsDef.clone(
    applyMasking = True,
    # deadFile = cms.FileInPath("RecoLocalMuon/GEMRecHit/data/deadStrips_testbeam_eMapV1.txt"),
    deadFile = cms.FileInPath("RecoLocalMuon/GEMRecHit/data/deadStrips_testbeam_eMapV2.txt"),
    maskFile = cms.FileInPath("RecoLocalMuon/GEMRecHit/data/maskedStrips_testbeam.txt")
    )

from Configuration.Eras.Modifier_phase2_GEM_cff import phase2_GEM
phase2_GEM.toModify(gemRecHits, gemDigiLabel = "simMuonGEMDigis")
