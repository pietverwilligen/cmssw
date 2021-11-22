// -*- C++ -*-
//
// Package:    MyTestBeamAnalyzer
// Class:      MyTestBeamAnalyzer
// 
/**\class MyTestBeamAnalyzer MyTestBeamAnalyzer.cc MyAnalyzers/MyTestBeamAnalyzer/src/MyTestBeamAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Piet Verwilligen,161 R-006,+41227676292,
//         Created:  Wed Oct 24 17:28:30 CEST 2012
//
//


// system include files
#include <memory>
#include <fstream>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>


// root include files
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TDirectoryFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include <DataFormats/GEMDigi/interface/GEMDigiCollection.h>
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
 
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include "DataFormats/Provenance/interface/Timestamp.h"


// #include "DQMServices/Core/interface/DQMStore.h"
// #include "DQMServices/Core/interface/MonitorElement.h"

#include <Geometry/GEMGeometry/interface/GEMGeometry.h>
#include <Geometry/DTGeometry/interface/DTGeometry.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>


#include <DataFormats/GEMDigi/interface/GEMDigiCollection.h>
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include <DataFormats/MuonDetId/interface/GEMDetId.h>
#include <DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>



//
// class declaration
//

class MyTestBeamAnalyzer : public edm::EDAnalyzer {
   public:
      explicit MyTestBeamAnalyzer(const edm::ParameterSet&);
      ~MyTestBeamAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void analyze(const edm::Event&, const edm::EventSetup&);

      // ----------member data ---------------------------
  edm::ESHandle<GEMGeometry> gemGeom;
  edm::EDGetTokenT<GEMRecHitCollection> GEMRecHit_Token;
  edm::EDGetTokenT<GEMDigiCollection>   GEMDigi_Token;

  bool dbg_test, dbg_info, dbg_dead, dbg_match;
  std::uint32_t mapVersion;
  std::string rootFileName;
  std::unique_ptr<TFile> outputfile;
  // std::vector< std::vector< int > > deadStripsTrackingGEM;

  /*
  std::unique_ptr<TH1F> BX_RecHits_GE1in_Plus,  BX_RecHits_GE1out_Plus,  BX_RecHits_GE1in_Minus,  BX_RecHits_GE1out_Minus;
  std::unique_ptr<TH1F> ST_RecHits_GE1in_Plus, ST_RecHits_GE1out_Plus, ST_RecHits_GE1in_Minus, ST_RecHits_GE1out_Minus;
  std::unique_ptr<TH1F> CL_RecHits_GE1in_Plus, CL_RecHits_GE1out_Plus, CL_RecHits_GE1in_Minus, CL_RecHits_GE1out_Minus;
  std::unique_ptr<TCanvas> BX_RecHits_GE1, ST_RecHits_GE1, CL_RecHits_GE1;

  // GEM Station 1 = GE1/1
  std::unique_ptr<TGraph>  GE1in_Plus_XY_All, GE1in_Minus_XY_All, GE1out_Plus_XY_All, GE1out_Minus_XY_All, GE1out_Plus_YZ_All, GE1out_Minus_YZ_All, GE1in_Plus_YZ_All, GE1in_Minus_YZ_All, GE1_Plus_YZ_All, GE1_Minus_YZ_All;
  std::unique_ptr<TCanvas> Canvas_GE1_Plus_XY, Canvas_GE1_Minus_XY, Canvas_GE1_Plus_YZ, Canvas_GE1_Minus_YZ;
  std::vector<double> x_n1i, y_n1i, z_n1i, r_n1i, x_n1o, y_n1o, z_n1o, r_n1o; // XYZR GE1 Minus in and out 
  std::vector<double> x_p1i, y_p1i, z_p1i, r_p1i, x_p1o, y_p1o, z_p1o, r_p1o; // XYZR GE1 Plus in and out
  std::vector<double> x_n1, y_n1, z_n1, r_n1, x_p1, y_p1, z_p1, r_p1;
  */

  // BX 
  std::unique_ptr<TH1F> BX_RecHits_GEM1X_Ch1iE1, BX_RecHits_GEM1Y_Ch1iE2, BX_RecHits_GEM2X_Ch1iE3, BX_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> BX_RecHits_GEM3X_Ch3iE1, BX_RecHits_GEM3Y_Ch3iE2, BX_RecHits_GEM4X_Ch3iE3, BX_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> BX_RecHits_GE21_iE13, BX_RecHits_GE21_iE14, BX_RecHits_GE21_iE15, BX_RecHits_GE21_iE16;
  std::unique_ptr<TH1F> BX_RecHits_ME0_iE1, BX_RecHits_ME0_iE2, BX_RecHits_ME0_iE3, BX_RecHits_ME0_iE4;
  std::unique_ptr<TH1F> BX_RecHits_ME0_iE5, BX_RecHits_ME0_iE6, BX_RecHits_ME0_iE7, BX_RecHits_ME0_iE8;

  // CLS
  std::unique_ptr<TH1F> CLS_RecHits_GEM1X_Ch1iE1, CLS_RecHits_GEM1Y_Ch1iE2, CLS_RecHits_GEM2X_Ch1iE3, CLS_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> CLS_RecHits_GEM3X_Ch3iE1, CLS_RecHits_GEM3Y_Ch3iE2, CLS_RecHits_GEM4X_Ch3iE3, CLS_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> CLS_RecHits_GE21_iE13, CLS_RecHits_GE21_iE14, CLS_RecHits_GE21_iE15, CLS_RecHits_GE21_iE16;
  std::unique_ptr<TH1F> CLS_RecHits_ME0_iE1, CLS_RecHits_ME0_iE2, CLS_RecHits_ME0_iE3, CLS_RecHits_ME0_iE4;
  std::unique_ptr<TH1F> CLS_RecHits_ME0_iE5, CLS_RecHits_ME0_iE6, CLS_RecHits_ME0_iE7, CLS_RecHits_ME0_iE8;

  // N-CLS / Event
  std::unique_ptr<TH1F> NHIT_RecHits_GEM1X_Ch1iE1, NHIT_RecHits_GEM1Y_Ch1iE2, NHIT_RecHits_GEM2X_Ch1iE3, NHIT_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> NHIT_RecHits_GEM3X_Ch3iE1, NHIT_RecHits_GEM3Y_Ch3iE2, NHIT_RecHits_GEM4X_Ch3iE3, NHIT_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> NHIT_RecHits_GE21_iE13, NHIT_RecHits_GE21_iE14, NHIT_RecHits_GE21_iE15, NHIT_RecHits_GE21_iE16;
  std::unique_ptr<TH1F> NHIT_RecHits_ME0_iE1, NHIT_RecHits_ME0_iE2, NHIT_RecHits_ME0_iE3, NHIT_RecHits_ME0_iE4;
  std::unique_ptr<TH1F> NHIT_RecHits_ME0_iE5, NHIT_RecHits_ME0_iE6, NHIT_RecHits_ME0_iE7, NHIT_RecHits_ME0_iE8;

  // MY CORR POS
  std::unique_ptr<TH1F> MYPOS_ALL_RecHits_GEM1X_Ch1iE1, MYPOS_ALL_RecHits_GEM1Y_Ch1iE2, MYPOS_ALL_RecHits_GEM2X_Ch1iE3, MYPOS_ALL_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> MYPOS_ALL_RecHits_GEM3X_Ch3iE1, MYPOS_ALL_RecHits_GEM3Y_Ch3iE2, MYPOS_ALL_RecHits_GEM4X_Ch3iE3, MYPOS_ALL_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> MYPOS_SH_RecHits_GEM1X_Ch1iE1,  MYPOS_SH_RecHits_GEM1Y_Ch1iE2,  MYPOS_SH_RecHits_GEM2X_Ch1iE3,  MYPOS_SH_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> MYPOS_SH_RecHits_GEM3X_Ch3iE1,  MYPOS_SH_RecHits_GEM3Y_Ch3iE2,  MYPOS_SH_RecHits_GEM4X_Ch3iE3,  MYPOS_SH_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> MYPOS_CL1_RecHits_GEM1X_Ch1iE1, MYPOS_CL1_RecHits_GEM1Y_Ch1iE2, MYPOS_CL1_RecHits_GEM2X_Ch1iE3, MYPOS_CL1_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> MYPOS_CL1_RecHits_GEM3X_Ch3iE1, MYPOS_CL1_RecHits_GEM3Y_Ch3iE2, MYPOS_CL1_RecHits_GEM4X_Ch3iE3, MYPOS_CL1_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> MYPOS_CL2_RecHits_GEM1X_Ch1iE1, MYPOS_CL2_RecHits_GEM1Y_Ch1iE2, MYPOS_CL2_RecHits_GEM2X_Ch1iE3, MYPOS_CL2_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> MYPOS_CL2_RecHits_GEM3X_Ch3iE1, MYPOS_CL2_RecHits_GEM3Y_Ch3iE2, MYPOS_CL2_RecHits_GEM4X_Ch3iE3, MYPOS_CL2_RecHits_GEM4Y_Ch3iE4; 

  // LOCPOS
  std::unique_ptr<TH1F> POS_RecHits_GEM1X_Ch1iE1, POS_RecHits_GEM1Y_Ch1iE2, POS_RecHits_GEM2X_Ch1iE3, POS_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> POS_RecHits_GEM3X_Ch3iE1, POS_RecHits_GEM3Y_Ch3iE2, POS_RecHits_GEM4X_Ch3iE3, POS_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> POS_RecHits_GE21_iE13, POS_RecHits_GE21_iE14, POS_RecHits_GE21_iE15, POS_RecHits_GE21_iE16;
  std::unique_ptr<TH1F> POS_RecHits_ME0_iE1, POS_RecHits_ME0_iE2, POS_RecHits_ME0_iE3, POS_RecHits_ME0_iE4;
  std::unique_ptr<TH1F> POS_RecHits_ME0_iE5, POS_RecHits_ME0_iE6, POS_RecHits_ME0_iE7, POS_RecHits_ME0_iE8;

  // STRIPS
  std::unique_ptr<TH1F> STR_RecHits_GEM1X_Ch1iE1, STR_RecHits_GEM1Y_Ch1iE2, STR_RecHits_GEM2X_Ch1iE3, STR_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> STR_RecHits_GEM3X_Ch3iE1, STR_RecHits_GEM3Y_Ch3iE2, STR_RecHits_GEM4X_Ch3iE3, STR_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> STR_CL2_RecHits_GEM1X_Ch1iE1, STR_CL2_RecHits_GEM1Y_Ch1iE2, STR_CL2_RecHits_GEM2X_Ch1iE3, STR_CL2_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> STR_CL2_RecHits_GEM3X_Ch3iE1, STR_CL2_RecHits_GEM3Y_Ch3iE2, STR_CL2_RecHits_GEM4X_Ch3iE3, STR_CL2_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> STR_RecHits_GE21_iE13, STR_RecHits_GE21_iE14, STR_RecHits_GE21_iE15, STR_RecHits_GE21_iE16;
  std::unique_ptr<TH1F> STR_RecHits_ME0_iE1, STR_RecHits_ME0_iE2, STR_RecHits_ME0_iE3, STR_RecHits_ME0_iE4;
  std::unique_ptr<TH1F> STR_RecHits_ME0_iE5, STR_RecHits_ME0_iE6, STR_RecHits_ME0_iE7, STR_RecHits_ME0_iE8;
  // ME0 DIGIS
  std::unique_ptr<TH1F> DIG_RecHits_ME0_iE1, DIG_RecHits_ME0_iE2, DIG_RecHits_ME0_iE3, DIG_RecHits_ME0_iE4;
  std::unique_ptr<TH1F> DIG_RecHits_ME0_iE5, DIG_RecHits_ME0_iE6, DIG_RecHits_ME0_iE7, DIG_RecHits_ME0_iE8;



  // 2D PROFILE
  std::unique_ptr<TH2F> PRF2D_ALL_RecHits_GEM1XY_Ch1iE1iE2, PRF2D_ALL_RecHits_GEM2XY_Ch1iE3iE4;
  std::unique_ptr<TH2F> PRF2D_ALL_RecHits_GEM3XY_Ch3iE1iE2, PRF2D_ALL_RecHits_GEM4XY_Ch3iE3iE4;
  std::unique_ptr<TH2F> PRF2D_SH_RecHits_GEM1XY_Ch1iE1iE2, PRF2D_SH_RecHits_GEM2XY_Ch1iE3iE4;
  std::unique_ptr<TH2F> PRF2D_SH_RecHits_GEM3XY_Ch3iE1iE2, PRF2D_SH_RecHits_GEM4XY_Ch3iE3iE4;
  std::unique_ptr<TH2F> PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2, PRF2D_RH_RecHits_GEM2XY_Ch1iE3iE4;
  std::unique_ptr<TH2F> PRF2D_RH_RecHits_GEM3XY_Ch3iE1iE2, PRF2D_RH_RecHits_GEM4XY_Ch3iE3iE4;

  std::unique_ptr<TH2F> PRF2D_ALL_RecHits_GE21, PRF2D_ALL_RecHits_ME0;
  std::unique_ptr<TH2F> eMapDebug_ME0_iE1, eMapDebug_ME0_iE2, eMapDebug_ME0_iE3, eMapDebug_ME0_iE4;
  std::unique_ptr<TH2F> eMapDebug_ME0_iE5, eMapDebug_ME0_iE6, eMapDebug_ME0_iE7, eMapDebug_ME0_iE8;

  // EFF
  std::unique_ptr<TH1F> TOT_RecHits_GEM, TOT_RecHits_GE21, TOT_RecHits_ME0;
  std::unique_ptr<TH1F> EFF_RecHits_GEM, EFF_RecHits_GE21, EFF_RecHits_ME0;

  // CLEAN HITS
  std::unique_ptr<TH1F> SH_CLS_RecHits_GEM1X_Ch1iE1, SH_CLS_RecHits_GEM1Y_Ch1iE2, SH_CLS_RecHits_GEM2X_Ch1iE3, SH_CLS_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> SH_CLS_RecHits_GEM3X_Ch3iE1, SH_CLS_RecHits_GEM3Y_Ch3iE2, SH_CLS_RecHits_GEM4X_Ch3iE3, SH_CLS_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> SH_POS_RecHits_GEM1X_Ch1iE1, SH_POS_RecHits_GEM1Y_Ch1iE2, SH_POS_RecHits_GEM2X_Ch1iE3, SH_POS_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> SH_POS_RecHits_GEM3X_Ch3iE1, SH_POS_RecHits_GEM3Y_Ch3iE2, SH_POS_RecHits_GEM4X_Ch3iE3, SH_POS_RecHits_GEM4Y_Ch3iE4; 

  // RECOVER HITS --> for distinguished clusters ...
  std::unique_ptr<TH1F> RH_CLS_RecHits_GEM1X_Ch1iE1, RH_CLS_RecHits_GEM1Y_Ch1iE2, RH_CLS_RecHits_GEM2X_Ch1iE3, RH_CLS_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> RH_CLS_RecHits_GEM3X_Ch3iE1, RH_CLS_RecHits_GEM3Y_Ch3iE2, RH_CLS_RecHits_GEM4X_Ch3iE3, RH_CLS_RecHits_GEM4Y_Ch3iE4; 
  std::unique_ptr<TH1F> RH_POS_RecHits_GEM1X_Ch1iE1, RH_POS_RecHits_GEM1Y_Ch1iE2, RH_POS_RecHits_GEM2X_Ch1iE3, RH_POS_RecHits_GEM2Y_Ch1iE4;
  std::unique_ptr<TH1F> RH_POS_RecHits_GEM3X_Ch3iE1, RH_POS_RecHits_GEM3Y_Ch3iE2, RH_POS_RecHits_GEM4X_Ch3iE3, RH_POS_RecHits_GEM4Y_Ch3iE4; 

  // CLUSTERSIZE matching ... 
  std::unique_ptr<TH2F> CORR_SH_CLS_GEM1XY, CORR_SH_CLS_GEM2XY, CORR_SH_CLS_GEM3XY, CORR_SH_CLS_GEM4XY;
  std::unique_ptr<TH2F> CORR_2H_CLS_GEM1XY, CORR_2H_CLS_GEM2XY, CORR_2H_CLS_GEM3XY, CORR_2H_CLS_GEM4XY;
  std::unique_ptr<TH2F> CORR_3H_CLS_GEM1XY, CORR_3H_CLS_GEM2XY, CORR_3H_CLS_GEM3XY, CORR_3H_CLS_GEM4XY;
  std::unique_ptr<TH2F> CORR_ALL_CLS_GEM1XY, CORR_ALL_CLS_GEM2XY, CORR_ALL_CLS_GEM3XY, CORR_ALL_CLS_GEM4XY;

  std::unique_ptr<TH2F> CORR_ALL_NHIT_GEM1XY, CORR_ALL_NHIT_GEM2XY, CORR_ALL_NHIT_GEM3XY, CORR_ALL_NHIT_GEM4XY;
  std::unique_ptr<TH1F> CORR_EVT_GEMXY_v1;
  std::unique_ptr<TH2F> CORR_EVT_GEMXY_v2;

  // 10X20
  std::unique_ptr<TH1F> BX_RecHits_10X20_Ch3iE5, CLS_RecHits_10X20_Ch3iE5, NHIT_RecHits_10X20_Ch3iE5, POS_RecHits_10X20_Ch3iE5, STR_RecHits_10X20_Ch3iE5;       
  std::unique_ptr<TH1F> MYPOS_ALL_RecHits_10X20_Ch3iE5, MYPOS_SH_RecHits_10X20_Ch3iE5, MYPOS_CL1_RecHits_10X20_Ch3iE5; 


  

  int nEvents;
};

