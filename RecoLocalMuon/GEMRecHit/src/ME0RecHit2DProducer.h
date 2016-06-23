#ifndef RecoLocalMuon_ME0RecHit2DProducer_h
#define RecoLocalMuon_ME0RecHit2DProducer_h

/** \class ME0RecHit2DProducer
 *  Module for ME0RecHit2D production. 
 *  
 *  $Date: 2014/02/04 10:53:23 $
 *  $Revision: 1.1 $
 *  \author M. Maggim -- INFN Bari
 */


#include <memory>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <bitset>
#include <map>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"



namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class ME0RecHit2DBaseAlgo;

class ME0RecHit2DProducer : public edm::EDProducer {

public:
  /// Constructor
  ME0RecHit2DProducer(const edm::ParameterSet& config);

  /// Destructor
  virtual ~ME0RecHit2DProducer();

  // Method that access the EventSetup for each run
  virtual void beginRun(const edm::Run&, const edm::EventSetup& ) override;

  /// The method which produces the rechits
  virtual void produce(edm::Event& event, const edm::EventSetup& setup) override;

private:

  // The label to be used to retrieve ME0 digis from the event
  edm::InputTag theME0DigiLabel;

  // The reconstruction algorithm
  ME0RecHit2DBaseAlgo *theAlgo;
  // static std::string theAlgoName;
  double theTimeRes;
};

#endif

