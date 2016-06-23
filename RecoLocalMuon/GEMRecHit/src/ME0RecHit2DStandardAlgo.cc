/*
 *  See header file for a description of this class.
 *
 *  $Date: 2013/04/24 17:16:35 $
 *  $Revision: 1.1 $
 *  \author M. Maggi -- INFN
 */

#include "ME0Cluster.h"
#include "RecoLocalMuon/GEMRecHit/src/ME0RecHit2DStandardAlgo.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "Geometry/GEMGeometry/interface/ME0EtaPartition.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/Exception.h"


ME0RecHit2DStandardAlgo::ME0RecHit2DStandardAlgo(const edm::ParameterSet& config) :
  ME0RecHit2DBaseAlgo(config) 
{
}

ME0RecHit2DStandardAlgo::~ME0RecHit2DStandardAlgo() 
{
}

void ME0RecHit2DStandardAlgo::setES(const edm::EventSetup& setup) 
{
}

// First Step
bool ME0RecHit2DStandardAlgo::compute(const ME0EtaPartition& roll,
				    const ME0Cluster& cluster,
				    LocalPoint& Point,
				    LocalError& error)  const
{
  // Get Average Strip position
  float fstrip = (roll.centreOfStrip(cluster.firstStrip())).x();
  float lstrip = (roll.centreOfStrip(cluster.lastStrip())).x();
  float centreOfCluster = (fstrip + lstrip)/2;

  LocalPoint loctemp2(centreOfCluster,0.,0.);
 
  Point = loctemp2;
  error = roll.localError((cluster.firstStrip()+cluster.lastStrip())/2., cluster.clusterSize());
  return true;
}


bool ME0RecHit2DStandardAlgo::compute(const ME0EtaPartition& roll,
				    const ME0Cluster& cl,
				    const float& angle,
				    const GlobalPoint& globPos, 
				    LocalPoint& Point,
				    LocalError& error)  const
{

  // Glob Pos and angle not used so far...
  if (globPos.z()<0){ } // Fake use to avoid warnings
  if (angle<0.){ }      // Fake use to avoid warnings
  this->compute(roll,cl,Point,error);
  return true;
}

