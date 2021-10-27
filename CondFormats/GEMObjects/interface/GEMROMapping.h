#ifndef CondFormats_GEMObjects_GEMROMapping_h
#define CondFormats_GEMObjects_GEMROMapping_h
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include <map>
#include <vector>
#include <algorithm>

class GEMROMapping {
  // EC electronics corrdinate
  // DC GEMDetId corrdinate
  // geb = GEM electronics board == OptoHybrid
public:
  struct sectorEC {
    unsigned int fedId;
    uint8_t amcNum;
    bool operator==(const sectorEC& r) const {
      if (fedId == r.fedId) {
        return amcNum == r.amcNum;
      } else {
        return false;
      }
    }
  };

  struct chamEC {
    unsigned int fedId;
    uint8_t amcNum;
    uint8_t gebId;
    bool operator<(const chamEC& r) const {
      if (fedId == r.fedId) {
        if (amcNum == r.amcNum) {
          return gebId < r.gebId;
        } else {
          return amcNum < r.amcNum;
        }
      } else {
        return fedId < r.fedId;
      }
    }
  };

  struct chamDC {
    GEMDetId detId;
    int chamberType;
    int vfatVer;
    bool operator<(const chamDC& r) const { return detId < r.detId; }
  };

  struct vfatEC {
    int iEta;
    uint16_t vfatAdd;
    bool operator<(const vfatEC& r) const {
      if (vfatAdd == r.vfatAdd) {
        return iEta < r.iEta;
      } else {
        return vfatAdd < r.vfatAdd;
      }
    }
  };

  struct vfatDC {
    int chamberType;
    uint16_t vfatAdd;
    bool operator<(const vfatDC& r) const {
      if (vfatAdd == r.vfatAdd) {
        return chamberType < r.chamberType;
      } else {
        return vfatAdd < r.vfatAdd;
      }
    }
  };

  struct channelNum {
    int chamberType;
    int vfatAdd;
    int chNum;
    bool operator<(const channelNum& c) const {
      if (chamberType == c.chamberType) {
        if (vfatAdd == c.vfatAdd) {
          return chNum < c.chNum;
        } else {
          return vfatAdd < c.vfatAdd;
        } 
      } else {
        return chamberType < c.chamberType;
      }
    }
  };

  struct stripNum {
    int chamberType;
    int iEta;
    int stNum;
    bool operator<(const stripNum& s) const {
      if (chamberType == s.chamberType) {
        if (iEta == s.iEta) {
          return stNum < s.stNum;
        } else {
          return iEta < s.iEta;
        }
      } else {
        return chamberType < s.chamberType;
      }
    }
  };

  GEMROMapping(){};

  bool isValidChamber(const chamEC& r) const { return chamberMap_.find(r) != chamberMap_.end(); }
  
  bool isValidChipID(const vfatDC& r) const { return chamIEtas_.find(r) != chamIEtas_.end(); }

  bool isValidAMC(const sectorEC& r) const { return std::find(amcVec_.begin(), amcVec_.end(), r) != amcVec_.end(); }

  void add(sectorEC e) { amcVec_.push_back(e); }

  const chamDC& chamberPos(const chamEC& r) const { return chamberMap_.at(r); }
  void add(chamEC e, chamDC d) { chamberMap_[e] = d; }

  const std::vector<vfatEC> getVfats(const int type) const { return chamVfats_.at(type); }
  void add(int type, vfatEC d) { chamVfats_[type].push_back(d); }

  const int getIEta(const vfatDC dc) const { return chamIEtas_.at(dc); }
  void add(vfatDC d, int iEta) { chamIEtas_[d] = iEta; }

  const channelNum& hitPos(const stripNum& s) const { return stChMap_.at(s); }
  const stripNum& hitPos(const channelNum& c) const { return chStMap_.at(c); }

  void add(channelNum c, stripNum s) { chStMap_[c] = s; }
  void add(stripNum s, channelNum c) { stChMap_[s] = c; }

private:
  std::vector<sectorEC> amcVec_;

  // electronics map to GEMDetId chamber
  std::map<chamEC, chamDC> chamberMap_;

  std::map<int, std::vector<vfatEC>> chamVfats_;
  std::map<vfatDC, int> chamIEtas_;

  std::map<channelNum, stripNum> chStMap_;
  std::map<stripNum, channelNum> stChMap_;
};
#endif
