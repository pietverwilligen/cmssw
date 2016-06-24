#ifndef RecoLocalMuon_ME0RecHit2DStandardAlgo_H
#define RecoLocalMuon_ME0RecHit2DStandardAlgo_H

/** \class ME0RecHit2DStandardAlgo
 *  Concrete implementation of ME0RecHit2DBaseAlgo.
 *
 *  $Date: 2013/04/24 17:16:36 $
 *  $Revision: 1.1 $
 *  \author M. Maggi -- INFN Bari
 */

#include "RecoLocalMuon/GEMRecHit/interface/ME0RecHit2DBaseAlgo.h"



class ME0RecHit2DStandardAlgo : public ME0RecHit2DBaseAlgo {
 public:
  /// Constructor
  ME0RecHit2DStandardAlgo(const edm::ParameterSet& config);

  /// Destructor
  virtual ~ME0RecHit2DStandardAlgo();

  // Operations

  /// Pass the Event Setup to the algo at each event
  virtual void setES(const edm::EventSetup& setup);


  virtual bool compute(const ME0EtaPartition& roll,
                       const ME0Cluster& cluster,
                       LocalPoint& point,
                       LocalError& error) const;


  virtual bool compute(const ME0EtaPartition& roll,
                       const ME0Cluster& cluster,
                       const float& angle,
                       const GlobalPoint& globPos, 
                       LocalPoint& point,
                       LocalError& error) const;

  virtual void setupEtaPartition(const ME0EtaPartition& roll); 

 private:
  int nEtaPart_;
  bool dEtaFixed_;
  std::vector<double> etaPartsE_, etaPartsY_;
};
#endif


