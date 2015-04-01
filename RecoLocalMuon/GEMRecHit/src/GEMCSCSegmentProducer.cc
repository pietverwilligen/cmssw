/** \file GEMCSCSegmentProducer.cc
 *
 */

#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentProducer.h>
#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentBuilder.h>

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegmentCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegment.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>

GEMCSCSegmentProducer::GEMCSCSegmentProducer(const edm::ParameterSet& pas) : iev(0) {
	
    inputObjectsTagCSC = pas.getParameter<edm::InputTag>("inputObjectsCSC");
    inputObjectsTagGEM = pas.getParameter<edm::InputTag>("inputObjectsGEM");
    segmentBuilder_    = new GEMCSCSegmentBuilder(pas);

  	// register what this produces
    produces<GEMCSCSegmentCollection>();
}

GEMCSCSegmentProducer::~GEMCSCSegmentProducer() {

    LogDebug("GEMCSCSegment|GEMCSC") << "deleting GEMCSCSegmentBuilder after " << iev << " events w/gem and csc data.";
    delete segmentBuilder_;
}

void GEMCSCSegmentProducer::produce(edm::Event& ev, const edm::EventSetup& setup) {
    LogDebug("GEMCSCSegment|GEMCSC") << "start producing segments for " << ++iev << "th event with gem and csc data";
	
    // find the geometry (& conditions?) for this event & cache it in the builder
    edm::ESHandle<CSCGeometry> cscg;
    setup.get<MuonGeometryRecord>().get(cscg);
    const CSCGeometry* cgeom = &*cscg;
    
    edm::ESHandle<GEMGeometry> gemg;
    setup.get<MuonGeometryRecord>().get(gemg);
    const GEMGeometry* ggeom = &*gemg;
    

    segmentBuilder_->setGeometry(ggeom,cgeom);
    // fill the map with matches between GEM and CSC chambers
    segmentBuilder_->setSetup(ggeom,cgeom);

    // get the collection of CSCSegment and GEMRecHits
    edm::Handle<GEMRecHitCollection> gemRecHits;
    ev.getByLabel(inputObjectsTagGEM, gemRecHits);
    
    edm::Handle<CSCSegmentCollection> cscSegment;
    ev.getByLabel(inputObjectsTagCSC, cscSegment);

    // create empty collection of Segments
    std::auto_ptr<GEMCSCSegmentCollection> oc( new GEMCSCSegmentCollection );

    // fill the collection
    segmentBuilder_->build(gemRecHits.product(), cscSegment.product(), setup, *oc); //@@ FILL oc
    
    // put collection in event
    ev.put(oc);
}
