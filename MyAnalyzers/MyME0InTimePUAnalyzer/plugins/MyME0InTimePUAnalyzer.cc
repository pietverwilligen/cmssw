// -*- C++ -*-
//
// Package:    MyME0InTimePUAnalyzer
// Class:      MyME0InTimePUAnalyzer
// 
/**\class MyME0InTimePUAnalyzer MyME0InTimePUAnalyzer.cc MyAnalyzers/MyME0InTimePUAnalyzer/plugins/MyME0InTimePUAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Piet Verwilligen
//         Created:  Wed, 07 Oct 2015 08:29:01 GMT
// $Id$
//
//


// ToDo ::
//         - implement RPC Matching ... some muons in the Barrel need it
//         - Investigate matching ... seems i am matching out of time ME0Segments with in time ME0SimHits ???
//         - Plot SimHit Time Resolution ... this is an inherent resolution, a smearing on top of the detector time resolution smearing
//         - Plot Quantities for all ME0Segments reconstructed ... for instance the SegTimeCreation
//         - Sort also in function of PT in case quality is the same ...
//           |--> study lambda functions
//           |--> implement a class for my std::vector<int,int>
//           |--> maybe i should implement the Association by Hits for both Tracker Track & ME0Segment
//         - SimTrack summary is a useless plot in case several root files are merged together ... need to find a good solution
//         - TrackIdME0HitMap probably need debugging
//         - tune matching of DT ... for now it seems to go wrong in the eta-SL. 
//               Not sure whether DT hits capture 1D or 2D information ... I guess 1D ...
//               eta-SL reads also along x-axis (just as phi-SL) so matching should only be done on that coordinate
//               try to understand few cases where #matched DT hits > # DT hits
//         - change to continuous quality variable instead of discrete number of hits / number of segments
//         - implement totalMuonHits matched and require total number of CSC/DT hits matched > 75%
//         - implement old matching as done by Amandeep

// system include files
#include <memory>
#include <fstream>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// root include files
#include "TDirectoryFile.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/MuonReco/interface/Muon.h"
// #include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonReco/interface/ME0Muon.h"
#include "DataFormats/MuonReco/interface/ME0MuonCollection.h"
#include "DataFormats/GEMRecHit/interface/ME0Segment.h" 
#include "DataFormats/GEMRecHit/interface/ME0SegmentCollection.h" 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

// #include "RecoMuon/MuonIdentification/interface/ME0MuonSelector.h"
#include "RecoMuon/MuonIdentification/plugins/ME0MuonSelector.cc"

#include "CommonTools/UtilAlgos/interface/TFileService.h"


//
// class declaration
//

class MyME0InTimePUAnalyzer : public edm::EDAnalyzer {
   public:
      explicit MyME0InTimePUAnalyzer(const edm::ParameterSet&);
      ~MyME0InTimePUAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  std::vector<reco::ME0Muon> theME0Muons;
  std::vector<reco::Muon> theMuons;



   private:

      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  // virtual void beginJob() override;
  // virtual void endJob() override;

  bool sortME0muons (const std::pair<int,int> &, const std::pair<int,int> &);
  // bool sortME0muons (const std::pair<int,int>, const std::pair<int,int>);
  // bool sortME0muons (const std::pair<int,int>::iterator, const std::pair<int,int>::iterator);
  bool checkVector(std::vector<int>&, int);
  double maxTimeSpan(const std::vector<ME0RecHit>);
  GlobalPoint AverageHitPos(const std::vector<ME0RecHit>);
  // Output Files / TFile Service
  // edm::Service<TFileService> fs;
  std::string rootFileName;
  std::unique_ptr<TFile> outputfile;

  // Info Bool
  bool printInfoHepMC, printInfoSignal, printInfoPU, printInfoAll, printInfoME0Match, printInfoMuonMatch, printInfoMuonMatchDetail, me0genpartfound;
  bool InvestigateOnlyME0;
  // For later use in 7XY releases:
  /*
  edm::EDGetTokenT<reco::GenParticleCollection> GENParticle_Token;
  edm::EDGetTokenT<edm::HepMCProduct>           HEPMCCol_Token;
  edm::EDGetTokenT<edm::SimTrackContainer>      SIMTrack_Token;
  edm::EDGetTokenT<CSCSegmentCollection>        CSCSegment_Token;
  edm::EDGetTokenT<ME0RecHitCollection>         ME0RecHit_Token;
  edm::EDGetTokenT<ME0SegmentCollection>        ME0Segment_Token;
  edm::EDGetTokenT<std::vector<reco::ME0Muon>>  ME0Muon_Token;
  edm::EDGetTokenT<std::vector<reco::Muon>      Muon_Token;
  */

  edm::ESHandle<ME0Geometry> me0Geom;
  edm::ESHandle<CSCGeometry> cscGeom;
  edm::ESHandle<DTGeometry> dtGeom;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
  // ----------member data for configuration parameters ---------------------------
  double preDigiSmearX, preDigiSmearY, cscDetResX, cscDetResY, dtDetResX, dtDetResY;
  int nMatchedHitsME0Seg, nMatchedHitsCSCSeg, nMatchedHitsDTSeg;
  // double matchQuality;
  double matchQualityME0, matchQualityReco;
  // ------------------------------------------------------------------------------


  // ----------member data for fundamental vectors in the analysis / matching -----
  // this way easy access can be granted to different functions (sorting functions)
  // sortME0muonsStruct mySortingStruct;
  std::vector<int> indmu, trkmu, vtxmu;
  std::vector< std::vector<const PSimHit*> > simhitme0mu; 
  std::vector< std::vector<const PSimHit*> > simhitrecomu; 
  std::vector<ME0DetId> me0mudetid;   // std::vector<CSCDetId> cscmudetid;  std::vector<DTDetId> dtmudetid;  
  std::vector< std::vector< std::pair<int,int> > > me0mu;         // old style :: pair of < [int] index to me0muon position in collection, [int] number of matched hits > 
  std::vector< std::vector< std::pair<int,int> > > recomu;        // old style :: pair of < [int] index to recomuon position in collection, [int] number of matched segments >
  std::vector< std::vector< std::pair<int,int> > > looseme0mu;    // old style :: pair of < [int] index to me0muon position in collection, [int] number of matched hits > 
  std::vector< std::vector< std::pair<int,int> > > tightme0mu;    // old style :: pair of < [int] index to me0muon position in collection, [int] number of matched hits > 
  // std::vector< std::vector< std::pair<int,double> > > me0mu;   // new style :: pair of < [int] index to me0muon position in collection, [double] match quality >
  // std::vector< std::vector< std::pair<int,double> > > recomu;  // new style :: pair of < [int] index to recomuon position in collection, [double] match quality >
  std::vector< std::map<uint32_t, std::vector<const PSimHit*> > > me0simhitmap;
  std::vector< std::map<uint32_t, std::vector<const PSimHit*> > > cscsimhitmap;
  std::vector< std::map<uint32_t, std::vector<const PSimHit*> > > dtsimhitmap;
  // -------------------------------------------------------------------------------



  // ----------member data for ROOT Files, ROOT Directories and Histograms --------
  // ----------my directories   ----------------------
  // std::unique_ptr<TDirectoryFile> NoMatch, OldMatch, NewMatch;
  std::unique_ptr<TDirectoryFile> Signal_ME0Mu, SimTrack_ME0Mu, NoMatch_AllME0Seg, NewMatch_Details;
  std::unique_ptr<TDirectoryFile> NoMatch_AllME0Mu, NoMatch_TightME0Mu, /*OldMatch_AllME0Mu,*/ NewMatch_AllME0Mu, NewMatch_LooseME0Mu, NewMatch_TightME0Mu;

  // ----------my histos   ---------------------------
  // implement a different ordering ... order by type of matching and ME0muon selection, such that all plots can be implemented first for NewMatch_TightME0Mu

  // --- SimTrack Investigations ---------
  std::unique_ptr<TH1F> SimTrack_All_Eta, SimTrack_ME0Hits_Eta, SimTrack_ME0Hits_NumbHits, SimTrack_All_NumbTracks, SimTrack_ME0Hits_NumbTracks, SimTrack_Summary;
  std::unique_ptr<TH1F> SimHits_ME0Hits_Eta, SimHits_ME0Hits_Phi, SimHits_ME0Hits_TOF;
  std::unique_ptr<TH1F> ME0RecHits_NumbHits, ME0Segments_NumbSeg, ME0Muons_NumbMuons;
  std::unique_ptr<TH2F> SimHits_ME0Hits_TOFvsR;
  // -------------------------------------
  std::unique_ptr<TH1F> Categories_EventInfo, Categories_NumberOfME0Segments, Categories_NumberOfME0Muons;

  std::unique_ptr<TH1F> Signal_GenPart_EtaDistr, Signal_GenPart_PhiDistr, Signal_GenPart_PTDistr, Signal_GenPart_InvariantMass;
  std::unique_ptr<TH1F> Signal_SimTrack_EtaDistr, Signal_SimTrack_PhiDistr, Signal_SimTrack_PTDistr, Signal_SimTrack_InvariantMass;

  std::unique_ptr<TH1F> NoMatch_AllME0Seg_SegTimeCreation, NoMatch_AllME0Seg_SegTimeCreaZoom;

  std::unique_ptr<TH1F> NoMatch_AllME0Mu_SegTimeValue, NoMatch_AllME0Mu_SegTimeValZoom, NoMatch_AllME0Mu_SegTimeUncrt, NoMatch_AllME0Mu_TrackPTDistr, NoMatch_AllME0Mu_PTResolution, NoMatch_AllME0Mu_ETAResolution;
  std::unique_ptr<TH1F> NoMatch_AllME0Mu_SegNumberOfHits, NoMatch_AllME0Mu_SegChi2NDof, NoMatch_AllME0Mu_TrackETADistr, NoMatch_AllME0Mu_TrackPHIDistr;
  std::unique_ptr<TH1F> NoMatch_AllME0Mu_NumbME0Muons, NoMatch_AllME0Mu_NumbME0Segments, NoMatch_AllME0Mu_SegTimeCreation, NoMatch_AllME0Mu_SegTimeCreaZoom;
  std::unique_ptr<TH1F> NoMatch_AllME0Mu_SegETADir, NoMatch_AllME0Mu_SegETAPos, NoMatch_AllME0Mu_SegPHIDir, NoMatch_AllME0Mu_SegPHIPos;
  std::unique_ptr<TH2F> NoMatch_AllME0Mu_SegPHIvsSimPT, NoMatch_AllME0Mu_ETAvsETARes, NoMatch_AllME0Mu_PTvsETARes, NoMatch_AllME0Mu_ETAvsETARes_5GeV;
  std::unique_ptr<TH1F> NoMatch_AllME0Mu_TrackETADistr_5GeV, NoMatch_AllME0Mu_ETAResolution_5GeV;

  std::unique_ptr<TH1F> NoMatch_TightME0Mu_SegTimeValue, NoMatch_TightME0Mu_SegTimeValZoom, NoMatch_TightME0Mu_SegTimeUncrt, NoMatch_TightME0Mu_TrackPTDistr, NoMatch_TightME0Mu_PTResolution, NoMatch_TightME0Mu_ETAResolution;
  std::unique_ptr<TH1F> NoMatch_TightME0Mu_SegNumberOfHits, NoMatch_TightME0Mu_SegChi2NDof, NoMatch_TightME0Mu_TrackETADistr, NoMatch_TightME0Mu_TrackPHIDistr;
  std::unique_ptr<TH1F> NoMatch_TightME0Mu_NumbME0Muons, NoMatch_TightME0Mu_NumbME0Segments, NoMatch_TightME0Mu_SegTimeCreation, NoMatch_TightME0Mu_SegTimeCreaZoom;
  std::unique_ptr<TH1F> NoMatch_TightME0Mu_SegETADir, NoMatch_TightME0Mu_SegETAPos, NoMatch_TightME0Mu_SegPHIDir, NoMatch_TightME0Mu_SegPHIPos;
  std::unique_ptr<TH2F> NoMatch_TightME0Mu_SegPHIvsSimPT, NoMatch_TightME0Mu_ETAvsETARes, NoMatch_TightME0Mu_PTvsETARes, NoMatch_TightME0Mu_ETAvsETARes_5GeV;
  std::unique_ptr<TH1F> NoMatch_TightME0Mu_TrackETADistr_5GeV, NoMatch_TightME0Mu_ETAResolution_5GeV;

  std::unique_ptr<TH1F> NewMatch_AllME0Mu_SegTimeValue, NewMatch_AllME0Mu_SegTimeValZoom, NewMatch_AllME0Mu_SegTimeUncrt, NewMatch_AllME0Mu_TrackPTDistr, NewMatch_AllME0Mu_PTResolution, NewMatch_AllME0Mu_ETAResolution;
  std::unique_ptr<TH1F> NewMatch_AllME0Mu_SegNumberOfHits, NewMatch_AllME0Mu_SegChi2NDof, NewMatch_AllME0Mu_TrackETADistr, NewMatch_AllME0Mu_TrackPHIDistr, NewMatch_AllME0Mu_SegTimeCreation, NewMatch_AllME0Mu_SegTimeCreaZoom;
  std::unique_ptr<TH1F> NewMatch_AllME0Mu_InvariantMass_All, NewMatch_AllME0Mu_InvariantMass_2ME0Mu, NewMatch_AllME0Mu_InvariantMass_ME0MuRecoMu, NewMatch_AllME0Mu_InvariantMass_2RecoMu;
  std::unique_ptr<TH1F> NewMatch_AllME0Mu_SegETADir, NewMatch_AllME0Mu_SegETAPos, NewMatch_AllME0Mu_SegPHIDir, NewMatch_AllME0Mu_SegPHIPos;
  std::unique_ptr<TH2F> NewMatch_AllME0Mu_SegPHIvsSimPT, NewMatch_AllME0Mu_ETAvsETARes, NewMatch_AllME0Mu_PTvsETARes;

  std::unique_ptr<TH1F> NewMatch_LooseME0Mu_SegTimeValue, NewMatch_LooseME0Mu_SegTimeValZoom, NewMatch_LooseME0Mu_SegTimeUncrt, NewMatch_LooseME0Mu_TrackPTDistr, NewMatch_LooseME0Mu_PTResolution, NewMatch_LooseME0Mu_ETAResolution,;
  std::unique_ptr<TH1F> NewMatch_LooseME0Mu_SegNumberOfHits, NewMatch_LooseME0Mu_SegChi2NDof, NewMatch_LooseME0Mu_TrackETADistr, NewMatch_LooseME0Mu_TrackPHIDistr, NewMatch_LooseME0Mu_SegTimeCreation, NewMatch_LooseME0Mu_SegTimeCreaZoom;
  std::unique_ptr<TH1F> NewMatch_LooseME0Mu_InvariantMass_All, NewMatch_LooseME0Mu_InvariantMass_2ME0Mu, NewMatch_LooseME0Mu_InvariantMass_ME0MuRecoMu, NewMatch_LooseME0Mu_InvariantMass_2RecoMu;
  std::unique_ptr<TH1F> NewMatch_LooseME0Mu_SegETADir, NewMatch_LooseME0Mu_SegETAPos, NewMatch_LooseME0Mu_SegPHIDir, NewMatch_LooseME0Mu_SegPHIPos;
  std::unique_ptr<TH2F> NewMatch_LooseME0Mu_SegPHIvsSimPT, NewMatch_LooseME0Mu_ETAvsETARes, NewMatch_LooseME0Mu_PTvsETARes;

  std::unique_ptr<TH1F> NewMatch_TightME0Mu_SegTimeValue, NewMatch_TightME0Mu_SegTimeValZoom, NewMatch_TightME0Mu_SegTimeUncrt, NewMatch_TightME0Mu_TrackPTDistr, NewMatch_TightME0Mu_PTResolution, NewMatch_TightME0Mu_ETAResolution,;
  std::unique_ptr<TH1F> NewMatch_TightME0Mu_SegNumberOfHits, NewMatch_TightME0Mu_SegChi2NDof, NewMatch_TightME0Mu_TrackETADistr, NewMatch_TightME0Mu_TrackPHIDistr, NewMatch_TightME0Mu_SegTimeCreation, NewMatch_TightME0Mu_SegTimeCreaZoom;
  std::unique_ptr<TH1F> NewMatch_TightME0Mu_SegETADir, NewMatch_TightME0Mu_SegETAPos, NewMatch_TightME0Mu_SegPHIDir, NewMatch_TightME0Mu_SegPHIPos;
  std::unique_ptr<TH1F> NewMatch_TightME0Mu_InvariantMass_All, NewMatch_TightME0Mu_InvariantMass_2ME0Mu, NewMatch_TightME0Mu_InvariantMass_ME0MuRecoMu, NewMatch_TightME0Mu_InvariantMass_2RecoMu;
  std::unique_ptr<TH2F> NewMatch_TightME0Mu_SegPHIvsSimPT, NewMatch_TightME0Mu_ETAvsETARes, NewMatch_TightME0Mu_PTvsETARes, NewMatch_TightME0Mu_ETAvsETARes_5GeV;
  std::unique_ptr<TH1F> NewMatch_TightME0Mu_TrackETADistr_5GeV, NewMatch_TightME0Mu_ETAResolution_5GeV;

  std::unique_ptr<TH1F> NewMatch_ME0HitsMatched, NewMatch_ME0HitsTotal, NewMatch_CSCHitsMatched, NewMatch_CSCHitsTotal, NewMatch_DTHitsMatched, NewMatch_DTHitsTotal, NewMatch_SegmentsMatched, NewMatch_SegmentsTotal;
  std::unique_ptr<TH1F> NewMatch_ME0MuonsMatched, NewMatch_RecoMuonsMatched, NewMatch_LooseME0MuonsMatched, NewMatch_TightME0MuonsMatched;
};

//
// constants, enums and typedefs
//
double me0mineta = 2.00;
double me0maxeta = 3.00;

//
// static data member definitions
//

