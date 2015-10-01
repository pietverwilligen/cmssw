/** Implementation of the ME0 Geometry Builder from DDD
 *
 *  \author M. Maggi - INFN Bari
 */

#include "Geometry/GEMGeometryBuilder/src/ME0GeometryBuilderFromDDD.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/GEMGeometry/interface/ME0Layer.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"

#include <DetectorDescription/Core/interface/DDFilter.h>
#include <DetectorDescription/Core/interface/DDFilteredView.h>
#include <DetectorDescription/Core/interface/DDSolid.h>

#include "Geometry/MuonNumbering/interface/MuonDDDNumbering.h"
#include "Geometry/MuonNumbering/interface/MuonBaseNumber.h"
#include "Geometry/MuonNumbering/interface/ME0NumberingScheme.h"

#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"

#include "DataFormats/GeometryVector/interface/Basic3DVector.h"

#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <algorithm>
#include <boost/lexical_cast.hpp>

ME0GeometryBuilderFromDDD::ME0GeometryBuilderFromDDD()
{ 
  edm::LogVerbatim("ME0GeometryBuilderfromDDD") <<"[ME0GeometryBuilderFromDDD::constructor]";
}

ME0GeometryBuilderFromDDD::~ME0GeometryBuilderFromDDD() 
{ }

ME0Geometry* ME0GeometryBuilderFromDDD::build(const DDCompactView* cview, const MuonDDDConstants& muonConstants)
{
  std::string attribute = "ReadOutName";
  std::string value     = "MuonME0Hits";
  DDValue val(attribute, value, 0.0);

  // Asking only for the MuonME0's
  DDSpecificsFilter filter;
  filter.setCriteria(val, // name & value of a variable 
		     DDCompOp::matches,
		     DDLogOp::AND, 
		     true, // compare strings otherwise doubles
		     true // use merged-specifics or simple-specifics
		     );
  DDFilteredView fview(*cview);
  fview.addFilter(filter);

  return this->buildGeometry(fview, muonConstants);
}

