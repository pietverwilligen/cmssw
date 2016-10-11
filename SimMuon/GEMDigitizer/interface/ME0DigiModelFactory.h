#ifndef GEMDigitizer_ME0DigiModelFactory_h
#define GEMDigitizer_ME0DigiModelFactory_h

#include "FWCore/PluginManager/interface/PluginFactory.h"

namespace edm{
  class ParameterSet;
}

class ME0DigiModel;

typedef edmplugin::PluginFactory<ME0DigiModel *(const edm::ParameterSet &)> ME0DigiModelFactory;

#endif
