#ifndef GEMRecHit_GEMSegmentAlgorithm_h
#define GEMRecHit_GEMSegmentAlgorithm_h

/** \class GEMSegmentAlgo derived from CSC
 * An abstract base class for algorithmic classes used to
 * build segments in one ensemble of GEM detector 
 *
 * Implementation notes: <BR>
 * For example, GEMSegmAlgoPV inherits from this class,
 *
 * \author Piet Verwilligen
 *
 */

#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMSegment.h>
#include <Geometry/GEMGeometry/interface/GEMChamber.h>
// #include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <map>
#include <vector>

class GEMSegmentAlgorithm {
public:
  typedef std::pair<const GEMChamber*, std::map<uint32_t, const GEMEtaPartition*> >GEMEnsemble; 

    /// Constructor
    explicit GEMSegmentAlgorithm(const edm::ParameterSet&) {};
    /// Destructor
    virtual ~GEMSegmentAlgorithm() {};

    /** Run the algorithm = build the segments in this chamber
    */
    virtual std::vector<GEMSegment> run(const GEMEnsemble& ensemble, const std::vector<const GEMRecHit*>& rechits) = 0;  

    private:
};

#endif
