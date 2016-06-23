#ifndef RecoLocalMuon_ME0Clusterizer_h
#define RecoLocalMuon_ME0Clusterizer_h
/** \class ME0Clusterizer
 *  $Date: 2013/04/24 17:16:34 $
 *  $Revision: 1.1 $
 *  \author M. Maggi -- INFN Bari
 */

#include "ME0ClusterContainer.h"
#include "DataFormats/GEMDigi/interface/ME0Digi2DCollection.h"

class ME0Cluster;
class ME0Clusterizer{
 public:
  ME0Clusterizer();
  ~ME0Clusterizer();
  ME0ClusterContainer doAction(const ME0Digi2DCollection::Range& digiRange, double timeres, bool recOnlyMuon, bool recOnlyPrompt);

 private:
  ME0ClusterContainer doActualAction(ME0ClusterContainer& initialclusters, double timeres);

 private:
  ME0ClusterContainer cls;
};
#endif
