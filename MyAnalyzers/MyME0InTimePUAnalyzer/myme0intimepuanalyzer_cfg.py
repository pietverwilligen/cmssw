import FWCore.ParameterSet.Config as cms

process = cms.Process("Analyzer")
process.load("FWCore.MessageService.MessageLogger_cfi")

# process.load('Configuration.Geometry.GeometryExtended2015MuonGEMDevReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
# CSCGeometry depends on alignment ==> necessary to provide GlobalPositionRecord
process.load("Alignment.CommonAlignmentProducer.FakeAlignmentSource_cfi") 
# process.load("Geometry.CSCGeometry.cscGeometry_cfi")

# process.load("Geometry.CSCGeometry.cscGeometry_cfi")
# process.load("Geometry.DTGeometry.dtGeometry_cfi")
# process.load("Geometry.GEMGeometry.gemGeometry_cfi")


process.maxEvents = cms.untracked.PSet( 
    input           = cms.untracked.int32(-1),
)

readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
                            fileNames = readFiles, 
                            secondaryFileNames = secFiles,
                            # eventsToProcess = cms.untracked.VEventRange('1:1157:115618'), # Run 1, Event 115618, LumiSection 1157
                            # eventsToProcess = cms.untracked.VEventRange('1:2010:200906'), # Run 1, Event 200906, LumiSection 2010
                            # eventsToProcess = cms.untracked.VEventRange('1:22779:540983'), # Run 1 LS 22779 Evt 540983
                            # eventsToProcess = cms.untracked.VEventRange('1:26043:618522'), # Run 1 LS 26043 Evt 618522
                            # eventsToProcess = cms.untracked.VEventRange('1:26044:618528'), # Run 1 LS 26044 Evt 618528
)
readFiles.extend([
        # 'file:DYToMuMu_M-20_HGCALGS_PU140_ME0_RECO_100ps_amandeep_116.root'
        # 'file:DYToMuMu_M-20_HGCALGS_PU140_ME0_RECO_100ps_amandeep_100.root'
        # 'file:DYToMuMu_M-20_HGCALGS_PU140_ME0_RECO_100ps_amandeep_100_v2.root'
        #################### Samples Piet V2 ###################################
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_1.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_10.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_101.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_102.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_103.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_104.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_105.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_107.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_108.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_v2_RECO/151021_081029/0000/out_reco_11.root',
        ########################################################################
        #################### Samples Piet Neutron Background ###################
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1006.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1037.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1107.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1348.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1464.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1632.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1744.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1793.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1800.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1839.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1897.root',
        # 'store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_250um_1cm_NeutrBkg_7ns_RECO/151114_144741/0001/out_reco_1932.root',
        ########################################################################
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_1.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_2.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_3.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_4.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_5.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_6.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_7.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_8.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_9.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_10.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_11.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_12.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_13.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_14.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_15.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_16.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_17.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_18.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_19.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_20.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_21.root', #problematic file
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_22.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_23.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_24.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_25.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_26.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_27.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_28.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_29.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_30.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_31.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_32.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_33.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_34.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_35.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_36.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_37.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_38.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_39.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_40.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_41.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_42.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_43.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_44.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_45.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_46.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_47.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_48.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_49.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_40.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_51.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_52.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_53.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_54.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_55.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_56.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_57.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_58.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_59.root',
        '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_100um_1cm_NeutrBkg_7ns_RECO/151114_144709/0000/out_reco_60.root',
        ########################################################################
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_1.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_2.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_3.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_4.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_5.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_6.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_7.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_8.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_9.root',
        # '/store/user/piet/ME0Segment_Time/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_GEN-SIM_v2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_1ns_1cm_1cm_NeutrBkg_7ns_RECO/151127_223249/0000/out_reco_10.root',
        ########################################################################
        # '/store/user/amartell/DYJetsToLL_M-50_13TeV-madgraph-pythia8/reDIGI-AODSIM_DYJetsToLL_M-50_13TeV_PHYS14_25_V1_HG/150713_131058/0000/RD_DYJetsToLL_M-50_13TeV_PHYS14_25_V1_HG_AODSIM_1.root'
       ]);
secFiles.extend([

        ]);

