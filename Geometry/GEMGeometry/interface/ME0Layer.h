#ifndef ME0Geometry_ME0Layer_h
#define ME0Geometry_ME0Layer_h

/** \class ME0Chamber
 *
 *  Model of a ME0 Layer.
 *   
 *  A layer is a GeomDet.
 *  The layer is composed of 1 or more etaPartitions  (GeomDetUnit).
 *
 *  \author P. Verwilligen
 */

#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"

class ME0EtaPartition;

class ME0Layer : public GeomDet {
public:
  /// Constructor
  ME0Layer(ME0DetId id, const ReferenceCountingPointer<BoundPlane>& plane);

  /// Destructor
  virtual ~ME0Layer();

  /// Return the ME0DetId of this chamber
  ME0DetId id() const;

  // Which subdetector
  virtual SubDetector subDetector() const {return GeomDetEnumerators::ME0;}

  /// equal if the id is the same
  bool operator==(const ME0Layer& ch) const;

  /// Add EtaPartition to the layer which takes ownership
  void add(ME0EtaPartition* roll);

  /// Return the rolls in the layer
  virtual std::vector<const GeomDet*> components() const;

  /// Return the sub-component (roll) with a given id in this layer
  virtual const GeomDet* component(DetId id) const;

  /// Return the eta partition corresponding to the given id 
  const ME0EtaPartition* etaPartition(ME0DetId id) const;

  const ME0EtaPartition* etaPartition(int isl) const;
  
  /// Return the eta partitions
  const std::vector<const ME0EtaPartition*>& etaPartitions() const;

  /// Return number of eta partitions
  int nEtaPartitions() const;

private:

  ME0DetId detId_;

  // vector of eta partitions for a layer
  std::vector<const ME0EtaPartition*> etaPartitions_;

};
#endif
