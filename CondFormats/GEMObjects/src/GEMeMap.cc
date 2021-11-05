#include "CondFormats/GEMObjects/interface/GEMeMap.h"
#include "CondFormats/GEMObjects/interface/GEMROMapping.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

GEMeMap::GEMeMap() : theVersion("") {}

GEMeMap::GEMeMap(const std::string& version) : theVersion(version) {}

GEMeMap::~GEMeMap() {}

const std::string& GEMeMap::version() const { return theVersion; }

void GEMeMap::convert(GEMROMapping& romap) {
  // fed->amc->geb mapping to GEMDetId
  for (auto imap : theChamberMap_) {
    for (unsigned int ix = 0; ix < imap.fedId.size(); ix++) {
      GEMROMapping::chamEC ec{imap.fedId[ix], imap.amcNum[ix], imap.gebId[ix]};
      GEMROMapping::chamDC dc;
      dc.detId = GEMDetId((imap.gemNum[ix] > 0) ? 1 : -1,
                          1,
                          abs(imap.gemNum[ix] / 1000),
                          abs(imap.gemNum[ix] / 100 % 10),
                          abs(imap.gemNum[ix] % 100),
                          0);
      dc.chamberType = imap.chamberType[ix];
      dc.vfatVer = imap.vfatVer[ix];
      romap.add(ec, dc);
      GEMROMapping::sectorEC amcEC = {imap.fedId[ix], imap.amcNum[ix]};
      if (!romap.isValidAMC(amcEC))
        romap.add(amcEC);
    }
  }

  // channel mapping
  for (auto imap : theStripMap_) {
    for (unsigned int ix = 0; ix < imap.chamberType.size(); ix++) {
      GEMROMapping::channelNum cMap;
      cMap.chamberType = imap.chamberType[ix];
      cMap.vfatAdd = imap.vfatAdd[ix];
      cMap.chNum = imap.vfatCh[ix];

      GEMROMapping::stripNum sMap;
      sMap.chamberType = imap.chamberType[ix];
      sMap.iEta = imap.iEta[ix];
      sMap.stNum = imap.strip[ix];

      romap.add(cMap, sMap);
      romap.add(sMap, cMap);
      if (cMap.chNum == 0) {
        GEMROMapping::vfatEC ec;
        ec.vfatAdd = cMap.vfatAdd;
        ec.iEta = sMap.iEta;

        GEMROMapping::vfatDC dc;
        dc.vfatAdd = cMap.vfatAdd;
        dc.chamberType = cMap.chamberType;

        romap.add(cMap.chamberType, ec);
        romap.add(dc, sMap.iEta);
      }
    }
  }
}

void GEMeMap::convertDummy(GEMROMapping& romap) {
  // 12 bits for vfat, 5 bits for geb, 8 bit long GLIB serial number
  unsigned int fedId = 0;

  for (int st = GEMDetId::minStationId0; st <= GEMDetId::maxStationId; ++st) {
    for (int re = -1; re <= 1; re = re + 2) {
      uint8_t amcNum = 1;  //amc
      uint8_t gebId = 0;
      int maxVFat = 0;
      int maxLayerId = GEMDetId::maxLayerId;
      int maxiEtaId = 0;
      if (st == 0) {
        maxVFat = maxVFatGE0_;
        fedId = (re == 1 ? FEDNumbering::MINGE0FEDID + 1 : FEDNumbering::MINGE0FEDID);
        maxLayerId = GEMDetId::maxLayerId0;
        maxiEtaId = maxiEtaIdGE0_;
      } else if (st == 1) {
        maxVFat = maxVFatGE11_;
        fedId = (re == 1 ? FEDNumbering::MINGEMFEDID + 1 : FEDNumbering::MINGEMFEDID);
        maxiEtaId = maxiEtaIdGE11_;
      } else if (st == 2) {
        maxVFat = maxVFatGE21_;
        fedId = (re == 1 ? FEDNumbering::MINGE21FEDID + 1 : FEDNumbering::MINGE21FEDID);
        maxiEtaId = maxiEtaIdGE21_;
      }

      uint16_t chipPos = 0;
      for (int lphi = 0; lphi < maxVFat; ++lphi) {
        for (int ieta = 1; ieta <= maxiEtaId; ++ieta) {
          for (int i = 0; i < maxChan_; ++i) {
            // only 1 vfat type for dummy map
            GEMROMapping::channelNum cMap;
            cMap.chamberType = st;
            cMap.vfatAdd = chipPos;
            cMap.chNum = i;

            GEMROMapping::stripNum sMap;
            sMap.chamberType = st;
            sMap.iEta = ieta;
            sMap.stNum = i;

            romap.add(cMap, sMap);
            romap.add(sMap, cMap);
            if (cMap.chNum == 0) {
              GEMROMapping::vfatEC ec;
              ec.vfatAdd = cMap.vfatAdd;
              ec.iEta = sMap.iEta;

              GEMROMapping::vfatDC dc;
              dc.vfatAdd = cMap.vfatAdd;
              dc.chamberType = st;

              romap.add(cMap.chamberType, ec);
              romap.add(dc, sMap.iEta);
            }
          }
          chipPos++;
        }
      }

      for (int ch = 1; ch <= GEMDetId::maxChamberId; ++ch) {
        for (int ly = 1; ly <= maxLayerId; ++ly) {
          GEMDetId gemId(re, 1, st, ly, ch, 0);

          GEMROMapping::chamEC ec;
          ec.fedId = fedId;
          ec.gebId = gebId;
          ec.amcNum = amcNum;

          GEMROMapping::chamDC dc;
          dc.detId = gemId;
          dc.chamberType = st;
          dc.vfatVer = vfatVerV3_;
          romap.add(ec, dc);

          GEMROMapping::sectorEC amcEC = {fedId, amcNum};
          if (!romap.isValidAMC(amcEC))
            romap.add(amcEC);

          // 5 bits for gebId
          if (st > 0 && gebId == maxGEB1_) {
            gebId = 0;
            amcNum += 2;  // only odd amc No. is used for GE11
          } else if (st == 0 && gebId == maxGEBs_) {
            gebId = 0;
            amcNum++;
          } else {
            // 1 geb per chamber
            gebId++;
          }
        }
      }
    }
  }
}
