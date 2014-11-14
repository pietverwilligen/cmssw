#ifndef GEMGeometry_GEMGeometryBuilderFromDDD_H
#define GEMGeometry_GEMGeometryBuilderFromDDD_H

/** \class  GEMGeometryBuilderFromDDD
 *  Build the GEMGeometry ftom the DDD description
 *
 *  \author M. Maggi - INFN Bari
 *
 */

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

class DDCompactView;
class DDFilteredView;
class GEMGeometry;
class GEMDetId;
class GEMEtaPartition;
class MuonDDDConstants;

class GEMGeometryBuilderFromDDD 
{ 
 public:

  GEMGeometryBuilderFromDDD();

  ~GEMGeometryBuilderFromDDD();

  boost::shared_ptr<GEMGeometry> build(const DDCompactView* cview, const MuonDDDConstants& muonConstants);

 private:
  boost::shared_ptr<GEMGeometry> buildGeometry(DDFilteredView& fview, const MuonDDDConstants& muonConstants);
  std::map<GEMDetId,std::vector<GEMDetId>> chids;
};

#endif
