#ifndef RecoLocalMuon_ME0Cluster_h
#define RecoLocalMuon_ME0Cluster_h
#include <boost/cstdint.hpp>
class ME0Cluster{
 public:
  ME0Cluster();
  ME0Cluster(int fs,int ls, int ys, double cltime);
  ~ME0Cluster();

  int firstStrip() const;
  int lastStrip() const;
  int etaPartition() const;
  int clusterSize() const;
  double tof() const;

  void merge(const ME0Cluster& cl, double timeres);

  bool operator<(const ME0Cluster& cl) const;
  bool operator==(const ME0Cluster& cl) const;
  bool isAdjacent(const ME0Cluster& cl, double timeres) const;

 private:
  uint16_t fstrip;
  uint16_t lstrip;
  uint16_t ystrip;
  double   cltime;
};
#endif
