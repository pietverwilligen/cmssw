/**  \class TevMuonProducer
 * 
 *   TeV muon reconstructor:
 *
 *
 *   $Date: 2013/01/06 19:17:48 $
 *   $Revision: 1.6 $
 *
 *   \author  Piotr Traczyk (SINS Warsaw)
 */

// Framework
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "RecoMuon/GlobalMuonProducer/src/TevMuonProducer.h"

// TrackFinder and specific GLB Trajectory Builder
#include "RecoMuon/GlobalTrackFinder/interface/GlobalMuonTrajectoryBuilder.h"
#include "RecoMuon/TrackingTools/interface/MuonTrackFinder.h"
#include "RecoMuon/TrackingTools/interface/MuonTrackLoader.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonRefitter.h"

// Input and output collection
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "DataFormats/TrackReco/interface/TrackToTrackMap.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

using namespace edm;
using namespace std;

//
// constructor with config
//
TevMuonProducer::TevMuonProducer(const ParameterSet& parameterSet) {

  LogDebug("Muon|RecoMuon|TevMuonProducer") << "constructor called" << endl;
  std::cout<<"Muon|RecoMuon|TevMuonProducer >> constructor called" << std::endl;

  // GLB Muon Collection Label
  theGLBCollectionLabel = parameterSet.getParameter<InputTag>("MuonCollectionLabel");

  // service parameters
  ParameterSet serviceParameters = parameterSet.getParameter<ParameterSet>("ServiceParameters");

  // the services
  theService = new MuonServiceProxy(serviceParameters);
  
  // TrackRefitter parameters
  ParameterSet refitterParameters = parameterSet.getParameter<ParameterSet>("RefitterParameters");
  theRefitter = new GlobalMuonRefitter(refitterParameters, theService);

  // TrackLoader parameters
  ParameterSet trackLoaderParameters = parameterSet.getParameter<ParameterSet>("TrackLoaderParameters");
  theTrackLoader = new MuonTrackLoader(trackLoaderParameters,theService);

  theRefits = parameterSet.getParameter< std::vector<std::string> >("Refits");
  theRefitIndex = parameterSet.getParameter< std::vector<int> >("RefitIndex");

  for(unsigned int ww=0;ww<theRefits.size();ww++){
    LogDebug("Muon|RecoMuon|TevMuonProducer") << "Refit " << theRefits[ww];
    produces<reco::TrackCollection>(theRefits[ww]);
    produces<TrackingRecHitCollection>(theRefits[ww]);
    produces<reco::TrackExtraCollection>(theRefits[ww]);
    produces<vector<Trajectory> >(theRefits[ww]) ;
    produces<TrajTrackAssociationCollection>(theRefits[ww]);
    produces<reco::TrackToTrackMap>(theRefits[ww]);
  }
}


//
// destructor
//
TevMuonProducer::~TevMuonProducer() {

  LogTrace("Muon|RecoMuon|TevMuonProducer") << "destructor called" << endl;
  std::cout<<"Muon|RecoMuon|TevMuonProducer >> destructor called"<<std::endl;
  if (theService) delete theService;
  if (theRefitter) delete theRefitter;
  if (theTrackLoader) delete theTrackLoader;
}


//
// reconstruct muons
//
void TevMuonProducer::produce(Event& event, const EventSetup& eventSetup) {

  const string metname = "Muon|RecoMuon|TevMuonProducer";  
  LogTrace(metname)<< endl << endl;
  LogTrace(metname)<< "TeV Muon Reconstruction started" << endl;  

  std::cout<<"Muon|RecoMuon|TevMuonProducer"<< std::endl << std::endl;
  std::cout<<"Muon|RecoMuon|TevMuonProducer >> TeV Muon Reconstruction started" << std::endl;  

  // Update the services
  theService->update(eventSetup);

  theRefitter->setEvent(event);

  theRefitter->setServices(theService->eventSetup());

  //Retrieve tracker topology from geometry
  edm::ESHandle<TrackerTopology> tTopoHand;
  eventSetup.get<IdealGeometryRecord>().get(tTopoHand);
  const TrackerTopology *tTopo=tTopoHand.product();


  // Take the GLB muon container(s)
  Handle<reco::TrackCollection> glbMuons;
  event.getByLabel(theGLBCollectionLabel,glbMuons);

  Handle<vector<Trajectory> > glbMuonsTraj;

  LogTrace(metname)<< "Taking " << glbMuons->size() << " Global Muons "<<theGLBCollectionLabel<<endl;
  std::cout<<"Muon|RecoMuon|TevMuonProducer >> Taking " << glbMuons->size() << " Global Muons "<<theGLBCollectionLabel<<std::endl;

  vector<MuonTrajectoryBuilder::TrackCand> glbTrackCands;

  event.getByLabel(theGLBCollectionLabel.label(), glbMuonsTraj);
    
  const reco::TrackCollection *glbTracks = glbMuons.product();
  
  std::cout<<"Muon|RecoMuon|TevMuonProducer >> starting refits :: size = "<<theRefits.size()<<std::endl;
  for(unsigned int ww=0;ww<theRefits.size();ww++) {
    LogDebug(metname)<<"TeVRefit for Refit: " <<theRefitIndex[ww];
    std::cout<<"Muon|RecoMuon|TevMuonProducer >> TeVRefit for Refit: " <<theRefitIndex[ww]<<std::endl;
    std::vector<std::pair<Trajectory*,reco::TrackRef> > miniMap;
    vector<Trajectory*> trajectories;
    reco::TrackRef::key_type trackIndex = 0;
    std::cout<<"\t Loop over glbTracks: "<<std::endl;
    for (reco::TrackCollection::const_iterator track = glbTracks->begin(); track!=glbTracks->end(); track++ , ++trackIndex) {
      reco::TrackRef glbRef(glbMuons,trackIndex);
      std::cout<<"\t\t made TrackRef glbRef now going to do refit: "<<std::endl;;
      vector<Trajectory> refitted=theRefitter->refit(*track,theRefitIndex[ww],tTopo);
      std::cout<<"\t\t refit done"<<std::endl; 
      if (refitted.size()>0) {
        Trajectory *refit = new Trajectory(refitted.front());
	LogDebug(metname)<<"TeVTrackLoader for Refit: " <<theRefits[ww];
	std::cout<<"\t\t TeVTrackLoader for Refit: " <<theRefits[ww]<<std::endl;
	trajectories.push_back(refit);
	std::cout<<"\t\t refit saved in vector trajectories "<<std::endl;;
	std::pair<Trajectory*,reco::TrackRef> thisPair(refit,glbRef);
	miniMap.push_back(thisPair);
	std::cout<<"\t\t pair <refit,glbRef> saved in miniMap "<<std::endl;;
      }
    }
    std::cout<<" before loadTracks ";
    theTrackLoader->loadTracks(trajectories,event,miniMap,theRefits[ww]);
    std::cout<<" after loadTracks ";
    std::cout<<""<<std::endl;
  }
    
  LogTrace(metname) << "Done." << endl;    
  std::cout<<" Done." << endl;    

}
