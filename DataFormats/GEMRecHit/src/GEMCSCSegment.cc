/** \file GEMCSCSegment.cc
 *
 *  $Date: $
 *  \author Raffaella Radogna
 */

#include <DataFormats/GEMRecHit/interface/GEMCSCSegment.h>
#include <iostream>

namespace {
  // define a Super Layer Id from the first layer of the first rechits, and put to first layer
  inline
  DetId buildDetId(CSCDetId id) {
    return CSCDetId (id.endcap(),id.station(),id.ring(),id.chamber(),0);
  }
  
}

GEMCSCSegment::GEMCSCSegment(const CSCSegment* csc_segment, const std::vector<const GEMRecHit*>& gem_rhs, LocalPoint origin, LocalVector direction, AlgebraicSymMatrix errors, double chi2) : 
//GEMCSCSegment::GEMCSCSegment(const std::vector<const GEMRecHit*>& gem_rhs, LocalPoint origin, LocalVector direction, AlgebraicSymMatrix errors, double chi2) : 
  //RecSegment(buildDetId(gem_rhs.front()->gemId())),
  RecSegment(buildDetId(csc_segment->cscDetId())),
  theOrigin(origin), 
  theLocalDirection(direction), theCovMatrix(errors), theChi2(chi2) {

  for(unsigned int i=0; i<gem_rhs.size(); ++i){
    theGEMRecHits.push_back(*gem_rhs[i]);}
    theCSCSegment = *csc_segment;
}

GEMCSCSegment::~GEMCSCSegment() {}

std::vector<const TrackingRecHit*> GEMCSCSegment::recHits() const{
  std::vector<const TrackingRecHit*> pointersOfRecHits;
  for (std::vector<GEMRecHit>::const_iterator irh = theGEMRecHits.begin(); irh!=theGEMRecHits.end(); ++irh) {
    pointersOfRecHits.push_back(&(*irh));
  }
  for (std::vector<CSCRecHit2D>::const_iterator irh = theCSCSegment.specificRecHits().begin(); irh!=theCSCSegment.specificRecHits().end(); ++irh) {
        pointersOfRecHits.push_back(&(*irh));
    }

  //pointersOfRecHits.insert( pointersOfRecHits.end(), theCSCSegment.specificRecHits().begin(), theCSCSegment.specificRecHits().end() );
  return pointersOfRecHits;
}

std::vector<TrackingRecHit*> GEMCSCSegment::recHits() {
  
  std::vector<TrackingRecHit*> pointersOfRecHits;
  for (std::vector<GEMRecHit>::iterator irh = theGEMRecHits.begin(); irh!=theGEMRecHits.end(); ++irh) {
    pointersOfRecHits.push_back(&(*irh));
  }
  //for (std::vector<CSCRecHit2D>::iterator irh = theCSCSegment.specificRecHits().begin(); irh!=theCSCSegment.specificRecHits().end(); ++irh) {
       // pointersOfRecHits.push_back(&(*irh));
 // }
  return pointersOfRecHits;
}

LocalError GEMCSCSegment::localPositionError() const {
  return LocalError(theCovMatrix[2][2], theCovMatrix[2][3], theCovMatrix[3][3]);
}

LocalError GEMCSCSegment::localDirectionError() const {
  return LocalError(theCovMatrix[0][0], theCovMatrix[0][1], theCovMatrix[1][1]); 
}


AlgebraicVector GEMCSCSegment::parameters() const {
  // For consistency with DT and CSC what we require for the TrackingRecHit interface,
  // the order of the parameters in the returned vector should be (dx/dz, dy/dz, x, z)
  
  AlgebraicVector result(4);

  result[0] = theLocalDirection.x()/theLocalDirection.z();
  result[1] = theLocalDirection.y()/theLocalDirection.z();    
  result[2] = theOrigin.x();
  result[3] = theOrigin.y();

  return result;
}


AlgebraicMatrix GEMCSCSegment::projectionMatrix() const {
  static AlgebraicMatrix theProjectionMatrix( 4, 5, 0);
  static bool isInitialized = false;
  if (!isInitialized) {
    theProjectionMatrix[0][1] = 1;
    theProjectionMatrix[1][2] = 1;
    theProjectionMatrix[2][3] = 1;
    theProjectionMatrix[3][4] = 1;
    isInitialized=true;
  }    
  return theProjectionMatrix;
}


void GEMCSCSegment::print() const {
  std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& os, const GEMCSCSegment& seg) {
  os << "GEMCSCSegment: local pos = " << seg.localPosition() << 
    " posErr = (" << sqrt(seg.localPositionError().xx())<<","<<sqrt(seg.localPositionError().yy())<<
    "0,)\n"<<
    "            dir = " << seg.localDirection() <<
    " dirErr = (" << sqrt(seg.localDirectionError().xx())<<","<<sqrt(seg.localDirectionError().yy())<<
    "0,)\n"<<
    "            chi2/ndf = " << seg.chi2()/double(seg.degreesOfFreedom()) << 
    " #rechits = " << seg.specificRecHits().size();

  return os;  
}


