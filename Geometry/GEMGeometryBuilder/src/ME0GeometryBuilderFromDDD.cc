#include "Geometry/GEMGeometryBuilder/src/ME0GeometryBuilderFromDDD.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"

#include <DetectorDescription/Core/interface/DDFilter.h>
#include <DetectorDescription/Core/interface/DDFilteredView.h>
#include <DetectorDescription/Core/interface/DDSolid.h>

#include "Geometry/MuonNumbering/interface/MuonDDDNumbering.h"
#include "Geometry/MuonNumbering/interface/MuonBaseNumber.h"
#include "Geometry/MuonNumbering/interface/ME0NumberingScheme.h"

#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"

#include "DataFormats/GeometryVector/interface/Basic3DVector.h"

#include "CLHEP/Units/GlobalSystemOfUnits.h"

#include <iostream>
#include <algorithm>
#include <boost/lexical_cast.hpp>

ME0GeometryBuilderFromDDD::ME0GeometryBuilderFromDDD()
{ }

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


ME0Geometry* ME0GeometryBuilderFromDDD::buildGeometry(DDFilteredView& fv, const MuonDDDConstants& muonConstants)
{

  ME0Geometry* geometry = new ME0Geometry();

  LogTrace("ME0GeometryBuilderFromDDD") <<"Building the geometry service";
  LogTrace("ME0GeometryBuilderFromDDD") <<"About to run through the ME0 structure\n" 
					<<"Top level logical part: "
					<<fv.logicalPart().name().name();
 

  
  bool doChambers = fv.firstChild();
  LogTrace("ME0GeometryBuilderFromDDD") << "doChamber = fv.firstChild() = " << doChambers;
  // ----------------------------------------------------------------------------------------------------------------------------------------------
  // LogTrace("ME0GeometryBuilderFromDDD") << "Second level logical part: " << fv.logicalPart().name().name(); // not good to do printout here ... not sure whether operation above returned 1 or 0
  // LogTrace("ME0GeometryBuilderFromDDD") << "start the loop over the ME0Chambers";
  while (doChambers) {
    // to etapartitions
    // std::cout<<"to layer "<<fv.firstChild()<<std::endl;
    // std::cout<<"to etapt "<<fv.firstChild()<<std::endl;
    MuonDDDNumbering mdddnum(muonConstants);
    ME0NumberingScheme me0Num(muonConstants);
    int rawId = me0Num.baseNumberToUnitNumber(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
    ME0DetId detId = ME0DetId(rawId);
    ME0DetId detIdCh = detId.chamberId();
    // back to chambers
    // std::cout<<"back to layer "<<fv.parent()<<std::endl;
    // std::cout<<"back to chamb "<<fv.parent()<<std::endl;
    // ok lets get started ...                             
    LogTrace("ME0GeometryBuilderFromDDD") << "In DoChambers Loop :: ME0DetId "<<detId<<" = "<<detId.rawId()<<" (which belongs to ME0Chamber "<<detIdCh<<" = "<<detIdCh.rawId()<<")";
    LogTrace("ME0GeometryBuilderFromDDD") << "Second level logical part: " << fv.logicalPart().name().name();
    DDBooleanSolid solid2 = (DDBooleanSolid)(fv.logicalPart().solid());
    std::vector<double> dpar2 = solid2.parameters();
    std::stringstream parameters2;
    for(unsigned int i=0; i<dpar2.size(); ++i) {
      parameters2 << " dpar["<<i<<"]="<< dpar2[i] << "cm ";
    }
    LogTrace("ME0GeometryBuilderFromDDD") << "Second level parameters: vector with size = "<<dpar2.size()<<" and elements "<<parameters2.str();
    bool doLayers = fv.firstChild();
    // --------------------------------------------------------------------------------------------------------------------------------------------
    // LogTrace("ME0GeometryBuilderFromDDD") << "Third level logical part: " << fv.logicalPart().name().name(); // not good to do printout here ... not sure whether operation above returned 1 or 0
    LogTrace("ME0GeometryBuilderFromDDD") << "doLayer = fv.firstChild() = " << doLayers;
    while (doLayers) {
      // to etapartitions
      // std::cout<<"to etapt "<<fv.firstChild()<<std::endl;
      MuonDDDNumbering mdddnum(muonConstants);
      ME0NumberingScheme me0Num(muonConstants);
      int rawId = me0Num.baseNumberToUnitNumber(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
      ME0DetId detId = ME0DetId(rawId);
      ME0DetId detIdLa = detId.layerId();
      // back to layers
      // std::cout<<"back to layer "<<fv.parent()<<std::endl;
      LogTrace("ME0GeometryBuilderFromDDD") << "In DoLayers Loop :: ME0DetId "<<detId<<" = "<<detId.rawId()<<" (which belongs to ME0Layer "<<detIdLa<<" = "<<detIdLa.rawId()<<")";
      LogTrace("ME0GeometryBuilderFromDDD") << "Third level logical part: " << fv.logicalPart().name().name();
      DDBooleanSolid solid3 = (DDBooleanSolid)(fv.logicalPart().solid());
      std::vector<double> dpar3 = solid3.parameters();
      std::stringstream parameters3;
      for(unsigned int i=0; i<dpar3.size(); ++i) {
	parameters3 << " dpar["<<i<<"]="<< dpar3[i] << "cm ";
      }
      LogTrace("ME0GeometryBuilderFromDDD") << "Third level parameters: vector with size = "<<dpar3.size()<<" and elements "<<parameters3.str();
      bool doEtaParts = fv.firstChild(); 
      // --------------------------------------------------------------------------------------------------------------------------------------------
      // LogTrace("ME0GeometryBuilderFromDDD") << "Fourth level logical part: " << fv.logicalPart().name().name(); // not good to do printout here ... not sure whether operation above returned 1 or 0
      LogTrace("ME0GeometryBuilderFromDDD") << "doEtaPart = fv.firstChild() = " << doEtaParts;
      while (doEtaParts) {
	LogTrace("ME0GeometryBuilderFromDDD") << "In DoEtaParts Loop";
	DDBooleanSolid solid4 = (DDBooleanSolid)(fv.logicalPart().solid());
	std::vector<double> dpar4 = solid4.parameters();
	std::stringstream parameters4;
	for(unsigned int i=0; i<dpar4.size(); ++i) {
	  parameters4 << " dpar["<<i<<"]="<< dpar4[i] << "cm ";
	}
	LogTrace("ME0GeometryBuilderFromDDD") << "Fourth level parameters: vector with size = "<<dpar4.size()<<" and elements "<<parameters4.str();
	// --------------------------------------------------------------------------------------------------------------------------------------------
	doEtaParts = fv.nextSibling();
	LogTrace("ME0GeometryBuilderFromDDD") << "doEtaPart = fv.nextSibling() = " << doEtaParts;
      }
      // fv.parent();
      LogTrace("ME0GeometryBuilderFromDDD") << "went back to parent :: name = "<<fv.logicalPart().name().name()<<" will now ask for nextSibling";
      doLayers = fv.nextSibling();
      LogTrace("ME0GeometryBuilderFromDDD") << "doLayer = fv.nextSibling() = " << doLayers;
    }
    // fv.parent();
    LogTrace("ME0GeometryBuilderFromDDD") << "went back to parent :: name = "<<fv.logicalPart().name().name()<<" will now ask for nextSibling";
    doChambers = fv.nextSibling();
    LogTrace("ME0GeometryBuilderFromDDD") << "doChamber = fv.nextSibling() = " << doChambers;
  }

  /*
  // loop over chambers
  bool doChambers = fv.firstChild();
  while (doChambers){
    // getting chamber id from eta partitions
    fv.firstChild();  // ---> go to layers
    fv.firstChild();  // ---> go to eta partitions
    MuonDDDNumbering mdddnum(muonConstants);
    ME0NumberingScheme me0Num(muonConstants);
    int rawId = me0Num.baseNumberToUnitNumber(mdddnum.geoHistoryToBaseNumber(fv.geoHistory()));
    ME0DetId detId = ME0DetId(rawId);
    // ME0DetId detIdCh = ME0DetId(rawid).chamberId();
    // back to chambers
    fv.parent();
    fv.parent();
    ME0Chamber *me0Chamber = buildChamber(fv, detId.chamberId());
    bool doLayers = fv.firstChild();    
    LogTrace("ME0GeometryBuilderFromDDD") << "doLayers = " << doLayers;
    LogTrace("ME0GeometryBuilderFromDDD") << "start the loop over the ME0Layers";
    // loop over layers
    while (doLayers){
      // ME0DetId detIdLa = ME0DetId(rawid).layerId();
      ME0Layer *me0Layer = buildLayer(fv, detId.layerId());
      bool doEtaParts = fv.firstChild();
      // loop over ME0EtaPartitions
      while (doEtaParts){
	ME0EtaPartition *etaPart = buildEtaPartition(fv, detId);
	me0Layer->add(etaPart);
	geometry->add(etaPart);
	doEtaParts = fv.nextSibling();
      }
      fv.parent();
      me0Chamber->add(me0Layer);
      geometry->add(me0Layer);
      doLayers = fv.nextSibling();
    }
    fv.parent();
    geometry->add(me0Chamber);
    doChambers = fv.nextSibling();
  } 
  */ 
  return geometry;
}

ME0Chamber* ME0GeometryBuilderFromDDD::buildChamber(DDFilteredView& fv, ME0DetId detId) const {
  LogTrace("ME0GeometryBuilderFromDDD") << "buildChamber "<<fv.logicalPart().name().name() <<" "<< detId <<std::endl;
  
  // here something goes wrong with dpar , solid.solidA and solid.solidB ... for ME0 these things are probably not defined ...  
  DDBooleanSolid solid = (DDBooleanSolid)(fv.logicalPart().solid());
  // std::vector<double> dpar = solid.solidA().parameters();
  std::vector<double> dpar = solid.parameters();
  // /*  
  double dy = dpar[0]/cm;// length is along local Y
  double dz = dpar[3]/cm;// thickness is long local Z
  double dx1= dpar[4]/cm;// bottom width is along local X
  double dx2= dpar[8]/cm;// top width is along local X
  // */
  // double dy = 1.0, dz = 1.0, dx1 = 1.0, dx2 = 1.0;

  ///*
  LogTrace("ME0GeometryBuilderFromDDD") << " name of logical part = "<<fv.logicalPart().name().name()<<std::endl;
  LogTrace("ME0GeometryBuilderFromDDD") << " dpar is vector with size = "<<dpar.size()<<std::endl;
  for(unsigned int i=0; i<dpar.size(); ++i) {
    LogTrace("ME0GeometryBuilderFromDDD") << " dpar ["<<i<<"] = "<< dpar[i] << " cm "<<std::endl;
  }
  //*/

  // dpar = solid.solidB().parameters();
  // dpar = solid.solidA().parameters();
  dz += dpar[3]/cm;      // layer thickness
  dz += 2.105;           // gap between chambers --- to be checked !!!
  dz *= 6;               // 6 layers in chamber
  
  LogTrace("ME0GeometryBuilderFromDDD") << "size "<< dx1 << " " << dx2 << " " << dy << " " << dz <<std::endl;

  bool isOdd = false; // detId.chamber()%2;
  RCPBoundPlane surf(boundPlane(fv, new TrapezoidalPlaneBounds(dx1,dx2,dy,dz), isOdd ));
  ME0Chamber* chamber = new ME0Chamber(detId.chamberId(), surf);
  return chamber;
}

ME0Layer* ME0GeometryBuilderFromDDD::buildLayer(DDFilteredView& fv, ME0DetId detId) const {
  LogTrace("ME0GeometryBuilderFromDDD") << "buildLayer "<<fv.logicalPart().name().name() <<" "<< detId <<std::endl;
  
  DDBooleanSolid solid = (DDBooleanSolid)(fv.logicalPart().solid());
  // std::vector<double> dpar = solid.solidA().parameters();
  std::vector<double> dpar = solid.parameters();
  
  double dy = dpar[0]/cm;// length is along local Y
  double dz = dpar[3]/cm;// thickness is long local Z
  double dx1= dpar[4]/cm;// bottom width is along local X
  double dx2= dpar[8]/cm;// top width is along local X
  // dpar = solid.solidB().parameters();
  // dz += dpar[3]/cm;     // layer thickness --- to be checked !!! layer thickness should be same as eta part thickness
  
  LogTrace("ME0GeometryBuilderFromDDD") << "size "<< dx1 << " " << dx2 << " " << dy << " " << dz <<std::endl;

  bool isOdd = false; // detId.chamber()%2;
  RCPBoundPlane surf(boundPlane(fv, new TrapezoidalPlaneBounds(dx1,dx2,dy,dz), isOdd ));
  ME0Layer* layer = new ME0Layer(detId.layerId(), surf);
  return layer;
}

ME0EtaPartition* ME0GeometryBuilderFromDDD::buildEtaPartition(DDFilteredView& fv, ME0DetId detId) const {
  LogTrace("ME0GeometryBuilderFromDDD") << "buildEtaPartition "<<fv.logicalPart().name().name() <<" "<< detId <<std::endl;
  
  // EtaPartition specific parameter (nstrips and npads) 
  DDValue numbOfStrips("nStrips");
  DDValue numbOfPads("nPads");
  std::vector<const DDsvalues_type* > specs(fv.specifics());
  std::vector<const DDsvalues_type* >::iterator is = specs.begin();
  double nStrips = 0., nPads = 0.;
  for (;is != specs.end(); is++){
    if (DDfetch( *is, numbOfStrips)) nStrips = numbOfStrips.doubles()[0];
    if (DDfetch( *is, numbOfPads))   nPads = numbOfPads.doubles()[0];
  }
  LogTrace("ME0GeometryBuilderFromDDD") 
    << ((nStrips == 0. ) ? ("No nStrips found!!") : ("Number of strips: " + boost::lexical_cast<std::string>(nStrips))); 
  LogTrace("ME0GeometryBuilderFromDDD") 
    << ((nPads == 0. ) ? ("No nPads found!!") : ("Number of pads: " + boost::lexical_cast<std::string>(nPads)));
  
  // EtaPartition specific parameter (size) 
  std::vector<double> dpar = fv.logicalPart().solid().parameters();

  double be = dpar[4]/cm; // half bottom edge
  double te = dpar[8]/cm; // half top edge
  double ap = dpar[0]/cm; // half apothem
  double ti = 0.4/cm;     // half thickness
  
  std::vector<float> pars;
  pars.push_back(be); 
  pars.push_back(te); 
  pars.push_back(ap); 
  pars.push_back(nStrips);
  pars.push_back(nPads);
  
  bool isOdd = false; // detId.chamber()%2;
  RCPBoundPlane surf(boundPlane(fv, new TrapezoidalPlaneBounds(be, te, ap, ti), isOdd ));
  std::string name = fv.logicalPart().name().name();
  ME0EtaPartitionSpecs* e_p_specs = new ME0EtaPartitionSpecs(GeomDetEnumerators::ME0, name, pars);
  
  LogTrace("ME0GeometryBuilderFromDDD") << "size "<< be << " " << te << " " << ap << " " << ti <<std::endl;
  ME0EtaPartition* etaPartition = new ME0EtaPartition(detId, surf, e_p_specs);
  return etaPartition;
}

ME0GeometryBuilderFromDDD::RCPBoundPlane
ME0GeometryBuilderFromDDD::boundPlane(const DDFilteredView& fv,
                                      Bounds* bounds, bool isOddChamber) const {
  // extract the position
  const DDTranslation & trans(fv.translation());
  const Surface::PositionType posResult(float(trans.x()/cm),
                                        float(trans.y()/cm),
                                        float(trans.z()/cm));

  // now the rotation
  //  DDRotationMatrix tmp = fv.rotation(); 
  // === DDD uses 'active' rotations - see CLHEP user guide === 
  //     ORCA uses 'passive' rotation.                          
  //     'active' and 'passive' rotations are inverse to each other
  //  DDRotationMatrix tmp = fv.rotation();                        
  DDRotationMatrix rotation = fv.rotation();//REMOVED .Inverse();  
  DD3Vector x, y, z;
  rotation.GetComponents(x,y,z);
  // LogTrace("GEMGeometryBuilderFromDDD") << "translation: "<< fv.translation() << std::endl;
  // LogTrace("GEMGeometryBuilderFromDDD") << "rotation   : "<< fv.rotation() << std::endl;   
  // LogTrace("GEMGeometryBuilderFromDDD") << "INVERSE rotation manually: \n"                 
  //        << x.X() << ", " << x.Y() << ", " << x.Z() << std::endl                           
  //        << y.X() << ", " << y.Y() << ", " << y.Z() << std::endl                          
  //        << z.X() << ", " << z.Y() << ", " << z.Z() << std::endl;                         

  Surface::RotationType rotResult(float(x.X()),float(x.Y()),float(x.Z()),
                                  float(y.X()),float(y.Y()),float(y.Z()),
                                  float(z.X()),float(z.Y()),float(z.Z()));

  //Change of axes for the forward
  Basic3DVector<float> newX(1.,0.,0.);
  Basic3DVector<float> newY(0.,0.,1.);
  Basic3DVector<float> newZ(0.,1.,0.);
  // Odd chambers are inverted in gem.xml
  if (isOddChamber) newY *= -1;

  rotResult.rotateAxes(newX, newY, newZ);

  return RCPBoundPlane( new BoundPlane( posResult, rotResult, bounds));
}