//
// constants, enums and typedefs
//

// Probably all strips should run from 0-255 or 0-357 or 0-383 .... !!! check this !!!
// for now add one additional bin to catch all ;-)

int n_bx  = 11;   double n1_bx  = -5.5,     n2_bx  = 5.5;
int n_cl  = 25;   double n1_cl  = 0.5,      n2_cl  = 25.5;
int n_st  = 359;  double n1_st  = -0.5,     n2_st  = 358.5;    // strips for 10X10 Trackers
int n_ps  = 715;  double n1_ps  = -44.6875, n2_ps  = 44.6875;  // position for Trackers in mm (-44.750 + 0.0625 (=quart-strip))
int m_st  = 385;  double m1_st  = -0.5,     m2_st  = 384.5;    // strips for GE21
int m_ps  = 630;  double m1_ps  = 0.0,      m2_ps  = 315;      // position for GE21; in mm --> #bins to be improved
int o_st  = 385;  double o1_st  = -0.5,     o2_st  = 384.5;    // strips for ME0
int o_ps  = 250;  double o1_ps  = -250,     o2_ps  = 250;      // position for ME0; in mm
int p_st  = 512;  double p1_st  = 0.5,      p2_st  = 512.5;    // strips for 10X20
int p_ps  = 1023; double p1_ps  = -102.300, p2_ps  = 102.300;  // position for 10X20; in mm (-51.2 + 0.1 (=quart-strip))
int q_st  = 385;  double q1_st  = -0.5,     q2_st  = 384.5;    // digis for ME0
// 10x10 GEM Tracking Chambers
// 358 strips of 250um pitch
// range: -44.75 mm to +44.75 mm
// for best binning: bins of 125um => 716 bins
// avoid 1/4 of outermost strips as no position value will be assigned
// => 715 bins starting -44.6875 to +44.6875 (44.750 - 0.0625)
// prefer to work in mm instead of cm ...

//
// static data member definitions
//

