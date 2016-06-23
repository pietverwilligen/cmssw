#ifndef RecoLocalMuon_ME0RecHit2DBaseAlgo_H
#define RecoLocalMuon_ME0RecHit2DBaseAlgo_H

/** \class ME0RecHit2DBaseAlgo
 *  Abstract algorithmic class to compute Rec Hit
 *  form a ME0 digi
 *
 *  $Date: 2014/02/04 10:16:32 $
 *  $Revision: 1.1 $
 *  \author M. Maggi -- INFN Bari
 */


#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GEMDigi/interface/ME0DigiPreRecoCollection.h"
#include "DataFormats/GEMDigi/interface/ME0Digi2DCollection.h"
#include "DataFormats/GEMRecHit/interface/ME0RecHit2D.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "RecoLocalMuon/GEMRecHit/src/ME0Clusterizer.h"

class ME0Cluster;
class ME0EtaPartition;
class ME0DetId;

namespace edm {
  class ParameterSet;
  class EventSetup;
}


class ME0RecHit2DBaseAlgo {

 public:
  
  /// Constructor
  ME0RecHit2DBaseAlgo(const edm::ParameterSet& config);

  /// Destructor
  virtual ~ME0RecHit2DBaseAlgo();  

  /// Pass the Event Setup to the algo at each event
  virtual void setES(const edm::EventSetup& setup) = 0;

  /// Build all hits in the range associated to the me0Id, at the 1st step.
  virtual edm::OwnVector<ME0RecHit2D> reconstruct(const ME0EtaPartition& roll, const ME0DetId& me0Id,
						const ME0Digi2DCollection::Range& digiRange, double timeres);

  /// standard local recHit computation Digi2D
  virtual bool compute(const ME0EtaPartition& roll, const ME0Cluster& cl,
                       LocalPoint& Point,
                       LocalError& error) const = 0;

  bool recOnlyMuons;
  bool recOnlyPrompt;
};
#endif
