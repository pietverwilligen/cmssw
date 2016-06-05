#ifndef GEMDigitizer_ME0Digi2DModelFactory_h
#define GEMDigitizer_ME0Digi2DModelFactory_h

/** \class ME0Digi2DModelFactory
 *
 * Factory of seal plugins for ME02DDigitizer
 */
#include "FWCore/PluginManager/interface/PluginFactory.h"

namespace edm{
  class ParameterSet;
}

class ME0Digi2DModel;

typedef edmplugin::PluginFactory<ME0Digi2DModel *(const edm::ParameterSet &)> ME0Digi2DModelFactory;

#endif