//
// constructors and destructor
//
MyME0InTimePUAnalyzer::MyME0InTimePUAnalyzer(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  // make plots with finer binning

  rootFileName  = iConfig.getUntrackedParameter<std::string>("RootFileName");
  InvestigateOnlyME0 = iConfig.getUntrackedParameter<bool>("InvestigateOnlyME0");
  outputfile.reset(TFile::Open(rootFileName.c_str(), "RECREATE"));

  preDigiSmearX      = iConfig.getUntrackedParameter<double>("preDigiSmearX");
  preDigiSmearY      = iConfig.getUntrackedParameter<double>("preDigiSmearY");
  cscDetResX         = iConfig.getUntrackedParameter<double>("cscDetResX");
  cscDetResY         = iConfig.getUntrackedParameter<double>("cscDetResY");
  dtDetResX          = iConfig.getUntrackedParameter<double>("dtDetResX");
  dtDetResY          = iConfig.getUntrackedParameter<double>("dtDetResY");
  nMatchedHitsME0Seg = iConfig.getUntrackedParameter<int>("nMatchedHitsME0Seg");
  nMatchedHitsCSCSeg = iConfig.getUntrackedParameter<int>("nMatchedHitsCSCSeg");
  nMatchedHitsDTSeg  = iConfig.getUntrackedParameter<int>("nMatchedHitsDTSeg");
  // matchQuality       = iConfig.getUntrackedParameter<double>("matchQuality");   // supose to replace the nMatchedHits parameters above .... although need to fix the DT matching ... something still goes wrong there ... for now working with > 3 hits is ok.
  matchQualityME0    = iConfig.getUntrackedParameter<double>("matchQualityME0");   
  matchQualityReco   = iConfig.getUntrackedParameter<double>("matchQualityReco");   

  printInfoHepMC  = iConfig.getUntrackedParameter<bool>("printInfoHepMC");
  printInfoSignal = iConfig.getUntrackedParameter<bool>("printInfoSignal");
  printInfoPU     = iConfig.getUntrackedParameter<bool>("printInfoPU");
  printInfoAll    = iConfig.getUntrackedParameter<bool>("printInfoAll");
  printInfoME0Match   = iConfig.getUntrackedParameter<bool>("printInfoME0Match");
  printInfoMuonMatch  = iConfig.getUntrackedParameter<bool>("printInfoMuonMatch");
  printInfoMuonMatchDetail  = iConfig.getUntrackedParameter<bool>("printInfoMuonMatchDetail");
  // For later use in 7XY releases:
  /*
  GENParticle_Token   = consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
  HEPMCCol_Token      = consumes<edm::HepMCProduct>(edm::InputTag("generator"));
  SIMVertex_Token     = consumes<edm::SimVertexContainer>(edm::InputTag("g4SimHits")); // or consumes<std::vector<SimVertex>>
  SIMTrack_Token      = consumes<edm::SimTrackContainer>(edm::InputTag("g4SimHits"));
  consumesMany<edm::PSimHitContainer>();
  PrimaryVertex_Token = consumes<std::vector<reco::Vertex>>(edm::InputTag("offlinePrimaryVertices"));
  ME0RecHit_Token    = consumes<ME0RecHitCollection>(edm::InputTag("me0RecHits"));
  ME0Segment_Token    = consumes<ME0SegmentCollection>(edm::InputTag("me0Segments"));
  ME0Muon_Token       = consumes<std::vector<reco::ME0Muon>>(edm::InputTag("me0SegmentMatching"));
  Muon_Token          = consumes<std::vector<reco::Muon>>(edm::InputTag("muons"));
  */


  Signal_ME0Mu        = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("Signal_ME0Mu",        "Signal_ME0Mu"));
  SimTrack_ME0Mu      = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("SimTrack_ME0Mu",      "SimTrack_ME0Mu"));
  NoMatch_AllME0Seg   = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("NoMatch_AllME0Seg",   "NoMatch_AllME0Seg"));
  NoMatch_AllME0Mu    = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("NoMatch_AllME0Mu",    "NoMatch_AllME0Mu"));
  NoMatch_TightME0Mu  = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("NoMatch_TightME0Mu",  "NoMatch_TightME0Mu"));
  // OldMatch_AllME0Mu   = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("OldMatch_AllME0Mu",   "OldMatch_AllME0Mu"));
  // OldMatch_TightME0Mu = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("OldMatch_TightME0Mu", "OldMatch_TightME0Mu"));
  NewMatch_Details    = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("NewMatch_Details",   "NewMatch_Details"));
  NewMatch_AllME0Mu   = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("NewMatch_AllME0Mu",   "NewMatch_AllME0Mu"));
  NewMatch_LooseME0Mu = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("NewMatch_LooseME0Mu", "NewMatch_LooseME0Mu"));
  NewMatch_TightME0Mu = std::unique_ptr<TDirectoryFile>(new TDirectoryFile("NewMatch_TightME0Mu", "NewMatch_TightME0Mu"));

  Signal_GenPart_EtaDistr          = std::unique_ptr<TH1F>(new TH1F("Signal_GenPart_EtaDistr",        "Signal_GenPart_EtaDistr", 100, 1.50,3.50));
  Signal_GenPart_PhiDistr          = std::unique_ptr<TH1F>(new TH1F("Signal_GenPart_PhiDistr",        "Signal_GenPart_PhiDistr", 144, -3.14,3.14));
  Signal_GenPart_PTDistr           = std::unique_ptr<TH1F>(new TH1F("Signal_GenPart_PTDistr",         "Signal_GenPart_PTDistr",  200, 000,100));
  Signal_GenPart_InvariantMass     = std::unique_ptr<TH1F>(new TH1F("Signal_GenPart_InvariantMass",   "Signal_GenPart_InvariantMass",  300, 000,150));
  Signal_SimTrack_EtaDistr         = std::unique_ptr<TH1F>(new TH1F("Signal_SimTrack_EtaDistr",       "Signal_SimTrack_EtaDistr", 100, 1.50,3.50));
  Signal_SimTrack_PhiDistr         = std::unique_ptr<TH1F>(new TH1F("Signal_SimTrack_PhiDistr",       "Signal_SimTrack_PhiDistr", 144, -3.14,3.14));
  Signal_SimTrack_PTDistr          = std::unique_ptr<TH1F>(new TH1F("Signal_SimTrack_PTDistr",        "Signal_SimTrack_PTDistr",  200, 000,100));
  Signal_SimTrack_InvariantMass    = std::unique_ptr<TH1F>(new TH1F("Signal_SimTrack_InvariantMass",  "Signal_SimTrack_InvariantMass",  300, 000,150));


  SimTrack_All_Eta                 = std::unique_ptr<TH1F>(new TH1F("SimTrack_All_Eta",                "SimTrack_All_Eta", 100, 1.50,3.50));
  SimTrack_ME0Hits_Eta             = std::unique_ptr<TH1F>(new TH1F("SimTrack_ME0Hits_Eta",            "SimTrack_ME0Hits_Eta", 100, 1.50,3.50));
  SimHits_ME0Hits_Eta              = std::unique_ptr<TH1F>(new TH1F("SimHits_ME0Hits_Eta",             "SimHits_ME0Hits_Eta", 100, 1.50,3.50));
  SimHits_ME0Hits_Phi              = std::unique_ptr<TH1F>(new TH1F("SimHits_ME0Hits_Phi",             "SimHits_ME0Hits_Phi", 144, -3.14,3.14));
  SimHits_ME0Hits_TOF              = std::unique_ptr<TH1F>(new TH1F("SimHits_ME0Hits_TOF",             "SimHits_ME0Hits_TOF", 100, 16.5,21.5));
  SimHits_ME0Hits_TOFvsR           = std::unique_ptr<TH2F>(new TH2F("SimHits_ME0Hits_TOFvsR",          "SimHits_ME0Hits_TOFvsR", 100, 500, 600, 100, 16.5,21.5));
  SimTrack_ME0Hits_NumbHits        = std::unique_ptr<TH1F>(new TH1F("SimTrack_ME0Hits_NumbHits",       "SimTrack_ME0Hits_NumbHits", 20, 0.5, 20.5));
  SimTrack_All_NumbTracks          = std::unique_ptr<TH1F>(new TH1F("SimTrack_All_NumbTracks",         "SimTrack_All_NumbTracks", 200, 000, 4000));
  SimTrack_ME0Hits_NumbTracks      = std::unique_ptr<TH1F>(new TH1F("SimTrack_ME0Hits_NumbTracks",     "SimTrack_ME0Hits_NumbTracks", 200, 000, 4000));
  ME0RecHits_NumbHits              = std::unique_ptr<TH1F>(new TH1F("ME0RecHits_Numbhits",             "ME0RecHits_NumbHits",         300, 000, 6000));
  ME0Segments_NumbSeg              = std::unique_ptr<TH1F>(new TH1F("ME0Segments_NumbSeg",             "ME0Segments_NumbSeg",         200, 000, 400));
  ME0Muons_NumbMuons               = std::unique_ptr<TH1F>(new TH1F("ME0Muons_NumbMuons",              "ME0Muons_NumbMuons",          200, 000, 4000));
  SimTrack_Summary                 = std::unique_ptr<TH1F>(new TH1F("SimTrack_Summary",                "SimTrack_Summary", 6, 0.5, 6.5));

  NewMatch_ME0HitsMatched          = std::unique_ptr<TH1F>(new TH1F("NewMatch_ME0HitsMatched",  "NewMatch_ME0HitsMatched", 20, 0.5, 20.5));
  NewMatch_ME0HitsTotal            = std::unique_ptr<TH1F>(new TH1F("NewMatch_ME0HitsTotal",    "NewMatch_ME0HitsTotal", 20, 0.5, 20.5));
  NewMatch_CSCHitsMatched          = std::unique_ptr<TH1F>(new TH1F("NewMatch_CSCHitsMatched",  "NewMatch_CSCHitsMatched", 20, 0.5, 20.5));  // ToDo :: Investigate why there are more CSC Hits matched than total CSC Hits
  NewMatch_CSCHitsTotal            = std::unique_ptr<TH1F>(new TH1F("NewMatch_CSCHitsTotal",    "NewMatch_CSCHitsTotal", 20, 0.5, 20.5));
  NewMatch_DTHitsMatched           = std::unique_ptr<TH1F>(new TH1F("NewMatch_DTHitsMatched",   "NewMatch_DTHitsMatched", 20, 0.5, 20.5));   // ToDo :: Investigate why there are more DT Hits matched than total DT Hits
  NewMatch_DTHitsTotal             = std::unique_ptr<TH1F>(new TH1F("NewMatch_DTHitsTotal",     "NewMatch_DTHitsTotal", 20, 0.5, 20.5));
  NewMatch_SegmentsMatched         = std::unique_ptr<TH1F>(new TH1F("NewMatch_SegmentsMatched", "NewMatch_SegmentsMatched", 10, 0.5, 10.5));
  NewMatch_SegmentsTotal           = std::unique_ptr<TH1F>(new TH1F("NewMatch_SegmentsTotal",   "NewMatch_SegmentsTotal", 10, 0.5, 10.5));
  NewMatch_ME0MuonsMatched         = std::unique_ptr<TH1F>(new TH1F("NewMatch_ME0MuonsMatched", "NewMatch_ME0MuonsMatched", 10, 0.5, 10.5));  
  NewMatch_LooseME0MuonsMatched    = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0MuonsMatched", "NewMatch_LooseME0MuonsMatched", 10, 0.5, 10.5));  
  NewMatch_TightME0MuonsMatched    = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0MuonsMatched", "NewMatch_TightME0MuonsMatched", 10, 0.5, 10.5));  
  NewMatch_RecoMuonsMatched        = std::unique_ptr<TH1F>(new TH1F("NewMatch_RecoMuonsMatched","NewMatch_RecoMuonsMatched", 10, 0.5, 10.5)); 

  Categories_EventInfo             = std::unique_ptr<TH1F>(new TH1F("Categories_EventInfo",            "Categories_EventInfo",            9, 0.5, 9.5));
  Categories_NumberOfME0Segments   = std::unique_ptr<TH1F>(new TH1F("Categories_NumberOfME0Segments",  "Categories_NumberOfME0Segments",  9, 0.5, 9.5));
  Categories_NumberOfME0Muons      = std::unique_ptr<TH1F>(new TH1F("Categories_NumberOfME0Muons",     "Categories_NumberOfME0Muons",     9, 0.5, 9.5));

  NoMatch_AllME0Seg_SegTimeCreation = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Seg_SegTimeCreation","NoMatch_AllME0Seg_SegTimeCreation",   100, 000,500));
  NoMatch_AllME0Seg_SegTimeCreaZoom = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Seg_SegTimeCreaZoom","NoMatch_AllME0Seg_SegTimeCreaZoom",   100, 000,5));

  NoMatch_AllME0Mu_SegTimeValue    = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegTimeValue",   "NoMatch_AllME0Mu_SegTimeValue",  5000,-350,150));
  NoMatch_AllME0Mu_SegTimeValZoom  = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegTimeValZoom", "NoMatch_AllME0Mu_SegTimeValZoom",  600,-10,50));
  NoMatch_AllME0Mu_SegTimeUncrt    = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegTimeUncrt",   "NoMatch_AllME0Mu_SegTimeUncrt",  1000, 000,10));
  NoMatch_AllME0Mu_SegTimeCreation = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegTimeCreation","NoMatch_AllME0Mu_SegTimeCreation",   100, 000,500));
  NoMatch_AllME0Mu_SegTimeCreaZoom = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegTimeCreaZoom","NoMatch_AllME0Mu_SegTimeCreaZoom",   100, 000,5));
  // NoMatch_AllME0Mu_InvariantMass   = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_InvariantMass",  "NoMatch_AllME0Mu_InvariantMass",  300, 000,150));
  NoMatch_AllME0Mu_TrackETADistr   = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_TrackETADistr",  "NoMatch_AllME0Mu_TrackETADistr", 100, 1.50,3.50));
  NoMatch_AllME0Mu_TrackPHIDistr   = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_TrackPHIDistr",  "NoMatch_AllME0Mu_TrackPHIDistr", 144, -3.14,3.14));
  NoMatch_AllME0Mu_TrackPTDistr    = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_TrackPTDistr",   "NoMatch_AllME0Mu_TrackPTDistr", 200, 000,100));
  NoMatch_AllME0Mu_SegETADir       = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegETADir",      "NoMatch_AllME0Mu_SegETADir", 100, 1.50,3.50));
  NoMatch_AllME0Mu_SegETAPos       = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegETAPos",      "NoMatch_AllME0Mu_SegETAPos", 100, 1.50,3.50));
  NoMatch_AllME0Mu_SegPHIDir       = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_TrackPHIDir",    "NoMatch_AllME0Mu_SegPHIDir", 144, -3.14,3.14));
  NoMatch_AllME0Mu_SegPHIPos       = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_TrackPHIPos",    "NoMatch_AllME0Mu_SegPHIPos", 144, -3.14,3.14));
  NoMatch_AllME0Mu_PTResolution    = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_PTResolution",   "NoMatch_AllME0Mu_PTResolution",   100, -0.25, 0.25));
  NoMatch_AllME0Mu_ETAResolution   = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_ETAResolution",  "NoMatch_AllME0Mu_ETAResolution",    40, -1.00, 1.00));
  NoMatch_AllME0Mu_SegNumberOfHits = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegNumberOfHits","NoMatch_AllME0Mu_SegNumberOfHits", 20, 0.5, 20.5));
  NoMatch_AllME0Mu_SegChi2NDof     = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_SegChi2NDof",    "NoMatch_AllME0Mu_SegChi2NDof",     100,000, 10));
  NoMatch_AllME0Mu_NumbME0Muons    = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_NumbME0Muons",   "NoMatch_AllME0Mu_NumbME0Muons",   250,000, 5000));
  NoMatch_AllME0Mu_NumbME0Segments = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_NumbME0Segments","NoMatch_AllME0Mu_NumbME0Segments",200,000, 200));
  NoMatch_AllME0Mu_TrackETADistr_5GeV = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_TrackETADistr_5GeV",  "NoMatch_AllME0Mu_TrackETADistr_5GeV", 100, 1.50,3.50)); 
  NoMatch_AllME0Mu_ETAResolution_5GeV = std::unique_ptr<TH1F>(new TH1F("NoMatch_AllME0Mu_ETAResolution_5GeV",  "NoMatch_AllME0Mu_ETAResolution_5GeV",    40, -1.00, 1.00));
  NoMatch_AllME0Mu_SegPHIvsSimPT   = std::unique_ptr<TH2F>(new TH2F("NoMatch_AllME0Mu_SegPHIvsSimPT",   "NoMatch_AllME0Mu_SegPHIvsSimPT",100,0.00,100,144,-3.14,3.14));
  NoMatch_AllME0Mu_ETAvsETARes     = std::unique_ptr<TH2F>(new TH2F("NoMatch_AllME0Mu_ETAvsETARes",     "NoMatch_AllME0Mu_ETAvsETARes",100, 1.50,3.50,40,-1.00,1.00));
  NoMatch_AllME0Mu_ETAvsETARes_5GeV= std::unique_ptr<TH2F>(new TH2F("NoMatch_AllME0Mu_ETAvsETARes_5GeV","NoMatch_AllME0Mu_ETAvsETARes_5GeV",100, 1.50,3.50,40,-1.00,1.00));
  NoMatch_AllME0Mu_PTvsETARes      = std::unique_ptr<TH2F>(new TH2F("NoMatch_AllME0Mu_PTvsETARes",     " NoMatch_AllME0Mu_PTvsETARes",100,0.00,100,40,-1.00,1.00));

  NoMatch_TightME0Mu_SegTimeValue    = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegTimeValue",   "NoMatch_TightME0Mu_SegTimeValue",  5000,-350,150));
  NoMatch_TightME0Mu_SegTimeValZoom  = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegTimeValZoom", "NoMatch_TightME0Mu_SegTimeValZoom",  600,-10,50));
  NoMatch_TightME0Mu_SegTimeUncrt    = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegTimeUncrt",   "NoMatch_TightME0Mu_SegTimeUncrt",  1000, 000,10));
  NoMatch_TightME0Mu_SegTimeCreation = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegTimeCreation","NoMatch_TightME0Mu_SegTimeCreation",  100, 000,500));
  NoMatch_TightME0Mu_SegTimeCreaZoom = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegTimeCreaZoom","NoMatch_TightME0Mu_SegTimeCreaZoom",  100, 000,5));
  // NoMatch_TightME0Mu_InvariantMass   = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_InvariantMass",  "NoMatch_TightME0Mu_InvariantMass",  300, 000,150));
  NoMatch_TightME0Mu_TrackETADistr   = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_TrackETADistr",  "NoMatch_TightME0Mu_TrackETADistr", 70, 1.80,3.20));
  NoMatch_TightME0Mu_TrackPHIDistr   = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_TrackPHIDistr",  "NoMatch_TightME0Mu_TrackPHIDistr", 144, -3.14,3.14));
  NoMatch_TightME0Mu_TrackPTDistr    = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_TrackPTDistr",   "NoMatch_TightME0Mu_TrackPTDistr", 200, 000,100));
  NoMatch_TightME0Mu_SegETADir       = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegETADir",      "NoMatch_TightME0Mu_SegETADir", 100, 1.50,3.50));
  NoMatch_TightME0Mu_SegETAPos       = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegETAPos",      "NoMatch_TightME0Mu_SegETAPos", 100, 1.50,3.50));
  NoMatch_TightME0Mu_SegPHIDir       = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_TrackPHIDir",    "NoMatch_TightME0Mu_SegPHIDir", 144, -3.14,3.14));
  NoMatch_TightME0Mu_SegPHIPos       = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_TrackPHIPos",    "NoMatch_TightME0Mu_SegPHIPos", 144, -3.14,3.14));
  NoMatch_TightME0Mu_PTResolution    = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_PTResolution",   "NoMatch_TightME0Mu_PTResolution",   100, -0.25, 0.25));
  NoMatch_TightME0Mu_ETAResolution   = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_ETAResolution",  "NoMatch_TightME0Mu_ETAResolution",    40, -1.00, 1.00));
  NoMatch_TightME0Mu_SegNumberOfHits = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegNumberOfHits","NoMatch_TightME0Mu_SegNumberOfHits", 20, 0.5, 20.5));
  NoMatch_TightME0Mu_SegChi2NDof     = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_SegChi2NDof",    "NoMatch_TightME0Mu_SegChi2NDof",     100,000, 10));
  NoMatch_TightME0Mu_NumbME0Muons    = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_NumbME0Muons",   "NoMatch_TightME0Mu_NumbME0Muons",   250,000, 5000));
  NoMatch_TightME0Mu_NumbME0Segments = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_NumbME0Segments","NoMatch_TightME0Mu_NumbME0Segments",200,000, 200));
  NoMatch_TightME0Mu_TrackETADistr_5GeV = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_TrackETADistr_5GeV",  "NoMatch_TightME0Mu_TrackETADistr_5GeV", 100, 1.50,3.50)); 
  NoMatch_TightME0Mu_ETAResolution_5GeV = std::unique_ptr<TH1F>(new TH1F("NoMatch_TightME0Mu_ETAResolution_5GeV",  "NoMatch_TightME0Mu_ETAResolution_5GeV",    40, -1.00, 1.00));
  NoMatch_TightME0Mu_SegPHIvsSimPT   = std::unique_ptr<TH2F>(new TH2F("NoMatch_TightME0Mu_SegPHIvsSimPT",  "NoMatch_TightME0Mu_SegPHIvsSimPT",100,0.00,100,144,-3.14,3.14));
  NoMatch_TightME0Mu_ETAvsETARes     = std::unique_ptr<TH2F>(new TH2F("NoMatch_TightME0Mu_ETAvsETARes",    "NoMatch_TightME0Mu_ETAvsETARes",100, 1.50,3.50,40,-1.00,1.00));
  NoMatch_TightME0Mu_ETAvsETARes_5GeV= std::unique_ptr<TH2F>(new TH2F("NoMatch_TightME0Mu_ETAvsETARes_5GeV","NoMatch_TightME0Mu_ETAvsETARes_5GeV",100, 1.50,3.50,40,-1.00,1.00));
  NoMatch_TightME0Mu_PTvsETARes      = std::unique_ptr<TH2F>(new TH2F("NoMatch_TightME0Mu_PTvsETARes",     "NoMatch_TightME0Mu_PTvsETARes",100, 0.00,100,40,-1.00,1.00));

  NewMatch_AllME0Mu_SegTimeValue     = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegTimeValue",   "NewMatch_AllME0Mu_SegTimeValue",  5000,-350,150));
  NewMatch_AllME0Mu_SegTimeValZoom   = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegTimeValZoom", "NewMatch_AllME0Mu_SegTimeValZoom",  600,-10,50));
  NewMatch_AllME0Mu_SegTimeUncrt     = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegTimeUncrt",   "NewMatch_AllME0Mu_SegTimeUncrt",  1000, 000,10));
  NewMatch_AllME0Mu_SegTimeCreation  = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegTimeCreation","NewMatch_AllME0Mu_SegTimeCreation",  100, 000,500));
  NewMatch_AllME0Mu_SegTimeCreaZoom  = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegTimeCreaZoom","NewMatch_AllME0Mu_SegTimeCreaZoom",  100, 000,5));
  // NewMatch_AllME0Mu_InvariantMass    = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_InvariantMass",  "NewMatch_AllME0Mu_InvariantMass",  300, 000,150));
  NewMatch_AllME0Mu_TrackETADistr    = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_TrackETADistr",  "NewMatch_AllME0Mu_TrackETADistr", 70, 1.80,3.20));
  NewMatch_AllME0Mu_TrackPHIDistr    = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_TrackPHIDistr",  "NewMatch_AllME0Mu_TrackPHIDistr", 144, -3.14,3.14));
  NewMatch_AllME0Mu_TrackPTDistr     = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_TrackPTDistr",   "NewMatch_AllME0Mu_TrackPTDistr", 200, 000,100));
  NewMatch_AllME0Mu_SegETADir        = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegETADir",     "NewMatch_AllME0Mu_SegETADir", 100, 1.50,3.50));
  NewMatch_AllME0Mu_SegETAPos        = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegETAPos",     "NewMatch_AllME0Mu_SegETAPos", 100, 1.50,3.50));
  NewMatch_AllME0Mu_SegPHIDir        = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_TrackPHIDir",    "NewMatch_AllME0Mu_SegPHIDir", 144, -3.14,3.14));
  NewMatch_AllME0Mu_SegPHIPos        = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_TrackPHIPos",    "NewMatch_AllME0Mu_SegPHIPos", 144, -3.14,3.14));
  NewMatch_AllME0Mu_PTResolution     = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_PTResolution",   "NewMatch_AllME0Mu_PTResolution",   100, -0.25, 0.25));
  NewMatch_AllME0Mu_ETAResolution    = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_ETAResolution",  "NewMatch_AllME0Mu_ETAResolution",    40, -1.00, 1.00));
  NewMatch_AllME0Mu_SegNumberOfHits  = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegNumberOfHits","NewMatch_AllME0Mu_SegNumberOfHits", 20, 0.5, 20.5));
  NewMatch_AllME0Mu_SegChi2NDof      = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_SegChi2NDof",    "NewMatch_AllME0Mu_SegChi2NDof",     100,000, 10));
  NewMatch_AllME0Mu_SegPHIvsSimPT    = std::unique_ptr<TH2F>(new TH2F("NewMatch_AllME0Mu_SegPHIvsSimPT",  "NewMatch_AllME0Mu_SegPHIvsSimPT",100,0.00,100,144,-3.14,3.14)); 
  NewMatch_AllME0Mu_ETAvsETARes      = std::unique_ptr<TH2F>(new TH2F("NewMatch_AllME0Mu_ETAvsETARes",    "NewMatch_AllME0Mu_ETAvsETARes",100, 1.50,3.50,40,-1.00,1.00));
  NewMatch_AllME0Mu_PTvsETARes       = std::unique_ptr<TH2F>(new TH2F("NewMatch_AllME0Mu_PTvsETARes",     "NewMatch_AlllME0Mu_PTvsETARes",100, 0.00,100,40,-1.00,1.00));
  NewMatch_AllME0Mu_InvariantMass_All         = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_InvariantMass_All",         "NewMatch_AllME0Mu_InvariantMass_All",          300, 000,150));
  NewMatch_AllME0Mu_InvariantMass_2ME0Mu      = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_InvariantMass_2ME0Mu",      "NewMatch_AllME0Mu_InvariantMass_2ME0Mu",       300, 000,150));
  NewMatch_AllME0Mu_InvariantMass_ME0MuRecoMu = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_InvariantMass_ME0MuRecoMu", "NewMatch_AllME0Mu_InvariantMass_ME0MuRecoMu",  300, 000,150));
  NewMatch_AllME0Mu_InvariantMass_2RecoMu     = std::unique_ptr<TH1F>(new TH1F("NewMatch_AllME0Mu_InvariantMass_2RecoMu",     "NewMatch_AllME0Mu_InvariantMass_2RecoMu",      300, 000,150));

  NewMatch_LooseME0Mu_SegTimeValue    = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegTimeValue",   "NewMatch_LooseME0Mu_SegTimeValue",  5000,-350,150));
  NewMatch_LooseME0Mu_SegTimeValZoom  = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegTimeValZoom", "NewMatch_LooseME0Mu_SegTimeValZoom",  600,-10,50));
  NewMatch_LooseME0Mu_SegTimeUncrt    = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegTimeUncrt",   "NewMatch_LooseME0Mu_SegTimeUncrt",  1000, 000,10));
  NewMatch_LooseME0Mu_SegTimeCreation = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegTimeCreation","NewMatch_LooseME0Mu_SegTimeCreation",  100, 000,500));
  NewMatch_LooseME0Mu_SegTimeCreaZoom = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegTimeCreaZoom","NewMatch_LooseME0Mu_SegTimeCreaZoom",  100, 000,5));
  // NewMatch_LooseME0Mu_InvariantMass   = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_InvariantMass",  "NewMatch_LooseME0Mu_InvariantMass",  300, 000,150));
  NewMatch_LooseME0Mu_TrackETADistr   = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_TrackETADistr",  "NewMatch_LooseME0Mu_TrackETADistr", 70, 1.80,3.20));
  NewMatch_LooseME0Mu_TrackPHIDistr   = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_TrackPHIDistr",  "NewMatch_LooseME0Mu_TrackPHIDistr", 144, -3.14,3.14));
  NewMatch_LooseME0Mu_TrackPTDistr    = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_TrackPTDistr",   "NewMatch_LooseME0Mu_TrackPTDistr", 200, 000,100));
  NewMatch_LooseME0Mu_SegETADir       = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegETADir",     "NewMatch_LooseME0Mu_SegETADir", 100, 1.50,3.50));
  NewMatch_LooseME0Mu_SegETAPos       = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegETAPos",     "NewMatch_LooseME0Mu_SegETAPos", 100, 1.50,3.50));
  NewMatch_LooseME0Mu_SegPHIDir       = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_TrackPHIDir",    "NewMatch_LooseME0Mu_SegPHIDir", 144, -3.14,3.14));
  NewMatch_LooseME0Mu_SegPHIPos       = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_TrackPHIPos",    "NewMatch_LooseME0Mu_SegPHIPos", 144, -3.14,3.14));
  NewMatch_LooseME0Mu_PTResolution    = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_PTResolution",   "NewMatch_LooseME0Mu_PTResolution",   100, -0.25, 0.25));
  NewMatch_LooseME0Mu_ETAResolution   = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_ETAResolution",  "NewMatch_LooseME0Mu_ETAResolution",    40, -1.00, 1.00));
  NewMatch_LooseME0Mu_SegNumberOfHits = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegNumberOfHits","NewMatch_LooseME0Mu_SegNumberOfHits", 20, 0.5, 20.5));
  NewMatch_LooseME0Mu_SegChi2NDof     = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_SegChi2NDof",    "NewMatch_LooseME0Mu_SegChi2NDof",     100,000, 10));
  NewMatch_LooseME0Mu_SegPHIvsSimPT   = std::unique_ptr<TH2F>(new TH2F("NewMatch_LooseME0Mu_SegPHIvsSimPT",  "NewMatch_LooseME0Mu_SegPHIvsSimPT",100,0.00,100,144,-3.14,3.14));
  NewMatch_LooseME0Mu_ETAvsETARes     = std::unique_ptr<TH2F>(new TH2F("NewMatch_LooseME0Mu_ETAvsETARes",    "NewMatch_LooseME0Mu_ETAvsETARes",100, 1.50,3.50,40,-1.00,1.00));
  NewMatch_LooseME0Mu_PTvsETARes      = std::unique_ptr<TH2F>(new TH2F("NewMatch_LooseME0Mu_PTvsETARes",     "NewMatch_LooselME0Mu_PTvsETARes",100, 0.00,100,40,-1.00,1.00));
  NewMatch_LooseME0Mu_InvariantMass_All         = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_InvariantMass_All",         "NewMatch_LooseME0Mu_InvariantMass_All",          300, 000,150));
  NewMatch_LooseME0Mu_InvariantMass_2ME0Mu      = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_InvariantMass_2ME0Mu",      "NewMatch_LooseME0Mu_InvariantMass_2ME0Mu",       300, 000,150));
  NewMatch_LooseME0Mu_InvariantMass_ME0MuRecoMu = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_InvariantMass_ME0MuRecoMu", "NewMatch_LooseME0Mu_InvariantMass_ME0MuRecoMu",  300, 000,150));
  NewMatch_LooseME0Mu_InvariantMass_2RecoMu     = std::unique_ptr<TH1F>(new TH1F("NewMatch_LooseME0Mu_InvariantMass_2RecoMu",     "NewMatch_LooseME0Mu_InvariantMass_2RecoMu",      300, 000,150));

  NewMatch_TightME0Mu_SegTimeValue    = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegTimeValue",   "NewMatch_TightME0Mu_SegTimeValue",  5000,-350,150));
  NewMatch_TightME0Mu_SegTimeValZoom  = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegTimeValZoom", "NewMatch_TightME0Mu_SegTimeValZoom",  600,-10,50));
  NewMatch_TightME0Mu_SegTimeUncrt    = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegTimeUncrt",   "NewMatch_TightME0Mu_SegTimeUncrt",  1000, 000,10));
  NewMatch_TightME0Mu_SegTimeCreation = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegTimeCreation","NewMatch_TightME0Mu_SegTimeCreation",  100, 000,500));
  NewMatch_TightME0Mu_SegTimeCreaZoom = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegTimeCreaZoom","NewMatch_TightME0Mu_SegTimeCreaZoom",  100, 000,5));
  NewMatch_TightME0Mu_TrackETADistr   = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_TrackETADistr",  "NewMatch_TightME0Mu_TrackETADistr", 70, 1.80,3.20));
  NewMatch_TightME0Mu_TrackPHIDistr   = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_TrackPHIDistr",  "NewMatch_TightME0Mu_TrackPHIDistr", 144, -3.14,3.14));
  NewMatch_TightME0Mu_TrackPTDistr    = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_TrackPTDistr",   "NewMatch_TightME0Mu_TrackPTDistr", 200, 000,100));
  NewMatch_TightME0Mu_SegETADir       = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegETADir",      "NewMatch_TightME0Mu_SegETADir", 100, 1.50,3.50));
  NewMatch_TightME0Mu_SegETAPos       = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegETAPos",      "NewMatch_TightME0Mu_SegETAPos", 100, 1.50,3.50));
  NewMatch_TightME0Mu_SegPHIDir       = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_TrackPHIDir",    "NewMatch_TightME0Mu_SegPHIDir", 144, -3.14,3.14));
  NewMatch_TightME0Mu_SegPHIPos       = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_TrackPHIPos",    "NewMatch_TightME0Mu_SegPHIPos", 144, -3.14,3.14));
  NewMatch_TightME0Mu_PTResolution    = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_PTResolution",   "NewMatch_TightME0Mu_PTResolution",   100, -0.25, 0.25));
  NewMatch_TightME0Mu_ETAResolution   = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_ETAResolution",  "NewMatch_TightME0Mu_ETAResolution",    40, -1.00, 1.00));
  NewMatch_TightME0Mu_SegNumberOfHits = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegNumberOfHits","NewMatch_TightME0Mu_SegNumberOfHits", 20, 0.5, 20.5));
  NewMatch_TightME0Mu_SegChi2NDof     = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_SegChi2NDof",    "NewMatch_TightME0Mu_SegChi2NDof",     100,000, 10));
  NewMatch_TightME0Mu_TrackETADistr_5GeV = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_TrackETADistr_5GeV",  "NewMatch_TightME0Mu_TrackETADistr_5GeV", 100, 1.50,3.50)); 
  NewMatch_TightME0Mu_ETAResolution_5GeV = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_ETAResolution_5GeV",  "NewMatch_TightME0Mu_ETAResolution_5GeV",    40, -1.00, 1.00));
  NewMatch_TightME0Mu_SegPHIvsSimPT   = std::unique_ptr<TH2F>(new TH2F("NewMatch_TightME0Mu_SegPHIvsSimPT",  "NewMatch_TightME0Mu_SegPHIvsSimPT",100,0.00,100,144,-3.14,3.14));
  NewMatch_TightME0Mu_ETAvsETARes     = std::unique_ptr<TH2F>(new TH2F("NewMatch_TightME0Mu_ETAvsETARes",    "NewMatch_TightME0Mu_ETAvsETARes",100, 1.50,3.50,40,-1.00,1.00));
  NewMatch_TightME0Mu_ETAvsETARes_5GeV= std::unique_ptr<TH2F>(new TH2F("NewMatch_TightME0Mu_ETAvsETARes_5GeV","NewMatch_TightME0Mu_ETAvsETARes_5GeV",100, 1.50,3.50,40,-1.00,1.00));
  NewMatch_TightME0Mu_PTvsETARes      = std::unique_ptr<TH2F>(new TH2F("NewMatch_TightME0Mu_PTvsETARes",     "NewMatch_TightlME0Mu_PTvsETARes",100, 0.00,100,40,-1.00,1.00));
  NewMatch_TightME0Mu_InvariantMass_All         = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_InvariantMass_All",         "NewMatch_TightME0Mu_InvariantMass_All",          300, 000,150));
  NewMatch_TightME0Mu_InvariantMass_2ME0Mu      = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_InvariantMass_2ME0Mu",      "NewMatch_TightME0Mu_InvariantMass_2ME0Mu",       300, 000,150));
  NewMatch_TightME0Mu_InvariantMass_ME0MuRecoMu = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_InvariantMass_ME0MuRecoMu", "NewMatch_TightME0Mu_InvariantMass_ME0MuRecoMu",  300, 000,150));
  NewMatch_TightME0Mu_InvariantMass_2RecoMu     = std::unique_ptr<TH1F>(new TH1F("NewMatch_TightME0Mu_InvariantMass_2RecoMu",     "NewMatch_TightME0Mu_InvariantMass_2RecoMu",      300, 000,150));
}


