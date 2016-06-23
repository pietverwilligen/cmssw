#ifndef RecoLocalMuon_ME0RecHit2DAlgoFactory_H
#define RecoLocalMuon_ME0RecHit2DAlgoFactory_H

/** \class ME0RecHitAlgoFactory
 *  Factory of seal plugins for 1D RecHit reconstruction algorithms.
 *  The plugins are concrete implementations of ME0RecHitBaseAlgo base class.
 *
 *  $Date: 2014/02/04 10:16:32 $
 *  $Revision: 1.1 $
 *  \author M. Maggi - INFN Torino
 */
#include "FWCore/PluginManager/interface/PluginFactory.h"
#include "RecoLocalMuon/GEMRecHit/interface/ME0RecHit2DBaseAlgo.h"

typedef edmplugin::PluginFactory<ME0RecHit2DBaseAlgo *(const edm::ParameterSet &)> ME0RecHit2DAlgoFactory;
#endif




