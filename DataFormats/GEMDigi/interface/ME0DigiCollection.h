#ifndef GEMDigi_ME0DigiCollection_h
#define GEMDigi_ME0DigiCollection_h

#include <DataFormats/MuonDetId/interface/ME0DetId.h>
#include <DataFormats/GEMDigi/interface/ME0Digi.h>
#include <DataFormats/MuonData/interface/MuonDigiCollection.h>

typedef MuonDigiCollection<ME0DetId, ME0Digi> ME0DigiCollection;

#endif