MyME0InTimePUAnalyzer::~MyME0InTimePUAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

  outputfile->cd();  
  Signal_ME0Mu->cd();
  Signal_GenPart_EtaDistr->Write(); Signal_GenPart_PhiDistr->Write(); Signal_GenPart_PTDistr->Write(); Signal_GenPart_InvariantMass->Write(); 
  Signal_SimTrack_EtaDistr->Write(); Signal_SimTrack_PhiDistr->Write(); Signal_SimTrack_PTDistr->Write(); Signal_SimTrack_InvariantMass->Write();
  outputfile->cd();

  SimTrack_ME0Mu->cd();
  SimTrack_All_Eta->Write(); SimTrack_ME0Hits_Eta->Write(); SimTrack_ME0Hits_NumbHits->Write(); SimTrack_All_NumbTracks->Write(); SimTrack_ME0Hits_NumbTracks->Write();
  SimHits_ME0Hits_Eta->Write(); SimHits_ME0Hits_Phi->Write(); SimHits_ME0Hits_TOF->Write(); SimHits_ME0Hits_TOFvsR->Write();
  ME0RecHits_NumbHits->Write(); ME0Segments_NumbSeg->Write(); ME0Muons_NumbMuons->Write();
  std::string catlabels[] = {"Events Analyzed", "2 Reco Mu", "1 Reco Mu - 1 ME0 Mu", "2 ME0 Mu"};
  for(int i=0; i<4; ++i) {  Categories_EventInfo->GetXaxis()->SetBinLabel(i+1, catlabels[i].c_str()); }
  Categories_EventInfo->Write(); Categories_NumberOfME0Segments->Write(); Categories_NumberOfME0Muons->Write();
  std::string sumlabels[] = {"All SimTracks", "SimTracks with Hits", "ME0 SimHits/6", "ME0 RecHits/6", "ME0 Segments", "ME0 Muons"};
  // This is something that does not work when merging several files together ... 
  // need to find a different solution for this ...
  SimTrack_Summary->SetBinContent(1,SimTrack_All_NumbTracks->GetMean(1));     SimTrack_Summary->SetBinError(1,SimTrack_All_NumbTracks->GetMeanError(1));
  SimTrack_Summary->SetBinContent(2,SimTrack_ME0Hits_NumbTracks->GetMean(1)); SimTrack_Summary->SetBinError(2,SimTrack_ME0Hits_NumbTracks->GetMeanError(1)); 
  SimTrack_Summary->SetBinContent(3,SimHits_ME0Hits_Eta->GetEntries()*1.0/6); SimTrack_Summary->SetBinError(3,sqrt(SimHits_ME0Hits_Eta->GetEntries())*1.0/6); 
  SimTrack_Summary->SetBinContent(4,ME0RecHits_NumbHits->GetMean(1)*1.0/6);   SimTrack_Summary->SetBinError(4,sqrt(ME0RecHits_NumbHits->GetMeanError(1))*1.0/6);
  SimTrack_Summary->SetBinContent(5,ME0Segments_NumbSeg->GetMean(1));         SimTrack_Summary->SetBinError(5,ME0Segments_NumbSeg->GetMeanError(1));
  SimTrack_Summary->SetBinContent(6,ME0Muons_NumbMuons->GetMean(1));          SimTrack_Summary->SetBinError(6,ME0Muons_NumbMuons->GetMeanError(1));
  for(int i=0; i<6; ++i) { SimTrack_Summary->GetXaxis()->SetBinLabel(i+1, sumlabels[i].c_str()); }
  SimTrack_Summary->Write();
  outputfile->cd();

  NoMatch_AllME0Seg->cd();
  NoMatch_AllME0Seg_SegTimeCreation->Write(); NoMatch_AllME0Seg_SegTimeCreaZoom->Write();
  outputfile->cd();

  NoMatch_AllME0Mu->cd();
  NoMatch_AllME0Mu_SegTimeValue->Write(); NoMatch_AllME0Mu_SegTimeValZoom->Write(); NoMatch_AllME0Mu_SegTimeUncrt->Write(); NoMatch_AllME0Mu_SegTimeCreation->Write(); NoMatch_AllME0Mu_SegTimeCreaZoom->Write(); 
  NoMatch_AllME0Mu_TrackPTDistr->Write(); NoMatch_AllME0Mu_PTResolution->Write(); NoMatch_AllME0Mu_ETAResolution->Write(); NoMatch_AllME0Mu_ETAResolution_5GeV->Write();  NoMatch_AllME0Mu_SegNumberOfHits->Write(); NoMatch_AllME0Mu_SegChi2NDof->Write(); 
  NoMatch_AllME0Mu_SegPHIvsSimPT->Write(); NoMatch_AllME0Mu_ETAvsETARes->Write(); NoMatch_AllME0Mu_PTvsETARes->Write(); NoMatch_AllME0Mu_ETAvsETARes_5GeV->Write();
  NoMatch_AllME0Mu_TrackETADistr->Write(); NoMatch_AllME0Mu_TrackETADistr_5GeV->Write(); NoMatch_AllME0Mu_TrackPHIDistr->Write(); NoMatch_AllME0Mu_NumbME0Muons->Write(); NoMatch_AllME0Mu_NumbME0Segments->Write();
  NoMatch_AllME0Mu_SegETADir->Write(); NoMatch_AllME0Mu_SegETAPos->Write(); NoMatch_AllME0Mu_SegPHIDir->Write(); NoMatch_AllME0Mu_SegPHIPos->Write(); 
  outputfile->cd();

  NoMatch_TightME0Mu->cd();
  NoMatch_TightME0Mu_SegTimeValue->Write(); NoMatch_TightME0Mu_SegTimeValZoom->Write(); NoMatch_TightME0Mu_SegTimeUncrt->Write(); NoMatch_TightME0Mu_SegTimeCreation->Write(); NoMatch_TightME0Mu_SegTimeCreaZoom->Write();
  NoMatch_TightME0Mu_TrackPTDistr->Write(); NoMatch_TightME0Mu_ETAResolution->Write(); NoMatch_TightME0Mu_ETAResolution_5GeV->Write(); NoMatch_TightME0Mu_SegNumberOfHits->Write(); NoMatch_TightME0Mu_SegChi2NDof->Write(); 
  NoMatch_TightME0Mu_SegPHIvsSimPT->Write(); NoMatch_TightME0Mu_ETAvsETARes->Write(); NoMatch_TightME0Mu_PTvsETARes->Write(); NoMatch_TightME0Mu_ETAvsETARes_5GeV->Write();
  NoMatch_TightME0Mu_TrackETADistr->Write(); NoMatch_TightME0Mu_TrackETADistr_5GeV->Write(); NoMatch_TightME0Mu_TrackPHIDistr->Write(); NoMatch_TightME0Mu_NumbME0Muons->Write(); NoMatch_TightME0Mu_NumbME0Segments->Write();
  NoMatch_TightME0Mu_SegETADir->Write(); NoMatch_TightME0Mu_SegETAPos->Write(); NoMatch_TightME0Mu_SegPHIDir->Write(); NoMatch_TightME0Mu_SegPHIPos->Write(); 
  outputfile->cd();

  NewMatch_Details->cd();
  NewMatch_ME0HitsMatched->Write(); NewMatch_ME0HitsTotal->Write(); NewMatch_CSCHitsMatched->Write(); NewMatch_CSCHitsTotal->Write(); NewMatch_DTHitsMatched->Write(); NewMatch_DTHitsTotal->Write(); 
  NewMatch_SegmentsMatched->Write(); NewMatch_SegmentsTotal->Write(); NewMatch_ME0MuonsMatched->Write(); NewMatch_LooseME0MuonsMatched->Write(); NewMatch_TightME0MuonsMatched->Write(); NewMatch_RecoMuonsMatched->Write();
  outputfile->cd();

  NewMatch_AllME0Mu->cd();
  NewMatch_AllME0Mu_SegTimeValue->Write(); NewMatch_AllME0Mu_SegTimeValZoom->Write(); NewMatch_AllME0Mu_SegTimeUncrt->Write(); NewMatch_AllME0Mu_SegTimeCreation->Write(); NewMatch_AllME0Mu_SegTimeCreaZoom->Write(); 
  NewMatch_AllME0Mu_TrackPTDistr->Write(); NewMatch_AllME0Mu_PTResolution->Write(); NewMatch_AllME0Mu_ETAResolution->Write(); NewMatch_AllME0Mu_SegNumberOfHits->Write(); NewMatch_AllME0Mu_SegChi2NDof->Write(); 
  NewMatch_AllME0Mu_SegPHIvsSimPT->Write(); NewMatch_AllME0Mu_ETAvsETARes->Write(); NewMatch_AllME0Mu_PTvsETARes->Write();
  NewMatch_AllME0Mu_TrackETADistr->Write(); NewMatch_AllME0Mu_TrackPHIDistr->Write(); NewMatch_AllME0Mu_SegTimeCreation->Write();
  NewMatch_AllME0Mu_SegETADir->Write(); NewMatch_AllME0Mu_SegETAPos->Write(); NewMatch_AllME0Mu_SegPHIDir->Write(); NewMatch_AllME0Mu_SegPHIPos->Write();
  NewMatch_AllME0Mu_InvariantMass_All->Write(); NewMatch_AllME0Mu_InvariantMass_2ME0Mu->Write(); NewMatch_AllME0Mu_InvariantMass_ME0MuRecoMu->Write(); NewMatch_AllME0Mu_InvariantMass_2RecoMu->Write();
  outputfile->cd();

  NewMatch_LooseME0Mu->cd();
  NewMatch_LooseME0Mu_SegTimeValue->Write(); NewMatch_LooseME0Mu_SegTimeValZoom->Write(); NewMatch_LooseME0Mu_SegTimeUncrt->Write(); NewMatch_LooseME0Mu_SegTimeCreation->Write(); NewMatch_LooseME0Mu_SegTimeCreaZoom->Write(); 
  NewMatch_LooseME0Mu_TrackPTDistr->Write(); NewMatch_LooseME0Mu_PTResolution->Write(); NewMatch_LooseME0Mu_ETAResolution->Write(); NewMatch_LooseME0Mu_SegNumberOfHits->Write(); NewMatch_LooseME0Mu_SegChi2NDof->Write(); 
  NewMatch_LooseME0Mu_SegPHIvsSimPT->Write(); NewMatch_LooseME0Mu_ETAvsETARes->Write(); NewMatch_LooseME0Mu_PTvsETARes->Write();
  NewMatch_LooseME0Mu_TrackETADistr->Write(); NewMatch_LooseME0Mu_TrackPHIDistr->Write(); 
  NewMatch_LooseME0Mu_SegETADir->Write(); NewMatch_LooseME0Mu_SegETAPos->Write(); NewMatch_LooseME0Mu_SegPHIDir->Write(); NewMatch_LooseME0Mu_SegPHIPos->Write();
  NewMatch_LooseME0Mu_InvariantMass_All->Write(); NewMatch_LooseME0Mu_InvariantMass_2ME0Mu->Write(); NewMatch_LooseME0Mu_InvariantMass_ME0MuRecoMu->Write(); NewMatch_LooseME0Mu_InvariantMass_2RecoMu->Write();
  outputfile->cd();

  NewMatch_TightME0Mu->cd();
  NewMatch_TightME0Mu_SegTimeValue->Write(); NewMatch_TightME0Mu_SegTimeValZoom->Write(); NewMatch_TightME0Mu_SegTimeUncrt->Write(); NewMatch_TightME0Mu_SegTimeCreation->Write(); NewMatch_TightME0Mu_SegTimeCreaZoom->Write(); 
  NewMatch_TightME0Mu_TrackPTDistr->Write(); NewMatch_TightME0Mu_PTResolution->Write(); NewMatch_TightME0Mu_ETAResolution->Write(); NewMatch_TightME0Mu_ETAResolution_5GeV->Write(); 
  NewMatch_TightME0Mu_SegNumberOfHits->Write(); NewMatch_TightME0Mu_SegChi2NDof->Write(); 
  NewMatch_TightME0Mu_SegPHIvsSimPT->Write(); NewMatch_TightME0Mu_ETAvsETARes->Write(); NewMatch_TightME0Mu_PTvsETARes->Write(); NewMatch_TightME0Mu_ETAvsETARes_5GeV->Write();
  NewMatch_TightME0Mu_TrackETADistr->Write(); NewMatch_TightME0Mu_TrackETADistr_5GeV->Write(); NewMatch_TightME0Mu_TrackPHIDistr->Write();
  NewMatch_TightME0Mu_SegETADir->Write(); NewMatch_TightME0Mu_SegETAPos->Write(); NewMatch_TightME0Mu_SegPHIDir->Write(); NewMatch_TightME0Mu_SegPHIPos->Write();
  NewMatch_TightME0Mu_InvariantMass_All->Write(); NewMatch_TightME0Mu_InvariantMass_2ME0Mu->Write(); NewMatch_TightME0Mu_InvariantMass_ME0MuRecoMu->Write(); NewMatch_TightME0Mu_InvariantMass_2RecoMu->Write();

  outputfile->cd();

  outputfile->Close();

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MyME0InTimePUAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  Categories_EventInfo->Fill(1); // 1 = number of Events

  // Get Geometries
  // ----------------------
  iSetup.get<MuonGeometryRecord>().get(me0Geom);
  iSetup.get<MuonGeometryRecord>().get(cscGeom);
  iSetup.get<MuonGeometryRecord>().get(dtGeom);
  // ----------------------

  // Reset Vectors
  theME0Muons.clear();
  theMuons.clear();

  // Access GenParticles
  // ----------------------
  // edm::Handle<reco::GenParticleCollection> genParticles;
  // iEvent.getByLabel("genParticles", genParticles);     // 62X
  // iEvent.getByToken(GENParticle_Token, genParticles);  // 7XY
  edm::Handle<edm::HepMCProduct> hepmcevent;
  iEvent.getByLabel("generator", hepmcevent);             // 62X
  // iEvent.getByToken(HEPMCCol_Token, hepmcevent);       // 7XY
  // -----------------------

  // Access SimVertices
  // -----------------------
  edm::Handle<std::vector<SimVertex>> simVertexCollection;
  iEvent.getByLabel("g4SimHits", simVertexCollection);         // 62X
  // iEvent.getByToken(SIMVertex_Token, simVertexCollection);  // 7XY
  std::vector<SimVertex> theSimVertices; 
  theSimVertices.insert(theSimVertices.end(),simVertexCollection->begin(),simVertexCollection->end()); // more useful than seems at first sight
  // -----------------------

  // Access SimTracks
  // -----------------------
  edm::Handle<edm::SimTrackContainer> SimTk;
  iEvent.getByLabel("g4SimHits",SimTk);                   // 62X
  // iEvent.getByToken(SIMTrack_Token,SimTk);             // 7XY
  // -----------------------

  // Access SimHits
  // -----------------------
  std::vector<edm::Handle<edm::PSimHitContainer> > theSimHitContainers;
  iEvent.getManyByType(theSimHitContainers);              // 62X & 7XY
  std::vector<PSimHit> theSimHits;
  for (int i = 0; i < int(theSimHitContainers.size()); ++i) {
    theSimHits.insert(theSimHits.end(),theSimHitContainers.at(i)->begin(),theSimHitContainers.at(i)->end());
  }
  // -----------------------

  // Access Primary Vertices
  // -----------------------
  edm::Handle<std::vector<reco::Vertex>> primaryVertexCollection;
  iEvent.getByLabel("offlinePrimaryVertices", primaryVertexCollection);  // 62X
  // iEvent.getByToken(PrimaryVertex_Token, primaryVertexCollection);    // 7XY
  std::vector<reco::Vertex> theRecoVertices;
  theRecoVertices.insert(theRecoVertices.end(),primaryVertexCollection->begin(),primaryVertexCollection->end()); // probably not necessary
  // -----------------------

  // Access ME0RecHits
  // -----------------------
  edm::Handle<ME0RecHitCollection> me0rechits;
  iEvent.getByLabel("me0RecHits", me0rechits);
  // iEvent.getByToken(ME0RecHit_Token, me0rechits);
  // -----------------------

  // Access ME0Segments
  // -----------------------
  edm::Handle<ME0SegmentCollection> me0segments;
  iEvent.getByLabel("me0Segments", me0segments);
  // iEvent.getByToken(ME0Segment_Token, me0segments);
  // -----------------------

  // Access ME0Muons
  // -----------------------
  edm::Handle <std::vector<reco::ME0Muon> > me0muons;
  iEvent.getByLabel("me0SegmentMatching", me0muons);
  // iEvent.getByToken(ME0Muon_Token, me0muons);

  theME0Muons.insert(theME0Muons.end(),me0muons->begin(),me0muons->end()); // probably not necessary
  // -----------------------

  // Access Muons
  // -----------------------
  edm::Handle <std::vector<reco::Muon> > muons;
  iEvent.getByLabel("muons", muons);
  // iEvent.getByToken(Muon_Token, muons);
  theMuons.insert(theMuons.end(),muons->begin(),muons->end()); // probably not necessary
  // -----------------------

  me0genpartfound = false;

  // Analysis of SimVertices
  // =======================
  // not sure whether this is useful
  // SimVertices are all vertices used in GEANT ... 
  // so also when a delta-ray is emitted in a muon detector
  double vtx_r = 0.0, vtx_x = 0.0, vtx_y = 0.0, vtx_z = 0.0;
  /*
  for (std::vector<SimVertex>::const_iterator iVertex = theSimVertices.begin(); iVertex != theSimVertices.end(); ++iVertex) {
    SimVertex simvertex = (*iVertex);
    unsigned int simvertexid = simvertex.vertexId();
    vtx_x = simvertex.position().x(); vtx_y = simvertex.position().y(); vtx_r = sqrt(pow(vtx_x,2)+pow(vtx_y,2)); vtx_z = simvertex.position().z();
    if( vtx_r < 2 && fabs(vtx_z) < 25 ) { // constrain area to beam spot: r < 2cm and |z| < 25 cm
      if(printInfo) std::cout<<"SimVertex with id = "<<simvertexid<<" and position (in cm) : [x,y,z] = ["<<vtx_x<<","<<vtx_y<<","<<vtx_z<<"] or [r,z] = ["<<vtx_r<<","<<vtx_z<<"]"<<std::endl;
    }
  }
  */
  // =======================

  // Save Particles in separate collection [heavy]
  // std::vector< std::unique_ptr<HepMC::GenParticle> >   GEN_muons_signal, GEN_muons_bkgnd;
  // std::vector< std::unique_ptr<SimTrack> >             SIM_muons_signal, SIM_muons_bkgnd;   

  // Save index to Particles in a vector
  /*
  std::vector<unsigned int> index_genpart_signal, index_genpart_background; index_genpart_signal_mother;
  std::vector<unsigned int> index_simtrck_signal, index_simtrck_background;
  index_genpart_signal.clear(); index_genpart_background.clear(); index_simtrck_signal.clear(); index_simtrck_background.clear();
  */

  // Analysis of GenParticles, SimTracks, SimVertices and SimHits
  // ====================================================================
  // Strategy: 
  // 1) select the two muons from the Z-decay in the GenParticles collection (using HepMC::GenEvent info) ==> save the index in a vector<int>
  // 2) select the corresponding SimTrack ==> save the index to the trackId in a vector<int> and save the index to the vertexId in a vector<int>
  // 3) vertexId --> SimVertex
  // 4) loop over ME0SimHits and select the SimHits made by the SimTrack
  // 5) loop over ME0Muon --> ME0Segment --> ME0RecHits and match these rechits to simhits above
  // 6) you obtained a ME0Muon matched to the genParticle of the Z-decay! 
  // 7) Repeat for RECO Muons: loop over DT and CSC SimHits and select SimHits made by the SimTrack      (added in step 4)
  // 8) loop over Muon-->(DT/CSC)Segment-->(DT/CSC)RecHits and match these rechits to the simhits above
  // 9) you obtained a Muon matched to the genParticle of the Z-decay!
  // --------------------------------------------------------------------
  /*
  // now made in constructor
  std::vector<int> indmu, trkmu, vtxmu;
  std::vector< std::vector<const PSimHit*> > simhitme0mu; 
  std::vector< std::vector<const PSimHit*> > simhitrecomu; 
  std::vector<ME0DetId> me0mudetid;   // std::vector<CSCDetId> cscmudetid;  std::vector<DTDetId> dtmudetid;  
  std::vector< std::vector< std::pair<int,int> > > me0mu;         // old style :: pair of < [int] index to me0muon position in collection, [int] number of matched hits > 
  std::vector< std::vector< std::pair<int,int> > > recomu;        // old style :: pair of < [int] index to recomuon position in collection, [int] number of matched segments >
  std::vector< std::vector< std::pair<int,int> > > looseme0mu;    // old style :: pair of < [int] index to me0muon position in collection, [int] number of matched hits > 
  std::vector< std::vector< std::pair<int,int> > > tightme0mu;    // old style :: pair of < [int] index to me0muon position in collection, [int] number of matched hits > 
  // std::vector< std::vector< std::pair<int,double> > > me0mu;   // new style :: pair of < [int] index to me0muon position in collection, [double] match quality >
  // std::vector< std::vector< std::pair<int,double> > > recomu;  // new style :: pair of < [int] index to recomuon position in collection, [double] match quality >
  std::vector< std::map<uint32_t, std::vector<const PSimHit*> > > me0simhitmap;
  std::vector< std::map<uint32_t, std::vector<const PSimHit*> > > cscsimhitmap;
  std::vector< std::map<uint32_t, std::vector<const PSimHit*> > > dtsimhitmap;
  */
  indmu.clear(); trkmu.clear(); vtxmu.clear();
  simhitme0mu.clear(); simhitrecomu.clear();  me0mudetid.clear();   
  me0mu.clear(); recomu.clear(); looseme0mu.clear(); tightme0mu.clear();
  me0simhitmap.clear(); cscsimhitmap.clear(); dtsimhitmap.clear();


  // 1) loop over GenParticle container
  // --------------------------------------------------------------------
  bool skip=false;    
  bool accepted = false;
  bool foundmuons=false;
  HepMC::GenEvent * myGenEvent = new  HepMC::GenEvent(*(hepmcevent->GetEvent()));
      
  for ( HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) { 
    if ( !accepted && ( (*p)->pdg_id() == 23 ) && (*p)->status() == 3 ) { 
      accepted=true;
      for( HepMC::GenVertex::particle_iterator aDaughter=(*p)->end_vertex()->particles_begin(HepMC::descendants); aDaughter !=(*p)->end_vertex()->particles_end(HepMC::descendants);aDaughter++){
	if ( abs((*aDaughter)->pdg_id())==13) {
	  foundmuons=true;
	  if ((*aDaughter)->status()!=1 ) {
	    for( HepMC::GenVertex::particle_iterator byaDaughter=(*aDaughter)->end_vertex()->particles_begin(HepMC::descendants); 
		 byaDaughter !=(*aDaughter)->end_vertex()->particles_end(HepMC::descendants);byaDaughter++){
	      if ((*byaDaughter)->status()==1 && abs((*byaDaughter)->pdg_id())==13) {
		bool found = checkVector(indmu,(*byaDaughter)->barcode());           
		if(!found) indmu.push_back((*byaDaughter)->barcode());
		if(printInfoHepMC) std::cout<<"Stable muon from Z with pdgId "<<std::showpos<<(*byaDaughter)->pdg_id()<<" and index "<<(*byaDaughter)->barcode()<<(found?" not":"")<<" added"<<std::endl;
	      }
	    }
	  }
	  else {
	    bool found = checkVector(indmu,(*aDaughter)->barcode());
	    if(!found) indmu.push_back((*aDaughter)->barcode());
	    if(printInfoHepMC) std::cout << "Stable muon from Z with pdgId "<<std::showpos<<(*aDaughter)->pdg_id()<<" and index "<<(*aDaughter)->barcode()<<(found?" not":"")<<" added"<<std::endl;
	  }       
	}           
      }
      if (!foundmuons){
	if(printInfoHepMC) std::cout << "No muons from Z ...skip event" << std::endl;
	skip=true;
      } 
    }
  }
     
  if ( !accepted) {
    if(printInfoHepMC) std::cout << "No Z particles in the event ...skip event" << std::endl;
    skip=true;
  }   
  else {
    skip=false; 
  }
  if(skip) return;

  // Ease debugging ::: run only over events that contain a GenParticle Muon within 2.00 < | eta | < 3.00
  /*
  bool forwardMuon = false;
  for(unsigned int i=0; i<indmu.size(); ++i) {
    double genparteta = myGenEvent->barcode_to_particle(indmu.at(i))->momentum().eta();
    if(fabs(genparteta) > 2.00 && fabs(genparteta) < 2.80) forwardMuon = true;
  }
  if(!forwardMuon) return; // stop program here
  */

  // Fill the categories ... 
  // int EventType = -2; // -1 = 2 RecoMu; 0 = 1 RecoMu + 1 ME0Mu; +1 = 2 ME0Mu;
  int MuInME0 = 0;
  for(unsigned int i=0; i<indmu.size(); ++i) {
    double genparteta = myGenEvent->barcode_to_particle(indmu.at(i))->momentum().eta();
    if(fabs(genparteta) > 2.00 && fabs(genparteta) < 2.80) { ++MuInME0; }
  }
  if(MuInME0==0) Categories_EventInfo->Fill(2);      
  else if(MuInME0==1) Categories_EventInfo->Fill(3); 
  else if(MuInME0==2) Categories_EventInfo->Fill(4); 
  else {}

  if(InvestigateOnlyME0 && (MuInME0==0)) return;

  if(printInfoAll) {
    for(unsigned int i=0; i<indmu.size(); ++i) {
      std::cout<<"GEN Muon: id = "<<std::showpos<<std::setw(2)<<myGenEvent->barcode_to_particle(indmu.at(i))->pdg_id()<<" | index = "<<indmu.at(i);
      std::cout<<" | eta = "<<std::setw(9)<<myGenEvent->barcode_to_particle(indmu.at(i))->momentum().eta()<<" | phi = "<<std::setw(9)<<myGenEvent->barcode_to_particle(indmu.at(i))->momentum().phi();
      std::cout<<" | pt = "<<std::setw(9)<<myGenEvent->barcode_to_particle(indmu.at(i))->momentum().perp()<<std::endl;
    }
  }
  // --------------------------------------------------------------------
  // pre-define the vectors based on the size of indmu vector
  for(unsigned int i=0; i<indmu.size(); ++i) { 
    trkmu.push_back(-1); vtxmu.push_back(-1);
    me0mudetid.push_back(ME0DetId(1,0,0,0)); // ME0DetId(region layer   chamber roll)
    // cscmudetid.push_back(CSCDetId());     // CSCDetId(region station ring    chamber    layer)
    // dtmudetid.push_back(DTWireId());      // DTWireId(wheel  station sector  superlayer layer wire)
    std::vector<const PSimHit*> tmp1; simhitme0mu.push_back(tmp1);
    std::vector<const PSimHit*> tmp2; simhitrecomu.push_back(tmp2);
    std::vector< std::pair<int,int> > tmp3; me0mu.push_back(tmp3); 
    std::vector< std::pair<int,int> > tmp4; recomu.push_back(tmp4); 
    std::vector< std::pair<int,int> > tmp5; looseme0mu.push_back(tmp5); 
    std::vector< std::pair<int,int> > tmp6; tightme0mu.push_back(tmp6); 
    std::map<uint32_t, std::vector<const PSimHit*> > tmpmap1; me0simhitmap.push_back(tmpmap1);
    std::map<uint32_t, std::vector<const PSimHit*> > tmpmap2; cscsimhitmap.push_back(tmpmap2);
    std::map<uint32_t, std::vector<const PSimHit*> > tmpmap3; dtsimhitmap.push_back(tmpmap3);
  }
  // --------------------------------------------------------------------


  // 2) loop over SimTrack container
  // --------------------------------------------------------------------
  for (edm::SimTrackContainer::const_iterator it = SimTk->begin(); it != SimTk->end(); ++it) {
    std::unique_ptr<SimTrack> simtrack = std::unique_ptr<SimTrack>(new SimTrack(*it));
    if(fabs(simtrack->type()) != 13) continue;
    // match to GenParticles
    if(it->genpartIndex() == indmu[0]) { 
      trkmu[0] = simtrack->trackId();    // !!! starts counting at 1, not at 0 !!!
      vtxmu[0] = simtrack->vertIndex();
    }
    if(it->genpartIndex() == indmu[1]) {
      trkmu[1] = simtrack->trackId();
      vtxmu[1] = simtrack->vertIndex();
    }
    // some printout
    if(it->genpartIndex() == indmu[0] || it->genpartIndex() == indmu[1]) { 
      // int simtrack_trackId = simtrack->trackId();
      // int simtrack_vertxId = simtrack->vertIndex();
      if(printInfoAll) {
	std::cout<<"SIM Muon: id = "<<std::setw(2)<<it->type()<<" | trackId = "<<it->trackId()<<" | vertexId = "<<it->vertIndex()<<" | genpartIndex = "<<it->genpartIndex();
	std::cout<<" | eta = "<<std::setw(9)<<it->momentum().eta()<<" | phi = "<<std::setw(9)<<it->momentum().phi();
	std::cout<<" | pt = "<<std::setw(9)<<it->momentum().pt()<<std::endl;
      }
    }
  }
  // --------------------------------------------------------------------


  // 3) pick up the associated SimVtx
  // --------------------------------------------------------------------
  for (unsigned int i=0; i<vtxmu.size(); ++i) {
    if(vtxmu[i] == -1) continue;
    SimVertex simvertex = theSimVertices.at(vtxmu[i]);
    unsigned int simvertexid = simvertex.vertexId();
    vtx_x = simvertex.position().x(); vtx_y = simvertex.position().y(); vtx_r = sqrt(pow(vtx_x,2)+pow(vtx_y,2)); vtx_z = simvertex.position().z();
    if(printInfoAll) {
      std::cout<<"|--> SimVertex with id = "<<simvertexid<<" and position (in cm) : [x,y,z] = ["<<vtx_x<<","<<vtx_y<<","<<vtx_z<<"] or [r,z] = ["<<vtx_r<<","<<vtx_z<<"]"<<std::endl;
      if(vtx_r < 2 && fabs(vtx_z) < 25) std::cout<<"     ==> is a Primary (or Secondary)Vertex"<<std::endl;
      else                              std::cout<<"     ==> must be a Decay Vertex"<<std::endl;
    }
  }
  // --------------------------------------------------------------------

      
  // 4) then loop over the SimHit Container
  // --------------------------------------------------------------------
  for (std::vector<PSimHit>::const_iterator iHit = theSimHits.begin(); iHit != theSimHits.end(); ++iHit) {
    DetId theDetUnitId((*iHit).detUnitId());
    DetId simdetid= DetId((*iHit).detUnitId());
    /*
      int pid            = (*iHit).particleType();
      int process        = (*iHit).processType();
      double time        = (*iHit).timeOfFlight();
      double log_time    = log10((*iHit).timeOfFlight());
      double log_energy  = log10((*iHit).momentumAtEntry().perp()*1000); // MeV
      double log_deposit = log10((*iHit).energyLoss()*1000000);          // keV
    */
    int simhit_trackId = (*iHit).trackId();
    
    if(simdetid.det()==DetId::Muon &&  simdetid.subdetId()== MuonSubdetId::ME0){ // Only ME0
      ME0DetId me0id(theDetUnitId);
      GlobalPoint ME0GlobalPoint = me0Geom->etaPartition(me0id)->toGlobal((*iHit).localPosition());      
      // If SimTrack Matches SimHit Mother then save a pointer to the SimHit
      for(unsigned int i=0; i<indmu.size(); ++i) {
	if(simhit_trackId == trkmu[i]) {
	  // --- old method ---
	  std::vector<const PSimHit*> tmp = simhitme0mu[i];
	  tmp.push_back(&(*iHit)); 
	  simhitme0mu[i] = tmp;
	  me0mudetid[i] = ME0DetId(me0id.region(), 0, me0id.chamber(),0); // ME0chamber id
	  // ------------------
	  // --- new method ---
	  ME0DetId detid(me0id.region(), 0, me0id.chamber(),0);
	  std::map<uint32_t, std::vector<const PSimHit*> > map = me0simhitmap[i];
	  std::map<uint32_t, std::vector<const PSimHit*> >::iterator it;
	  it = map.find(detid.rawId());
	  if (it != map.end()) { // detid found in the map
	    std::vector<const PSimHit*> vec = map[detid.rawId()];
	    vec.push_back(&(*iHit));
	    map[detid.rawId()] = vec;
	  }
	  else{ // detid not found in the map: create entry
	    std::vector<const PSimHit*> vec;
	    vec.push_back(&(*iHit));
	    map[detid.rawId()] = vec;
	  }
	  me0simhitmap[i] = map;
	  // ------------------
	}
      }
      if(simhit_trackId == trkmu[0] || simhit_trackId == trkmu[1]) {
	if(printInfoAll) {
	  std::cout<<"ME0 SimHit in "<<std::setw(12)<<(int)me0id<<me0id<<" from simtrack with trackId = "<<std::setw(2)<<(*iHit).trackId();
	  std::cout<<" | time t = "<<std::setw(12)<<(*iHit).timeOfFlight()<<" | phi = "<<std::setw(12)<<ME0GlobalPoint.phi()<<" | eta = "<<std::setw(12)<<ME0GlobalPoint.eta();
	  std::cout<<" | global position = "<<ME0GlobalPoint;
	  std::cout<<""<<std::endl;
	}
      }
    }
    else if(simdetid.det()==DetId::Muon &&  simdetid.subdetId()== MuonSubdetId::CSC){ // Only CSC
      CSCDetId layerId(theDetUnitId);
      GlobalPoint CSCGlobalPoint = cscGeom->idToDet(layerId)->toGlobal((*iHit).localPosition());
      // If SimTrack Matches SimHit Mother then save a pointer to the SimHit
      for(unsigned int i=0; i<indmu.size(); ++i) {
	if(simhit_trackId == trkmu[i]) {
	  // --- old method ---
	  std::vector<const PSimHit*> tmp = simhitrecomu[i];
	  tmp.push_back(&(*iHit)); 
	  simhitrecomu[i] = tmp;
	  // recomudetid[i] = layerId.chamberId(); // no point in keeping CSCchamber id
	  // ------------------
	  // --- new method ---
	  CSCDetId detid = layerId.chamberId();
	  std::map<uint32_t, std::vector<const PSimHit*> > map = cscsimhitmap[i];
	  std::map<uint32_t, std::vector<const PSimHit*> >::iterator it;
	  it = map.find(detid.rawId());
	  if (it != map.end()) { // detid found in the map
	    std::vector<const PSimHit*> vec = map[detid.rawId()];
	    vec.push_back(&(*iHit));
	    map[detid.rawId()] = vec;
	  }
	  else{ // detid not found in the map: create entry
	    std::vector<const PSimHit*> vec;
	    vec.push_back(&(*iHit));
	    map[detid.rawId()] = vec;
	  }
	  cscsimhitmap[i] = map;
	  // ------------------
	}
      }
      if(simhit_trackId == trkmu[0] || simhit_trackId == trkmu[1]) {
	if(printInfoAll) {
	  std::cout<<"CSC SimHit in "<<std::setw(12)<<(int)layerId<<layerId<<" from simtrack with trackId = "<<std::setw(2)<<(*iHit).trackId();
	  std::cout<<" | time t = "<<std::setw(12)<<(*iHit).timeOfFlight()<<" | phi = "<<std::setw(12)<<CSCGlobalPoint.phi()<<" | eta = "<<std::setw(12)<<CSCGlobalPoint.eta();
	  std::cout<<" | global position = "<<CSCGlobalPoint;
	  std::cout<<""<<std::endl;
	}
      }
    }
    else if(simdetid.det()==DetId::Muon &&  simdetid.subdetId()== MuonSubdetId::DT){ // Only DT
      DTWireId wireId(theDetUnitId);
      GlobalPoint DTGlobalPoint = dtGeom->idToDet(wireId)->toGlobal((*iHit).localPosition());
      // If SimTrack Matches SimHit Mother then save a pointer to the SimHit
      for(unsigned int i=0; i<indmu.size(); ++i) {
	if(simhit_trackId == trkmu[i]) {
	  // --- old method ---
	  std::vector<const PSimHit*> tmp = simhitrecomu[i];
	  tmp.push_back(&(*iHit)); 
	  simhitrecomu[i] = tmp;
	  // recomudetid[i] = wireId.chamberId(); // no point in keeping DTchamber id
	  // ------------------
	  // --- new method ---
	  DTChamberId detid = wireId.chamberId();
	  std::map<uint32_t, std::vector<const PSimHit*> > map = dtsimhitmap[i];
	  std::map<uint32_t, std::vector<const PSimHit*> >::iterator it;
	  it = map.find(detid.rawId());
	  if (it != map.end()) { // detid found in the map
	    std::vector<const PSimHit*> vec = map[detid.rawId()];
	    vec.push_back(&(*iHit));
	    map[detid.rawId()] = vec;
	  }
	  else{ // detid not found in the map: create entry
	    std::vector<const PSimHit*> vec;
	    vec.push_back(&(*iHit));
	    map[detid.rawId()] = vec;
	  }
	  dtsimhitmap[i] = map;
	  // ------------------
	}
      }
      if(simhit_trackId == trkmu[0] || simhit_trackId == trkmu[1]) {
	if(printInfoAll) {
	  std::cout<<"DT  SimHit in "<<std::setw(12)<<(int)wireId<<wireId<<" from simtrack with trackId = "<<std::setw(2)<<(*iHit).trackId();
	  std::cout<<" | time t = "<<std::setw(12)<<(*iHit).timeOfFlight()<<" | phi = "<<std::setw(12)<<DTGlobalPoint.phi()<<" | eta = "<<std::setw(12)<<DTGlobalPoint.eta();
	  std::cout<<" | global position = "<<DTGlobalPoint;
	  std::cout<<""<<std::endl;
	}
      }
    }
    else {}
  }
  // --------------------------------------------------------------------


  // 5) Loop over ME0Muons and ask for the ME0RecHits of the ME0Segment
  // --------------------------------------------------------------------
  if(printInfoME0Match) {
    std::cout<<" Number of ME0Muons in this event = "<<me0muons->size()<<std::endl;
    std::cout<<" Number of ME0Sgmts in this event = "<<me0segments->size()<<std::endl;
    std::cout<<" =====     Start Matching     ===== "<<std::endl;
  }
  int me0muonpos = -1;
  for(std::vector<reco::ME0Muon>::const_iterator it=me0muons->begin(); it!=me0muons->end(); ++it) {

    ++me0muonpos;

    // 0) Neglect ME0Muons if quality is not good or innerTrack does not exist
    // if (!muon::isGoodMuon(me0Geom, *it, muon::Tight)) continue;
    // if(!it->innerTrack()) continue;

    // 1) Neglect ME0 Muons for which the ME0Segment is not in the same chamber as the Signal SimHits
    ME0DetId       segId = ME0DetId(it->me0segment().geographicalId());
    int matchedGENMu = -1;
    if      (segId.region() == me0mudetid[0].region() && segId.chamber() == me0mudetid[0].chamber()) matchedGENMu = 0; 
    else if (segId.region() == me0mudetid[1].region() && segId.chamber() == me0mudetid[1].chamber()) matchedGENMu = 1;
    else continue;

    // 2) Print Out
    reco::TrackRef tkRef = it->innerTrack();
    ME0Segment    segRef = it->me0segment();

    if(printInfoME0Match){
      std::cout<<"ME0Muon in "<<segId<<" with eta = "<<it->eta()<<" phi = "<<it->phi()<<" pt = "<<it->pt()<<std::endl;
      std::cout<<"        InnerTrack :: eta = "<<tkRef->eta()<<" phi = "<<tkRef->phi()<<" pt = "<<tkRef->pt()<<std::endl;
      std::cout<<"           Segment :: eta = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localPosition())).eta()
	       <<" phi = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localPosition())).phi()
	       <<" dir eta = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localDirection())).eta()
	       <<" dir phi = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localDirection())).phi()
	       <<" ME0SegRefId = "<<it->me0segid()<<" time = "<<segRef.time()<<" +/- "<<segRef.timeErr()<<" Nhits = "<<segRef.nRecHits()<<" index = "<<me0muonpos<<std::endl;
    }


    // 3) Perform Matching based on global position of SimHits and RecHits
    // Loop first over SimHits => reduce running time
    int NmatchedToSegment = 0;

    for(unsigned int j=0; j<simhitme0mu[matchedGENMu].size(); ++j) {
      ME0DetId simhitME0id(simhitme0mu[matchedGENMu][j]->detUnitId());

      if(printInfoME0Match){      
	std::cout<<"     === ME0 SimHit in "<<std::setw(12)<<simhitME0id.rawId()<<" = "<<simhitME0id<<" from simtrack with trackId = ";
	std::cout<<std::setw(9)<<simhitme0mu[matchedGENMu][j]->trackId()<<" | time t = "<<std::setw(12)<<simhitme0mu[matchedGENMu][j]->timeOfFlight();
	std::cout<<" | phi = "<<std::setw(12)<<((me0Geom->etaPartition(simhitME0id))->toGlobal(simhitme0mu[matchedGENMu][j]->localPosition())).phi();
	std::cout<<" | eta = "<<std::setw(12)<<((me0Geom->etaPartition(simhitME0id))->toGlobal(simhitme0mu[matchedGENMu][j]->localPosition())).eta()<<std::endl;
      }
      
      const std::vector<ME0RecHit> me0rechits = segRef.specificRecHits();
      for(std::vector<ME0RecHit>::const_iterator rh=me0rechits.begin(); rh!=me0rechits.end(); ++rh) {
	ME0DetId rechitME0id = rh->me0Id();
	
	// 3a verify whether simhits and rechits are in same detid
	if(rechitME0id != simhitME0id) continue;

	if(printInfoME0Match){
	  std::cout<<"          === ME0 RecHit in "<<std::setw(12)<<rechitME0id.rawId()<<" = "<<rechitME0id<<" from ME0Muon with sgmntId = ";
	  std::cout<<std::setw(9)<<it->me0segid()<<" | time t = "<<std::setw(12)<<rh->tof();
	  std::cout<<" | phi = "<<std::setw(12)<<((me0Geom->etaPartition(rechitME0id))->toGlobal(rh->localPosition())).phi();
	  std::cout<<" | eta = "<<std::setw(12)<<((me0Geom->etaPartition(rechitME0id))->toGlobal(rh->localPosition())).eta()<<std::endl;
	}

	// 3b compare global position of simhit and rechit
	GlobalPoint rechitGP = (me0Geom->etaPartition(rechitME0id))->toGlobal(rh->localPosition());
	GlobalPoint simhitGP = (me0Geom->etaPartition(simhitME0id))->toGlobal(simhitme0mu[matchedGENMu][j]->localPosition());
	double drGlob = sqrt(pow(rechitGP.x()-simhitGP.x(),2)+pow(rechitGP.y()-simhitGP.y(),2)+pow(rechitGP.z()-simhitGP.z(),2));
	double dRGlob = sqrt(pow(rechitGP.x()-simhitGP.x(),2)+pow(rechitGP.y()-simhitGP.y(),2));
	// given that we are in the same eta partition, we can just work with local coordinates
	LocalPoint rechitLP = rh->localPosition();
	LocalPoint simhitLP = simhitme0mu[matchedGENMu][j]->localPosition();
	LocalError rechitLPE = rh->localPositionError();
	double dRLoc  = sqrt(pow(rechitLP.x()-simhitLP.x(),2)+pow(rechitLP.y()-simhitLP.y(),2));
	double dXLoc = rechitLP.x()-simhitLP.x(); 
	double dYLoc = rechitLP.y()-simhitLP.y(); 
	if(printInfoME0Match){
	  std::cout<<"          === Comparison :: Local dR = "<<std::setw(9)<<dRLoc<<" | Global dR = "<<std::setw(9)<<dRGlob<<" Global dr = "<<std::setw(9)<<drGlob
		   <<" dXLoc = "<<std::setw(9)<<dXLoc<<" +/- "<<sqrt(rechitLPE.xx())<<" [cm] dYLoc = "<<std::setw(9)<<dYLoc<<" +/- "<<sqrt(rechitLPE.yy())<<" [cm]"<<std::endl;
	}
	// allow matching within 3 sigma for both local X and local Y:: look at smearing values in PseudoDigitizer (0.05 for X and 1.0 for Y)
	if(fabs(dXLoc) < 3*preDigiSmearX && fabs(dYLoc) < 3*preDigiSmearY) {
	  if(printInfoME0Match) std::cout<<"          === Matched :: |dXLoc| = "<<fabs(dXLoc)<<" < 3*sigX = "<<3*preDigiSmearX<<" && |dYLoc| = "<<fabs(dYLoc)<<" < 3*sigY = "<<3*preDigiSmearY<<std::endl;
	  ++NmatchedToSegment;
	}
      }
    }
    if(printInfoME0Match) std::cout<<"=== Number of Matched Hits :: "<<NmatchedToSegment<<std::endl;
    if(NmatchedToSegment > (nMatchedHitsME0Seg-1)) { // NmatchedToSegment >= nMatchedHits ==> consider the segment matched to genparticle
      std::vector< std::pair<int,int> > tmp = me0mu[matchedGENMu];
      tmp.push_back(std::make_pair(me0muonpos,NmatchedToSegment));
      me0mu[matchedGENMu] = tmp;
      // me0mu[matchedGENMu]=me0muonpos;
      // me0muMatchedHits[matchedGENMu] = NmatchedToSegment;
      // Loose ME0 Muon Matched
      if (muon::isGoodMuon(me0Geom, *it, muon::Loose)) {
	std::vector< std::pair<int,int> > tmp5 = looseme0mu[matchedGENMu];
	tmp5.push_back(std::make_pair(me0muonpos,NmatchedToSegment));
	looseme0mu[matchedGENMu] = tmp5;
      }
      // Tight ME0 Muon Matched
      if (muon::isGoodMuon(me0Geom, *it, muon::Tight)) {
	std::vector< std::pair<int,int> > tmp6 = tightme0mu[matchedGENMu];
	tmp6.push_back(std::make_pair(me0muonpos,NmatchedToSegment));
	tightme0mu[matchedGENMu] = tmp6;
      }
      NewMatch_ME0HitsMatched->Fill(NmatchedToSegment);
      NewMatch_ME0HitsTotal->Fill(segRef.nRecHits());  
      if(printInfoME0Match){ std::cout<<"NewMatch_ME0HitsMatched->Fill("<<NmatchedToSegment<<") wrt NewMatch_ME0HitsTotal->Fill("<<segRef.nRecHits()<<")"<<std::endl;}
    }
  }
  // --------------------------------------------------------------------
  for(unsigned int i=0; i<indmu.size(); ++i) { 
    NewMatch_ME0MuonsMatched->Fill(me0mu[i].size()); 
    NewMatch_LooseME0MuonsMatched->Fill(looseme0mu[i].size()); 
    NewMatch_TightME0MuonsMatched->Fill(tightme0mu[i].size()); 
  }
  // in case there is more than one ME0Muon matched to a single GenParticle Muon, sort on highest quality
  for(unsigned int i=0; i<indmu.size(); ++i) {
    // Sorting with C++11 compiler using lambdas:
    // see: http://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair
    if(me0mu[i].size() > 1) {
      std::sort(me0mu[i].begin(), me0mu[i].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) { return left.second < right.second; });
    } 
    if(looseme0mu[i].size() > 1) {
      std::sort(looseme0mu[i].begin(), looseme0mu[i].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) { return left.second < right.second; });
    } 
    if(tightme0mu[i].size() > 1) {
      std::sort(tightme0mu[i].begin(), tightme0mu[i].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) { return left.second < right.second; });
    } 
    /*
    if(me0mu[i].size() > 1) {
      std::sort(me0mu[i].begin(), me0mu[i].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) { 
	  if(left.second == right.second) {return theME0Muons.at(left.first).pt() < theME0Muons.at(right.first).pt();}
	  else {return left.second < right.second;} 
	  });
    } 
    */
    //*/
    /*
    if(me0mu[i].size() > 1) {
      std::sort(me0mu[i].begin(), me0mu[i].end(), sortME0muons);
    } 
    if(looseme0mu[i].size() > 1) {
      std::sort(looseme0mu[i].begin(), looseme0mu[i].end(), sortME0muons);
    } 
    if(tightme0mu[i].size() > 1) {
      std::sort(tightme0mu[i].begin(), tightme0mu[i].end(), sortME0muons);
    } 
    */
    /*
    if(me0mu[i].size() > 1) {
      std::sort(me0mu[i].begin(), me0mu[i].end(), mySortingStruct);
    }    
    */
  }
  // --------------------------------------------------------------------



  // 6) Loop over Muons and ask for the (CSC/DT)RecHits of the (CSC/DT)Segment
  // --------------------------------------------------------------------
  if(printInfoMuonMatch) {
    std::cout<<" Number of Muons in this event = "<<muons->size()<<std::endl;
    // std::cout<<" Number of CSCSgmts in this event = "<<me0segments->size()<<std::endl;
    // std::cout<<" Number of DT Sgmts in this event = "<<me0segments->size()<<std::endl;
    std::cout<<" =====     Start Matching     ===== "<<std::endl;
  }
  int recomuonpos = -1;
  for(std::vector<reco::Muon>::const_iterator it=muons->begin(); it!=muons->end(); ++it) {

    ++recomuonpos;
    if(printInfoMuonMatch) {
      std::cout<<"recoMuon :: ch = "<<std::setw(2)<<it->charge()<<" | eta = "<<std::setw(9)<<it->eta(); 
      std::cout<<" | phi = "<<std::setw(9)<<it->phi()<<" | pt = "<<std::setw(9)<<it->pt();     
      std::cout<<" | RECOMuonId  = "<<std::setw(3)<<recomuonpos;
      std::cout<<" | OuterTrack?  = "<<it->outerTrack().isNonnull();
      std::cout<<" | InnerTrack?  = "<<it->innerTrack().isNonnull();
      std::cout<<" | globalTrack?  = "<<it->globalTrack().isNonnull();
      std::cout<<" | isGlobalMuon?  = "<<it->isGlobalMuon();
      if(it->outerTrack().isNonnull()!=0) {
	std::cout<<" | Nhits = "<<it->outerTrack().get()->recHitsSize();
	std::cout<<" | Chi2/ndof = "<<it->outerTrack().get()->chi2()<<"/"<<it->outerTrack().get()->ndof();
      }
      if(it->innerTrack().isNonnull()!=0) {
	std::cout<<" | dxy = "<<it->innerTrack().get()->dxy()<<" [cm] | dz = "<<it->innerTrack().get()->dz()<<" [cm]";
      }
      std::cout<<std::endl;  
    }

    // 0) Neglect Muons if they are not reconstructed as global muon or not identified as tight muon
    //    I know ... introduces a bias ... but I want to perform a decent matching in the muon system
    //    Anyway Global Muon / Tight Muon efficiency on signal (from Drell Yan) is pretty high
    if (!it->isGlobalMuon()) continue;
    if(!it->outerTrack()) continue;
    if(!it->innerTrack()) continue;
    // if(!muon::isTightMuon(*it, *(theRecoVertices.begin()))) continue;

    // 1) Print Out

    // 2) Compare the chamberId of the segments to the chamberId of the SimHits
    // and neglect Muons for which no match is found
    // require at least 2 matched segments (I know ... again a bias, because forgets about the RPCs)
    // one can see this already as a prematching, 
    // to reduce the real matching only to the muons with simhits and rechits in the same chamber
    for(unsigned int i=0; i<indmu.size(); ++i) {

      int nCSCChambersMatched = 0, nDTChambersMatched = 0, nSegmentsTotal = 0, matchedGENMu = -1;
      trackingRecHit_iterator rhbegin = it->outerTrack().get()->recHitsBegin();
      trackingRecHit_iterator rhend = it->outerTrack().get()->recHitsEnd();
      for(trackingRecHit_iterator recHit = rhbegin; recHit != rhend; ++recHit) {
	DetId detid = DetId((*recHit)->geographicalId());
	// std::cout<<"Tracking RecHit in DetId "<<detid.rawId()<<" ==> det = "<<detid.det()<<" subdet = "<<detid.subdetId()<<" [Muon det = "<<DetId::Muon
	//          <<" DT subdet = "<<MuonSubdetId::DT<<" CSC subdet = "<<MuonSubdetId::CSC<<" RPC subdet = "<<MuonSubdetId::RPC<<" GEM subdet = "<<MuonSubdetId::GEM<<"]"<<std::endl;
	// 2a) if the segment is a CSC segment
	// -----------------------------------
	if(detid.det()==DetId::Muon && detid.subdetId()== MuonSubdetId::CSC) {
	  ++nSegmentsTotal;
	  CSCDetId chamberId(detid);
	  if(printInfoMuonMatchDetail) std::cout<<"Tracking RecHit in DetId "<<detid.rawId()<<" = CSCDetId "<<chamberId<<std::endl;
	  std::map<uint32_t, std::vector<const PSimHit*> > map = cscsimhitmap[i];
	  std::map<uint32_t, std::vector<const PSimHit*> >::iterator it;
	  if(printInfoMuonMatchDetail) {for(it=map.begin();it!=map.end();++it){std::cout<<"CSC SimHit map ["<<i<<"] ==> element < detid = "<<it->first<<" = "<<CSCDetId(it->first)<<" has "<<it->second.size()<<" simhits"<<std::endl;}}
          it = map.find(chamberId.rawId());
          if (it != map.end()) { // detid found in the map           
	    // 2b) perform now detailed matching
	    int nCSCHitsMatched = 0;
	    //     obtain Rechits of this segment
	    std::vector<const TrackingRecHit*> CSCRecHits = (*recHit)->recHits();
	    //     obtain Simhits in this chamber
	    std::vector<const PSimHit*> CSCSimHits = map[chamberId.rawId()];
	    //     nested loop to compare them
	    for(std::vector<const TrackingRecHit*>::const_iterator rh = CSCRecHits.begin(); rh != CSCRecHits.end(); ++rh) {
	      CSCDetId rhCSCid((*rh)->geographicalId());
	      LocalPoint rechitLP = (*rh)->localPosition();
	      LocalError rechitLPE = (*rh)->localPositionError();
	      if(printInfoMuonMatchDetail) {
		std::cout<<"     === CSC RecHit in "<<std::setw(12)<<rhCSCid.rawId()<<" = "<<rhCSCid<<" from muon with index = ";
		std::cout<<std::setw(9)<<recomuonpos<</*" | wire t = "<<std::setw(12)<<(*rh)->wireTime()*/" | no time info TrackingRecHit";
		std::cout<<" | X = "<<std::setw(12)<<(*rh)->localPosition().x();
		std::cout<<" | Y = "<<std::setw(12)<<(*rh)->localPosition().y()<<std::endl;
	      }
	      for(std::vector<const PSimHit*>::const_iterator sh = CSCSimHits.begin(); sh != CSCSimHits.end(); ++sh) {
		CSCDetId shCSCid((*sh)->detUnitId());
		// if SimHit DetId not exactly the same as RecHit DetId => skip
		if(shCSCid != rhCSCid) continue; 
		LocalPoint simhitLP = (*sh)->localPosition();
		double dXLoc = rechitLP.x()-simhitLP.x();
		double dYLoc = rechitLP.y()-simhitLP.y();
		if(printInfoMuonMatchDetail) {
		  std::cout<<"          === CSC SimHit in "<<std::setw(12)<<shCSCid.rawId()<<" = "<<shCSCid<<" from simtrack with trackId = ";
		  std::cout<<std::setw(9)<<(*sh)->trackId()<<" | time t = "<<std::setw(12)<<(*sh)->timeOfFlight();
		  std::cout<<" | X = "<<std::setw(12)<<(*sh)->localPosition().x();
		  std::cout<<" | Y = "<<std::setw(12)<<(*sh)->localPosition().y()<<std::endl;
		}
		if(printInfoMuonMatchDetail){
		  std::cout<<"          === Comparison :: dXLoc = "<<std::setw(9)<<dXLoc<<" +/- "<<sqrt(rechitLPE.xx())
		  	   <<" [cm] dYLoc = "<<std::setw(9)<<dYLoc<<" +/- "<<sqrt(rechitLPE.yy())<<" [cm]"<<std::endl;
		}
		// allow matching within 3 sigma for both local X and local Y:: look at detector resolution of CSC (75-150um = 0.015cm for X and 5cm for Y)
		if(fabs(dXLoc) < 3*cscDetResX && fabs(dYLoc) < 3*cscDetResY) {
		  if(printInfoMuonMatchDetail) {
		    std::cout<<"          === Matched :: |dXLoc| = "<<fabs(dXLoc)<<" < 3*sigX = "<<3*cscDetResX<<" && |dYLoc| = "<<fabs(dYLoc)<<" < 3*sigY = "<<3*cscDetResY<<std::endl;
		  }
		  ++nCSCHitsMatched;
		}
	      } // end of SimHit Loop
	    } // end of RecHit Loop
	    if(nCSCHitsMatched > nMatchedHitsCSCSeg) { // consider a segment as matched if at least 3 hits are matched to simhits
	      ++nCSCChambersMatched;
	      NewMatch_CSCHitsMatched->Fill(nCSCHitsMatched); 
	      NewMatch_CSCHitsTotal->Fill(CSCRecHits.size()); 
	      if(printInfoMuonMatchDetail) {std::cout<<"NewMatch_CSCHitsMatched->Fill("<<nCSCHitsMatched<<") wrt NewMatch_CSCHitsTotal->Fill("<<CSCRecHits.size()<<")"<<std::endl;}
	    }
	    matchedGENMu = i; // need to implement this somehow .. do printout for now
	    if(printInfoMuonMatchDetail) std::cout<<"matchedGENMu = "<<matchedGENMu<<std::endl;
	  } // end of IF(detId in simhit map)
	} // end DetId = CSC DetId
	// 2c) if the segment is a DT segment
	// -----------------------------------
	else if(detid.det()==DetId::Muon && detid.subdetId()== MuonSubdetId::DT) {
	  ++nSegmentsTotal;
	  DTChamberId chamberId(detid);
	  if(printInfoMuonMatchDetail) std::cout<<"Tracking RecHit in DetId "<<detid.rawId()<<" = DTChamberId "<<chamberId<<std::endl;
	  std::map<uint32_t, std::vector<const PSimHit*> > map = dtsimhitmap[i];
	  std::map<uint32_t, std::vector<const PSimHit*> >::iterator it;
	  if(printInfoMuonMatchDetail) {for(it=map.begin();it!=map.end();++it){std::cout<<"DT SimHit map ["<<i<<"] ==> element < detid = "<<it->first<<" = "<<DTChamberId(it->first)<<" has "<<it->second.size()<<" simhits"<<std::endl;}}
          it = map.find(chamberId.rawId());
          if (it != map.end()) { // detid found in the map           
	    // 2d) perform now detailed matching
	    int nDTHitsMatched = 0;
	    int nDTHitsTotal = 0;
	    //     obtain Simhits in this chamber
	    std::vector<const PSimHit*> DTSimHits = map[chamberId.rawId()];
	    //     obtain Rechits of this segment
	    //     not as easy as the case of the CSC segment
	    //     the DT Segment is build of 2 or 3 SuperLayerSegments, which are made of DTRecHits
	    std::vector<const TrackingRecHit*> DTRecHitsL1 = (*recHit)->recHits();
	    for(std::vector<const TrackingRecHit*>::const_iterator rhL1 = DTRecHitsL1.begin(); rhL1 != DTRecHitsL1.end(); ++rhL1) {
	      std::vector< const TrackingRecHit * > DTRecHitsL2 = (*rhL1)->recHits();
	      for(std::vector< const TrackingRecHit * >::const_iterator rhL2 = DTRecHitsL2.begin(); rhL2 != DTRecHitsL2.end(); ++rhL2) {
		DTChamberId rhDTid((*rhL2)->geographicalId());
	        DTLayerId   rhDTLayerid((*rhL2)->geographicalId());
		// DTWireId rhDTWireid((*rhL2)->geographicalId());
		LocalPoint rechitLP = (*rhL2)->localPosition();
		LocalError rechitLPE = (*rhL2)->localPositionError();
		++nDTHitsTotal;
		if(printInfoMuonMatchDetail) {
		  std::cout<<"     === DT RecHit in chamber "<<rhDTid<<" :: layer "<<std::setw(12)<<rhDTLayerid.rawId()<<" = "<<rhDTLayerid<<" from muon with index = ";
		  std::cout<<std::setw(9)<<recomuonpos<</*" | wire t = "<<std::setw(12)<<(*rhL2)->wireTime()*/" | no time info TrackingRecHit";
		  std::cout<<" | X = "<<std::setw(12)<<(*rhL2)->localPosition().x();
		  std::cout<<" | Y = "<<std::setw(12)<<(*rhL2)->localPosition().y()<<std::endl;
		}
		for(std::vector<const PSimHit*>::const_iterator sh = DTSimHits.begin(); sh != DTSimHits.end(); ++sh) {
		  DTChamberId shDTid((*sh)->detUnitId());
		  DTLayerId   shDTLayerid((*sh)->detUnitId());
		  // DTWireId shDTWireid((*sh)->detUnitId());
		  // if SimHit DetId not exactly the same as RecHit DetId => skip
		  // if(shDTid != rhDTid) continue; 
		  if(shDTLayerid != rhDTLayerid) continue; 
		  // if(shDTWireid != rhDTWireid) continue; 
		  LocalPoint simhitLP = (*sh)->localPosition();
		  double dXLoc = rechitLP.x()-simhitLP.x();
		  double dYLoc = rechitLP.y()-simhitLP.y();
		  if(printInfoMuonMatchDetail) {
		    std::cout<<"          === DT SimHit in chamber "<<" = "<<shDTid<<" :: layer"<<std::setw(12)<<shDTLayerid.rawId()<<" = "<<shDTLayerid<<" from simtrack with trackId = ";
		    std::cout<<std::setw(9)<<(*sh)->trackId()<<" | time t = "<<std::setw(12)<<(*sh)->timeOfFlight();
		    std::cout<<" | X = "<<std::setw(12)<<(*sh)->localPosition().x();
		    std::cout<<" | Y = "<<std::setw(12)<<(*sh)->localPosition().y()<<std::endl;
		  }
		  if(printInfoMuonMatchDetail){
		    std::cout<<"          === Comparison :: dXLoc = "<<std::setw(9)<<dXLoc<<" +/- "<<sqrt(rechitLPE.xx())
			     <<" [cm] dYLoc = "<<std::setw(9)<<dYLoc<<" +/- "<<sqrt(rechitLPE.yy())<<" [cm]"<<std::endl;
		  }
		  // allow matching within 3 sigma for both local X and local Y:: look at detector resolution of DT (75-125um = 0.0125cm for X and 0.0400um for Y)
		  // DT station 4 is special station because has no measurement of Y-coordinate, therefore match only the X coordinate
		  if((rhDTid.station() != 4) && (fabs(dXLoc) < 3*dtDetResX && fabs(dYLoc) < 3*dtDetResY)) {
		    if(printInfoMuonMatchDetail) {
		      std::cout<<"          === Matched :: |dXLoc| = "<<fabs(dXLoc)<<" < 3*sigX = "<<3*dtDetResX<<" && |dYLoc| = "<<fabs(dYLoc)<<" < 3*sigY = "<<3*dtDetResY<<std::endl;
		    }
		    ++nDTHitsMatched;
		  }
		  else if((rhDTid.station() == 4) && (fabs(dXLoc) < 3*dtDetResX)) {
		    if(printInfoMuonMatchDetail) {
		      std::cout<<"          === Matched :: |dXLoc| = "<<fabs(dXLoc)<<" < 3*sigX = "<<3*dtDetResX<<std::endl;
		    }
		    ++nDTHitsMatched;
		  }
		  else {}
		} // end of SimHit Loop
	      } // end of RecHitL2 Loop
	    }// end of RecHitL1 Loop
	    if(printInfoMuonMatchDetail) std::cout<<"=== Number of matched hits in this chamber :: "<<nDTHitsMatched<<std::endl; 
	    if(nDTHitsMatched > nMatchedHitsDTSeg) { // consider a segment as matched if at least 6 hits are matched to simhits
	      ++nDTChambersMatched;
	      NewMatch_DTHitsMatched->Fill(nDTHitsMatched); 
	      NewMatch_DTHitsTotal->Fill(nDTHitsTotal);     
	      if(printInfoMuonMatchDetail) {std::cout<<"NewMatch_DTHitsMatched->Fill("<<nDTHitsMatched<<") wrt NewMatch_DTHitsTotal->Fill("<<nDTHitsTotal<<")" <<std::endl;}
	    }
	    matchedGENMu = i; // need to implement this somehow .. do printout for now
	    if(printInfoMuonMatchDetail) std::cout<<"matchedGENMu = "<<matchedGENMu<<std::endl;
	  } // end of IF(detId in simhit map)
	} // end DetId = DT DetId
	else {}
      }// end loop over rechits
      if(nCSCChambersMatched+nDTChambersMatched>1) {
	if(printInfoMuonMatch) { std::cout<<"=== Matched :: Number of CSC segments matched = "<<nCSCChambersMatched<<" Number of DT segments matched = "<<nDTChambersMatched<<" ==> recoMuon ["<<recomuonpos<<"] is Matched to genMuon"<<std::endl; }
	std::vector< std::pair<int,int> > tmp = recomu[matchedGENMu];
	tmp.push_back(std::make_pair(recomuonpos,nCSCChambersMatched+nDTChambersMatched));
	recomu[matchedGENMu] = tmp;
	NewMatch_SegmentsMatched->Fill(nCSCChambersMatched+nDTChambersMatched);
	NewMatch_SegmentsTotal->Fill(nSegmentsTotal);                          
	if(printInfoMuonMatch) {std::cout<<"NewMatch_SegmentsMatched->Fill("<<nCSCChambersMatched+nDTChambersMatched<<") wrt NewMatch_SegmentsTotal->Fill("<<nSegmentsTotal<<")"<<std::endl;}
      }
    } // end Loop over 2 GenParticle Muons

    /*
    if(printInfoMuonMatch){
      std::cout<<"ME0Muon in "<<segId<<" with eta = "<<it->eta()<<" phi = "<<it->phi()<<" pt = "<<it->pt()<<std::endl;
      std::cout<<"        InnerTrack :: eta = "<<tkRef->eta()<<" phi = "<<tkRef->phi()<<" pt = "<<tkRef->pt()<<std::endl;
      std::cout<<"           Segment :: eta = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localPosition())).eta()
               <<" phi = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localPosition())).phi()
               <<" dir eta = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localDirection())).eta()
               <<" dir phi = "<<(me0Geom->etaPartition(segId)->toGlobal(segRef.localDirection())).phi()
               <<" ME0SegRefId = "<<it->me0segid()<<" time = "<<segRef.time()<<" +/- "<<segRef.timeErr()<<" Nhits = "<<segRef.nRecHits()<<" index = "<<me0muonpos<<std::endl;
    }
    */
  }
  // --------------------------------------------------------------------
  for(unsigned int i=0; i<indmu.size(); ++i) { NewMatch_RecoMuonsMatched->Fill(recomu[i].size()); }
  // in case there is more than one ME0Muon matched to a single GenParticle Muon, sort on highest quality
  for(unsigned int i=0; i<indmu.size(); ++i) {
    if(recomu[i].size() > 1) {
      // Sorting with C++11 compiler using lambdas:
      // see: http://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair
      std::sort(recomu[i].begin(), recomu[i].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) { return left.second < right.second; });
    } 
  }
  // --------------------------------------------------------------------


  // Do a print out of all saved information
  // ---------------------------------------
  if(printInfoSignal) {
    for(unsigned int i=0; i<indmu.size(); ++i) {
      std::cout<<"=========================="<<std::endl;
      std::cout<<"=== Muon "<<i+1<<" Information ==="<<std::endl;
      std::cout<<"=========================="<<std::endl;
      if(indmu[i] != -1) {
	std::cout<<"=== GEN Muon :: id = "<<std::showpos<<std::setw(2)<<myGenEvent->barcode_to_particle(indmu.at(i))->pdg_id();
	std::cout<<" | eta = "<<std::setw(9)<<myGenEvent->barcode_to_particle(indmu.at(i))->momentum().eta()<<" | phi = "<<std::setw(9)<<myGenEvent->barcode_to_particle(indmu.at(i))->momentum().phi();
	std::cout<<" | pt = "<<std::setw(9)<<myGenEvent->barcode_to_particle(indmu.at(i))->momentum().perp()<<" |        index = "<<indmu.at(i)<<std::endl;
      }
      if(trkmu[i] != -1) {
	std::cout<<"=== SIM Muon :: id = "<<std::setw(2)<<SimTk->at(trkmu.at(i)-1).type()<<" | eta = "<<std::setw(9)<<SimTk->at(trkmu.at(i)-1).momentum().eta(); // !!! starts counting at 1, not at 0 !!!
	std::cout<<" | phi = "<<std::setw(9)<<SimTk->at(trkmu.at(i)-1).momentum().phi()<<" | pt = "<<std::setw(9)<<SimTk->at(trkmu.at(i)-1).momentum().pt();     // trackId = 1 is accessed by SimTk->at(0)
	std::cout<<" | genpartIndex = "<<SimTk->at(trkmu.at(i)-1).genpartIndex()<<" | trackId = "<<SimTk->at(trkmu.at(i)-1).trackId();
	std::cout<<" | vertexId = "<<SimTk->at(trkmu.at(i)-1).vertIndex()<<std::endl;
      }
      if(vtxmu[i] != -1) {
	std::cout<<"=== SIM Vtx :: vtx = "<<std::setw(2)<<theSimVertices.at(vtxmu[i]).vertexId()<<" and position (in cm) : [x,y,z] = [";
	std::cout<<theSimVertices.at(vtxmu[i]).position().x()<<","<<theSimVertices.at(vtxmu[i]).position().y()<<","<<theSimVertices.at(vtxmu[i]).position().z()<<"] or [r,z] = [";
	std::cout<<sqrt(pow(theSimVertices.at(vtxmu[i]).position().x(),2)+pow(theSimVertices.at(vtxmu[i]).position().y(),2))<<","<<theSimVertices.at(vtxmu[i]).position().z()<<"] (units in cm)"<<std::endl;
      }
      // if(simhitme0mu.size()>i-1) {
	std::cout<<"=== SIM Hits in ME0 :: "<<std::setw(2)<<simhitme0mu[i].size()<<std::endl;
	std::cout<<"--------------------------"<<std::endl;
	for(unsigned int j=0; j<simhitme0mu[i].size(); ++j) {
	  ME0DetId me0id(simhitme0mu[i][j]->detUnitId());
	  std::cout<<"=== ME0 SimHit in "<<std::setw(12)<<simhitme0mu[i][j]->detUnitId()<<" = "<<me0id<<" from simtrack with trackId = ";
	  std::cout<<std::setw(9)<<simhitme0mu[i][j]->trackId()<<" | time t = "<<std::setw(12)<<simhitme0mu[i][j]->timeOfFlight();
	  std::cout<<" | phi = "<<std::setw(12)<<((me0Geom->etaPartition(me0id))->toGlobal(simhitme0mu[i][j]->localPosition())).phi();
	  std::cout<<" | eta = "<<std::setw(12)<<((me0Geom->etaPartition(me0id))->toGlobal(simhitme0mu[i][j]->localPosition())).eta()<<std::endl;
       //}
      }
      std::cout<<"--------------------------"<<std::endl;
      for(unsigned int j=0; j<me0mu[i].size(); ++j) {
	std::cout<<"=== ME0 Muon :: ch = "<<std::setw(2)<<theME0Muons.at(me0mu[i][j].first).charge()<<" | eta = "<<std::setw(9)<<theME0Muons.at(me0mu[i][j].first).eta(); 
	std::cout<<" | phi = "<<std::setw(9)<<theME0Muons.at(me0mu[i][j].first).phi()<<" | pt = "<<std::setw(9)<<theME0Muons.at(me0mu[i][j].first).pt();     
	std::cout<<" | ME0MuonId  = "<<std::setw(3)<<me0mu[i][j].first<<" | ME0SegRefId  = "<<std::setw(3)<<theME0Muons.at(me0mu[i][j].first).me0segid();
	std::cout<<" | time = "<<theME0Muons.at(me0mu[i][j].first).me0segment().time()<<" +/- "<<theME0Muons.at(me0mu[i][j].first).me0segment().timeErr();
	std::cout<<" | Nhits = "<<theME0Muons.at(me0mu[i][j].first).me0segment().nRecHits()<<" | matched = "<<me0mu[i][j].second /*<<std::endl*/ ;
	// std::cout<<" | Track Hits = "<<theME0Muons.at(me0mu[i][j].first).innerTrack().recHitsSize();
	std::cout<<" | Chi2/ndof = "<<theME0Muons.at(me0mu[i][j].first).innerTrack().get()->chi2()<<"/"<<theME0Muons.at(me0mu[i][j].first).innerTrack().get()->ndof();
	std::cout<<" | dxy = "<<theME0Muons.at(me0mu[i][j].first).innerTrack().get()->dxy()<<" [cm] | dz = "<<theME0Muons.at(me0mu[i][j].first).innerTrack().get()->dz()<<" [cm]"<<std::endl;  
      }
      std::cout<<"--------------------------"<<std::endl;
      for(unsigned int j=0; j<looseme0mu[i].size(); ++j) {
	std::cout<<"=== ME0 Loos :: ch = "<<std::setw(2)<<theME0Muons.at(looseme0mu[i][j].first).charge()<<" | eta = "<<std::setw(9)<<theME0Muons.at(looseme0mu[i][j].first).eta(); 
	std::cout<<" | phi = "<<std::setw(9)<<theME0Muons.at(looseme0mu[i][j].first).phi()<<" | pt = "<<std::setw(9)<<theME0Muons.at(looseme0mu[i][j].first).pt();     
	std::cout<<" | ME0MuonId  = "<<std::setw(3)<<looseme0mu[i][j].first<<" | ME0SegRefId  = "<<std::setw(3)<<theME0Muons.at(looseme0mu[i][j].first).me0segid();
	std::cout<<" | time = "<<theME0Muons.at(looseme0mu[i][j].first).me0segment().time()<<" +/- "<<theME0Muons.at(looseme0mu[i][j].first).me0segment().timeErr();
	std::cout<<" | Nhits = "<<theME0Muons.at(looseme0mu[i][j].first).me0segment().nRecHits()<<" | matched = "<<me0mu[i][j].second /*<<std::endl*/ ;
	// std::cout<<" | Track Hits = "<<theME0Muons.at(looseme0mu[i][j].first).innerTrack().recHitsSize();
	std::cout<<" | Chi2/ndof = "<<theME0Muons.at(looseme0mu[i][j].first).innerTrack().get()->chi2()<<"/"<<theME0Muons.at(looseme0mu[i][j].first).innerTrack().get()->ndof();
	std::cout<<" | dxy = "<<theME0Muons.at(looseme0mu[i][j].first).innerTrack().get()->dxy()<<" [cm] | dz = "<<theME0Muons.at(looseme0mu[i][j].first).innerTrack().get()->dz()<<" [cm]"<<std::endl;  
      }
      std::cout<<"--------------------------"<<std::endl;
      for(unsigned int j=0; j<tightme0mu[i].size(); ++j) {
	std::cout<<"=== ME0 Tigh :: ch = "<<std::setw(2)<<theME0Muons.at(tightme0mu[i][j].first).charge()<<" | eta = "<<std::setw(9)<<theME0Muons.at(tightme0mu[i][j].first).eta(); 
	std::cout<<" | phi = "<<std::setw(9)<<theME0Muons.at(tightme0mu[i][j].first).phi()<<" | pt = "<<std::setw(9)<<theME0Muons.at(tightme0mu[i][j].first).pt();     
	std::cout<<" | ME0MuonId  = "<<std::setw(3)<<tightme0mu[i][j].first<<" | ME0SegRefId  = "<<std::setw(3)<<theME0Muons.at(tightme0mu[i][j].first).me0segid();
	std::cout<<" | time = "<<theME0Muons.at(tightme0mu[i][j].first).me0segment().time()<<" +/- "<<theME0Muons.at(tightme0mu[i][j].first).me0segment().timeErr();
	std::cout<<" | Nhits = "<<theME0Muons.at(tightme0mu[i][j].first).me0segment().nRecHits()<<" | matched = "<<me0mu[i][j].second /*<<std::endl*/ ;
	// std::cout<<" | Track Hits = "<<theME0Muons.at(tightme0mu[i][j].first).innerTrack().recHitsSize();
	std::cout<<" | Chi2/ndof = "<<theME0Muons.at(tightme0mu[i][j].first).innerTrack().get()->chi2()<<"/"<<theME0Muons.at(tightme0mu[i][j].first).innerTrack().get()->ndof();
	std::cout<<" | dxy = "<<theME0Muons.at(tightme0mu[i][j].first).innerTrack().get()->dxy()<<" [cm] | dz = "<<theME0Muons.at(tightme0mu[i][j].first).innerTrack().get()->dz()<<" [cm]"<<std::endl;  
      }
      std::cout<<"--------------------------"<<std::endl;
      for(unsigned int j=0; j<recomu[i].size(); ++j) {
	std::cout<<"=== RECOMuon :: ch = "<<std::setw(2)<<theMuons.at(recomu[i][j].first).charge()<<" | eta = "<<std::setw(9)<<theMuons.at(recomu[i][j].first).eta(); 
	std::cout<<" | phi = "<<std::setw(9)<<theMuons.at(recomu[i][j].first).phi()<<" | pt = "<<std::setw(9)<<theMuons.at(recomu[i][j].first).pt();     
	std::cout<<" | RECOMuonId  = "<<std::setw(3)<<recomu[i][j].first;
	std::cout<<" | Nhits = "<<theMuons.at(recomu[i][j].first).outerTrack().get()->recHitsSize()<<" | matched = "<<recomu[i][j].second /*<<std::endl*/ ;
	std::cout<<" | Chi2/ndof = "<<theMuons.at(recomu[i][j].first).outerTrack().get()->chi2()<<"/"<<theMuons.at(recomu[i][j].first).outerTrack().get()->ndof();
	std::cout<<" | dxy = "<<theMuons.at(recomu[i][j].first).innerTrack().get()->dxy()<<" [cm] | dz = "<<theMuons.at(recomu[i][j].first).innerTrack().get()->dz()<<" [cm]"<<std::endl;  
      }
      std::cout<<"==========================\n"<<std::endl;
    }
  } // end printInfoSignal
  // =================================




  // =================================
  // Histograms for Out-Of-Time-PU Analysis
  // =================================
  // GEN-level plots
  // --------------------------------------------------------------------
  for(unsigned int i=0; i<indmu.size(); ++i) {
    if(indmu[i] != -1) {
      Signal_GenPart_EtaDistr->Fill(myGenEvent->barcode_to_particle(indmu.at(i))->momentum().eta());
      Signal_GenPart_PhiDistr->Fill(myGenEvent->barcode_to_particle(indmu.at(i))->momentum().phi());
      Signal_GenPart_PTDistr->Fill(myGenEvent->barcode_to_particle(indmu.at(i))->momentum().perp());
    }
    if(trkmu[i] != -1) {
      Signal_SimTrack_EtaDistr->Fill(SimTk->at(trkmu.at(i)-1).momentum().eta());
      Signal_SimTrack_PhiDistr->Fill(SimTk->at(trkmu.at(i)-1).momentum().phi());
      Signal_SimTrack_PTDistr->Fill(SimTk->at(trkmu.at(i)-1).momentum().pt());
    }
  }
  if(indmu.size()>1) { // enforce that 2 GENMuons are found
    TLorentzVector muon1,muon2,muon3,muon4;
    double mu1pt = myGenEvent->barcode_to_particle(indmu.at(0))->momentum().perp();
    double mu2pt = myGenEvent->barcode_to_particle(indmu.at(1))->momentum().perp();
    double mu1eta = myGenEvent->barcode_to_particle(indmu.at(0))->momentum().eta();
    double mu2eta = myGenEvent->barcode_to_particle(indmu.at(1))->momentum().eta();
    double mu1phi = myGenEvent->barcode_to_particle(indmu.at(0))->momentum().phi();
    double mu2phi = myGenEvent->barcode_to_particle(indmu.at(1))->momentum().phi();
    muon1.SetPtEtaPhiM(mu1pt,mu1eta,mu1phi,0);
    muon2.SetPtEtaPhiM(mu2pt,mu2eta,mu2phi,0);
    Signal_GenPart_InvariantMass->Fill((muon1+muon2).M());
    if(trkmu[0] != -1 && trkmu[1] != -1) {
      muon3.SetPtEtaPhiM(SimTk->at(trkmu.at(0)-1).momentum().pt(),SimTk->at(trkmu.at(0)-1).momentum().eta(),SimTk->at(trkmu.at(0)-1).momentum().phi(),0);
      muon4.SetPtEtaPhiM(SimTk->at(trkmu.at(1)-1).momentum().pt(),SimTk->at(trkmu.at(1)-1).momentum().eta(),SimTk->at(trkmu.at(1)-1).momentum().phi(),0);
      Signal_SimTrack_InvariantMass->Fill((muon3+muon4).M());
    }
  }


  // --------------------------------------------------------------------


  // SimTrack & SimHit plots
  // --------------------------------------------------------------------
  // SimTrack_All_Eta :: All SimTracks in ME0 EtaRange
  int countSimTracks = 0;
  for (edm::SimTrackContainer::const_iterator it = SimTk->begin(); it != SimTk->end(); ++it) {
    std::unique_ptr<SimTrack> simtrack = std::unique_ptr<SimTrack>(new SimTrack(*it));
    if(fabs(simtrack->momentum().eta()) > me0mineta && fabs(simtrack->momentum().eta()) < me0maxeta) { SimTrack_All_Eta->Fill(fabs(simtrack->momentum().eta())); ++countSimTracks; }
  }
  SimTrack_All_NumbTracks->Fill(countSimTracks);
  // SimTrack_ME0Hits_Eta & SimTrack_ME0Hits_NumbHits :: All SimTracks that leave SimHits in ME0 Sensitive Volume 
  // SimHits_ME0Hits_Eta  & SimHits_ME0Hits_Phi       :: SimHit Plots
  std::map<int,int> TrackIdME0HitMap; // map that stores the ME0SimHit multiplicity for each trackId
  std::map<int,int>::iterator it;     // iterator for this map
  for (std::vector<PSimHit>::const_iterator iHit = theSimHits.begin(); iHit != theSimHits.end(); ++iHit) {
    DetId theDetUnitId((*iHit).detUnitId());
    DetId simdetid= DetId((*iHit).detUnitId());
    int simhit_trackId = (*iHit).trackId();
    if(simdetid.det()==DetId::Muon &&  simdetid.subdetId()== MuonSubdetId::ME0){ // Only ME0                                                                                                                
      ME0DetId me0id(theDetUnitId);
      GlobalPoint ME0GlobalPoint = me0Geom->etaPartition(me0id)->toGlobal((*iHit).localPosition());
      // std::cout<<"ME0 SimHit in "<<me0id<<" from a SimTrack with trackId = "<<simhit_trackId<<" and with time"<<(*iHit).timeOfFlight()<<" [SimTrack size :: "<<SimTk->size()<<"]"<<std::endl;
      SimHits_ME0Hits_Eta->Fill(ME0GlobalPoint.eta());
      SimHits_ME0Hits_Phi->Fill(ME0GlobalPoint.phi());
      SimHits_ME0Hits_TOF->Fill((*iHit).tof());
      SimHits_ME0Hits_TOFvsR->Fill(ME0GlobalPoint.mag(),(*iHit).tof());
      // search for SimTrackId in the map
      it = TrackIdME0HitMap.find(simhit_trackId);
      if (it != TrackIdME0HitMap.end()) { // trackId found in the map
	int nhits = TrackIdME0HitMap[simhit_trackId];
	++nhits;
	TrackIdME0HitMap[simhit_trackId] = nhits;
      }
      else{ // trackId not found in the map: create entry                                                                                                                                                  
	TrackIdME0HitMap[simhit_trackId] = 1;
      }
    }
  }
  for(it=TrackIdME0HitMap.begin(); it!=TrackIdME0HitMap.end(); ++it) {
    SimTrack_ME0Hits_NumbHits->Fill(it->second);
    // std::cout<<"inside TrackIdME0HitMap map :: <"<<it->first<<","<<it->second<<">"<<std::endl;
    if(int(it->first - 1) < int(SimTk->size())) { SimTrack_ME0Hits_Eta->Fill(SimTk->at((it->first)-1).momentum().eta()); }
  }
  SimTrack_ME0Hits_NumbTracks->Fill(TrackIdME0HitMap.size());
  // Additionally for the summary :: SimTrack_Summary
  ME0RecHits_NumbHits->Fill(me0rechits->size());
  ME0Segments_NumbSeg->Fill(me0segments->size());
  ME0Muons_NumbMuons->Fill(me0muons->size());
  // --------------------------------------------------------------------


  // No Matching plots :: ME0Segment
  // --------------------------------------------------------------------
  for(ME0SegmentCollection::const_iterator it=me0segments->begin(); it!=me0segments->end(); ++it) {
    double creationtime = maxTimeSpan(it->specificRecHits());
    NoMatch_AllME0Seg_SegTimeCreation->Fill(creationtime);
    NoMatch_AllME0Seg_SegTimeCreaZoom->Fill(creationtime);
  }
  // --------------------------------------------------------------------


  // No Matching plots :: ME0Muon
  // --------------------------------------------------------------------
  // Categories_NumberOfME0Muons->Fill()  should be filled in destructor, should give the average amount of ME0Muons / Event
  int NoMatch_TightME0Muons = 0;
  for(std::vector<reco::ME0Muon>::const_iterator it=me0muons->begin(); it!=me0muons->end(); ++it) {
    // these plots make sense also if no signal muon is in the ME0 coverage
    NoMatch_AllME0Mu_SegTimeValue->Fill(it->me0segment().time());
    NoMatch_AllME0Mu_SegTimeValZoom->Fill(it->me0segment().time());
    NoMatch_AllME0Mu_SegTimeUncrt->Fill(it->me0segment().timeErr()); 
    NoMatch_AllME0Mu_TrackETADistr->Fill(it->eta());      if(it->pt() > 5) {NoMatch_AllME0Mu_TrackETADistr_5GeV->Fill(it->eta());}
    NoMatch_AllME0Mu_TrackPHIDistr->Fill(it->phi());
    NoMatch_AllME0Mu_TrackPTDistr->Fill(it->pt());
    NoMatch_AllME0Mu_SegNumberOfHits->Fill(it->me0segment().nRecHits());
    if(it->innerTrack().isNonnull()) { if(it->innerTrack().get()->ndof() > 0) NoMatch_AllME0Mu_SegChi2NDof->Fill(it->innerTrack().get()->chi2()/it->innerTrack().get()->ndof()); }
    ME0DetId segME0id = it->me0segment().me0DetId();
    GlobalPoint  segPos = (me0Geom->etaPartition(segME0id))->toGlobal(it->me0segment().localPosition());
    GlobalVector segDir = (me0Geom->etaPartition(segME0id))->toGlobal(it->me0segment().localDirection());
    NoMatch_AllME0Mu_SegETAPos->Fill(segPos.eta());
    NoMatch_AllME0Mu_SegETADir->Fill(segDir.eta());
    NoMatch_AllME0Mu_SegPHIPos->Fill(segPos.phi().value());
    NoMatch_AllME0Mu_SegPHIDir->Fill(segDir.phi().value());
    double creationtime = maxTimeSpan(it->me0segment().specificRecHits());
    NoMatch_AllME0Mu_SegTimeCreation->Fill(creationtime);
    NoMatch_AllME0Mu_SegTimeCreaZoom->Fill(creationtime);
    // double etares = SimTk->at(trkmu.at(i)-1).momentum().eta() - it->eta();
    double etares = segPos.eta() - it->eta();
    NoMatch_AllME0Mu_ETAResolution->Fill(etares);         if(it->pt() > 5) {NoMatch_AllME0Mu_ETAResolution_5GeV->Fill(etares);}
    NoMatch_AllME0Mu_ETAvsETARes->Fill(it->eta(),etares); if(it->pt() > 5) {NoMatch_AllME0Mu_ETAvsETARes_5GeV->Fill(it->eta(),etares);}
    NoMatch_AllME0Mu_PTvsETARes->Fill(it->pt(),etares);

    // these plots only make sense if there is signal muon in the ME0 coverage
    // only fill them for OldMatch
    // ME0DetId segME0id   = it->me0segment().me0DetId();
    // GlobalVector segDir = (me0Geom->etaPartition(segME0id))->toGlobal(it->me0segment()->localDirection());
    // OldMatch_TightME0Mu_SegPHIvsSimPT->Fill(SimTk->at(trkmu.at(i)-1).momentum().pt(),segDir.phi().value());
    // double qSimPTSim    = SimTk->at(trkmu.at(i)-1).charge()/SimTk->at(trkmu.at(i)-1).momentum().pt();
    // double qRecoPTReco  = it->charge()/it->pt();
    // double qOverPT      = (qSimPTSim-qRecoPTReco)/qSimPTSim;
    // OldMatch_TightME0Mu_PTResolution->Fill(qOverPT);
    // std::cout<<"Up to here :: ok :: 2"<<std::endl;

    // Loose ME0Muon
    // Tight ME0Muon
    if (muon::isGoodMuon(me0Geom, *it, muon::Tight)) {
      ++NoMatch_TightME0Muons;
      NoMatch_TightME0Mu_SegTimeValue->Fill(it->me0segment().time()); 
      NoMatch_TightME0Mu_SegTimeValZoom->Fill(it->me0segment().time()); 
      NoMatch_TightME0Mu_SegTimeUncrt->Fill(it->me0segment().timeErr()); 
      NoMatch_TightME0Mu_TrackETADistr->Fill(it->eta());     if(it->pt() > 5) {NoMatch_TightME0Mu_TrackETADistr_5GeV->Fill(it->eta());}
      NoMatch_TightME0Mu_TrackPHIDistr->Fill(it->phi());
      NoMatch_TightME0Mu_TrackPTDistr->Fill(it->pt());
      NoMatch_TightME0Mu_SegNumberOfHits->Fill(it->me0segment().nRecHits()); 
      if(it->innerTrack().isNonnull()) { if(it->innerTrack().get()->ndof() > 0) NoMatch_TightME0Mu_SegChi2NDof->Fill(it->innerTrack().get()->chi2()/it->innerTrack().get()->ndof()); }
      NoMatch_TightME0Mu_SegETAPos->Fill(segPos.eta());
      NoMatch_TightME0Mu_SegETADir->Fill(segDir.eta());
      NoMatch_TightME0Mu_SegPHIPos->Fill(segPos.phi().value());
      NoMatch_TightME0Mu_SegPHIDir->Fill(segDir.phi().value());
      NoMatch_TightME0Mu_SegTimeCreation->Fill(creationtime);
      NoMatch_TightME0Mu_SegTimeCreaZoom->Fill(creationtime);
      NoMatch_TightME0Mu_ETAResolution->Fill(etares);         if(it->pt() > 5) {NoMatch_TightME0Mu_ETAResolution_5GeV->Fill(etares);}
      NoMatch_TightME0Mu_ETAvsETARes->Fill(it->eta(),etares); if(it->pt() > 5) {NoMatch_TightME0Mu_ETAvsETARes_5GeV->Fill(it->eta(),etares);}
      NoMatch_TightME0Mu_PTvsETARes->Fill(it->pt(),etares);
    }
  }
  NoMatch_AllME0Mu_NumbME0Muons->Fill(me0muons->size());
  NoMatch_AllME0Mu_NumbME0Segments->Fill(me0segments->size());   // ill defined, to have only segments used by ME0Muons, one has to count precisely, for now just save size of entire collection
  NoMatch_TightME0Mu_NumbME0Muons->Fill(NoMatch_TightME0Muons);
  NoMatch_TightME0Mu_NumbME0Segments->Fill(me0segments->size()); // ill defined, to have only segments used by Tight ME0Muons, one has to count precisely, for now just save size of entire collection

  // std::cout<<"Up to here :: ok :: 3"<<std::endl;

  // int NoMatch_NumbME0Segments = 0;
  // Loop here over all ME0Segments
  // --------------------------------------------------------------------

  // My Matching plots
  // --------------------------------------------------------------------
  // All ME0Muon
  // --------------------------------------------------------------------
  for(unsigned int i=0; i<indmu.size(); ++i) {
    for(unsigned int j=0; j<me0mu[i].size(); ++j) {
      NewMatch_AllME0Mu_SegTimeValue->Fill(theME0Muons.at(me0mu[i][j].first).me0segment().time()); 
      NewMatch_AllME0Mu_SegTimeValZoom->Fill(theME0Muons.at(me0mu[i][j].first).me0segment().time()); 
      NewMatch_AllME0Mu_SegTimeUncrt->Fill(theME0Muons.at(me0mu[i][j].first).me0segment().timeErr()); 
      NewMatch_AllME0Mu_TrackETADistr->Fill(theME0Muons.at(me0mu[i][j].first).eta());             
      NewMatch_AllME0Mu_TrackPHIDistr->Fill(theME0Muons.at(me0mu[i][j].first).phi());
      NewMatch_AllME0Mu_TrackPTDistr->Fill(theME0Muons.at(me0mu[i][j].first).pt());
      NewMatch_AllME0Mu_SegNumberOfHits->Fill(theME0Muons.at(me0mu[i][j].first).me0segment().nRecHits()); 
      if(theME0Muons.at(me0mu[i][j].first).innerTrack().get()->ndof()>0) {
	NewMatch_AllME0Mu_SegChi2NDof->Fill(theME0Muons.at(me0mu[i][j].first).innerTrack().get()->chi2()/theME0Muons.at(me0mu[i][j].first).innerTrack().get()->ndof()); 
      }
      ME0DetId segME0id = theME0Muons.at(me0mu[i][j].first).me0segment().me0DetId();
      GlobalPoint  segPos = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(me0mu[i][j].first).me0segment().localPosition());
      GlobalVector segDir = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(me0mu[i][j].first).me0segment().localDirection());
      NewMatch_AllME0Mu_SegETAPos->Fill(segPos.eta());
      NewMatch_AllME0Mu_SegETADir->Fill(segDir.eta());
      NewMatch_AllME0Mu_SegPHIPos->Fill(segPos.phi().value());
      NewMatch_AllME0Mu_SegPHIDir->Fill(segDir.phi().value());
      NewMatch_AllME0Mu_SegPHIvsSimPT->Fill(SimTk->at(trkmu.at(i)-1).momentum().pt(),segDir.phi().value());
      // double etares = SimTk->at(trkmu.at(i)-1).momentum().eta() - theME0Muons.at(me0mu[i][j].first).eta();
      double etares = segPos.eta() - theME0Muons.at(me0mu[i][j].first).eta();
      NewMatch_AllME0Mu_ETAResolution->Fill(etares);                                              
      NewMatch_AllME0Mu_ETAvsETARes->Fill(theME0Muons.at(me0mu[i][j].first).eta(),etares);        
      NewMatch_AllME0Mu_PTvsETARes->Fill(theME0Muons.at(me0mu[i][j].first).pt(),etares);
      double creationtime = maxTimeSpan(theME0Muons.at(me0mu[i][j].first).me0segment().specificRecHits());
      NewMatch_AllME0Mu_SegTimeCreation->Fill(creationtime);
      NewMatch_AllME0Mu_SegTimeCreaZoom->Fill(creationtime);
      double qSimPTSim   = SimTk->at(trkmu.at(i)-1).charge()/SimTk->at(trkmu.at(i)-1).momentum().pt();
      double qRecoPTReco = theME0Muons.at(me0mu[i][j].first).charge()/theME0Muons.at(me0mu[i][j].first).pt();
      double qOverPT = (qSimPTSim-qRecoPTReco)/qSimPTSim;
      NewMatch_AllME0Mu_PTResolution->Fill(qOverPT);
    }
  }
  // Outside of the Muon Loop, because it is a property of the Muon Pair
  if(indmu.size()>1) { // enforce that 2 GENMuons are found
    TLorentzVector muon1,muon2; 
    // Strategy:
    // ----------------------------------------------------------
    // Ask first whether the muons are reconstructed as ME0 Muons
    // if reconstructed as ME0Muon then they get the priority over normal Muons
    // (in 2.0 < | eta | < 2.4 the muon is often reconstructed both as reco::Muon and reco::ME0Muon)
    // therefore check first whether a muon is reconstructed as ME0 Muon and fill the histograms
    // only in case a muon is outside the ME0 range or is not reconstructed as ME0Muon, use reco::Muon.
    // Further on, use only the first muon matched, so element [0]. 
    // Will sort out later a way to find out in case more are matched. Probably for ME0 I could save more muons
    // and at a later stage ask whether they are Tight or Loose and fill the histograms here ...
    // ----------------------------------------------------------
    // Both muons in ME0
    // -----------------
    if(me0mu[0].size() > 0 && me0mu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theME0Muons.at(me0mu[0][0].first).pt(),theME0Muons.at(me0mu[0][0].first).eta(),theME0Muons.at(me0mu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theME0Muons.at(me0mu[1][0].first).pt(),theME0Muons.at(me0mu[1][0].first).eta(),theME0Muons.at(me0mu[1][0].first).phi(),0);
      NewMatch_AllME0Mu_InvariantMass_All->Fill((muon1+muon2).M()); 
      NewMatch_AllME0Mu_InvariantMass_2ME0Mu->Fill((muon1+muon2).M()); 
    }
    // One muon in ME0, the other outside :: case 1
    // ----------------------------------------------
    else if(me0mu[0].size() > 0 && recomu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theME0Muons.at(me0mu[0][0].first).pt(),theME0Muons.at(me0mu[0][0].first).eta(),theME0Muons.at(me0mu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theMuons.at(recomu[1][0].first).pt(),theMuons.at(recomu[1][0].first).eta(),theMuons.at(recomu[1][0].first).phi(),0);
      NewMatch_AllME0Mu_InvariantMass_All->Fill((muon1+muon2).M());
      NewMatch_AllME0Mu_InvariantMass_ME0MuRecoMu->Fill((muon1+muon2).M());

    }
    // One muon in ME0, the other outside :: case 2
    // -------------------------------------------- 
    else if(me0mu[1].size() > 0 && recomu[0].size() > 0) {
      muon1.SetPtEtaPhiM(theMuons.at(recomu[0][0].first).pt(),theMuons.at(recomu[0][0].first).eta(),theMuons.at(recomu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theME0Muons.at(me0mu[1][0].first).pt(),theME0Muons.at(me0mu[1][0].first).eta(),theME0Muons.at(me0mu[1][0].first).phi(),0);
      NewMatch_AllME0Mu_InvariantMass_All->Fill((muon1+muon2).M());
      NewMatch_AllME0Mu_InvariantMass_ME0MuRecoMu->Fill((muon1+muon2).M());
    }
    // Both muons outside ME0
    // ----------------------
    else if(recomu[0].size() > 0 && recomu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theMuons.at(recomu[0][0].first).pt(),theMuons.at(recomu[0][0].first).eta(),theMuons.at(recomu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theMuons.at(recomu[1][0].first).pt(),theMuons.at(recomu[1][0].first).eta(),theMuons.at(recomu[1][0].first).phi(),0);
      NewMatch_AllME0Mu_InvariantMass_All->Fill((muon1+muon2).M()); 
      NewMatch_AllME0Mu_InvariantMass_2RecoMu->Fill((muon1+muon2).M()); 
    }
    else {}
  }
  // --------------------------------------------------------------------

  // Loose ME0Muon ID
  // --------------------------------------------------------------------
  for(unsigned int i=0; i<indmu.size(); ++i) {
    for(unsigned int j=0; j<looseme0mu[i].size(); ++j) {
      NewMatch_LooseME0Mu_SegTimeValue->Fill(theME0Muons.at(looseme0mu[i][j].first).me0segment().time()); 
      NewMatch_LooseME0Mu_SegTimeValZoom->Fill(theME0Muons.at(looseme0mu[i][j].first).me0segment().time()); 
      NewMatch_LooseME0Mu_SegTimeUncrt->Fill(theME0Muons.at(looseme0mu[i][j].first).me0segment().timeErr()); 
      NewMatch_LooseME0Mu_TrackETADistr->Fill(theME0Muons.at(looseme0mu[i][j].first).eta());      
      NewMatch_LooseME0Mu_TrackPHIDistr->Fill(theME0Muons.at(looseme0mu[i][j].first).phi());
      NewMatch_LooseME0Mu_TrackPTDistr->Fill(theME0Muons.at(looseme0mu[i][j].first).pt());
      NewMatch_LooseME0Mu_SegNumberOfHits->Fill(theME0Muons.at(looseme0mu[i][j].first).me0segment().nRecHits()); 
      if(theME0Muons.at(looseme0mu[i][j].first).innerTrack().get()->ndof()>0) {
	NewMatch_LooseME0Mu_SegChi2NDof->Fill(theME0Muons.at(looseme0mu[i][j].first).innerTrack().get()->chi2()/theME0Muons.at(looseme0mu[i][j].first).innerTrack().get()->ndof()); 
      }
      ME0DetId segME0id = theME0Muons.at(looseme0mu[i][j].first).me0segment().me0DetId();
      GlobalPoint  segPos = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(looseme0mu[i][j].first).me0segment().localPosition());
      GlobalVector segDir = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(looseme0mu[i][j].first).me0segment().localDirection());
      NewMatch_LooseME0Mu_SegETAPos->Fill(segPos.eta());
      NewMatch_LooseME0Mu_SegETADir->Fill(segDir.eta());
      NewMatch_LooseME0Mu_SegPHIPos->Fill(segPos.phi().value());
      NewMatch_LooseME0Mu_SegPHIDir->Fill(segDir.phi().value());
      NewMatch_LooseME0Mu_SegPHIvsSimPT->Fill(SimTk->at(trkmu.at(i)-1).momentum().pt(),segDir.phi().value());
      // double etares = SimTk->at(trkmu.at(i)-1).momentum().eta() - theME0Muons.at(looseme0mu[i][j].first).eta();
      double etares = segPos.eta() - theME0Muons.at(looseme0mu[i][j].first).eta();
      NewMatch_LooseME0Mu_ETAResolution->Fill(etares);                                            
      NewMatch_LooseME0Mu_ETAvsETARes->Fill(theME0Muons.at(looseme0mu[i][j].first).eta(),etares); 
      NewMatch_LooseME0Mu_PTvsETARes->Fill(theME0Muons.at(looseme0mu[i][j].first).pt(),etares);
      double creationtime = maxTimeSpan(theME0Muons.at(looseme0mu[i][j].first).me0segment().specificRecHits());
      NewMatch_LooseME0Mu_SegTimeCreation->Fill(creationtime);
      NewMatch_LooseME0Mu_SegTimeCreaZoom->Fill(creationtime);
      double qSimPTSim   = SimTk->at(trkmu.at(i)-1).charge()/SimTk->at(trkmu.at(i)-1).momentum().pt();
      double qRecoPTReco = theME0Muons.at(looseme0mu[i][j].first).charge()/theME0Muons.at(looseme0mu[i][j].first).pt();
      double qOverPT = (qSimPTSim-qRecoPTReco)/qSimPTSim;
      NewMatch_LooseME0Mu_PTResolution->Fill(qOverPT);
    }
  }
  // Outside of the Muon Loop, because it is a property of the Muon Pair
  if(indmu.size()>1) { // enforce that 2 GENMuons are found
    TLorentzVector muon1,muon2; 
    // Strategy:
    // ----------------------------------------------------------
    // Ask first whether the muons are reconstructed as ME0 Muons
    // if reconstructed as ME0Muon then they get the priority over normal Muons
    // (in 2.0 < | eta | < 2.4 the muon is often reconstructed both as reco::Muon and reco::ME0Muon)
    // therefore check first whether a muon is reconstructed as ME0 Muon and fill the histograms
    // only in case a muon is outside the ME0 range or is not reconstructed as ME0Muon, use reco::Muon.
    // Further on, use only the first muon matched, so element [0]. 
    // Will sort out later a way to find out in case more are matched. Probably for ME0 I could save more muons
    // and at a later stage ask whether they are Tight or Loose and fill the histograms here ...
    // ----------------------------------------------------------
    // Both muons in ME0
    // -----------------
    if(looseme0mu[0].size() > 0 && looseme0mu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theME0Muons.at(looseme0mu[0][0].first).pt(),theME0Muons.at(looseme0mu[0][0].first).eta(),theME0Muons.at(looseme0mu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theME0Muons.at(looseme0mu[1][0].first).pt(),theME0Muons.at(looseme0mu[1][0].first).eta(),theME0Muons.at(looseme0mu[1][0].first).phi(),0);
      NewMatch_LooseME0Mu_InvariantMass_All->Fill((muon1+muon2).M()); 
      NewMatch_LooseME0Mu_InvariantMass_2ME0Mu->Fill((muon1+muon2).M()); 
    }
    // One muon in ME0, the other outside :: case 1
    // ----------------------------------------------
    else if(looseme0mu[0].size() > 0 && recomu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theME0Muons.at(looseme0mu[0][0].first).pt(),theME0Muons.at(looseme0mu[0][0].first).eta(),theME0Muons.at(looseme0mu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theMuons.at(recomu[1][0].first).pt(),theMuons.at(recomu[1][0].first).eta(),theMuons.at(recomu[1][0].first).phi(),0);
      NewMatch_LooseME0Mu_InvariantMass_All->Fill((muon1+muon2).M());
      NewMatch_LooseME0Mu_InvariantMass_ME0MuRecoMu->Fill((muon1+muon2).M());

    }
    // One muon in ME0, the other outside :: case 2
    // -------------------------------------------- 
    else if(looseme0mu[1].size() > 0 && recomu[0].size() > 0) {
      muon1.SetPtEtaPhiM(theMuons.at(recomu[0][0].first).pt(),theMuons.at(recomu[0][0].first).eta(),theMuons.at(recomu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theME0Muons.at(looseme0mu[1][0].first).pt(),theME0Muons.at(looseme0mu[1][0].first).eta(),theME0Muons.at(looseme0mu[1][0].first).phi(),0);
      NewMatch_LooseME0Mu_InvariantMass_All->Fill((muon1+muon2).M());
      NewMatch_LooseME0Mu_InvariantMass_ME0MuRecoMu->Fill((muon1+muon2).M());
    }
    // Both muons outside ME0
    // ----------------------
    else if(recomu[0].size() > 0 && recomu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theMuons.at(recomu[0][0].first).pt(),theMuons.at(recomu[0][0].first).eta(),theMuons.at(recomu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theMuons.at(recomu[1][0].first).pt(),theMuons.at(recomu[1][0].first).eta(),theMuons.at(recomu[1][0].first).phi(),0);
      NewMatch_LooseME0Mu_InvariantMass_All->Fill((muon1+muon2).M()); 
      NewMatch_LooseME0Mu_InvariantMass_2RecoMu->Fill((muon1+muon2).M()); 
    }
    else {}
  }
  // --------------------------------------------------------------------

  // Tight ME0Muon ID
  // --------------------------------------------------------------------
  for(unsigned int i=0; i<indmu.size(); ++i) {
    for(unsigned int j=0; j<tightme0mu[i].size(); ++j) {
      NewMatch_TightME0Mu_SegTimeValue->Fill(theME0Muons.at(tightme0mu[i][j].first).me0segment().time()); 
      NewMatch_TightME0Mu_SegTimeValZoom->Fill(theME0Muons.at(tightme0mu[i][j].first).me0segment().time()); 
      NewMatch_TightME0Mu_SegTimeUncrt->Fill(theME0Muons.at(tightme0mu[i][j].first).me0segment().timeErr()); 
      NewMatch_TightME0Mu_TrackETADistr->Fill(theME0Muons.at(tightme0mu[i][j].first).eta());      if(theME0Muons.at(tightme0mu[i][j].first).pt() > 5) {NewMatch_TightME0Mu_TrackETADistr_5GeV->Fill(theME0Muons.at(tightme0mu[i][j].first).eta());}
      NewMatch_TightME0Mu_TrackPHIDistr->Fill(theME0Muons.at(tightme0mu[i][j].first).phi());
      NewMatch_TightME0Mu_TrackPTDistr->Fill(theME0Muons.at(tightme0mu[i][j].first).pt());
      NewMatch_TightME0Mu_SegNumberOfHits->Fill(theME0Muons.at(tightme0mu[i][j].first).me0segment().nRecHits()); 
      if(theME0Muons.at(tightme0mu[i][j].first).innerTrack().get()->ndof()>0) {
	NewMatch_TightME0Mu_SegChi2NDof->Fill(theME0Muons.at(tightme0mu[i][j].first).innerTrack().get()->chi2()/theME0Muons.at(tightme0mu[i][j].first).innerTrack().get()->ndof()); 
      }
      ME0DetId segME0id = theME0Muons.at(tightme0mu[i][j].first).me0segment().me0DetId();
      GlobalPoint  segPos = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(tightme0mu[i][j].first).me0segment().localPosition());
      GlobalVector segDir = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(tightme0mu[i][j].first).me0segment().localDirection());
      NewMatch_TightME0Mu_SegETAPos->Fill(segPos.eta());
      NewMatch_TightME0Mu_SegETADir->Fill(segDir.eta());
      NewMatch_TightME0Mu_SegPHIPos->Fill(segPos.phi().value());
      NewMatch_TightME0Mu_SegPHIDir->Fill(segDir.phi().value());
      NewMatch_TightME0Mu_SegPHIvsSimPT->Fill(SimTk->at(trkmu.at(i)-1).momentum().pt(),segDir.phi().value());
      // double etares = SimTk->at(trkmu.at(i)-1).momentum().eta() - theME0Muons.at(tightme0mu[i][j].first).eta();
      double etares = segPos.eta() - theME0Muons.at(tightme0mu[i][j].first).eta();
      NewMatch_TightME0Mu_ETAResolution->Fill(etares);                                            if(theME0Muons.at(tightme0mu[i][j].first).pt() > 5) {NewMatch_TightME0Mu_ETAResolution_5GeV->Fill(etares);}
      NewMatch_TightME0Mu_ETAvsETARes->Fill(theME0Muons.at(tightme0mu[i][j].first).eta(),etares); if(theME0Muons.at(tightme0mu[i][j].first).pt() > 5) {NewMatch_TightME0Mu_ETAvsETARes_5GeV->Fill(theME0Muons.at(tightme0mu[i][j].first).eta(),etares);}
      NewMatch_TightME0Mu_PTvsETARes->Fill(theME0Muons.at(tightme0mu[i][j].first).pt(),etares);
      double creationtime = maxTimeSpan(theME0Muons.at(tightme0mu[i][j].first).me0segment().specificRecHits());
      NewMatch_TightME0Mu_SegTimeCreation->Fill(creationtime);
      NewMatch_TightME0Mu_SegTimeCreaZoom->Fill(creationtime);
      double qSimPTSim   = SimTk->at(trkmu.at(i)-1).charge()/SimTk->at(trkmu.at(i)-1).momentum().pt();
      double qRecoPTReco = theME0Muons.at(tightme0mu[i][j].first).charge()/theME0Muons.at(tightme0mu[i][j].first).pt();
      double qOverPT = (qSimPTSim-qRecoPTReco)/qSimPTSim;
      NewMatch_TightME0Mu_PTResolution->Fill(qOverPT);
    }
  }
  // Outside of the Muon Loop, because it is a property of the Muon Pair
  if(indmu.size()>1) { // enforce that 2 GENMuons are found
    TLorentzVector muon1,muon2; 
    // Strategy:
    // ----------------------------------------------------------
    // Ask first whether the muons are reconstructed as ME0 Muons
    // if reconstructed as ME0Muon then they get the priority over normal Muons
    // (in 2.0 < | eta | < 2.4 the muon is often reconstructed both as reco::Muon and reco::ME0Muon)
    // therefore check first whether a muon is reconstructed as ME0 Muon and fill the histograms
    // only in case a muon is outside the ME0 range or is not reconstructed as ME0Muon, use reco::Muon.
    // Further on, use only the first muon matched, so element [0]. 
    // Will sort out later a way to find out in case more are matched. Probably for ME0 I could save more muons
    // and at a later stage ask whether they are Tight or Loose and fill the histograms here ...
    // ----------------------------------------------------------
    // Both muons in ME0
    // -----------------
    if(tightme0mu[0].size() > 0 && tightme0mu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theME0Muons.at(tightme0mu[0][0].first).pt(),theME0Muons.at(tightme0mu[0][0].first).eta(),theME0Muons.at(tightme0mu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theME0Muons.at(tightme0mu[1][0].first).pt(),theME0Muons.at(tightme0mu[1][0].first).eta(),theME0Muons.at(tightme0mu[1][0].first).phi(),0);
      NewMatch_TightME0Mu_InvariantMass_All->Fill((muon1+muon2).M()); 
      NewMatch_TightME0Mu_InvariantMass_2ME0Mu->Fill((muon1+muon2).M()); 
    }
    // One muon in ME0, the other outside :: case 1
    // ----------------------------------------------
    else if(tightme0mu[0].size() > 0 && recomu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theME0Muons.at(tightme0mu[0][0].first).pt(),theME0Muons.at(tightme0mu[0][0].first).eta(),theME0Muons.at(tightme0mu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theMuons.at(recomu[1][0].first).pt(),theMuons.at(recomu[1][0].first).eta(),theMuons.at(recomu[1][0].first).phi(),0);
      NewMatch_TightME0Mu_InvariantMass_All->Fill((muon1+muon2).M());
      NewMatch_TightME0Mu_InvariantMass_ME0MuRecoMu->Fill((muon1+muon2).M());

    }
    // One muon in ME0, the other outside :: case 2
    // -------------------------------------------- 
    else if(tightme0mu[1].size() > 0 && recomu[0].size() > 0) {
      muon1.SetPtEtaPhiM(theMuons.at(recomu[0][0].first).pt(),theMuons.at(recomu[0][0].first).eta(),theMuons.at(recomu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theME0Muons.at(tightme0mu[1][0].first).pt(),theME0Muons.at(tightme0mu[1][0].first).eta(),theME0Muons.at(tightme0mu[1][0].first).phi(),0);
      NewMatch_TightME0Mu_InvariantMass_All->Fill((muon1+muon2).M());
      NewMatch_TightME0Mu_InvariantMass_ME0MuRecoMu->Fill((muon1+muon2).M());
    }
    // Both muons outside ME0
    // ----------------------
    else if(recomu[0].size() > 0 && recomu[1].size() > 0) {
      muon1.SetPtEtaPhiM(theMuons.at(recomu[0][0].first).pt(),theMuons.at(recomu[0][0].first).eta(),theMuons.at(recomu[0][0].first).phi(),0);
      muon2.SetPtEtaPhiM(theMuons.at(recomu[1][0].first).pt(),theMuons.at(recomu[1][0].first).eta(),theMuons.at(recomu[1][0].first).phi(),0);
      NewMatch_TightME0Mu_InvariantMass_All->Fill((muon1+muon2).M()); 
      NewMatch_TightME0Mu_InvariantMass_2RecoMu->Fill((muon1+muon2).M()); 
    }
    else {}
  }
  // --------------------------------------------------------------------
  // --------------------------------------------------------------------

  // Old Matching plots
  // --------------------------------------------------------------------
  // --------------------------------------------------------------------

  // =================================




  // =================================
  // In-Time-PU Analysis
  // =================================
  for(unsigned int i=0; i<indmu.size(); ++i) {
    for(unsigned int j=0; j<tightme0mu[i].size(); ++j) {

      // ME0Muon :: time and pt
      double time = theME0Muons.at(tightme0mu[i][j].first).me0segment().time();

      if( time < 6.00 || time > 31.00) continue;

      double timeErr = theME0Muons.at(tightme0mu[i][j].first).me0segment().timeErr();
      double pt = theME0Muons.at(tightme0mu[i][j].first).pt();
      // ME0Muon :: Segment Position
      ME0DetId segME0id = theME0Muons.at(tightme0mu[i][j].first).me0segment().me0DetId();
      GlobalPoint  segPos = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(tightme0mu[i][j].first).me0segment().localPosition());
      // GlobalVector segDir = (me0Geom->etaPartition(segME0id))->toGlobal(theME0Muons.at(tightme0mu[i][j].first).me0segment().localDirection());
      // ME0Muon :: Average Hit Position
      GlobalPoint hitPos = AverageHitPos(theME0Muons.at(tightme0mu[i][j].first).me0segment().specificRecHits());      

      // extrapolated position along beamline:
      double c = 29.9792458; // [cm / ns]
      // Segment Position (x1,y1,z1), Position along Beamline = (0,0,z0)
      // later on we can specify the position along the Beamline as (x0, y0, z0), 
      // where x0 and y0 are (measured) beamspot positions, for now assume (0,0,z0)
      // c*t = sqrt( (x1-x0)^2 + (y1-y0)^2 + (z1-z0)^2 ) = sqrt(x1^2 + y1^2 + (z1-z0)^2 )
      // z0 = z1 - sqrt((ct)^2-x1^2-y1^2)
      double z0 = hitPos.z() - sqrt(pow(c*time,2)-pow(hitPos.x(),2)-pow(hitPos.y(),2));
      // uncertainty on z0

      // Print Info
      std::cout<<"========================================================"<<std::endl;
      std::cout<<"=== In-Time PU Analysis :: In-Time ME0Muon "<<i+1<<" ==="<<std::endl;
      std::cout<<"========================================================"<<std::endl;

      std::cout<<"=== ME0 Tigh :: ch = "<<std::setw(2)<<theME0Muons.at(tightme0mu[i][j].first).charge()<<" | eta = "<<std::setw(9)<<theME0Muons.at(tightme0mu[i][j].first).eta();
      std::cout<<" | phi = "<<std::setw(9)<<theME0Muons.at(tightme0mu[i][j].first).phi()<<" | pt = "<<std::setw(9)<<pt;
      std::cout<<" | time = "<<time<<" +/- "<<timeErr<<" | SegBased z pos = "<<segPos.z()<<" [cm] | HitBased z pos = "<<hitPos.z()<<" [cm]";
      std::cout<<" SimVertex Z Pos = "<<theSimVertices.at(vtxmu[i]).position().z()<<" RecoVertex Z Pos = [Not Yet Implemented]"<<" extrap Z Pos = "<<z0<<std::endl;

    }
  }
  // =================================


}

