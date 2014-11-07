#ifndef GEMCSCSegment_GEMCSCSegmentBuilder_h
#define GEMCSCSegment_GEMCSCSegmentBuilder_h

/** \class GEMCSCSegmentBuilder 
 * Algorithm to build GEMCSCSegment's from GEMRecHit and CSCSegment collections
 * by implementing a 'build' function required by GEMCSCSegmentProducer.
 *
 *
 * $Date:  $
 * $Revision: 1.3 $
 * \author Raffaella Radogna
 *
 *
 */
 #include "FWCore/Framework/interface/Event.h"
 
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>           
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>

#include <DataFormats/GEMRecHit/interface/GEMCSCSegmentCollection.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

class GEMCSCSegmentAlgorithm;

class GEMCSCSegmentBuilder {
public:
   
    explicit GEMCSCSegmentBuilder(const edm::ParameterSet&);
    /// Destructor
    ~GEMCSCSegmentBuilder();

    /** Find rechits in GEMChambers and Segment in CSCChamber, build GEMCSCSegment,
     *  and fill into output collection. 
     */
    void build(const GEMRecHitCollection* rechits,const CSCSegmentCollection* cscsegments, const edm::EventSetup& setup, GEMCSCSegmentCollection& oc); 

    /** Cache pointer to geometry _for current event_
     */
    void setGeometry(const GEMGeometry* gemgeom, const CSCGeometry* cscgeom); 
    //void setSetup(const edm::EventSetup&);
     private:
    GEMCSCSegmentAlgorithm* algo;
    const GEMGeometry* gemgeom_;
    const CSCGeometry* cscgeom_;

    //const edm::EventSetup& setup_;

};

#endif
