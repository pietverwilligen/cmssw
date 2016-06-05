#ifndef GEMDigi_ME0Digi2DCollection_h
#define GEMDigi_ME0Digi2DCollection_h
/** \class ME0Digi2DCollection
 *  
 *  \author Piet Verwilligen
 */

#include <DataFormats/MuonDetId/interface/ME0DetId.h>
#include <DataFormats/GEMDigi/interface/ME0Digi2D.h>
#include <DataFormats/MuonData/interface/MuonDigiCollection.h>

typedef MuonDigiCollection<ME0DetId, ME0Digi2D> ME0Digi2DCollection;

#endif

