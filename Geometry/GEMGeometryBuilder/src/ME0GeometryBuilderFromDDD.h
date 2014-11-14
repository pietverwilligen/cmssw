#ifndef ME0Geometry_ME0GeometryBuilderFromDDD_H
#define ME0Geometry_ME0GeometryBuilderFromDDD_H

/** \class  ME0GeometryBuilderFromDDD
 *  Build the ME0Geometry ftom the DDD description
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
class ME0Geometry;
class ME0DetId;
class ME0EtaPartition;
class MuonDDDConstants;

class ME0GeometryBuilderFromDDD 
{ 
 public:

  ME0GeometryBuilderFromDDD();

  ~ME0GeometryBuilderFromDDD();

  boost::shared_ptr<ME0Geometry> build(const DDCompactView* cview, const MuonDDDConstants& muonConstants);

 private:
  boost::shared_ptr<ME0Geometry> buildGeometry(DDFilteredView& fview, const MuonDDDConstants& muonConstants);
  std::map<ME0DetId,std::vector<ME0DetId>> chids;
};

#endif