//
// constructors and destructor
//
MyTestBeamAnalyzer::MyTestBeamAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  GEMRecHit_Token = consumes<GEMRecHitCollection>(edm::InputTag("gemRecHits"));
  GEMDigi_Token   = consumes<GEMDigiCollection>(edm::InputTag("muonGEMDigis"));
  mapVersion      = iConfig.getUntrackedParameter<std::uint32_t>("MapVersion");
  dbg_test        = iConfig.getUntrackedParameter<bool>("DebugTest");
  dbg_info        = iConfig.getUntrackedParameter<bool>("DebugInfo");
  dbg_dead        = iConfig.getUntrackedParameter<bool>("DebugDead");
  dbg_match       = iConfig.getUntrackedParameter<bool>("DebugMatching");
  rootFileName    = iConfig.getUntrackedParameter<std::string>("RootFileName");
  outputfile.reset(TFile::Open(rootFileName.c_str(), "RECREATE"));


  // deadStripsTrackingGEM
  // std::vector<int> deadStrips_GEM1X = {140,211,240}; deadStripsTrackingGEM.push_back(deadStrips_GEM1X);
  // std::vector<int> deadStrips_GEM1Y = {};            deadStripsTrackingGEM.push_back(deadStrips_GEM1Y);
  // std::vector<int> deadStrips_GEM2X = {7,236};       deadStripsTrackingGEM.push_back(deadStrips_GEM2X);
  // std::vector<int> deadStrips_GEM2Y = {32};          deadStripsTrackingGEM.push_back(deadStrips_GEM2Y);
  // std::vector<int> deadStrips_GEM3X = {};            deadStripsTrackingGEM.push_back(deadStrips_GEM3X);
  // std::vector<int> deadStrips_GEM3Y = {};            deadStripsTrackingGEM.push_back(deadStrips_GEM3Y);
  // std::vector<int> deadStrips_GEM4X = {};            deadStripsTrackingGEM.push_back(deadStrips_GEM4X);
  // std::vector<int> deadStrips_GEM4Y = {};            deadStripsTrackingGEM.push_back(deadStrips_GEM4Y);



  nEvents = 0;
  
  // GEM TRACKERS
  BX_RecHits_GEM1X_Ch1iE1        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM1X_Ch1iE1",         "BX_RecHits_GEM1X_Ch1iE1",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GEM1Y_Ch1iE2        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM1Y_Ch1iE2",         "BX_RecHits_GEM1Y_Ch1iE2",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GEM2X_Ch1iE3        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM2X_Ch1iE3",         "BX_RecHits_GEM2X_Ch1iE3",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GEM2Y_Ch1iE4        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM2Y_Ch1iE4",         "BX_RecHits_GEM2Y_Ch1iE4",         n_bx, n1_bx, n2_bx));

  BX_RecHits_GEM3X_Ch3iE1        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM3X_Ch3iE1",         "BX_RecHits_GEM3X_Ch3iE1",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GEM3Y_Ch3iE2        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM3Y_Ch3iE2",         "BX_RecHits_GEM3Y_Ch3iE2",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GEM4X_Ch3iE3        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM4X_Ch3iE3",         "BX_RecHits_GEM4X_Ch3iE3",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GEM4Y_Ch3iE4        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GEM4Y_Ch3iE4",         "BX_RecHits_GEM4Y_Ch3iE4",         n_bx, n1_bx, n2_bx));


  CLS_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM1X_Ch1iE1",       "CLS_RecHits_GEM1X_Ch1iE1",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM1Y_Ch1iE2",       "CLS_RecHits_GEM1Y_Ch1iE2",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM2X_Ch1iE3",       "CLS_RecHits_GEM2X_Ch1iE3",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM2Y_Ch1iE4",       "CLS_RecHits_GEM2Y_Ch1iE4",       n_cl, n1_cl, n2_cl));

  CLS_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM3X_Ch3iE1",       "CLS_RecHits_GEM3X_Ch3iE1",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM3Y_Ch3iE2",       "CLS_RecHits_GEM3Y_Ch3iE2",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM4X_Ch3iE3",       "CLS_RecHits_GEM4X_Ch3iE3",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GEM4Y_Ch3iE4",       "CLS_RecHits_GEM4Y_Ch3iE4",       n_cl, n1_cl, n2_cl));


  NHIT_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM1X_Ch1iE1",       "NHIT_RecHits_GEM1X_Ch1iE1",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM1Y_Ch1iE2",       "NHIT_RecHits_GEM1Y_Ch1iE2",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM2X_Ch1iE3",       "NHIT_RecHits_GEM2X_Ch1iE3",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM2Y_Ch1iE4",       "NHIT_RecHits_GEM2Y_Ch1iE4",       n_cl, n1_cl, n2_cl));

  NHIT_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM3X_Ch3iE1",       "NHIT_RecHits_GEM3X_Ch3iE1",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM3Y_Ch3iE2",       "NHIT_RecHits_GEM3Y_Ch3iE2",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM4X_Ch3iE3",       "NHIT_RecHits_GEM4X_Ch3iE3",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GEM4Y_Ch3iE4",       "NHIT_RecHits_GEM4Y_Ch3iE4",       n_cl, n1_cl, n2_cl));


  MYPOS_ALL_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM1X_Ch1iE1",       "MYPOS_ALL_RecHits_GEM1X_Ch1iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_ALL_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM1Y_Ch1iE2",       "MYPOS_ALL_RecHits_GEM1Y_Ch1iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_ALL_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM2X_Ch1iE3",       "MYPOS_ALL_RecHits_GEM2X_Ch1iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_ALL_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM2Y_Ch1iE4",       "MYPOS_ALL_RecHits_GEM2Y_Ch1iE4",       n_ps, n1_ps, n2_ps));

  MYPOS_ALL_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM3X_Ch3iE1",       "MYPOS_ALL_RecHits_GEM3X_Ch3iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_ALL_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM3Y_Ch3iE2",       "MYPOS_ALL_RecHits_GEM3Y_Ch3iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_ALL_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM4X_Ch3iE3",       "MYPOS_ALL_RecHits_GEM4X_Ch3iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_ALL_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_GEM4Y_Ch3iE4",       "MYPOS_ALL_RecHits_GEM4Y_Ch3iE4",       n_ps, n1_ps, n2_ps));

  MYPOS_SH_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM1X_Ch1iE1",       "MYPOS_SH_RecHits_GEM1X_Ch1iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_SH_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM1Y_Ch1iE2",       "MYPOS_SH_RecHits_GEM1Y_Ch1iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_SH_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM2X_Ch1iE3",       "MYPOS_SH_RecHits_GEM2X_Ch1iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_SH_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM2Y_Ch1iE4",       "MYPOS_SH_RecHits_GEM2Y_Ch1iE4",       n_ps, n1_ps, n2_ps));

  MYPOS_SH_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM3X_Ch3iE1",       "MYPOS_SH_RecHits_GEM3X_Ch3iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_SH_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM3Y_Ch3iE2",       "MYPOS_SH_RecHits_GEM3Y_Ch3iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_SH_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM4X_Ch3iE3",       "MYPOS_SH_RecHits_GEM4X_Ch3iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_SH_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_GEM4Y_Ch3iE4",       "MYPOS_SH_RecHits_GEM4Y_Ch3iE4",       n_ps, n1_ps, n2_ps));

  MYPOS_CL1_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM3X_Ch3iE1",       "MYPOS_CL1_RecHits_GEM3X_Ch3iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_CL1_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM3Y_Ch3iE2",       "MYPOS_CL1_RecHits_GEM3Y_Ch3iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_CL1_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM4X_Ch3iE3",       "MYPOS_CL1_RecHits_GEM4X_Ch3iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_CL1_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM4Y_Ch3iE4",       "MYPOS_CL1_RecHits_GEM4Y_Ch3iE4",       n_ps, n1_ps, n2_ps));

  MYPOS_CL1_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM1X_Ch1iE1",       "MYPOS_CL1_RecHits_GEM1X_Ch1iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_CL1_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM1Y_Ch1iE2",       "MYPOS_CL1_RecHits_GEM1Y_Ch1iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_CL1_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM2X_Ch1iE3",       "MYPOS_CL1_RecHits_GEM2X_Ch1iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_CL1_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_GEM2Y_Ch1iE4",       "MYPOS_CL1_RecHits_GEM2Y_Ch1iE4",       n_ps, n1_ps, n2_ps));

  MYPOS_CL2_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM3X_Ch3iE1",       "MYPOS_CL2_RecHits_GEM3X_Ch3iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_CL2_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM3Y_Ch3iE2",       "MYPOS_CL2_RecHits_GEM3Y_Ch3iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_CL2_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM4X_Ch3iE3",       "MYPOS_CL2_RecHits_GEM4X_Ch3iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_CL2_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM4Y_Ch3iE4",       "MYPOS_CL2_RecHits_GEM4Y_Ch3iE4",       n_ps, n1_ps, n2_ps));

  MYPOS_CL2_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM1X_Ch1iE1",       "MYPOS_CL2_RecHits_GEM1X_Ch1iE1",       n_ps, n1_ps, n2_ps));
  MYPOS_CL2_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM1Y_Ch1iE2",       "MYPOS_CL2_RecHits_GEM1Y_Ch1iE2",       n_ps, n1_ps, n2_ps));
  MYPOS_CL2_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM2X_Ch1iE3",       "MYPOS_CL2_RecHits_GEM2X_Ch1iE3",       n_ps, n1_ps, n2_ps));
  MYPOS_CL2_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL2_RecHits_GEM2Y_Ch1iE4",       "MYPOS_CL2_RecHits_GEM2Y_Ch1iE4",       n_ps, n1_ps, n2_ps));



  POS_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM1X_Ch1iE1",       "POS_RecHits_GEM1X_Ch1iE1",       n_ps, n1_ps, n2_ps));
  POS_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM1Y_Ch1iE2",       "POS_RecHits_GEM1Y_Ch1iE2",       n_ps, n1_ps, n2_ps));
  POS_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM2X_Ch1iE3",       "POS_RecHits_GEM2X_Ch1iE3",       n_ps, n1_ps, n2_ps));
  POS_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM2Y_Ch1iE4",       "POS_RecHits_GEM2Y_Ch1iE4",       n_ps, n1_ps, n2_ps));

  POS_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM3X_Ch3iE1",       "POS_RecHits_GEM3X_Ch3iE1",       n_ps, n1_ps, n2_ps));
  POS_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM3Y_Ch3iE2",       "POS_RecHits_GEM3Y_Ch3iE2",       n_ps, n1_ps, n2_ps));
  POS_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM4X_Ch3iE3",       "POS_RecHits_GEM4X_Ch3iE3",       n_ps, n1_ps, n2_ps));
  POS_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GEM4Y_Ch3iE4",       "POS_RecHits_GEM4Y_Ch3iE4",       n_ps, n1_ps, n2_ps));


  STR_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM1X_Ch1iE1",       "STR_RecHits_GEM1X_Ch1iE1",       n_st, n1_st, n2_st));
  STR_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM1Y_Ch1iE2",       "STR_RecHits_GEM1Y_Ch1iE2",       n_st, n1_st, n2_st));
  STR_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM2X_Ch1iE3",       "STR_RecHits_GEM2X_Ch1iE3",       n_st, n1_st, n2_st));
  STR_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM2Y_Ch1iE4",       "STR_RecHits_GEM2Y_Ch1iE4",       n_st, n1_st, n2_st));

  STR_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM3X_Ch3iE1",       "STR_RecHits_GEM3X_Ch3iE1",       n_st, n1_st, n2_st));
  STR_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM3Y_Ch3iE2",       "STR_RecHits_GEM3Y_Ch3iE2",       n_st, n1_st, n2_st));
  STR_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM4X_Ch3iE3",       "STR_RecHits_GEM4X_Ch3iE3",       n_st, n1_st, n2_st));
  STR_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GEM4Y_Ch3iE4",       "STR_RecHits_GEM4Y_Ch3iE4",       n_st, n1_st, n2_st));


  STR_CL2_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM1X_Ch1iE1",       "STR_CL2_RecHits_GEM1X_Ch1iE1",       n_st, n1_st, n2_st));
  STR_CL2_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM1Y_Ch1iE2",       "STR_CL2_RecHits_GEM1Y_Ch1iE2",       n_st, n1_st, n2_st));
  STR_CL2_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM2X_Ch1iE3",       "STR_CL2_RecHits_GEM2X_Ch1iE3",       n_st, n1_st, n2_st));
  STR_CL2_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM2Y_Ch1iE4",       "STR_CL2_RecHits_GEM2Y_Ch1iE4",       n_st, n1_st, n2_st));

  STR_CL2_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM3X_Ch3iE1",       "STR_CL2_RecHits_GEM3X_Ch3iE1",       n_st, n1_st, n2_st));
  STR_CL2_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM3Y_Ch3iE2",       "STR_CL2_RecHits_GEM3Y_Ch3iE2",       n_st, n1_st, n2_st));
  STR_CL2_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM4X_Ch3iE3",       "STR_CL2_RecHits_GEM4X_Ch3iE3",       n_st, n1_st, n2_st));
  STR_CL2_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("STR_CL2_RecHits_GEM4Y_Ch3iE4",       "STR_CL2_RecHits_GEM4Y_Ch3iE4",       n_st, n1_st, n2_st));



  PRF2D_ALL_RecHits_GEM1XY_Ch1iE1iE2 = std::unique_ptr<TH2F>(new TH2F("PRF2D_ALL_RecHits_GEM1XY_Ch1iE1iE2", "PRF2D_ALL_RecHits_GEM1XY_Ch1iE1iE2", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_ALL_RecHits_GEM2XY_Ch1iE3iE4 = std::unique_ptr<TH2F>(new TH2F("PRF2D_ALL_RecHits_GEM2XY_Ch1iE3iE4", "PRF2D_ALL_RecHits_GEM2XY_Ch1iE3iE4", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_ALL_RecHits_GEM3XY_Ch3iE1iE2 = std::unique_ptr<TH2F>(new TH2F("PRF2D_ALL_RecHits_GEM3XY_Ch3iE1iE2", "PRF2D_ALL_RecHits_GEM3XY_Ch3iE1iE2", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_ALL_RecHits_GEM4XY_Ch3iE3iE4 = std::unique_ptr<TH2F>(new TH2F("PRF2D_ALL_RecHits_GEM4XY_Ch3iE3iE4", "PRF2D_ALL_RecHits_GEM4XY_Ch3iE3iE4", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));

  PRF2D_SH_RecHits_GEM1XY_Ch1iE1iE2 = std::unique_ptr<TH2F>(new TH2F("PRF2D_SH_RecHits_GEM1XY_Ch1iE1iE2", "PRF2D_SH_RecHits_GEM1XY_Ch1iE1iE2", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_SH_RecHits_GEM2XY_Ch1iE3iE4 = std::unique_ptr<TH2F>(new TH2F("PRF2D_SH_RecHits_GEM2XY_Ch1iE3iE4", "PRF2D_SH_RecHits_GEM2XY_Ch1iE3iE4", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_SH_RecHits_GEM3XY_Ch3iE1iE2 = std::unique_ptr<TH2F>(new TH2F("PRF2D_SH_RecHits_GEM3XY_Ch3iE1iE2", "PRF2D_SH_RecHits_GEM3XY_Ch3iE1iE2", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_SH_RecHits_GEM4XY_Ch3iE3iE4 = std::unique_ptr<TH2F>(new TH2F("PRF2D_SH_RecHits_GEM4XY_Ch3iE3iE4", "PRF2D_SH_RecHits_GEM4XY_Ch3iE3iE4", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));

  PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2 = std::unique_ptr<TH2F>(new TH2F("PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2", "PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_RH_RecHits_GEM2XY_Ch1iE3iE4 = std::unique_ptr<TH2F>(new TH2F("PRF2D_RH_RecHits_GEM2XY_Ch1iE3iE4", "PRF2D_RH_RecHits_GEM2XY_Ch1iE3iE4", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_RH_RecHits_GEM3XY_Ch3iE1iE2 = std::unique_ptr<TH2F>(new TH2F("PRF2D_RH_RecHits_GEM3XY_Ch3iE1iE2", "PRF2D_RH_RecHits_GEM3XY_Ch3iE1iE2", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));
  PRF2D_RH_RecHits_GEM4XY_Ch3iE3iE4 = std::unique_ptr<TH2F>(new TH2F("PRF2D_RH_RecHits_GEM4XY_Ch3iE3iE4", "PRF2D_RH_RecHits_GEM4XY_Ch3iE3iE4", n_ps, n1_ps, n2_ps, n_ps, n1_ps, n2_ps));

  PRF2D_ALL_RecHits_GE21 = std::unique_ptr<TH2F>(new TH2F("PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2", "PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2", m_ps, m1_ps, m2_ps, n_ps, n1_ps, n2_ps)); // to-be defined better 2nd coordinate
  PRF2D_ALL_RecHits_ME0 = std::unique_ptr<TH2F>(new TH2F("PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2", "PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2",  n_ps, n1_ps, n2_ps, o_ps, o1_ps, o2_ps)); // to-be defined better 2nd coordinate

  eMapDebug_ME0_iE1 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE1", "eMapDebug_ME0_iE1", 384, 0.5, 384.5, 384, 0.5, 384.5));
  eMapDebug_ME0_iE2 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE2", "eMapDebug_ME0_iE2", 384, 0.5, 384.5, 384, 0.5, 384.5));
  eMapDebug_ME0_iE3 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE3", "eMapDebug_ME0_iE3", 384, 0.5, 384.5, 384, 0.5, 384.5));
  eMapDebug_ME0_iE4 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE4", "eMapDebug_ME0_iE4", 384, 0.5, 384.5, 384, 0.5, 384.5));
  eMapDebug_ME0_iE5 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE5", "eMapDebug_ME0_iE5", 384, 0.5, 384.5, 384, 0.5, 384.5));
  eMapDebug_ME0_iE6 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE6", "eMapDebug_ME0_iE6", 384, 0.5, 384.5, 384, 0.5, 384.5));
  eMapDebug_ME0_iE7 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE7", "eMapDebug_ME0_iE7", 384, 0.5, 384.5, 384, 0.5, 384.5));
  eMapDebug_ME0_iE8 = std::unique_ptr<TH2F>(new TH2F("eMapDebug_ME0_iE8", "eMapDebug_ME0_iE8", 384, 0.5, 384.5, 384, 0.5, 384.5));

  TOT_RecHits_GEM                = std::unique_ptr<TH1F>(new TH1F("TOT_RecHits_GEM",                "TOT_RecHits_GEM",                13,   0.5,    13.5));
  EFF_RecHits_GEM                = std::unique_ptr<TH1F>(new TH1F("EFF_RecHits_GEM",                "EFF_RecHits_GEM",                13,   0.5,    13.5));

  // Clean Hits
  SH_CLS_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM1X_Ch1iE1",       "SH_CLS_RecHits_GEM1X_Ch1iE1",       n_cl, n1_cl, n2_cl));
  SH_CLS_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM1Y_Ch1iE2",       "SH_CLS_RecHits_GEM1Y_Ch1iE2",       n_cl, n1_cl, n2_cl));
  SH_CLS_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM2X_Ch1iE3",       "SH_CLS_RecHits_GEM2X_Ch1iE3",       n_cl, n1_cl, n2_cl));
  SH_CLS_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM2Y_Ch1iE4",       "SH_CLS_RecHits_GEM2Y_Ch1iE4",       n_cl, n1_cl, n2_cl));

  SH_CLS_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM3X_Ch3iE1",       "SH_CLS_RecHits_GEM3X_Ch3iE1",       n_cl, n1_cl, n2_cl));
  SH_CLS_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM3Y_Ch3iE2",       "SH_CLS_RecHits_GEM3Y_Ch3iE2",       n_cl, n1_cl, n2_cl));
  SH_CLS_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM4X_Ch3iE3",       "SH_CLS_RecHits_GEM4X_Ch3iE3",       n_cl, n1_cl, n2_cl));
  SH_CLS_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("SH_CLS_RecHits_GEM4Y_Ch3iE4",       "SH_CLS_RecHits_GEM4Y_Ch3iE4",       n_cl, n1_cl, n2_cl));

  SH_POS_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM1X_Ch1iE1",       "SH_POS_RecHits_GEM1X_Ch1iE1",       n_ps, n1_ps, n2_ps));
  SH_POS_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM1Y_Ch1iE2",       "SH_POS_RecHits_GEM1Y_Ch1iE2",       n_ps, n1_ps, n2_ps));
  SH_POS_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM2X_Ch1iE3",       "SH_POS_RecHits_GEM2X_Ch1iE3",       n_ps, n1_ps, n2_ps));
  SH_POS_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM2Y_Ch1iE4",       "SH_POS_RecHits_GEM2Y_Ch1iE4",       n_ps, n1_ps, n2_ps));

  SH_POS_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM3X_Ch3iE1",       "SH_POS_RecHits_GEM3X_Ch3iE1",       n_ps, n1_ps, n2_ps));
  SH_POS_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM3Y_Ch3iE2",       "SH_POS_RecHits_GEM3Y_Ch3iE2",       n_ps, n1_ps, n2_ps));
  SH_POS_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM4X_Ch3iE3",       "SH_POS_RecHits_GEM4X_Ch3iE3",       n_ps, n1_ps, n2_ps));
  SH_POS_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("SH_POS_RecHits_GEM4Y_Ch3iE4",       "SH_POS_RecHits_GEM4Y_Ch3iE4",       n_ps, n1_ps, n2_ps));

  // Recover Hits
  RH_CLS_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM1X_Ch1iE1",       "RH_CLS_RecHits_GEM1X_Ch1iE1",       n_cl, n1_cl, n2_cl));
  RH_CLS_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM1Y_Ch1iE2",       "RH_CLS_RecHits_GEM1Y_Ch1iE2",       n_cl, n1_cl, n2_cl));
  RH_CLS_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM2X_Ch1iE3",       "RH_CLS_RecHits_GEM2X_Ch1iE3",       n_cl, n1_cl, n2_cl));
  RH_CLS_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM2Y_Ch1iE4",       "RH_CLS_RecHits_GEM2Y_Ch1iE4",       n_cl, n1_cl, n2_cl));

  RH_CLS_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM3X_Ch3iE1",       "RH_CLS_RecHits_GEM3X_Ch3iE1",       n_cl, n1_cl, n2_cl));
  RH_CLS_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM3Y_Ch3iE2",       "RH_CLS_RecHits_GEM3Y_Ch3iE2",       n_cl, n1_cl, n2_cl));
  RH_CLS_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM4X_Ch3iE3",       "RH_CLS_RecHits_GEM4X_Ch3iE3",       n_cl, n1_cl, n2_cl));
  RH_CLS_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("RH_CLS_RecHits_GEM4Y_Ch3iE4",       "RH_CLS_RecHits_GEM4Y_Ch3iE4",       n_cl, n1_cl, n2_cl));

  RH_POS_RecHits_GEM1X_Ch1iE1       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM1X_Ch1iE1",       "RH_POS_RecHits_GEM1X_Ch1iE1",       n_ps, n1_ps, n2_ps));
  RH_POS_RecHits_GEM1Y_Ch1iE2       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM1Y_Ch1iE2",       "RH_POS_RecHits_GEM1Y_Ch1iE2",       n_ps, n1_ps, n2_ps));
  RH_POS_RecHits_GEM2X_Ch1iE3       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM2X_Ch1iE3",       "RH_POS_RecHits_GEM2X_Ch1iE3",       n_ps, n1_ps, n2_ps));
  RH_POS_RecHits_GEM2Y_Ch1iE4       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM2Y_Ch1iE4",       "RH_POS_RecHits_GEM2Y_Ch1iE4",       n_ps, n1_ps, n2_ps));

  RH_POS_RecHits_GEM3X_Ch3iE1       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM3X_Ch3iE1",       "RH_POS_RecHits_GEM3X_Ch3iE1",       n_ps, n1_ps, n2_ps));
  RH_POS_RecHits_GEM3Y_Ch3iE2       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM3Y_Ch3iE2",       "RH_POS_RecHits_GEM3Y_Ch3iE2",       n_ps, n1_ps, n2_ps));
  RH_POS_RecHits_GEM4X_Ch3iE3       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM4X_Ch3iE3",       "RH_POS_RecHits_GEM4X_Ch3iE3",       n_ps, n1_ps, n2_ps));
  RH_POS_RecHits_GEM4Y_Ch3iE4       = std::unique_ptr<TH1F>(new TH1F("RH_POS_RecHits_GEM4Y_Ch3iE4",       "RH_POS_RecHits_GEM4Y_Ch3iE4",       n_ps, n1_ps, n2_ps));

  // Clusersize Correlation
  CORR_SH_CLS_GEM1XY = std::unique_ptr<TH2F>(new TH2F("CORR_SH_CLS_GEM1XY", "CORR_SH_CLS_GEM1XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_SH_CLS_GEM2XY = std::unique_ptr<TH2F>(new TH2F("CORR_SH_CLS_GEM2XY", "CORR_SH_CLS_GEM2XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_SH_CLS_GEM3XY = std::unique_ptr<TH2F>(new TH2F("CORR_SH_CLS_GEM3XY", "CORR_SH_CLS_GEM3XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_SH_CLS_GEM4XY = std::unique_ptr<TH2F>(new TH2F("CORR_SH_CLS_GEM4XY", "CORR_SH_CLS_GEM4XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));

  CORR_2H_CLS_GEM1XY = std::unique_ptr<TH2F>(new TH2F("CORR_2H_CLS_GEM1XY", "CORR_2H_CLS_GEM1XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_2H_CLS_GEM2XY = std::unique_ptr<TH2F>(new TH2F("CORR_2H_CLS_GEM2XY", "CORR_2H_CLS_GEM2XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_2H_CLS_GEM3XY = std::unique_ptr<TH2F>(new TH2F("CORR_2H_CLS_GEM3XY", "CORR_2H_CLS_GEM3XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_2H_CLS_GEM4XY = std::unique_ptr<TH2F>(new TH2F("CORR_2H_CLS_GEM4XY", "CORR_2H_CLS_GEM4XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));

  CORR_3H_CLS_GEM1XY = std::unique_ptr<TH2F>(new TH2F("CORR_3H_CLS_GEM1XY", "CORR_3H_CLS_GEM1XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_3H_CLS_GEM2XY = std::unique_ptr<TH2F>(new TH2F("CORR_3H_CLS_GEM2XY", "CORR_3H_CLS_GEM2XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_3H_CLS_GEM3XY = std::unique_ptr<TH2F>(new TH2F("CORR_3H_CLS_GEM3XY", "CORR_3H_CLS_GEM3XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_3H_CLS_GEM4XY = std::unique_ptr<TH2F>(new TH2F("CORR_3H_CLS_GEM4XY", "CORR_3H_CLS_GEM4XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));

  CORR_ALL_CLS_GEM1XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_CLS_GEM1XY", "CORR_ALL_CLS_GEM1XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_ALL_CLS_GEM2XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_CLS_GEM2XY", "CORR_ALL_CLS_GEM2XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_ALL_CLS_GEM3XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_CLS_GEM3XY", "CORR_ALL_CLS_GEM3XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_ALL_CLS_GEM4XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_CLS_GEM4XY", "CORR_ALL_CLS_GEM4XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));

  CORR_ALL_NHIT_GEM1XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_NHIT_GEM1XY", "CORR_ALL_NHIT_GEM1XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_ALL_NHIT_GEM2XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_NHIT_GEM2XY", "CORR_ALL_NHIT_GEM2XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_ALL_NHIT_GEM3XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_NHIT_GEM3XY", "CORR_ALL_NHIT_GEM3XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));
  CORR_ALL_NHIT_GEM4XY = std::unique_ptr<TH2F>(new TH2F("CORR_ALL_NHIT_GEM4XY", "CORR_ALL_NHIT_GEM4XY", n_cl, n1_cl, n2_cl, n_cl, n1_cl, n2_cl));

  CORR_EVT_GEMXY_v1 = std::unique_ptr<TH1F>(new TH1F("CORR_EVT_GEMXY_v1", "CORR_EVT_GEMXY_v1", 12, 0.5, 12.5));
  std::vector< std::string > Corr_evt_gemxy_labels = {"0 hits", "1-5 hits", "6-7 hits", "8 hits (TRK)", "1 TRK + 1-5 hits", "1 TRK + 6-7 hits", "2 TRK", "2 TRK + 1-5 hits", "2 TRK + 6-7 hits", 
						      "3 TRK", ">3 hits on Single Det", "not-categorized"};
  for(int i=0; i<12; ++i) { CORR_EVT_GEMXY_v1->GetXaxis()->SetBinLabel(i+1, Corr_evt_gemxy_labels[i].c_str()); }

  CORR_EVT_GEMXY_v2 = std::unique_ptr<TH2F>(new TH2F("CORR_EVT_GEMXY_v2", "CORR_EVT_GEMXY_v2", 40, 0.5, 40.5, 40, 0.5, 40.5));
  std::vector< std::string > Corr_evt_gemxy_labbs1 = {"GEM 1X", "GEM1Y", "GEM 2X", "GEM2Y", "GEM 3X", "GEM3Y", "GEM 4X", "GEM4Y"};
  std::vector< std::string > Corr_evt_gemxy_labbs2 = {"0 hits", "1 hit", "2 hits", "3 hits", ">3 hits"};
  for(int i=0; i<8; ++i) {
    for(int j=0; j<5; ++j) {
      std::stringstream labstr; labstr<<Corr_evt_gemxy_labbs1[i]<<": "<<Corr_evt_gemxy_labbs2[j];
      std::string labstring = labstr.str();
      CORR_EVT_GEMXY_v2->GetXaxis()->SetBinLabel(i+8*j+1,labstring.c_str());
      CORR_EVT_GEMXY_v2->GetYaxis()->SetBinLabel(i+8*j+1,labstring.c_str());
    }
  }

  // GE21 
  BX_RecHits_GE21_iE13        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GE21_iE13",         "BX_RecHits_GE21_iE13",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GE21_iE14        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GE21_iE14",         "BX_RecHits_GE21_iE14",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GE21_iE15        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GE21_iE15",         "BX_RecHits_GE21_iE15",         n_bx, n1_bx, n2_bx));
  BX_RecHits_GE21_iE16        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_GE21_iE16",         "BX_RecHits_GE21_iE16",         n_bx, n1_bx, n2_bx));

  CLS_RecHits_GE21_iE13       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GE21_iE13",       "CLS_RecHits_GE21_iE13",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GE21_iE14       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GE21_iE14",       "CLS_RecHits_GE21_iE14",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GE21_iE15       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GE21_iE15",       "CLS_RecHits_GE21_iE15",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_GE21_iE16       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_GE21_iE16",       "CLS_RecHits_GE21_iE16",       n_cl, n1_cl, n2_cl));

  NHIT_RecHits_GE21_iE13       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GE21_iE13",       "NHIT_RecHits_GE21_iE13",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GE21_iE14       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GE21_iE14",       "NHIT_RecHits_GE21_iE14",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GE21_iE15       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GE21_iE15",       "NHIT_RecHits_GE21_iE15",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_GE21_iE16       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_GE21_iE16",       "NHIT_RecHits_GE21_iE16",       n_cl, n1_cl, n2_cl));

  POS_RecHits_GE21_iE13       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GE21_iE13",       "POS_RecHits_GE21_iE13",       m_ps, m1_ps, m2_ps));
  POS_RecHits_GE21_iE14       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GE21_iE14",       "POS_RecHits_GE21_iE14",       m_ps, m1_ps, m2_ps));
  POS_RecHits_GE21_iE15       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GE21_iE15",       "POS_RecHits_GE21_iE15",       m_ps, m1_ps, m2_ps));
  POS_RecHits_GE21_iE16       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_GE21_iE16",       "POS_RecHits_GE21_iE16",       m_ps, m1_ps, m2_ps));

  STR_RecHits_GE21_iE13       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GE21_iE13",       "STR_RecHits_GE21_iE13",       m_st, m1_st, m2_st));
  STR_RecHits_GE21_iE14       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GE21_iE14",       "STR_RecHits_GE21_iE14",       m_st, m1_st, m2_st));
  STR_RecHits_GE21_iE15       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GE21_iE15",       "STR_RecHits_GE21_iE15",       m_st, m1_st, m2_st));
  STR_RecHits_GE21_iE16       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_GE21_iE16",       "STR_RecHits_GE21_iE16",       m_st, m1_st, m2_st));

  TOT_RecHits_GE21               = std::unique_ptr<TH1F>(new TH1F("TOT_RecHits_GE21",                "TOT_RecHits_GE21",                5,   0.5,    5.5));
  EFF_RecHits_GE21               = std::unique_ptr<TH1F>(new TH1F("EFF_RecHits_GE21",                "EFF_RecHits_GE21",                5,   0.5,    5.5));


  // ME0
  BX_RecHits_ME0_iE1        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE1",         "BX_RecHits_ME0_iE1",         n_bx, n1_bx, n2_bx));
  BX_RecHits_ME0_iE2        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE2",         "BX_RecHits_ME0_iE2",         n_bx, n1_bx, n2_bx));
  BX_RecHits_ME0_iE3        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE3",         "BX_RecHits_ME0_iE3",         n_bx, n1_bx, n2_bx));
  BX_RecHits_ME0_iE4        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE4",         "BX_RecHits_ME0_iE4",         n_bx, n1_bx, n2_bx));
  BX_RecHits_ME0_iE5        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE5",         "BX_RecHits_ME0_iE5",         n_bx, n1_bx, n2_bx));
  BX_RecHits_ME0_iE6        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE6",         "BX_RecHits_ME0_iE6",         n_bx, n1_bx, n2_bx));
  BX_RecHits_ME0_iE7        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE7",         "BX_RecHits_ME0_iE7",         n_bx, n1_bx, n2_bx));
  BX_RecHits_ME0_iE8        = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_ME0_iE8",         "BX_RecHits_ME0_iE8",         n_bx, n1_bx, n2_bx));

  CLS_RecHits_ME0_iE1       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE1",       "CLS_RecHits_ME0_iE1",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_ME0_iE2       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE2",       "CLS_RecHits_ME0_iE2",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_ME0_iE3       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE3",       "CLS_RecHits_ME0_iE3",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_ME0_iE4       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE4",       "CLS_RecHits_ME0_iE4",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_ME0_iE5       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE5",       "CLS_RecHits_ME0_iE5",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_ME0_iE6       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE6",       "CLS_RecHits_ME0_iE6",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_ME0_iE7       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE7",       "CLS_RecHits_ME0_iE7",       n_cl, n1_cl, n2_cl));
  CLS_RecHits_ME0_iE8       = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_ME0_iE8",       "CLS_RecHits_ME0_iE8",       n_cl, n1_cl, n2_cl));

  NHIT_RecHits_ME0_iE1       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE1",       "NHIT_RecHits_ME0_iE1",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_ME0_iE2       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE2",       "NHIT_RecHits_ME0_iE2",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_ME0_iE3       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE3",       "NHIT_RecHits_ME0_iE3",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_ME0_iE4       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE4",       "NHIT_RecHits_ME0_iE4",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_ME0_iE5       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE5",       "NHIT_RecHits_ME0_iE5",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_ME0_iE6       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE6",       "NHIT_RecHits_ME0_iE6",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_ME0_iE7       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE7",       "NHIT_RecHits_ME0_iE7",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_ME0_iE8       = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_ME0_iE8",       "NHIT_RecHits_ME0_iE8",       n_cl, n1_cl, n2_cl));

  POS_RecHits_ME0_iE1       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE1",       "POS_RecHits_ME0_iE1",       o_ps, o1_ps, o2_ps));
  POS_RecHits_ME0_iE2       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE2",       "POS_RecHits_ME0_iE2",       o_ps, o1_ps, o2_ps));
  POS_RecHits_ME0_iE3       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE3",       "POS_RecHits_ME0_iE3",       o_ps, o1_ps, o2_ps));
  POS_RecHits_ME0_iE4       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE4",       "POS_RecHits_ME0_iE4",       o_ps, o1_ps, o2_ps));
  POS_RecHits_ME0_iE5       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE5",       "POS_RecHits_ME0_iE5",       o_ps, o1_ps, o2_ps));
  POS_RecHits_ME0_iE6       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE6",       "POS_RecHits_ME0_iE6",       o_ps, o1_ps, o2_ps));
  POS_RecHits_ME0_iE7       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE7",       "POS_RecHits_ME0_iE7",       o_ps, o1_ps, o2_ps));
  POS_RecHits_ME0_iE8       = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_ME0_iE8",       "POS_RecHits_ME0_iE8",       o_ps, o1_ps, o2_ps));

  STR_RecHits_ME0_iE1       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE1",       "STR_RecHits_ME0_iE1",       o_st, o1_st, o2_st));
  STR_RecHits_ME0_iE2       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE2",       "STR_RecHits_ME0_iE2",       o_st, o1_st, o2_st));
  STR_RecHits_ME0_iE3       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE3",       "STR_RecHits_ME0_iE3",       o_st, o1_st, o2_st));
  STR_RecHits_ME0_iE4       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE4",       "STR_RecHits_ME0_iE4",       o_st, o1_st, o2_st));
  STR_RecHits_ME0_iE5       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE5",       "STR_RecHits_ME0_iE5",       o_st, o1_st, o2_st));
  STR_RecHits_ME0_iE6       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE6",       "STR_RecHits_ME0_iE6",       o_st, o1_st, o2_st));
  STR_RecHits_ME0_iE7       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE7",       "STR_RecHits_ME0_iE7",       o_st, o1_st, o2_st));
  STR_RecHits_ME0_iE8       = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_ME0_iE8",       "STR_RecHits_ME0_iE8",       o_st, o1_st, o2_st));

  DIG_RecHits_ME0_iE1       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE1",       "DIG_RecHits_ME0_iE1",       q_st, q1_st, q2_st));
  DIG_RecHits_ME0_iE2       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE2",       "DIG_RecHits_ME0_iE2",       q_st, q1_st, q2_st));
  DIG_RecHits_ME0_iE3       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE3",       "DIG_RecHits_ME0_iE3",       q_st, q1_st, q2_st));
  DIG_RecHits_ME0_iE4       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE4",       "DIG_RecHits_ME0_iE4",       q_st, q1_st, q2_st));
  DIG_RecHits_ME0_iE5       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE5",       "DIG_RecHits_ME0_iE5",       q_st, q1_st, q2_st));
  DIG_RecHits_ME0_iE6       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE6",       "DIG_RecHits_ME0_iE6",       q_st, q1_st, q2_st));
  DIG_RecHits_ME0_iE7       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE7",       "DIG_RecHits_ME0_iE7",       q_st, q1_st, q2_st));
  DIG_RecHits_ME0_iE8       = std::unique_ptr<TH1F>(new TH1F("DIG_RecHits_ME0_iE8",       "DIG_RecHits_ME0_iE8",       q_st, q1_st, q2_st));


  TOT_RecHits_ME0               = std::unique_ptr<TH1F>(new TH1F("TOT_RecHits_ME0",                "TOT_RecHits_ME0",                5,   0.5,    5.5));
  EFF_RecHits_ME0               = std::unique_ptr<TH1F>(new TH1F("EFF_RecHits_ME0",                "EFF_RecHits_ME0",                5,   0.5,    5.5));


  // ME0-Prototype 10x20
  BX_RecHits_10X20_Ch3iE5              = std::unique_ptr<TH1F>(new TH1F("BX_RecHits_10X20_Ch3iE5",        "BX_RecHits_10X20_Ch3iE5",        n_bx, n1_bx, n2_bx));
  CLS_RecHits_10X20_Ch3iE5             = std::unique_ptr<TH1F>(new TH1F("CLS_RecHits_10X20_Ch3iE5",       "CLS_RecHits_10X20_Ch3iE5",       n_cl, n1_cl, n2_cl));
  NHIT_RecHits_10X20_Ch3iE5            = std::unique_ptr<TH1F>(new TH1F("NHIT_RecHits_10X20_Ch3iE5",      "NHIT_RecHits_10X20_Ch3iE5",      n_cl, n1_cl, n2_cl));
  POS_RecHits_10X20_Ch3iE5             = std::unique_ptr<TH1F>(new TH1F("POS_RecHits_10X20_Ch3iE5",       "POS_RecHits_10X20_Ch3iE5",       p_ps, p1_ps, p2_ps));
  STR_RecHits_10X20_Ch3iE5             = std::unique_ptr<TH1F>(new TH1F("STR_RecHits_10X20_Ch3iE5",       "STR_RecHits_10X20_Ch3iE5",       p_st, p1_st, p2_st));
  MYPOS_ALL_RecHits_10X20_Ch3iE5       = std::unique_ptr<TH1F>(new TH1F("MYPOS_ALL_RecHits_10X20_Ch3iE5", "MYPOS_ALL_RecHits_10X20_Ch3iE5", p_ps, p1_ps, p2_ps));
  MYPOS_SH_RecHits_10X20_Ch3iE5        = std::unique_ptr<TH1F>(new TH1F("MYPOS_SH_RecHits_10X20_Ch3iE5",  "MYPOS_SH_RecHits_10X20_Ch3iE5",  p_ps, p1_ps, p2_ps));
  MYPOS_CL1_RecHits_10X20_Ch3iE5       = std::unique_ptr<TH1F>(new TH1F("MYPOS_CL1_RecHits_10X20_Ch3iE5", "POS_CL1_RecHits_10X20_Ch3iE5",   p_ps, p1_ps, p2_ps));

}


MyTestBeamAnalyzer::~MyTestBeamAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
 
  outputfile->cd();

  // BX 
  BX_RecHits_GEM1X_Ch1iE1->Write(); BX_RecHits_GEM1Y_Ch1iE2->Write(); BX_RecHits_GEM2X_Ch1iE3->Write(); BX_RecHits_GEM2Y_Ch1iE4->Write();
  BX_RecHits_GEM3X_Ch3iE1->Write(); BX_RecHits_GEM3Y_Ch3iE2->Write(); BX_RecHits_GEM4X_Ch3iE3->Write(); BX_RecHits_GEM4Y_Ch3iE4->Write(); 
  BX_RecHits_GE21_iE13->Write(); BX_RecHits_GE21_iE14->Write(); BX_RecHits_GE21_iE15->Write(); BX_RecHits_GE21_iE16->Write();
  BX_RecHits_ME0_iE1->Write(); BX_RecHits_ME0_iE2->Write(); BX_RecHits_ME0_iE3->Write(); BX_RecHits_ME0_iE4->Write();
  BX_RecHits_ME0_iE5->Write(); BX_RecHits_ME0_iE6->Write(); BX_RecHits_ME0_iE7->Write(); BX_RecHits_ME0_iE8->Write();

  // CLS
  CLS_RecHits_GEM1X_Ch1iE1->Write(); CLS_RecHits_GEM1Y_Ch1iE2->Write(); CLS_RecHits_GEM2X_Ch1iE3->Write(); CLS_RecHits_GEM2Y_Ch1iE4->Write();
  CLS_RecHits_GEM3X_Ch3iE1->Write(); CLS_RecHits_GEM3Y_Ch3iE2->Write(); CLS_RecHits_GEM4X_Ch3iE3->Write(); CLS_RecHits_GEM4Y_Ch3iE4->Write(); 
  CLS_RecHits_GE21_iE13->Write(); CLS_RecHits_GE21_iE14->Write(); CLS_RecHits_GE21_iE15->Write(); CLS_RecHits_GE21_iE16->Write();
  CLS_RecHits_ME0_iE1->Write(); CLS_RecHits_ME0_iE2->Write(); CLS_RecHits_ME0_iE3->Write(); CLS_RecHits_ME0_iE4->Write();
  CLS_RecHits_ME0_iE5->Write(); CLS_RecHits_ME0_iE6->Write(); CLS_RecHits_ME0_iE7->Write(); CLS_RecHits_ME0_iE8->Write();

  // NHIT
  NHIT_RecHits_GEM1X_Ch1iE1->Write(); NHIT_RecHits_GEM1Y_Ch1iE2->Write(); NHIT_RecHits_GEM2X_Ch1iE3->Write(); NHIT_RecHits_GEM2Y_Ch1iE4->Write();
  NHIT_RecHits_GEM3X_Ch3iE1->Write(); NHIT_RecHits_GEM3Y_Ch3iE2->Write(); NHIT_RecHits_GEM4X_Ch3iE3->Write(); NHIT_RecHits_GEM4Y_Ch3iE4->Write(); 
  NHIT_RecHits_GE21_iE13->Write(); NHIT_RecHits_GE21_iE14->Write(); NHIT_RecHits_GE21_iE15->Write(); NHIT_RecHits_GE21_iE16->Write();
  NHIT_RecHits_ME0_iE1->Write(); NHIT_RecHits_ME0_iE2->Write(); NHIT_RecHits_ME0_iE3->Write(); NHIT_RecHits_ME0_iE4->Write();
  NHIT_RecHits_ME0_iE5->Write(); NHIT_RecHits_ME0_iE6->Write(); NHIT_RecHits_ME0_iE7->Write(); NHIT_RecHits_ME0_iE8->Write();

  // MYPOS
  MYPOS_ALL_RecHits_GEM1X_Ch1iE1->Write(); MYPOS_ALL_RecHits_GEM1Y_Ch1iE2->Write(); MYPOS_ALL_RecHits_GEM2X_Ch1iE3->Write(); MYPOS_ALL_RecHits_GEM2Y_Ch1iE4->Write();
  MYPOS_ALL_RecHits_GEM3X_Ch3iE1->Write(); MYPOS_ALL_RecHits_GEM3Y_Ch3iE2->Write(); MYPOS_ALL_RecHits_GEM4X_Ch3iE3->Write(); MYPOS_ALL_RecHits_GEM4Y_Ch3iE4->Write(); 
  MYPOS_SH_RecHits_GEM1X_Ch1iE1->Write(); MYPOS_SH_RecHits_GEM1Y_Ch1iE2->Write(); MYPOS_SH_RecHits_GEM2X_Ch1iE3->Write(); MYPOS_SH_RecHits_GEM2Y_Ch1iE4->Write();
  MYPOS_SH_RecHits_GEM3X_Ch3iE1->Write(); MYPOS_SH_RecHits_GEM3Y_Ch3iE2->Write(); MYPOS_SH_RecHits_GEM4X_Ch3iE3->Write(); MYPOS_SH_RecHits_GEM4Y_Ch3iE4->Write(); 
  MYPOS_CL1_RecHits_GEM1X_Ch1iE1->Write(); MYPOS_CL1_RecHits_GEM1Y_Ch1iE2->Write(); MYPOS_CL1_RecHits_GEM2X_Ch1iE3->Write(); MYPOS_CL1_RecHits_GEM2Y_Ch1iE4->Write();
  MYPOS_CL1_RecHits_GEM3X_Ch3iE1->Write(); MYPOS_CL1_RecHits_GEM3Y_Ch3iE2->Write(); MYPOS_CL1_RecHits_GEM4X_Ch3iE3->Write(); MYPOS_CL1_RecHits_GEM4Y_Ch3iE4->Write(); 
  MYPOS_CL2_RecHits_GEM1X_Ch1iE1->Write(); MYPOS_CL2_RecHits_GEM1Y_Ch1iE2->Write(); MYPOS_CL2_RecHits_GEM2X_Ch1iE3->Write(); MYPOS_CL1_RecHits_GEM2Y_Ch1iE4->Write();
  MYPOS_CL2_RecHits_GEM3X_Ch3iE1->Write(); MYPOS_CL2_RecHits_GEM3Y_Ch3iE2->Write(); MYPOS_CL2_RecHits_GEM4X_Ch3iE3->Write(); MYPOS_CL1_RecHits_GEM4Y_Ch3iE4->Write(); 

  // LOCPOS
  POS_RecHits_GEM1X_Ch1iE1->Write(); POS_RecHits_GEM1Y_Ch1iE2->Write(); POS_RecHits_GEM2X_Ch1iE3->Write(); POS_RecHits_GEM2Y_Ch1iE4->Write();
  POS_RecHits_GEM3X_Ch3iE1->Write(); POS_RecHits_GEM3Y_Ch3iE2->Write(); POS_RecHits_GEM4X_Ch3iE3->Write(); POS_RecHits_GEM4Y_Ch3iE4->Write(); 
  POS_RecHits_GE21_iE13->Write(); POS_RecHits_GE21_iE14->Write(); POS_RecHits_GE21_iE15->Write(); POS_RecHits_GE21_iE16->Write();
  POS_RecHits_ME0_iE1->Write(); POS_RecHits_ME0_iE2->Write(); POS_RecHits_ME0_iE3->Write(); POS_RecHits_ME0_iE4->Write();
  POS_RecHits_ME0_iE5->Write(); POS_RecHits_ME0_iE6->Write(); POS_RecHits_ME0_iE7->Write(); POS_RecHits_ME0_iE8->Write();

  // STRIPS
  STR_RecHits_GEM1X_Ch1iE1->Write(); STR_RecHits_GEM1Y_Ch1iE2->Write(); STR_RecHits_GEM2X_Ch1iE3->Write(); STR_RecHits_GEM2Y_Ch1iE4->Write();
  STR_RecHits_GEM3X_Ch3iE1->Write(); STR_RecHits_GEM3Y_Ch3iE2->Write(); STR_RecHits_GEM4X_Ch3iE3->Write(); STR_RecHits_GEM4Y_Ch3iE4->Write(); 
  STR_CL2_RecHits_GEM1X_Ch1iE1->Write(); STR_CL2_RecHits_GEM1Y_Ch1iE2->Write(); STR_CL2_RecHits_GEM2X_Ch1iE3->Write(); STR_CL2_RecHits_GEM2Y_Ch1iE4->Write();
  STR_CL2_RecHits_GEM3X_Ch3iE1->Write(); STR_CL2_RecHits_GEM3Y_Ch3iE2->Write(); STR_CL2_RecHits_GEM4X_Ch3iE3->Write(); STR_CL2_RecHits_GEM4Y_Ch3iE4->Write(); 

  STR_RecHits_GE21_iE13->Write(); STR_RecHits_GE21_iE14->Write(); STR_RecHits_GE21_iE15->Write(); STR_RecHits_GE21_iE16->Write();
  STR_RecHits_ME0_iE1->Write(); STR_RecHits_ME0_iE2->Write(); STR_RecHits_ME0_iE3->Write(); STR_RecHits_ME0_iE4->Write();
  STR_RecHits_ME0_iE5->Write(); STR_RecHits_ME0_iE6->Write(); STR_RecHits_ME0_iE7->Write(); STR_RecHits_ME0_iE8->Write();
  // DIGIS
  DIG_RecHits_ME0_iE1->Write(); DIG_RecHits_ME0_iE2->Write(); DIG_RecHits_ME0_iE3->Write(); DIG_RecHits_ME0_iE4->Write();
  DIG_RecHits_ME0_iE5->Write(); DIG_RecHits_ME0_iE6->Write(); DIG_RecHits_ME0_iE7->Write(); DIG_RecHits_ME0_iE8->Write();


  // 2D PROFILE
  PRF2D_ALL_RecHits_GEM1XY_Ch1iE1iE2->Write(); PRF2D_ALL_RecHits_GEM2XY_Ch1iE3iE4->Write();
  PRF2D_ALL_RecHits_GEM3XY_Ch3iE1iE2->Write(); PRF2D_ALL_RecHits_GEM4XY_Ch3iE3iE4->Write();
  PRF2D_SH_RecHits_GEM1XY_Ch1iE1iE2->Write(); PRF2D_SH_RecHits_GEM2XY_Ch1iE3iE4->Write();
  PRF2D_SH_RecHits_GEM3XY_Ch3iE1iE2->Write(); PRF2D_SH_RecHits_GEM4XY_Ch3iE3iE4->Write();
  PRF2D_RH_RecHits_GEM1XY_Ch1iE1iE2->Write(); PRF2D_RH_RecHits_GEM2XY_Ch1iE3iE4->Write();
  PRF2D_RH_RecHits_GEM3XY_Ch3iE1iE2->Write(); PRF2D_RH_RecHits_GEM4XY_Ch3iE3iE4->Write();
  PRF2D_ALL_RecHits_GE21->Write();            PRF2D_ALL_RecHits_ME0->Write();

  // EFF
  TOT_RecHits_GEM->Write();
  TOT_RecHits_GE21->Write();
  TOT_RecHits_ME0->Write();
  // TODO :: divide by nEvents
  EFF_RecHits_GEM->Write();
  EFF_RecHits_GE21->Write();
  EFF_RecHits_ME0->Write();

  // Clean Hits
  RH_CLS_RecHits_GEM1X_Ch1iE1->Write(); SH_CLS_RecHits_GEM1Y_Ch1iE2->Write(); SH_CLS_RecHits_GEM2X_Ch1iE3->Write(); SH_CLS_RecHits_GEM2Y_Ch1iE4->Write();
  SH_CLS_RecHits_GEM3X_Ch3iE1->Write(); SH_CLS_RecHits_GEM3Y_Ch3iE2->Write(); SH_CLS_RecHits_GEM4X_Ch3iE3->Write(); SH_CLS_RecHits_GEM4Y_Ch3iE4->Write(); 
  SH_POS_RecHits_GEM1X_Ch1iE1->Write(); SH_POS_RecHits_GEM1Y_Ch1iE2->Write(); SH_POS_RecHits_GEM2X_Ch1iE3->Write(); SH_POS_RecHits_GEM2Y_Ch1iE4->Write();
  SH_POS_RecHits_GEM3X_Ch3iE1->Write(); SH_POS_RecHits_GEM3Y_Ch3iE2->Write(); SH_POS_RecHits_GEM4X_Ch3iE3->Write(); SH_POS_RecHits_GEM4Y_Ch3iE4->Write(); 

  // Recover Hits
  RH_CLS_RecHits_GEM1X_Ch1iE1->Write(); RH_CLS_RecHits_GEM1Y_Ch1iE2->Write(); RH_CLS_RecHits_GEM2X_Ch1iE3->Write(); RH_CLS_RecHits_GEM2Y_Ch1iE4->Write();
  RH_CLS_RecHits_GEM3X_Ch3iE1->Write(); RH_CLS_RecHits_GEM3Y_Ch3iE2->Write(); RH_CLS_RecHits_GEM4X_Ch3iE3->Write(); RH_CLS_RecHits_GEM4Y_Ch3iE4->Write(); 
  RH_POS_RecHits_GEM1X_Ch1iE1->Write(); RH_POS_RecHits_GEM1Y_Ch1iE2->Write(); RH_POS_RecHits_GEM2X_Ch1iE3->Write(); RH_POS_RecHits_GEM2Y_Ch1iE4->Write();
  RH_POS_RecHits_GEM3X_Ch3iE1->Write(); RH_POS_RecHits_GEM3Y_Ch3iE2->Write(); RH_POS_RecHits_GEM4X_Ch3iE3->Write(); RH_POS_RecHits_GEM4Y_Ch3iE4->Write(); 

  // Clustersize correlations
  CORR_SH_CLS_GEM1XY->Write(); CORR_SH_CLS_GEM2XY->Write(); CORR_SH_CLS_GEM3XY->Write(); CORR_SH_CLS_GEM4XY->Write();
  CORR_2H_CLS_GEM1XY->Write(); CORR_2H_CLS_GEM2XY->Write(); CORR_2H_CLS_GEM3XY->Write(); CORR_2H_CLS_GEM4XY->Write();
  CORR_3H_CLS_GEM1XY->Write(); CORR_3H_CLS_GEM2XY->Write(); CORR_3H_CLS_GEM3XY->Write(); CORR_3H_CLS_GEM4XY->Write();
  CORR_ALL_CLS_GEM1XY->Write(); CORR_ALL_CLS_GEM2XY->Write(); CORR_ALL_CLS_GEM3XY->Write(); CORR_ALL_CLS_GEM4XY->Write();

  CORR_ALL_NHIT_GEM1XY->Write(); CORR_ALL_NHIT_GEM2XY->Write(); CORR_ALL_NHIT_GEM3XY->Write(); CORR_ALL_NHIT_GEM4XY->Write();
  CORR_EVT_GEMXY_v1->Write();    CORR_EVT_GEMXY_v2->Write();

  // 10 x 20 Prototype
  BX_RecHits_10X20_Ch3iE5->Write();  CLS_RecHits_10X20_Ch3iE5->Write();  NHIT_RecHits_10X20_Ch3iE5->Write();  POS_RecHits_10X20_Ch3iE5->Write();  STR_RecHits_10X20_Ch3iE5->Write();       
  MYPOS_ALL_RecHits_10X20_Ch3iE5->Write(); MYPOS_SH_RecHits_10X20_Ch3iE5->Write(); MYPOS_CL1_RecHits_10X20_Ch3iE5->Write(); 

  // eMap Debug
  eMapDebug_ME0_iE1->Write(); eMapDebug_ME0_iE2->Write(); eMapDebug_ME0_iE3->Write(); eMapDebug_ME0_iE4->Write();
  eMapDebug_ME0_iE5->Write(); eMapDebug_ME0_iE6->Write(); eMapDebug_ME0_iE7->Write(); eMapDebug_ME0_iE8->Write();

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MyTestBeamAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  ++nEvents;

  edm::EventAuxiliary iEventAux = iEvent.eventAuxiliary();
  if(dbg_info) {
    std::cout<<"\n - - - - - new Event :: run "<<iEvent.id().run()<<" event "<<iEvent.id().event()<<" orbit "<<iEventAux.orbitNumber()<<" bunch crossing " <<iEventAux.bunchCrossing();
    std::cout<<" time stamp value "<<iEventAux.time().value()<<" time stamp Unix Time "<<iEventAux.time().unixTime()<<std::endl;
  }

  iSetup.get<MuonGeometryRecord>().get(gemGeom);

  // ================
  // GEM recHits
  // ================
  edm::Handle<GEMRecHitCollection> gemRecHits; 
  iEvent.getByToken(GEMRecHit_Token,gemRecHits);

  // count the number of GEM rechits
  // - - - - - - - - - - - - - - - - - - -
  int nGEM = 0;
  GEMRecHitCollection::const_iterator recHit;
  for (recHit = gemRecHits->begin(); recHit != gemRecHits->end(); recHit++) {
    nGEM++;
  }
  if(dbg_test) std::cout<<"The Number of RecHits is "<<nGEM<<std::endl;         
  // count the number of GEM rechits for the different detectors
  std::vector<int> nGEMXY = {0,0,0,0,0,0,0,0};
  std::vector<int> nGE21  = {0,0,0,0};
  std::vector<int> nME0   = {0,0,0,0};
  // - - - - - - - - - - - - - - - - - - -


  // Save Local Positions to reco 2D pos
  // - - - - - - - - - - - - - - - - - - -
  std::vector<double> GEM1X_pos, GEM1Y_pos, GEM2X_pos, GEM2Y_pos, GEM3X_pos, GEM3Y_pos, GEM4X_pos, GEM4Y_pos;
  std::vector<double> GEM1X_cls, GEM1Y_cls, GEM2X_cls, GEM2Y_cls, GEM3X_cls, GEM3Y_cls, GEM4X_cls, GEM4Y_cls;
  // - - - - - - - - - - - - - - - - - - -


  // Loop over GEM RecHits
  // - - - - - - - - - - - - - - - - - - -
  for (recHit = gemRecHits->begin(); recHit != gemRecHits->end(); recHit++) {
    GEMDetId rollId = (GEMDetId)(*recHit).gemId();
    LocalPoint recHitPos=recHit->localPosition();
    const GEMEtaPartition* gemEtaPart = gemGeom->etaPartition(rollId);
    const BoundPlane & GEMSurface = gemEtaPart->surface(); 
    // GlobalPoint GEMGlobalPoint = GEMSurface.toGlobal(recHitPos);

    int region  = rollId.region();
    int station = rollId.station();
    // int ring    = rollId.ring();
    int layer   = rollId.layer();
    int etapart = rollId.roll();
    int chamber = rollId.chamber();

    int bx = recHit->BunchX();
    int cl = recHit->clusterSize();
    int st = recHit->firstClusterStrip();
    int pos = recHitPos.x()*10;             // I prefer to work in mm

    // Position Calculation for GEM 10x10 by myself 
    // - - - - - - - - - - - - - - - - - - - - - - -
    // x = 0 @ strip 179 - 180 interface
    // strip 001 = + 44.75 mm
    // strip 358 = - 44.75 mm
    double pitch = 0.250; // mm
    double centerstrip = st + (cl-1)*0.5;    //  cl=1 => st; cl=3 => st+1; cl=5 => st+2; also ok for even cls
    double pos2x = -44.75 + pitch*(centerstrip-0.5); // in mm
    double pos2y = 44.75 - pitch*(centerstrip-0.5); // in mm
    // keep pos for GE2/1


    // Global Position does not make sense ... Test Beam Geometry not yet loaded ....
    // std::cout<<"GEM Rec Hit in [DetId] = ["<<rollId<<"] with BX = "<<recHit->BunchX()<<" and Global Position = "<<GEMGlobalPoint<<std::endl; 
    if(dbg_test) {
      std::cout<<"GEM Rec Hit in [DetId] = ["<<rollId.rawId()<<"]["<<rollId<<"] with BX = "<<recHit->BunchX()<<" CLS = "<<recHit->clusterSize();
      std::cout<<" and Local Position = "<<recHitPos<<" pos = "<<recHitPos.x()<<" mycalc (x) = "<<pos2x<<" mycalc (y) = "<<pos2y<<std::endl;
      std::cout<<"            --> strips: [";
      for(int i=0; i<cl; ++i) {
	std::cout<<st+i;
	if(i<cl-1){ std::cout<<","; } 
	else { std::cout<<"]"; }
      }
      std::cout<<std::endl;
      std::cout<<"            --> Geom Info DetId :: nstrips = "<<gemEtaPart->nstrips()<<" strip pitch = "<<gemEtaPart->pitch()<<" local pos first strip = "<<gemEtaPart->centreOfStrip(0).x()<<" local pos last strip = "<<gemEtaPart->centreOfStrip(gemEtaPart->nstrips()-1).x()<<std::endl;
    }

    // GEM 1 and GEM2
    if((mapVersion==1 && region == -1 && station == 1 && layer==1 && chamber==1) || (mapVersion==2 && region == 1 && station == 1 && layer==1 && chamber==2)) {
      if(etapart==1) { // GEM1 X
	BX_RecHits_GEM1X_Ch1iE1->Fill(bx);
	CLS_RecHits_GEM1X_Ch1iE1->Fill(cl);
	POS_RecHits_GEM1X_Ch1iE1->Fill(pos);
	MYPOS_ALL_RecHits_GEM1X_Ch1iE1->Fill(pos2x);
	if(cl==1) MYPOS_CL1_RecHits_GEM1X_Ch1iE1->Fill(pos2x); // CLS 1 only
	else MYPOS_CL2_RecHits_GEM1X_Ch1iE1->Fill(pos2x);      // CLS 2 and more
	// if(dbg_test) { std::cout<<"Filling GEM1X histograms with bx = "<<bx<<" cls = "<<cl<<" pos = "<<pos2x<<std::endl;}
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM1X_Ch1iE1->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM1X_Ch1iE1->Fill(st+i);}}
	nGEMXY[0] += 1;
	GEM1X_pos.push_back(pos2x);
	GEM1X_cls.push_back(cl);

	// Verify whether dead strips @ 140, 211 and 240 are correctly treated ...
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// print out information of event for clusters containing strip 210 and 212 ...
	if(dbg_dead) {
	  bool str211 = false, str213 = false, str239 = false, str241 = false;
	  if(st==211 || (st<211 && st+cl>211)) str211 = true;
	  if(st==213 || (st<213 && st+cl>213)) str213 = true;
	  if(st==239 || (st<239 && st+cl>239)) str239 = true;
	  if(st==241 || (st<241 && st+cl>241)) str241 = true;
	  if(str211 || str213 || str239 || str241) {}
	  if(str211 || str213) {
	    std::cout<<"GEM Rec Hit in [DetId] = ["<<rollId.rawId()<<"]["<<rollId<<"] with BX = "<<recHit->BunchX()<<" CLS = "<<recHit->clusterSize();
	    std::cout<<" and Local Position = "<<recHitPos<<" pos = "<<recHitPos.x()<<" mycalc = "<<pos2x<<std::endl;
	    std::cout<<"            --> strips: [";
	    for(int i=0; i<cl; ++i) {
	      std::cout<<st+i;
	      if(i<cl-1){ std::cout<<","; }
	      else { std::cout<<"]"; }
	    }
	    std::cout<<std::endl;
	  }
	} 
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Re-Clustering with dead strips works only when dead strip is intermediate to 2 fired strips
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

      }
      if(etapart==2) { // GEM1 Y
	BX_RecHits_GEM1Y_Ch1iE2->Fill(bx);
	CLS_RecHits_GEM1Y_Ch1iE2->Fill(cl);
	POS_RecHits_GEM1Y_Ch1iE2->Fill(pos);
	MYPOS_ALL_RecHits_GEM1Y_Ch1iE2->Fill(pos2y);
	if(cl==1) MYPOS_CL1_RecHits_GEM1Y_Ch1iE2->Fill(pos2y);
	MYPOS_CL1_RecHits_GEM1Y_Ch1iE2->Fill(pos2y);
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM1Y_Ch1iE2->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM1Y_Ch1iE2->Fill(st+i);}}
	nGEMXY[1] += 1;
	GEM1Y_pos.push_back(pos2y);
	GEM1Y_cls.push_back(cl);
      }
      if(etapart==3) { // GEM2 X
	BX_RecHits_GEM2X_Ch1iE3->Fill(bx);
	CLS_RecHits_GEM2X_Ch1iE3->Fill(cl);
	POS_RecHits_GEM2X_Ch1iE3->Fill(pos);
	MYPOS_ALL_RecHits_GEM2X_Ch1iE3->Fill(pos2x);
	if(cl==1) MYPOS_CL1_RecHits_GEM2X_Ch1iE3->Fill(pos2x);
	else MYPOS_CL2_RecHits_GEM2X_Ch1iE3->Fill(pos2x);
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM2X_Ch1iE3->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM2X_Ch1iE3->Fill(st+i);}}
	nGEMXY[2] += 1;
	GEM2X_pos.push_back(pos2x);
	GEM2X_cls.push_back(cl);
      }
      if(etapart==4) { // GEM2 Y
	BX_RecHits_GEM2Y_Ch1iE4->Fill(bx);
	CLS_RecHits_GEM2Y_Ch1iE4->Fill(cl);
	POS_RecHits_GEM2Y_Ch1iE4->Fill(pos);
	MYPOS_ALL_RecHits_GEM2Y_Ch1iE4->Fill(pos2y);
	if(cl==1) MYPOS_CL1_RecHits_GEM2Y_Ch1iE4->Fill(pos2y);
	else MYPOS_CL2_RecHits_GEM2Y_Ch1iE4->Fill(pos2y);
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM2Y_Ch1iE4->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM2Y_Ch1iE4->Fill(st+i);}}
	nGEMXY[3] += 1;	
	GEM2Y_pos.push_back(pos2y);
	GEM2Y_cls.push_back(cl);
      }
    }
    // GEM 3 and GEM 4
    if((mapVersion==1 && region == -1 && station == 1 && layer==1 && chamber==3) || (mapVersion==2 && region == 1 && station == 1 && layer==1 && chamber==4)) {
      if(etapart==1) { // GEM3 X
	BX_RecHits_GEM3X_Ch3iE1->Fill(bx);
	CLS_RecHits_GEM3X_Ch3iE1->Fill(cl);
	POS_RecHits_GEM3X_Ch3iE1->Fill(pos);
	MYPOS_ALL_RecHits_GEM3X_Ch3iE1->Fill(pos2x);
	if(cl==1) MYPOS_CL1_RecHits_GEM3X_Ch3iE1->Fill(pos2x);
	else MYPOS_CL2_RecHits_GEM3X_Ch3iE1->Fill(pos2x);
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM3X_Ch3iE1->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM3X_Ch3iE1->Fill(st+i);}}
	nGEMXY[4] += 1;
	GEM3X_pos.push_back(pos2x);
	GEM3X_cls.push_back(cl);
      }
      if(etapart==2) { // GEM3 Y
	BX_RecHits_GEM3Y_Ch3iE2->Fill(bx);
	CLS_RecHits_GEM3Y_Ch3iE2->Fill(cl);
	POS_RecHits_GEM3Y_Ch3iE2->Fill(pos);
	MYPOS_ALL_RecHits_GEM3Y_Ch3iE2->Fill(pos2y);
	if(cl==1) MYPOS_CL1_RecHits_GEM3Y_Ch3iE2->Fill(pos2y);
        else MYPOS_CL2_RecHits_GEM3Y_Ch3iE2->Fill(pos2y);
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM3Y_Ch3iE2->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM3Y_Ch3iE2->Fill(st+i);}}
	nGEMXY[5] += 1;
	GEM3Y_pos.push_back(pos2y);
	GEM3Y_cls.push_back(cl);
      }
      if(etapart==3) { // GEM4 X
	BX_RecHits_GEM4X_Ch3iE3->Fill(bx);
	CLS_RecHits_GEM4X_Ch3iE3->Fill(cl);
	POS_RecHits_GEM4X_Ch3iE3->Fill(pos);
	MYPOS_ALL_RecHits_GEM4X_Ch3iE3->Fill(pos2x);
	if(cl==1) MYPOS_CL1_RecHits_GEM4X_Ch3iE3->Fill(pos2x);
	else MYPOS_CL2_RecHits_GEM4X_Ch3iE3->Fill(pos2x);
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM4X_Ch3iE3->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM4X_Ch3iE3->Fill(st+i);}}
	nGEMXY[6] += 1;
	GEM4X_pos.push_back(pos2x);
	GEM4X_cls.push_back(cl);
      }
      if(etapart==4) { // GEM4 Y
	BX_RecHits_GEM4Y_Ch3iE4->Fill(bx);
	CLS_RecHits_GEM4Y_Ch3iE4->Fill(cl);
	POS_RecHits_GEM4Y_Ch3iE4->Fill(pos);
	MYPOS_ALL_RecHits_GEM4Y_Ch3iE4->Fill(pos2y);
	if(cl==1) MYPOS_CL1_RecHits_GEM4Y_Ch3iE4->Fill(pos2y);
	else MYPOS_CL2_RecHits_GEM4Y_Ch3iE4->Fill(pos2y);
	for(int i=0; i<cl; ++i) { STR_RecHits_GEM4Y_Ch3iE4->Fill(st+i); if(cl>1) {STR_CL2_RecHits_GEM4Y_Ch3iE4->Fill(st+i);}}
	nGEMXY[7] += 1;
	GEM4Y_pos.push_back(pos2y);
	GEM4Y_cls.push_back(cl);
      }
      if(etapart==5) { // 10X20
	BX_RecHits_10X20_Ch3iE5->Fill(bx);
	CLS_RecHits_10X20_Ch3iE5->Fill(cl);
	POS_RecHits_10X20_Ch3iE5->Fill(pos);
	// re-calculate position
	double pitch_10x20 = 0.400; // mm
	double pos2_10x20  = -51.20 + pitch_10x20*(centerstrip-0.5); // in mm
	MYPOS_ALL_RecHits_10X20_Ch3iE5->Fill(pos2_10x20);
	if(cl==1) MYPOS_CL1_RecHits_10X20_Ch3iE5->Fill(pos2_10x20);
	for(int i=0; i<cl; ++i) { STR_RecHits_10X20_Ch3iE5->Fill(st+i); }
	// to-be implemented
	// nGEMXY[7] += 1;
	// GEM4Y_pos.push_back(pos2y);
	// GEM4Y_cls.push_back(cl);
      }
    }
    // GE21
    if((mapVersion==1 && region == -1 && station == 2 && layer==1 && chamber==1) || (mapVersion==2 && region == 1 && station == 2 && layer==1 && chamber==1)) {
      if(etapart==13) {
	BX_RecHits_GE21_iE13->Fill(bx);
	CLS_RecHits_GE21_iE13->Fill(cl);
	POS_RecHits_GE21_iE13->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_GE21_iE13->Fill(st+i); }
	nGE21[0] += 1;
      }
      if(etapart==14) {
	BX_RecHits_GE21_iE14->Fill(bx);
	CLS_RecHits_GE21_iE14->Fill(cl);
	POS_RecHits_GE21_iE14->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_GE21_iE14->Fill(st+i); }
	nGE21[1] += 1;
      }
      if(etapart==15) {
	BX_RecHits_GE21_iE15->Fill(bx);
	CLS_RecHits_GE21_iE15->Fill(cl);
	POS_RecHits_GE21_iE15->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_GE21_iE15->Fill(st+i); }
	nGE21[2] += 1;
      }
      if(etapart==16) {
	BX_RecHits_GE21_iE16->Fill(bx);
	CLS_RecHits_GE21_iE16->Fill(cl);
	POS_RecHits_GE21_iE16->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_GE21_iE16->Fill(st+i); }
	nGE21[3] += 1;
      }
    }
    // ME0
    if(region == 1 && station == 0 && layer==1 && chamber==1) {
      if(etapart==1) {
	BX_RecHits_ME0_iE1->Fill(bx);
	CLS_RecHits_ME0_iE1->Fill(cl);
	POS_RecHits_ME0_iE1->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE1->Fill(st+i); }
	// nME0[0] += 1;
      }
      if(etapart==2) {
	BX_RecHits_ME0_iE2->Fill(bx);
	CLS_RecHits_ME0_iE2->Fill(cl);
	POS_RecHits_ME0_iE2->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE2->Fill(st+i); }
	// nME0[1] += 1;
      }
      if(etapart==3) {
	BX_RecHits_ME0_iE3->Fill(bx);
	CLS_RecHits_ME0_iE3->Fill(cl);
	POS_RecHits_ME0_iE3->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE3->Fill(st+i); }
	// nME0[2] += 1;
      }
      if(etapart==4) {
	BX_RecHits_ME0_iE4->Fill(bx);
	CLS_RecHits_ME0_iE4->Fill(cl);
	POS_RecHits_ME0_iE4->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE4->Fill(st+i); }
	// nME0[3] += 1;
      }
      if(etapart==5) {
	BX_RecHits_ME0_iE5->Fill(bx);
	CLS_RecHits_ME0_iE5->Fill(cl);
	POS_RecHits_ME0_iE5->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE5->Fill(st+i); }
	nME0[0] += 1;
      }
      if(etapart==6) {
	BX_RecHits_ME0_iE6->Fill(bx);
	CLS_RecHits_ME0_iE6->Fill(cl);
	POS_RecHits_ME0_iE6->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE6->Fill(st+i); }
	nME0[1] += 1;
      }
      if(etapart==7) {
	BX_RecHits_ME0_iE7->Fill(bx);
	CLS_RecHits_ME0_iE7->Fill(cl);
	POS_RecHits_ME0_iE7->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE7->Fill(st+i); }
	nME0[2] += 1;
      }
      if(etapart==8) {
	BX_RecHits_ME0_iE8->Fill(bx);
	CLS_RecHits_ME0_iE8->Fill(cl);
	POS_RecHits_ME0_iE8->Fill(pos);
	for(int i=0; i<cl; ++i) { STR_RecHits_ME0_iE8->Fill(st+i); }
	nME0[3] += 1;
      }
    }
  } // END LOOP OVER RECHITS

  // Fill NHits/Event
  NHIT_RecHits_GEM1X_Ch1iE1->Fill(nGEMXY[0]);  NHIT_RecHits_GEM1Y_Ch1iE2->Fill(nGEMXY[1]);  CORR_ALL_NHIT_GEM1XY->Fill(nGEMXY[0],nGEMXY[1]);
  NHIT_RecHits_GEM2X_Ch1iE3->Fill(nGEMXY[2]);  NHIT_RecHits_GEM2Y_Ch1iE4->Fill(nGEMXY[3]);  CORR_ALL_NHIT_GEM2XY->Fill(nGEMXY[2],nGEMXY[3]);
  NHIT_RecHits_GEM3X_Ch3iE1->Fill(nGEMXY[4]);  NHIT_RecHits_GEM3Y_Ch3iE2->Fill(nGEMXY[5]);  CORR_ALL_NHIT_GEM3XY->Fill(nGEMXY[4],nGEMXY[5]);
  NHIT_RecHits_GEM4X_Ch3iE3->Fill(nGEMXY[6]);  NHIT_RecHits_GEM4Y_Ch3iE4->Fill(nGEMXY[7]);  CORR_ALL_NHIT_GEM4XY->Fill(nGEMXY[6],nGEMXY[7]);

  int n_count_1sthit = 0, n_count_2ndhit = 0, n_count_3rdhit = 0;
  bool many_hit = false;
  bool event_tagged = false;
  for(int j=0; j<8; ++j) {
    if(nGEMXY[j] == 1) { ++n_count_1sthit; }
    if(nGEMXY[j] == 2) { ++n_count_2ndhit; }
    if(nGEMXY[j] == 3) { ++n_count_3rdhit; }
    if(nGEMXY[j] > 3)  { many_hit = true;  }
  }
  if(!many_hit && n_count_1sthit==0 && n_count_2ndhit==0 && n_count_3rdhit==0) {
    CORR_EVT_GEMXY_v1->Fill(1);
    event_tagged = true;
  }
  if(!many_hit && n_count_1sthit>0 && !n_count_2ndhit && !n_count_3rdhit) {
    if(n_count_1sthit < 6)      CORR_EVT_GEMXY_v1->Fill(2);
    else if(n_count_1sthit < 8) CORR_EVT_GEMXY_v1->Fill(3);
    else CORR_EVT_GEMXY_v1->Fill(4);
    event_tagged = true;
  }
  if(!many_hit && n_count_2ndhit>0 && !n_count_3rdhit) {
    if(n_count_2ndhit < 6) CORR_EVT_GEMXY_v1->Fill(5);
    else if(n_count_2ndhit < 8) CORR_EVT_GEMXY_v1->Fill(6);
    else CORR_EVT_GEMXY_v1->Fill(7);
    event_tagged = true;
  }
  if(!many_hit && n_count_3rdhit>0) {
    if(n_count_3rdhit < 6) CORR_EVT_GEMXY_v1->Fill(8);
    else if(n_count_3rdhit < 8) CORR_EVT_GEMXY_v1->Fill(9);
    else CORR_EVT_GEMXY_v1->Fill(10);
    event_tagged = true;
  }
  if(many_hit) {CORR_EVT_GEMXY_v1->Fill(11); event_tagged = true; }
  if(!event_tagged) CORR_EVT_GEMXY_v1->Fill(12);

  // BIN  1 :: rubbish ... 0-5 tracking hits in different detectors                || 0 < n_count_1sthit < 6
  // BIN  2 :: low quality ... 6-7 tracking hits                                   || 5 < n_count_1sthit < 8
  // BIN  3 :: high quality ... all trackers (XY) have exactly 1 hit               || n_count_1sthit == 8
  // BIN  4 :: high Q + Noise ... all trackers (XY) have > 1 hit (but not all 2)   || n_count_1sthit == 8 && 0 < n_count_2ndhit < 6
  // BIN  5 :: low quality double track candidate ... 6-7 detectors have 2 hits    || n_count_1sthit == 8 && 5 < n_count_2ndhit < 8
  // BIN  6 :: high quality double track candidate .. all trackers have 2 hits     || n_count_1sthit == 8 && n_count_2ndhit == 8
  // BIN  7 :: 2-track + Noise                                                     ||
  // BIN  8:: low quality triple track
  // BIN  9 :: high quality triple track        
  // bin 10 :: more hits
  // bin 11 :: rubbish ... some detectors with 2-3 hits but not all detectors with at least 1 hit

  // 2D Correlation Plot
  // BINS :: [ 1X 0hit | 1X 1hit | 1X 2 hit | 1X 3 hit | 1X > 3hit ] x 8 detectors = 40x40
  for(int i=0; i<8; ++i) {
    int nhits_x_gemxy = nGEMXY[i]; if(nhits_x_gemxy > 3) {nhits_x_gemxy=4;}
    int bin_x = i + 8*nhits_x_gemxy + 1;
    for(int j=0; j<8; ++j) {
      int nhits_y_gemxy = nGEMXY[j]; if(nhits_y_gemxy > 3) {nhits_y_gemxy=4;}
      int bin_y = j + 8*nhits_y_gemxy + 1;
      CORR_EVT_GEMXY_v2->Fill(bin_x,bin_y);
    }
  }


  NHIT_RecHits_GE21_iE13->Fill(nGE21[0]);
  NHIT_RecHits_GE21_iE14->Fill(nGE21[1]);
  NHIT_RecHits_GE21_iE15->Fill(nGE21[2]);
  NHIT_RecHits_GE21_iE16->Fill(nGE21[3]);
  NHIT_RecHits_ME0_iE5->Fill(nME0[0]);
  NHIT_RecHits_ME0_iE6->Fill(nME0[1]);
  NHIT_RecHits_ME0_iE7->Fill(nME0[2]);
  NHIT_RecHits_ME0_iE8->Fill(nME0[3]);


  // Fill counted RECHITS
  TOT_RecHits_GEM->Fill(1); // nEvents;
  for(int i=0; i<8; ++i) {
    if(nGEMXY[i]  > 0) TOT_RecHits_GEM->Fill(i+2);
  }
  TOT_RecHits_GE21->Fill(1); // nEvents;
  for(int i=0; i<8; ++i) {
    if(nGE21[i]  > 0) TOT_RecHits_GE21->Fill(i+2);
  }
  TOT_RecHits_ME0->Fill(1); // nEvents;
  for(int i=0; i<8; ++i) {
    if(nME0[i]  > 0) TOT_RecHits_ME0->Fill(i+2);
  }

  // TRY TO MATCH X and Y RecHits
  std::vector<std::string> detname = {"GEM1XY", "GEM2XY", "GEM3XY", "GEM4XY"};
  std::vector< std::vector< double > > GEMX_pos, GEMY_pos, GEMX_cls, GEMY_cls;
  GEMX_pos.push_back(GEM1X_pos);   GEMX_pos.push_back(GEM2X_pos);   GEMX_pos.push_back(GEM3X_pos);   GEMX_pos.push_back(GEM4X_pos);
  GEMY_pos.push_back(GEM1Y_pos);   GEMY_pos.push_back(GEM2Y_pos);   GEMY_pos.push_back(GEM3Y_pos);   GEMY_pos.push_back(GEM4Y_pos);
  GEMX_cls.push_back(GEM1X_cls);   GEMX_cls.push_back(GEM2X_cls);   GEMX_cls.push_back(GEM3X_cls);   GEMX_cls.push_back(GEM4X_cls);
  GEMY_cls.push_back(GEM1Y_cls);   GEMY_cls.push_back(GEM2Y_cls);   GEMY_cls.push_back(GEM3Y_cls);   GEMY_cls.push_back(GEM4Y_cls);
  // std::vector< TH2F* > PRF2D_RecHits_GEMXY; 
  // PRF2D_RecHits_GEMXY.push_back(PRF2D_RecHits_GEM1XY_Ch1iE1iE2);   PRF2D_RecHits_GEMXY.push_back(PRF2D_RecHits_GEM2XY_Ch1iE3iE4); 
  // PRF2D_RecHits_GEMXY.push_back(PRF2D_RecHits_GEM3XY_Ch3iE1iE2);   PRF2D_RecHits_GEMXY.push_back(PRF2D_RecHits_GEM4XY_Ch3iE3iE4); 

  for(int i=0; i<4; ++i) { // Loop over GEM Trackers
    int n_hitsx = nGEMXY[2*i];
    int n_hitsy = nGEMXY[2*i+1];

    if(n_hitsx==1 && n_hitsy==1) { // select only events with 1 hit in X and Y
      // Make POS profile of clean events
      // Make CLS distribution of clean events
      if(i==0) { 
	SH_CLS_RecHits_GEM1X_Ch1iE1->Fill(GEMX_cls[0][0]); SH_POS_RecHits_GEM1X_Ch1iE1->Fill(GEMX_pos[0][0]); 
	SH_CLS_RecHits_GEM1Y_Ch1iE2->Fill(GEMY_cls[0][0]); SH_POS_RecHits_GEM1Y_Ch1iE2->Fill(GEMY_pos[0][0]);
	CORR_SH_CLS_GEM1XY->Fill(GEMX_cls[0][0],GEMY_cls[0][0]);
      }
      if(i==1) { 
	SH_CLS_RecHits_GEM2X_Ch1iE3->Fill(GEMX_cls[1][0]); SH_POS_RecHits_GEM2X_Ch1iE3->Fill(GEMX_pos[1][0]); 
	SH_CLS_RecHits_GEM2Y_Ch1iE4->Fill(GEMY_cls[1][0]); SH_POS_RecHits_GEM2Y_Ch1iE4->Fill(GEMY_pos[1][0]);
	CORR_SH_CLS_GEM2XY->Fill(GEMX_cls[1][0],GEMY_cls[1][0]);
      }
      if(i==2) { 
	SH_CLS_RecHits_GEM3X_Ch3iE1->Fill(GEMX_cls[2][0]); SH_POS_RecHits_GEM3X_Ch3iE1->Fill(GEMX_pos[2][0]); 
	SH_CLS_RecHits_GEM3Y_Ch3iE2->Fill(GEMY_cls[2][0]); SH_POS_RecHits_GEM3Y_Ch3iE2->Fill(GEMY_pos[2][0]);
	CORR_SH_CLS_GEM3XY->Fill(GEMX_cls[2][0],GEMY_cls[2][0]);
      }
      if(i==3) { SH_CLS_RecHits_GEM4X_Ch3iE3->Fill(GEMX_cls[3][0]); SH_POS_RecHits_GEM4X_Ch3iE3->Fill(GEMX_pos[3][0]); 
	SH_CLS_RecHits_GEM4Y_Ch3iE4->Fill(GEMY_cls[3][0]); SH_POS_RecHits_GEM4Y_Ch3iE4->Fill(GEMY_pos[3][0]);
	CORR_SH_CLS_GEM4XY->Fill(GEMX_cls[3][0],GEMY_cls[3][0]);
      }
      // Loop over hits (formally .... we have only 1 hit here)
      for(int j=0; j<n_hitsx; ++j) { // Loop over X-hits
	for(int k=0; k<n_hitsy; ++k) { // Loop over Y-hits
	  // PRF2D_RecHits_GEMXY[i]->Fill(GEMX_pos[i][j],GEMY_pos[i][k]);
	  if(i==0) PRF2D_SH_RecHits_GEM1XY_Ch1iE1iE2->Fill(GEMX_pos[i][j],GEMY_pos[i][k]);
	  if(i==1) PRF2D_SH_RecHits_GEM2XY_Ch1iE3iE4->Fill(GEMX_pos[i][j],GEMY_pos[i][k]);
	  if(i==2) PRF2D_SH_RecHits_GEM3XY_Ch3iE1iE2->Fill(GEMX_pos[i][j],GEMY_pos[i][k]);
	  if(i==3) PRF2D_SH_RecHits_GEM4XY_Ch3iE3iE4->Fill(GEMX_pos[i][j],GEMY_pos[i][k]);
	}
      }
      // Print-out good matching
      if(dbg_match) {
	std::cout<<" - - - - Good Matching in "<<detname[i]<<" - - - - "<<std::endl;
	for(int j=0; j<n_hitsx; ++j) { // Loop over X-hits
	  std::cout<<" - - - - - - - - GEM X hit: pos = "<<GEMX_pos[i][j]<<" with cls = "<<GEMX_cls[i][j]<<std::endl;
	  for(int k=0; k<n_hitsy; ++k) { // Loop over Y-hits
	    std::cout<<" - - - - - - - - - - - - GEM Y hit: pos = "<<GEMY_pos[i][k]<<" with cls = "<<GEMY_cls[i][k]<<std::endl;
	  }
	}
      }
    } // END selection of hits with 1X and 1Y hit
 
    // TODO :: perform some tests ... when multiple hits... std::cout cls of hits
    else {
      // Print-out ambigious matching
      if(dbg_match) {
	std::cout<<" - - - - Ambigious Matching in "<<detname[i]<<" - - - - "<<std::endl;
	for(int j=0; j<n_hitsx; ++j) { // Loop over X-hits
	  std::cout<<" - - - - - - - - GEM X hit: pos = "<<GEMX_pos[i][j]<<" with cls = "<<GEMX_cls[i][j]<<std::endl;
	  for(int k=0; k<n_hitsy; ++k) { // Loop over Y-hits
	  std::cout<<" - - - - - - - - - - - - GEM Y hit: pos = "<<GEMY_pos[i][k]<<" with cls = "<<GEMY_cls[i][k]<<std::endl;
	  }
	}
      }
      // Try to recover --- starting from most easy situations first
      if(n_hitsx < 3 && n_hitsy < 3) {
	for(int j=0; j<n_hitsx; ++j) { // Loop over X-hits
	  for(int k=0; k<n_hitsy; ++k) { // Loop over Y-hits
	    // Fill histograms ...
	    if(i==0) CORR_2H_CLS_GEM1XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
	    if(i==1) CORR_2H_CLS_GEM2XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
	    if(i==2) CORR_2H_CLS_GEM3XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
	    if(i==3) CORR_2H_CLS_GEM4XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
	    // search for matches
	    if(GEMX_cls[i][j] == GEMY_cls[i][k]) // match found ... should do something clever to remove combination from the loop
	      {}
	  }
	}
      }
      else if(n_hitsx < 4 && n_hitsy < 4) {
        for(int j=0; j<n_hitsx; ++j) { // Loop over X-hits
          for(int k=0; k<n_hitsy; ++k) { // Loop over Y-hits
            // Fill histograms ...
            if(i==0) CORR_3H_CLS_GEM1XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
            if(i==1) CORR_3H_CLS_GEM2XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
            if(i==2) CORR_3H_CLS_GEM3XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
            if(i==3) CORR_3H_CLS_GEM4XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);
            // search for matches 
            if(GEMX_cls[i][j] == GEMY_cls[i][k]) // match found ... should do something clever to remove combination from the loop
              {}
          }
        }
      } 
      else {} 
    } // END ELSE nHits > 

    // Brutto Rutto ... make plot of all combinations
    for(int j=0; j<n_hitsx; ++j) { // Loop over X-hits
      for(int k=0; k<n_hitsy; ++k) { // Loop over Y-hits
	// PRF2D_RecHits_GEMXY[i]->Fill(GEMX_pos[i][j],GEMY_pos[i][k]);
	if(i==0) { PRF2D_ALL_RecHits_GEM1XY_Ch1iE1iE2->Fill(GEMX_pos[i][j],GEMY_pos[i][k]); CORR_ALL_CLS_GEM1XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]); }
	if(i==1) { PRF2D_ALL_RecHits_GEM2XY_Ch1iE3iE4->Fill(GEMX_pos[i][j],GEMY_pos[i][k]); CORR_ALL_CLS_GEM2XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);}
	if(i==2) { PRF2D_ALL_RecHits_GEM3XY_Ch3iE1iE2->Fill(GEMX_pos[i][j],GEMY_pos[i][k]); CORR_ALL_CLS_GEM3XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);}
	if(i==3) { PRF2D_ALL_RecHits_GEM4XY_Ch3iE3iE4->Fill(GEMX_pos[i][j],GEMY_pos[i][k]); CORR_ALL_CLS_GEM4XY->Fill(GEMX_cls[i][j],GEMY_cls[i][k]);}
      }
    }
  }


  // ================
  // GEM digis
  // ================
  edm::Handle<GEMDigiCollection> gemDigis;
  iEvent.getByToken(GEMDigi_Token,gemDigis);

  for( GEMDigiCollection::DigiRangeIterator collectionItr=gemDigis->begin(); collectionItr!=gemDigis->end(); ++collectionItr){
    
    GEMDetId rollId = (GEMDetId)(*collectionItr).first;
    int region  = rollId.region();
    int station = rollId.station();
    int layer   = rollId.layer();
    int chamber = rollId.chamber();
    int etapart = rollId.roll();

    // skip un-necessary rolls 
    if(!(region == 1 && station == 0 && layer==1 && chamber==1)) continue;

    // continue for ME0 chamber in TB
    GEMDigiCollection::const_iterator digiItr1, digiItr2;
    std::vector<GEMDigi> MyDigiVector;
    std::vector<GEMDigi>::const_iterator MyDigiVectorItr;

    // count digis in this roll
    int nDigis = 0;
    for (digiItr1 =(*collectionItr).second.first; digiItr1 != (*collectionItr).second.second; ++digiItr1){ 
      ++nDigis;
      GEMDigi digi1  = (GEMDigi)(*digiItr1);
      int     strip1 = digi1.strip();
      if(etapart==1) {DIG_RecHits_ME0_iE1->Fill(strip1);} 
      if(etapart==2) {DIG_RecHits_ME0_iE2->Fill(strip1);} 
      if(etapart==3) {DIG_RecHits_ME0_iE3->Fill(strip1);} 
      if(etapart==4) {DIG_RecHits_ME0_iE4->Fill(strip1);} 
      if(etapart==5) {DIG_RecHits_ME0_iE5->Fill(strip1);} 
      if(etapart==6) {DIG_RecHits_ME0_iE6->Fill(strip1);} 
      if(etapart==7) {DIG_RecHits_ME0_iE7->Fill(strip1);} 
      if(etapart==8) {DIG_RecHits_ME0_iE8->Fill(strip1);} 
    }

    // loop on digis of given roll    
    for (digiItr1 =(*collectionItr).second.first; digiItr1 != (*collectionItr).second.second; ++digiItr1){
      if(dbg_test) {
	std::cout<<"GEM Digi in ["<<rollId<<"] (total = "<<nDigis<<"digis) : strip = "<<std::setw(2)<<(*digiItr1).strip()<<" bx = "<<std::setw(2)<<(*digiItr1).bx()<<std::endl;
      }
      GEMDigi digi1  = (GEMDigi)(*digiItr1);
      int     strip1 = digi1.strip();

      // make correlations within the roll :: loop again over all digis in this roll
      // clean up ... only if there are few digis in the roll ... given avg cls of ~3
      if(nDigis>3) continue; 

      for (digiItr2 =(*collectionItr).second.first; digiItr2 != (*collectionItr).second.second; ++digiItr2){ // better to have linking in both directions
      // for (digiItr2 =(*collectionItr).second.first; digiItr2 != digiItr1; ++digiItr2){ // only 1 diagonal half is enough
	GEMDigi digi2  = (GEMDigi)(*digiItr2);
	int     strip2 = digi2.strip();
	if(etapart==1) {  eMapDebug_ME0_iE1->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE1 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
	if(etapart==2) {  eMapDebug_ME0_iE2->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE2 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
	if(etapart==3) {  eMapDebug_ME0_iE3->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE3 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
	if(etapart==4) {  eMapDebug_ME0_iE4->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE4 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
	if(etapart==5) {  eMapDebug_ME0_iE5->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE5 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
	if(etapart==6) {  eMapDebug_ME0_iE6->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE6 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
	if(etapart==7) {  eMapDebug_ME0_iE7->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE7 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
	if(etapart==8) {  eMapDebug_ME0_iE8->Fill(strip2,strip1); if(dbg_test) {std::cout<<"Filling ME0 iE8 with strips ["<<strip2<<","<<strip1<<"]"<<std::endl;}}
      } 
      // end loop over digi2
    } 
    // end loop over digi1
  } 
  // end loop over rolls in Digi collection
  



}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyTestBeamAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyTestBeamAnalyzer);
