/*
 *  See header file for a description of this class.
 *
 *  $Date: 2013/04/24 16:54:24 $
 *  $Revision: 1.1 $
 *  \author M. Maggi -- INFN Bari
 */


#include "DataFormats/GEMRecHit/interface/ME0RecHit2D.h"


ME0RecHit2D::ME0RecHit2D(const ME0DetId& me0Id, float tof) :  RecHit2DLocalPos(me0Id),
  theME0Id(me0Id), theTOF(tof), theFirstStrip(0), theClusterSize(0), theLocalPosition(), theLocalError()
{
}

ME0RecHit2D::ME0RecHit2D() :  RecHit2DLocalPos(),
  theME0Id(), theTOF(0.), theFirstStrip(0), theClusterSize(0), theLocalPosition(), theLocalError()
{
}


ME0RecHit2D::ME0RecHit2D(const ME0DetId& me0Id, float tof, const LocalPoint& pos) :  RecHit2DLocalPos(me0Id),
  theME0Id(me0Id), theTOF(tof), theFirstStrip(0), theClusterSize(0), theLocalPosition(pos), theLocalError()
{
}



// Constructor from a local position and error, wireId and digi time.
ME0RecHit2D::ME0RecHit2D(const ME0DetId& me0Id,
		     float tof, 
                     int firstStrip,
		     int clustSize,                     
		     const LocalPoint& pos,
		     const LocalError& err) :  RecHit2DLocalPos(me0Id),
					       theME0Id(me0Id), theTOF(tof), theFirstStrip(firstStrip), theClusterSize(clustSize), theLocalPosition(pos), theLocalError(err)
{
}

// Destructor
ME0RecHit2D::~ME0RecHit2D()
{
}



ME0RecHit2D * ME0RecHit2D::clone() const {
  return new ME0RecHit2D(*this);
}


// Access to component RecHits.
// No components rechits: it returns a null vector
std::vector<const TrackingRecHit*> ME0RecHit2D::recHits() const {
  std::vector<const TrackingRecHit*> nullvector;
  return nullvector; 
}
// Non-const access to component RecHits.
// No components rechits: it returns a null vector
std::vector<TrackingRecHit*> ME0RecHit2D::recHits() {
  std::vector<TrackingRecHit*> nullvector;
  return nullvector; 
}

// Comparison operator, based on the wireId and the digi time
bool ME0RecHit2D::operator==(const ME0RecHit2D& hit) const {
  return this->geographicalId() == hit.geographicalId(); 
}


// The ostream operator
std::ostream& operator<<(std::ostream& os, const ME0RecHit2D& hit) {
  os << "pos: x = " << hit.localPosition().x() ; 
  os << " +/- " << sqrt(hit.localPositionError().xx())<<" cm";
  os<< " y = " << hit.localPosition().y() ;
  os << " +/- " << sqrt(hit.localPositionError().yy())<<" cm";
  os<< " t = " << hit.tof() ;
  os << " +/- " << "[not implemented]" <<" ns";
  return os;
}
