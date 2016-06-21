#ifndef DataFormats_ME0RecHit2DCollection_H
#define DataFormats_ME0RecHit2DCollection_H

/** \class ME0RecHit2DCollection
 *  Collection of ME0RecHit2D for storage in the event
 *
 *  $Date: 2013/04/24 16:54:23 $
 *  $Revision: 1.1 $
 *  \author M. Maggi - INFN Bari
 */


#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHit2D.h"
#include "DataFormats/Common/interface/RangeMap.h"
#include "DataFormats/Common/interface/ClonePolicy.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include <functional>

typedef edm::RangeMap <ME0DetId,
		       edm::OwnVector<ME0RecHit2D,edm::ClonePolicy<ME0RecHit2D> >,
		       edm::ClonePolicy<ME0RecHit2D> > ME0RecHit2DCollection;


#endif




