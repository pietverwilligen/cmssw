#include "CondFormats/GEMObjects/interface/GEMELMap.h"
#include "CondFormats/GEMObjects/interface/GEMROmap.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"

GEMELMap::GEMELMap() : theVersion("") {}

GEMELMap::GEMELMap(const std::string& version) : theVersion(version) {}

GEMELMap::~GEMELMap() {}

const std::string& GEMELMap::version() const { return theVersion; }

void GEMELMap::convert(GEMROmap& romap) {
  for (auto imap : theChamberMap_) {
    for (unsigned int ix = 0; ix < imap.chamberType.size(); ix++) {
      GEMROmap::eCoord ec;
      ec.gebId = imap.gebId[ix];
      ec.amcId = imap.amcId[ix];

      int st = std::abs(imap.z_direction[ix]);
      GEMROmap::dCoord dc;
      dc.gemDetId = GEMDetId(imap.z_direction[ix], 1, st, imap.depth[ix], imap.sec[ix], 0);
      dc.chamberType = imap.chamberType[ix];

      romap.add(ec, dc);
      romap.add(dc, ec);
    }
  }

  for (auto imap : theStripMap_) {
    for (unsigned int ix = 0; ix < imap.chamberType.size(); ix++) {
      GEMROmap::channelNum cMap;
      cMap.chamberType = imap.chamberType[ix];
      cMap.vfatAdd = imap.vfatId[ix];
      cMap.chNum = imap.vfatCh[ix];

      GEMROmap::stripNum sMap;
      sMap.chamberType = imap.chamberType[ix];
      sMap.iEta = imap.iEta[ix];
      sMap.stNum = imap.strip[ix];

      romap.add(cMap, sMap);
      romap.add(sMap, cMap);
    }
  }
}

void GEMELMap::convertDummy(GEMROmap& romap) {
  // 12 bits for vfat, 5 bits for geb, 8 bit long GLIB serial number
  uint16_t amcId = 1;  //amc
  uint16_t gebId = 0;

  for (int re = -1; re <= 1; re = re + 2) {
    for (int st = GEMDetId::minStationId; st <= GEMDetId::maxStationId; ++st) {
      int maxVFat = maxVFatGE11_;
      if (st == 2)
        maxVFat = maxVFatGE21_;
      if (st == 0)
        maxVFat = maxVFatGE0_;


      uint16_t chipId = 0;
      for (int roll = 1; roll <= GEMDetId::maxRollId; ++roll) {

        for (int nphi = 1; nphi <= maxVFat; ++nphi) {

          for (int i = 0; i < maxChan_; ++i) {
            // only 1 chamber type for dummy map
            GEMROmap::channelNum cMap;
            cMap.chamberType = st;
            cMap.chNum = i;

            GEMROmap::stripNum sMap;
            sMap.chamberType = st;
            sMap.stNum = i + 1;

            romap.add(cMap, sMap);
            romap.add(sMap, cMap);
          }
          chipId++;
        }
      }

      for (int ch = 1; ch <= GEMDetId::maxChamberId; ++ch) {
        for (int ly = 1; ly <= GEMDetId::maxLayerId; ++ly) {
          // 1 geb per chamber
          GEMDetId gemId(re, 1, st, ly, ch, 0);
          gebId++;

          GEMROmap::eCoord ec;
          ec.gebId = gebId;
          ec.amcId = amcId;

          GEMROmap::dCoord dc;
          dc.gemDetId = gemId;
          dc.chamberType = st;

          romap.add(ec, dc);
          romap.add(dc, ec);
          // 5 bits for geb
          if (gebId == maxGEBs_) {
            // 24 gebs per amc
            gebId = 0;
            amcId++;
          }
        }
      }
    }
  }
}
