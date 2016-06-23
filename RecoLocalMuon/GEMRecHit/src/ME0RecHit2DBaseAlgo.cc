/*
* See header file for a description of this class.
*
* $Date: 2014/02/04 10:16:35 $
* $Revision: 1.1 $
* \author M. Maggi -- INFN Bari
*/

#include "RecoLocalMuon/GEMRecHit/interface/ME0RecHit2DBaseAlgo.h"
#include "RecoLocalMuon/GEMRecHit/src/ME0ClusterContainer.h"
#include "RecoLocalMuon/GEMRecHit/src/ME0Cluster.h"
#include "RecoLocalMuon/GEMRecHit/src/ME0Clusterizer.h"

#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "DataFormats/GEMDigi/interface/ME0Digi2DCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"



ME0RecHit2DBaseAlgo::ME0RecHit2DBaseAlgo(const edm::ParameterSet& config) {
  recOnlyMuons  = config.getParameter<bool>("recOnlyMuons");
  recOnlyPrompt = config.getParameter<bool>("recOnlyPrompt");
}

ME0RecHit2DBaseAlgo::~ME0RecHit2DBaseAlgo(){}

// Build all hits in the range associated to the layerId, at the 1st step.
edm::OwnVector<ME0RecHit2D> ME0RecHit2DBaseAlgo::reconstruct(const ME0EtaPartition& roll, const ME0DetId& me0Id,
                                                         const ME0Digi2DCollection::Range& digiRange, double timeres) {
  edm::OwnVector<ME0RecHit2D> result;

  ME0Clusterizer clizer;
  ME0ClusterContainer cls = clizer.doAction(digiRange, timeres, recOnlyMuons, recOnlyPrompt);

  for (ME0ClusterContainer::const_iterator cl = cls.begin();
       cl != cls.end(); cl++){

    LocalError tmpErr;
    LocalPoint point;
    // Call the compute method
    bool OK = this->compute(roll, *cl, point, tmpErr);
    if (!OK) continue;
    // if (recOnlyMuons && std::abs(digi->pdgid()) != 13)  continue;
    // if (recOnlyPrompt && !digi->prompt()) continue;
    // Build a new pair of 1D rechit
    // int firstClustStrip= cl->firstStrip();
    // int clusterSize=cl->clusterSize();
    ME0RecHit2D*  recHit = new ME0RecHit2D(me0Id,cl->tof(),cl->firstStrip(),cl->clusterSize(), point,tmpErr);
    result.push_back(recHit);
  }
  return result;
}
