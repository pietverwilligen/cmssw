#ifndef Geometry_GEMGeometry_ME0Geometry_h
#define Geometry_GEMGeometry_ME0Geometry_h

/** \class ME0Geometry
 *
 *  The model of the geometry of ME0.
 *
 *  \author M. Maggi - INFN Bari
 */

#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "Geometry/GEMGeometry/interface/ME0Chamber.h"
#include <vector>
#include <map>

class GeomDetType;

class ME0Geometry : public TrackingGeometry {

 public:
  /// Default constructor
  ME0Geometry();

  /// Destructor
  virtual ~ME0Geometry();

  // Return a vector of all det types
  virtual const DetTypeContainer&  detTypes() const;

  // Return a vector of all GeomDetUnit
  virtual const DetUnitContainer& detUnits() const;

  // Return a vector of all GeomDet
  virtual const DetContainer& dets() const;
  
  // Return a vector of all GeomDetUnit DetIds
  virtual const DetIdContainer& detUnitIds() const;

  // Return a vector of all GeomDet DetIds
  virtual const DetIdContainer& detIds() const;

  // Return the pointer to the GeomDetUnit corresponding to a given DetId
  virtual const GeomDetUnit* idToDetUnit(DetId) const;

  // Return the pointer to the GeomDet corresponding to a given DetId
  virtual const GeomDet* idToDet(DetId) const;


  //---- Extension of the interface

  /// Return a vector of all ME0 eta partitions
  const std::vector<ME0EtaPartition const*>& etaPartitions() const;

  /// Return a vector of all ME0 layers
  const std::vector<ME0Layer const*>& layers() const;

  /// Return a vector of all ME0 chambers
  const std::vector<ME0Chamber const*>& chambers() const;

  /// Return a etaPartition given its id
  const ME0EtaPartition* etaPartition(ME0DetId id) const;

  /// Return a layer given its id
  const ME0Layer* layer(ME0DetId id) const;

  /// Return a chamber given its id
  const ME0Chamber* chamber(ME0DetId id) const;

  /// Add a ME0 etaPartition  to the Geometry
  void add(ME0EtaPartition* etaPartition);

  /// Add a ME0 layer  to the Geometry
  void add(ME0Layer* layer);

  /// Add a ME0 chamber  to the Geometry
  void add(ME0Chamber* chamber);

 private:
  DetUnitContainer theEtaPartitions;
  DetContainer theDets;
  DetTypeContainer theEtaPartitionTypes;
  DetIdContainer theEtaPartitionIds;
  DetIdContainer theDetIds;
  
  // Map for efficient lookup by DetId 
  mapIdToDet theMap;

  std::vector<ME0EtaPartition const*> allEtaPartitions; // Are not owned by this class; are owned by their layer.
  std::vector<ME0Layer const*> allLayers;               // Are not owned by this class; are owned by their chamber        
  std::vector<ME0Chamber const*> allChambers;           // I do not know what the statements above exactly mean        

};

#endif
