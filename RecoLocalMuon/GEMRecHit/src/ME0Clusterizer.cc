#include "ME0Clusterizer.h"
#include "ME0Cluster.h"
#include "ME0ClusterContainer.h"


ME0Clusterizer::ME0Clusterizer()
{
}

ME0Clusterizer::~ME0Clusterizer()
{
}
 
ME0ClusterContainer
ME0Clusterizer::doAction(const ME0Digi2DCollection::Range& digiRange, double timeres, bool recOnlyMuons, bool recOnlyPrompt){
  ME0ClusterContainer cls;
  for (ME0Digi2DCollection::const_iterator digi = digiRange.first;
       digi != digiRange.second;
       digi++) {
    // do not cluster prompt background hits / non-prompt background hits upon request
    if (recOnlyMuons && std::abs(digi->pdgid()) != 13)  continue;
    if (recOnlyPrompt && !digi->prompt()) continue;
    ME0Cluster cl(digi->stripx(),digi->stripx(),digi->stripy(), digi->tof());
    cls.insert(cl);
  }
  ME0ClusterContainer clsNew =this->doActualAction(cls, timeres);
  return clsNew;
}

ME0ClusterContainer
ME0Clusterizer::doActualAction(ME0ClusterContainer& initialclusters, double timeres){
  
  ME0ClusterContainer finalCluster;
  ME0Cluster prev;

  unsigned int j = 0;
  for(ME0ClusterContainer::const_iterator i=initialclusters.begin();
      i != initialclusters.end(); i++){
    ME0Cluster cl = *i;

    if(i==initialclusters.begin()){
      prev = cl;
      j++;
      if(j == initialclusters.size()){
	finalCluster.insert(prev);
      }
      else if(j < initialclusters.size()){
	continue;
      }
    }

    if(prev.isAdjacent(cl, timeres)) {
      prev.merge(cl, timeres);
      j++;
      if(j == initialclusters.size()){
	finalCluster.insert(prev);
      }
    }
    else {
      j++;
      if(j < initialclusters.size()){
	finalCluster.insert(prev);
	prev = cl;
      }
      if(j == initialclusters.size()){
	finalCluster.insert(prev);
	finalCluster.insert(cl);
      }
    }
  }

  return finalCluster;
} 
 