// Sorting with a function
// =======================
bool MyME0InTimePUAnalyzer::sortME0muons (const std::pair<int,int> &left, const std::pair<int,int> &right) {
  if(left.second == right.second) {return theME0Muons.at(left.first).pt() < theME0Muons.at(right.first).pt();}
  else {return left.second < right.second;}
}
/*
bool MyME0InTimePUAnalyzer::sortME0muons (const std::pair<int,int> left, const std::pair<int,int> right) {
  if(left.second == right.second) {return theME0Muons.at(left.first).pt() < theME0Muons.at(right.first).pt();}
  else {return left.second < right.second;}
}
*/
/*
bool MyME0InTimePUAnalyzer::sortME0muons (const std::pair<int,int>::iterator left, const std::pair<int,int>::iterator right) {
  if(left->second == right->second) {return theME0Muons.at(left->first).pt() < theME0Muons.at(right->first).pt();}
  else {return left->second < right->second;}
}
*/
// usage :: std::sort(vector.begin(), vector.end(), sort_pred());

// Sorting with a struct
// =====================
/*
struct sortME0muonsStruct {
  bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
    if(left.second == right.second) {return theME0Muons.at(left.first).pt() < theME0Muons.at(right.first).pt();}
    else {return left.second < right.second;}         
  }
};
*/