ME0Geometry* ME0GeometryBuilderFromDDD::buildGeometry(DDFilteredView& fview, const MuonDDDConstants& muonConstants)
{
  edm::LogVerbatim("ME0GeometryBuilderFromDDD") <<"Building the geometry service";
  ME0Geometry* geometry = new ME0Geometry();

  edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "About to run through the ME0 structure\n" 
					<<" First logical part "
					<<fview.logicalPart().name().name();


  bool doSubDets = fview.firstChild();
 
  edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "doSubDets = " << doSubDets;

  edm::LogVerbatim("ME0GeometryBuilderFromDDD") <<"start the loop"; 
  while (doSubDets)
  {
    // Get the Base Muon Number
    MuonDDDNumbering mdddnum(muonConstants);
    edm::LogVerbatim("ME0GeometryBuilderFromDDD") <<"Getting the Muon base Number";
    MuonBaseNumber mbn = mdddnum.geoHistoryToBaseNumber(fview.geoHistory());

    edm::LogVerbatim("ME0GeometryBuilderFromDDD") <<"Start the ME0 Numbering Schema";
    ME0NumberingScheme me0num(muonConstants);

    ME0DetId rollDetId(me0num.baseNumberToUnitNumber(mbn));
    edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "ME0 eta partition rawId: " << rollDetId.rawId() << ", detId: " << rollDetId;

    std::vector<double> dpar=fview.logicalPart().solid().parameters();
    std::string name = fview.logicalPart().name().name();
    DDTranslation tran = fview.translation();
    DDRotationMatrix rota = fview.rotation();
    Surface::PositionType pos(tran.x()/cm, tran.y()/cm, tran.z()/cm);
    // CLHEP way
    // Surface::RotationType rot(rota.xx(),rota.xy(),rota.xz(),
    //           	      rota.yx(),rota.yy(),rota.yz(),
    // 			      rota.zx(),rota.zy(),rota.zz());

    //ROOT::Math way
    DD3Vector x, y, z;
    rota.GetComponents(x,y,z);
    // doesn't this just re-inverse???
    Surface::RotationType rot(float(x.X()), float(x.Y()), float(x.Z()),
			      float(y.X()), float(y.Y()), float(y.Z()),
			      float(z.X()), float(z.Y()), float(z.Z())); 
    
    float be = dpar[4]/cm; // half bottom edge
    float te = dpar[8]/cm; // half top edge
    float ap = dpar[0]/cm; // half apothem
    float ti = 0.4/cm;     // half thickness

    //  TrapezoidalPlaneBounds* 
    Bounds* bounds = new TrapezoidalPlaneBounds(be, te, ap, ti);

    std::vector<float> pars;
    pars.push_back(be); 
    pars.push_back(te); 
    pars.push_back(ap); 
    //    pars.push_back(nStrips);
    // pars.push_back(nPads);

    edm::LogVerbatim("ME0GeometryBuilderFromDDD") 
      << "ME0 " << name << " par " << be << " " << te << " " << ap << " " << dpar[0];
    
    ME0EtaPartitionSpecs* e_p_specs = new ME0EtaPartitionSpecs(GeomDetEnumerators::ME0, name, pars);

      //Change of axes for the forward
    Basic3DVector<float> newX(1.,0.,0.);
    Basic3DVector<float> newY(0.,0.,1.);
    //      if (tran.z() > 0. )
    newY *= -1;
    Basic3DVector<float> newZ(0.,1.,0.);
    rot.rotateAxes (newX, newY, newZ);
    
    BoundPlane* bp = new BoundPlane(pos, rot, bounds);
    ReferenceCountingPointer<BoundPlane> surf(bp);
    ME0EtaPartition* mep = new ME0EtaPartition(rollDetId, surf, e_p_specs);

    // Add the eta partition to the geometry
    geometry->add(mep);
    // edm::LogVerbatim("ME0GeometryBuilderFromDDD") << " Eta Partition added, size of the etaPartitions vector = "<<(geometry->etaPartitions()).size();
    // go to next layer
    doSubDets = fview.nextSibling(); 
  }

  // build the layers and add them to the geometry
  // -------------------------------------------------------------------
  // note that this is done slightly differently 
  // w.r.t. GEMGeometryBuilder because the logic
  // fails on layers consisting of 1 etapartition
  // logic: 
  // in loop for element i, the eta partition (i-1) gets treated
  // and eta partition i gets pushed back for treatment in loop at i+1
  // caveat: if i is at ultimate element of loop, treat both (i-1) and i
  // -------------------------------------------------------------------
  auto& partitions(geometry->etaPartitions());
  std::vector<ME0DetId> vDetId;
  vDetId.clear();
  int oldRollNumber = 1;
  for (unsigned i=1; i<=partitions.size(); ++i) {
    ME0DetId detId(partitions.at(i-1)->id());
    const int rollNumber(detId.roll());

    // new batch of eta partitions --> new layer
    // - - - - - - - - - - - - - - - - - - - - -
    if (rollNumber < oldRollNumber) {
      ME0DetId fId(vDetId.front());
      ME0DetId layerId(fId.layerId());
      // compute the overall boundplane using the first eta partition
      const ME0EtaPartition* p(geometry->etaPartition(fId));
      const BoundPlane& bps = p->surface();
      BoundPlane* bp = const_cast<BoundPlane*>(&bps);
      ReferenceCountingPointer<BoundPlane> surf(bp);
      
      ME0Layer* la = new ME0Layer(layerId, surf);
      edm::LogVerbatim("ME0GeometryBuilderFromDDD")  << "Creating ME0 Layer " << layerId << " with " << vDetId.size() << " eta partitions" << std::endl;
      
      for(auto id : vDetId){
        edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding eta partition " << id << " to ME0 Layer " << std::endl;
        la->add(const_cast<ME0EtaPartition*>(geometry->etaPartition(id)));
      }
      
      edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding the layer to the geometry" << std::endl;
      geometry->add(la);
      vDetId.clear();
      oldRollNumber = 1;
    }

    // don't forget the last eta partition
    // --> repeat operation above 
    // - - - - - - - - - - - - - - - - - - 
    if(i == partitions.size()) {
      vDetId.push_back(detId);
      ME0DetId fId(vDetId.front());
      ME0DetId layerId(fId.layerId());
      // compute the overall boundplane using the first eta partition
      const ME0EtaPartition* p(geometry->etaPartition(fId));
      const BoundPlane& bps = p->surface();
      BoundPlane* bp = const_cast<BoundPlane*>(&bps);
      ReferenceCountingPointer<BoundPlane> surf(bp);
      
      ME0Layer * la = new ME0Layer(layerId, surf);
      edm::LogVerbatim("ME0GeometryBuilderFromDDD")  << "Creating ME0 Layer " << layerId << " with " << vDetId.size() << " eta partitions" << std::endl;
      
      for(auto id : vDetId){
	edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding eta partition " << id << " to ME0 Layer " << std::endl;
	la->add(const_cast<ME0EtaPartition*>(geometry->etaPartition(id)));
      }
      
      edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding the layer to the geometry" << std::endl;
      geometry->add(la);
      vDetId.clear();
    }

    // if not last eta partition 
    // then push back the current eta partition
    // for processing during next loop 
    // - - - - - - - - - - - - - - - - - - - - -
    else {
      vDetId.push_back(detId);
      ++oldRollNumber;
    }
  }

  // build the layers and add them to the geometry                                                                                                                                                         
  // -------------------------------------------------------------------
  // logic: 
  // in loop for element i, the eta partition (i-1) gets treated
  // and eta partition i gets pushed back for treatment in loop at i+1
  // caveat: if i is at ultimate element of loop, treat both (i-1) and i
  // -------------------------------------------------------------------
  auto& layers(geometry->layers());
  vDetId.clear();
  int oldLayerNumber = 1;
  for (unsigned i=1; i<=layers.size(); ++i) {
    ME0DetId detId(layers.at(i-1)->id());
    const int layerNumber(detId.layer());

    // new batch of layers --> new chamber
    // - - - - - - - - - - - - - - - - - -
    if (layerNumber < oldLayerNumber) {
      ME0DetId fId(vDetId.front());
      ME0DetId chamberId(fId.chamberId());
      // compute the overall boundplane using the first layer
      const ME0Layer * p = geometry->layer(fId);
      const BoundPlane& bps = p->surface();
      BoundPlane* bp = const_cast<BoundPlane*>(&bps);
      ReferenceCountingPointer<BoundPlane> surf(bp);
      ME0Chamber* ch = new ME0Chamber(chamberId, surf);
      edm::LogVerbatim("ME0GeometryBuilderFromDDD")  << "Creating ME0 Chamber " << chamberId << " with " << vDetId.size() << " layers" << std::endl;
      
      for(auto id : vDetId){
        edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding layer " << id << " to ME0 Chamber " << std::endl;
        ch->add(const_cast<ME0Layer*>(geometry->layer(id)));
      }
      
      edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding the chamber to the geometry" << std::endl;
      geometry->add(ch);
      vDetId.clear();
      oldLayerNumber = 1;
    }

    // don't forget the last layer
    // --> repeat operation above
    // - - - - - - - - - - - - - -
    if (i == layers.size()) {
      vDetId.push_back(detId);
      ME0DetId fId(vDetId.front());
      ME0DetId chamberId(fId.chamberId());
      // compute the overall boundplane using the first layer
      const ME0Layer * p = geometry->layer(fId);
      const BoundPlane& bps = p->surface();
      BoundPlane* bp = const_cast<BoundPlane*>(&bps);
      ReferenceCountingPointer<BoundPlane> surf(bp);

      ME0Chamber* ch = new ME0Chamber(chamberId, surf);
      edm::LogVerbatim("ME0GeometryBuilderFromDDD")  << "Creating ME0 Chamber " << chamberId << " with " << vDetId.size() << " layers" << std::endl;
      
      for(auto id : vDetId){
	edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding layer " << id << " to ME0 Chamber " << std::endl;
	  ch->add(const_cast<ME0Layer*>(geometry->layer(id)));
      }
      
      edm::LogVerbatim("ME0GeometryBuilderFromDDD") << "Adding the chamber to the geometry" << std::endl;
      geometry->add(ch);
      vDetId.clear();
      
    }

    // if not last layer
    // then push back the current layer 
    // for processing during next loop
    // - - - - - - - - - - - - - - - - 
    else {
      vDetId.push_back(detId);
      ++oldLayerNumber;
    }
  }

  return geometry;
}
