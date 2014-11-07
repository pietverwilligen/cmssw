#ifndef GEMRecHit_GEMCSCSegmentAlgorithm_h
#define GEMRecHit_GEMCSCSegmentAlgorithm_h

/** \class GEMCSCSegmentAlgo
 *
 * $Date:  $
 * $Revision: 1.7 $
 * \author Raffaella Radogna
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegment.h>
#include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <map>
#include <vector>

class GEMCSCSegmentAlgorithm {
public:
typedef std::pair<const CSCChamber*, std::map<uint32_t, const GEMEtaPartition*> >GEMCSCEnsamble; 

    /// Constructor
    explicit GEMCSCSegmentAlgorithm(const edm::ParameterSet&) {};
    /// Destructor
    virtual ~GEMCSCSegmentAlgorithm() {};

    virtual std::vector<GEMCSCSegment> run(GEMCSCEnsamble ensamble, const std::vector<const CSCSegment*>& cscsegments, const std::vector<const GEMRecHit*>& rechits) = 0;

    private:
};

#endif