bool MyME0InTimePUAnalyzer::checkVector(std::vector<int>& myvec, int myint) {
  bool found = false;
  for(std::vector<int>::const_iterator it=myvec.begin(); it<myvec.end(); ++it){ 
    if((*it)==myint) found = true;
  }
  return found;
}

double MyME0InTimePUAnalyzer::maxTimeSpan(const std::vector<ME0RecHit> rechits) {
  double minvalue = +1000, maxvalue = -1000;
  for (auto rh=rechits.begin(); rh!=rechits.end(); ++rh){
    if((*rh).tof() < minvalue) minvalue = (*rh).tof();  
    if((*rh).tof() > maxvalue) maxvalue = (*rh).tof();                                          
  }
  return fabs(maxvalue-minvalue);
}

GlobalPoint MyME0InTimePUAnalyzer::AverageHitPos(const std::vector<ME0RecHit> rechits) {
  double ave_x = 0.0, ave_y = 0.0, ave_z = 0.0;
  for (auto rh=rechits.begin(); rh!=rechits.end(); ++rh){
    ME0DetId hitME0id  = (*rh).me0Id();
    GlobalPoint hitPos = (me0Geom->etaPartition(hitME0id))->toGlobal((*rh).localPosition());
    ave_x += hitPos.x(); ave_y +=hitPos.y(); ave_z += hitPos.z();
  }
  if(rechits.size() != 0) { ave_x = ave_x/rechits.size(); ave_y = ave_y/rechits.size(); ave_z = ave_z/rechits.size(); }
  GlobalPoint position(ave_x, ave_y, ave_z);
  return position;
}

