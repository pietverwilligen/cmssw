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
#include "Geometry/GEMGeometry/interface/ME0EtaPartitionSpecs.h"
#include "Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/Exception.h"


ME0RecHit2DStandardAlgo::ME0RecHit2DStandardAlgo(const edm::ParameterSet& config) :
  ME0RecHit2DBaseAlgo(config),
  nEtaPart_(config.getParameter<int>("nEtaPart"))   
{
}

ME0RecHit2DStandardAlgo::~ME0RecHit2DStandardAlgo() 
{
}

void ME0RecHit2DStandardAlgo::setES(const edm::EventSetup& setup) 
{
}

void ME0RecHit2DStandardAlgo::setupEtaPartition(const ME0EtaPartition& roll)
{
  // --- DISCLAIMER ------------------------------------------------------------
  // This method should be abandonned at the point that a real eta partition    
  // description becomes available in the ME0 Geometry description in XML       
  // for now we will artificially (and not very precise) introduce rolls in C++ 
  // ---------------------------------------------------------------------------

  // this should be moved to the ME0RecHit Producer, and called only once for each event

  // Extract detailed information from the Strip Topology
  // base_bottom, base_top, height, strips, pads         
  // note that (0,0) is in the middle of the roll ==> all param are at all half length

  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll.topology())));
  auto& parameters(roll.specs()->parameters());
  double bottomLength(parameters[0]); bottomLength = 2*bottomLength; // bottom is largest length, so furtest away from beamline
  double topLength(parameters[1]);    topLength    = 2*topLength;    // top is shortest length, so closest to beamline         
  double height(parameters[2]);       height       = 2*height;
  double rollRadius = top_->radius();

  double me0top = rollRadius + height/2;      //             /|                                                                
  double me0bot = rollRadius - height/2;      //            / |
                                              //        C /   |
  // conversion from spherical coords to eta,phi         /   B|
  // eta = - ln ( tan (theta/2) )                       /     |
  // tan theta/2 = sin theta / (1 + cos theta)         /__A___|                                                                                                                                                                                                                
  // sin theta = A/C                                           
  // cos theta = B/C                                           
  // ==> eta = - ln ( A / (sqrt(A^2 + B^2) + B)               
  // For B take middle of layer 3 anad 4 :: 540 cm            
  // ==> eta becomes function of height A only                

  for(int i=0; i<=nEtaPart_; ++i) {
    double A = me0bot+i*1.0/nEtaPart_*(me0top-me0bot);
    double B = 540.;
    etaPartsE_.push_back(-log(A/(sqrt(A*A+B*B)+B))); // save here eta value
    etaPartsY_.push_back(A);                         // save here the height
  }

  edm::LogVerbatim("ME0RecHit2DStandardAlgo") << "[ME0RecHit2D] :: vector with y-coordinates bottom/top Eta Partitions: ";
  for(int i=0; i<=nEtaPart_; ++i){
    edm::LogVerbatim("ME0RecHit2DStandardAlgo") << etaPartsY_[i];
  }
  edm::LogVerbatim("ME0RecHit2DStandardAlgo") << "[ME0RecHit2D] :: vector with eta-coordinates bottom/top Eta Partitions: ";
  for(int i=0; i<=nEtaPart_; ++i){
    edm::LogVerbatim("ME0RecHit2DStandardAlgo") << etaPartsE_[i];
  }

}


// First Step
bool ME0RecHit2DStandardAlgo::compute(const ME0EtaPartition& roll,
				    const ME0Cluster& cluster,
				    LocalPoint& Point,
				    LocalError& error)  const
{
  // Get Average Strip position for x
  float fstrip = (roll.centreOfStrip(cluster.firstStrip())).x();
  float lstrip = (roll.centreOfStrip(cluster.lastStrip())).x();
  float centreOfCluster = (fstrip + lstrip)/2;

  // Translate Strip position for y
  const TrapezoidalStripTopology* top_(dynamic_cast<const TrapezoidalStripTopology*>(&(roll.topology())));
  double rollRadius = top_->radius();
  double middleCurrentEtaPart = (etaPartsY_[cluster.etaPartition()-1]+etaPartsY_[cluster.etaPartition()])/2;
  // double middleLayer          = (etaPartsY_[nEtaPart_]-etaPartsY_[0])/2;
  double centreOfYStrip       = middleCurrentEtaPart - rollRadius;



  LocalPoint loctemp2(centreOfCluster,centreOfYStrip,0.);
 
  Point = loctemp2;
  // this is only local Error in X, not in Y ... 
  // in Y one takes automatically the height of the eta partition divided by sqrt(12)
  // we need to devide this by the number of eta partitions ... not implemented yet ...
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