process.me0timeanalyzer = cms.EDAnalyzer('MyME0InTimePUAnalyzer',
                              # ----------------------------------------------------------------------
                              RootFileName       = cms.untracked.string("ME0InTimeOutOfTimePUtHistograms.root"),
                              InvestigateOnlyME0 = cms.untracked.bool(True),    # require at least one signal muon in 2.0 < | eta | < 2.8
                              # ----------------------------------------------------------------------
                              me0DetResX         = cms.untracked.double(1.0),    # [in cm] (single layer resolution)
                              me0DetResY         = cms.untracked.double(5.0),    # [in cm] (single layer resolution)
                              cscDetResX         = cms.untracked.double(0.0150), # [in cm] (chamber resolution :: 75-150um, take here 150um)
                              cscDetResY         = cms.untracked.double(5.0),    # [in cm]
                              dtDetResX          = cms.untracked.double(0.0400), # [in cm] (chamber resolution ::  75-125um in r-phi, take here 125um) ==> seems to fail to often ... take 0.4 mm now
                              dtDetResY          = cms.untracked.double(0.5000), # [in cm] (chamber resolution :: 150-400um in r-z  , take here 400um) ==> seems to fail to often ... take 5.0 mm now
                              nMatchedHitsME0Seg = cms.untracked.int32(3),
                              nMatchedHitsCSCSeg = cms.untracked.int32(3),
                              nMatchedHitsDTSeg  = cms.untracked.int32(6),
                              matchQualityME0    = cms.untracked.double(0.75),   # Percentage of matched hits (matched hits / total hits) >= 75% ==> 3/3 or 3/4, 4/5, 5/6
                              matchQualityReco   = cms.untracked.double(0.75),   # not using this for now ... problem with DTs 2B solved first
                              # ----------------------------------------------------------------------
                              printInfoHepMC           = cms.untracked.bool(False),
                              printInfoSignal          = cms.untracked.bool(False),
                              printInfoPU              = cms.untracked.bool(False),
                              printInfoVtx             = cms.untracked.bool(False),
                              printInfoAll             = cms.untracked.bool(False),
                              printInfoME0Match        = cms.untracked.bool(False),
                              printInfoMuonMatch       = cms.untracked.bool(False),
                              printInfoMuonMatchDetail = cms.untracked.bool(False),
                              printInfoInTimePU        = cms.untracked.bool(False),
                              # ----------------------------------------------------------------------

)

process.p = cms.Path(process.me0timeanalyzer)

        #################### Samples Cesare ####################################
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_1000_1_BIa.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_1001_1_NBL.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_1002_1_sj1.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_1003_1_sHM.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_1004_1_Lso.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_100_1_N6C.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_101_1_6A2.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_102_1_hZL.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_103_1_5xq.root',
        # '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_104_1_7Pu.root',
        #################### Samples David  ####################################
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_101_1_PUU.root'
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_103_1_wht.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_104_1_YDt.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_106_1_Mzi.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_107_1_kWi.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_109_1_Td4.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_111_1_uMD.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_113_1_FDR.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_117_1_AHh.root',
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_118_1_htl.root',
        ########################################################################
        # '/store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_896_2_3B7.root'
        ########################################################################
        #################### Samples Amandeep ##################################
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_78_1_ccv.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_68_1_o2i.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_69_1_6bu.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_79_1_JKc.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_55_1_vxk.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_20_1_mhD.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_27_1_xkl.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_2_1_m0w.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_30_1_C5Q.root',
        # '/store/user/amkalsi/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_MinBias_ME0_RECO_TimingResolution_100ps/234f53f4ed8c29359bbc4ab1d08753f2/out_reco_23_1_g3h.root',
        ########################################################################
        #################### Samples Piet V1 ###################################
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_2_1_vGt.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_24_1_GJ6.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_18_1_uME.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_5_1_799.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_9_1_ijp.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_40_1_sCu.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_35_1_VJM.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_21_1_H5S.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_30_1_2Xy.root',
        # '/store/user/piet/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_HGCALGS_PU140_100ps_1p5ns_RECO/a6c1ab73bd1959e4a7fbbca874362562/out_reco_3_1_6rT.root',
        ########################################################################


# only for first 3 files of Cesare's sample (4 step2 root files per step 3 root file)
#####################################################################################
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1293_3_vb2.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1294_3_SoG.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1295_3_hSz.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1296_3_8lt.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1297_3_i83.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1298_3_f44.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1299_3_qFl.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1300_2_1FR.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1301_3_97D.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1302_1_t0z.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1303_1_g6o.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_1304_3_kzy.root',
#####################################################################################
#### parent files for Davids Nash Special Event #####################################
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_2001_1_2d6.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_2002_1_kiy.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_2003_1_C7r.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_2Step_2/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-DIGI-RAW_CMSSW_6_2_0_SLHC23patch1_2023Scenario_2Step_GEMSH2/75266629395cd3363487f66c667220a2/step2_2004_1_lV2.root',
# '/store/group/upgrade/muon/RecoFolder/DYToMuMu_2023_3Step/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/calabria_DYToMuMu_GEN-SIM-RECO_CMSSW_6_2_0_SLHC23patch1_2023_3Step_OKFS3/2dad437730bcb898314ced9a1ae33ee0/step3_745_1_m7z.root',
#####################################################################################