// ------------ method called once each job just before starting event loop  ------------
/*
void 
MyME0InTimePUAnalyzer::beginJob()
{
}
*/

// ------------ method called once each job just after ending the event loop  ------------
/*
void 
MyME0InTimePUAnalyzer::endJob() 
{
}
*/

// ------------ method called when starting to processes a run  ------------
/*
void 
MyME0InTimePUAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MyME0InTimePUAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MyME0InTimePUAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MyME0InTimePUAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MyME0InTimePUAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyME0InTimePUAnalyzer);

//backup
  /*
  std::unique_ptr<const HepMC::GenEvent> myGenEvent = std::unique_ptr<const HepMC::GenEvent>(new HepMC::GenEvent(*(hepmcevent->GetEvent())));
  for(HepMC::GenEvent::particle_const_iterator it = myGenEvent->particles_begin(); it != myGenEvent->particles_end(); ++it) {
    std::unique_ptr<HepMC::GenParticle> genpart = std::unique_ptr<HepMC::GenParticle>(new HepMC::GenParticle(*(*it)));

    if(abs(genpart->pdg_id()) == 13 && genpart->isPromptFinalState()) {
      index_genpart_signal.push_back(genpart->barcode());
    }
    else if(abs(genpart->pdg_id()) == 13 && genpart->isPromptDecayed()) {
      index_genpart_background.push_back(genpart->barcode());
    }
    else if(abs(genpart->pdg_id()) == 13 && genpart->isDirectPromptTauDecayProductFinalState()) {
      index_genpart_signal.push_back(genpart->barcode());
    }
    else {}
  }
  */  
  /*
  for(unsigned int i=0; i<genParticles->size(); ++i) {
    // 1) consider only muons
    if(abs(genParticles->at(i).pdgId()) == 13 && genParticles->at(i).status() == 1 && genParticles->at(i).numberOfMothers() > 0) { 
      // 2) if mother of genparticle is a Z, save the genparticle index (i) and save mother index
      if(fabs(genParticles->at(i).mother()->pdgId()) == 23) { 
	index_genpart_signal.push_back(i); 
	index_genpart_signal_mother.push_back(genParticles->at(i).mother()->barcode());
	if(fabs(genParticles->at(i).eta())>me0mineta) me0genpartfound = true; 
      }
      // 3) if mother of particle is the same particle, then go one step deeper in hierarchy and repeat
      else if(abs(genParticles->at(i).pdgId()) == abs(genParticles->at(i).mother()->pdgId())) {
	if(genParticles->at(i).mother()->numberOfMothers() > 0) {
	  if(abs(genParticles->at(i).mother()->mother()->pdgId()) == 23) { 
	    index_genpart_signal.push_back(i);
	    index_genpart_signal_mother.push_back(genParticles->at(i).mother()->mother()->barcode()); 
	    if(fabs(genParticles->at(i).eta())>me0mineta) me0genpartfound = true;
	  }  
	  else{ index_genpart_background.push_back(i); }
	}
	else { index_genpart_background.push_back(i); }
      }
      else { index_genpart_background.push_back(i); }
    }
  }
  */
