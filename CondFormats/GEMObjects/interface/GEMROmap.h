#ifndef CondFormats_GEMObjects_GEMROmap_h
#define CondFormats_GEMObjects_GEMROmap_h
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include <map>

class GEMROmap {
public:
  struct eCoord {
    uint16_t amcId;
    uint16_t gebId;
    bool operator<(const eCoord& r) const {
      if (amcId == r.amcId) {
          return gebId < r.gebId;
      } else {
        return amcId < r.amcId;
      }
    }
  };

  struct dCoord {
    int chamberType;
    GEMDetId gemDetId;
    bool operator<(const dCoord& r) const {
      if (chamberType == r.chamberType) {
        return gemDetId < r.gemDetId;
      } else {
        return chamberType < r.chamberType;
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

  GEMROmap(){};

  bool isValidChipID(const eCoord& r) const { return roMapED_.find(r) != roMapED_.end(); }
  const dCoord& hitPosition(const eCoord& r) const { return roMapED_.at(r); }
  const eCoord& hitPosition(const dCoord& r) const { return roMapDE_.at(r); }

  void add(eCoord e, dCoord d) { roMapED_[e] = d; }
  void add(dCoord d, eCoord e) { roMapDE_[d] = e; }

  const std::map<eCoord, dCoord>* getRoMap() const { return &roMapED_; }

  void add(channelNum c, stripNum s) { chStMap_[c] = s; }
  void add(stripNum s, channelNum c) { stChMap_[s] = c; }

  const channelNum& hitPosition(const stripNum& s) const { return stChMap_.at(s); }
  const stripNum& hitPosition(const channelNum& c) const { return chStMap_.at(c); }

private:
  std::map<eCoord, dCoord> roMapED_;
  std::map<dCoord, eCoord> roMapDE_;

  std::map<channelNum, stripNum> chStMap_;
  std::map<stripNum, channelNum> stChMap_;
};
#endif
