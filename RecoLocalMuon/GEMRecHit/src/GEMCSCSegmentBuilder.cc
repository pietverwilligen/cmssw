
/** \file GEMCSCSegmentBuilder.cc
 *
 *
 */

#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentBuilder.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/MuonDetId/interface/GEMDetId.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <DataFormats/CSCRecHit/interface/CSCRangeMapAccessor.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>


#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentAlgorithm.h>
#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentBuilderPluginFactory.h>
#include <RecoLocalMuon/GEMRecHit/interface/CSCSegtoGEM.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 



GEMCSCSegmentBuilder::GEMCSCSegmentBuilder(const edm::ParameterSet& ps) : gemgeom_(0), cscgeom_(0) {

    // Algo name
    std::string algoName = ps.getParameter<std::string>("algo_name");
        
    LogDebug("GEMCSCSegment|GEMCSC")<< "GEMCSCSegmentBuilder algorithm name: " << algoName;

    // SegAlgo parameter set 	  
    edm::ParameterSet segAlgoPSet = ps.getParameter<edm::ParameterSet>("algo_psets");
 
    // Ask factory to build this algorithm, giving it appropriate ParameterSet
    algo = GEMCSCSegmentBuilderPluginFactory::get()->create(algoName, segAlgoPSet);

}

GEMCSCSegmentBuilder::~GEMCSCSegmentBuilder() {
    delete algo;
}

void GEMCSCSegmentBuilder::build(const GEMRecHitCollection* recHits, const CSCSegmentCollection* cscsegments, const edm::EventSetup& setup, GEMCSCSegmentCollection& oc) {
  	
    LogDebug("GEMCSCSegment|GEMCSC")<< "Total number of rechits in this event: " << recHits->size()<< "Total number of csc segments" << cscsegments->size();

    std::vector<CSCDetId> chambers;
    std::vector<CSCDetId>::const_iterator chIt;
    edm::ESHandle<GEMGeometry> gemGeo;
    edm::ESHandle<CSCGeometry> cscGeo;
  
    setup.get<MuonGeometryRecord>().get(gemGeo);
    setup.get<MuonGeometryRecord>().get(cscGeo);
    
    std::map<uint32_t, std::vector<GEMRecHit*> > ensembleRH;
    std::map<uint32_t, std::vector<const CSCSegment*> > ensembleS;
    std::map<uint32_t, std::vector<const CSCSegment*> > ensembleS_noME1b;
    
    
for(CSCSegmentCollection::const_iterator segm = cscsegments->begin(); segm != cscsegments->end(); segm++) {   
    CSCDetId CSCId = segm->cscDetId();
    
    if(CSCId.station()==1 && CSCId.ring()==1 ){

        std::vector<const CSCSegment* > ss = ensembleS[CSCId.rawId()];
        ss.push_back(segm->clone());
        ensembleS[CSCId.rawId()]=ss;
    
        int cscEndCap = CSCId.endcap();
        int cscStation = CSCId.station();
        int cscRing = CSCId.ring();
        int gemRegion = 1; if(cscEndCap==2) gemRegion= -1;//Relacion entre las endcaps
        int gemRing = cscRing;
        //if(cscRing==4)gemRing =1; ////csc ring=4 è me1A, mentre ring 1 è me1B, gemring=1
        int gemStation = cscStation;
        int gemChamber = CSCId.chamber();
        ObjectMapCSC* TheObjectCSC = ObjectMapCSC::GetInstance(setup);
        CSCStationIndex theindex(gemRegion,gemStation,gemRing,gemChamber,1);// n rolls in layer1 == n rolls in layer 2
        std::set<GEMDetId> rollsForThisCSC = TheObjectCSC->GetInstance(setup)->GetRolls(theindex);
        
        for(GEMRecHitCollection::const_iterator it2 = recHits->begin(); it2 != recHits->end(); it2++) {
        for (std::set<GEMDetId>::iterator iteraRoll = rollsForThisCSC.begin();iteraRoll != rollsForThisCSC.end(); iteraRoll++){
           const GEMEtaPartition* rollasociated = gemGeo->etaPartition(*iteraRoll);
           GEMDetId gemIdfromSegm = rollasociated->id();
           if(it2->gemId().roll()== gemIdfromSegm.roll() && it2->gemId().chamber()==CSCId.chamber() && ((it2->gemId().region()==1 && CSCId.endcap()==1 )||(it2->gemId().region()==-1 && CSCId.endcap()==2 ))){ //NEW match chamber && endcap
               
               std::vector<GEMRecHit* > pp = ensembleRH[CSCId.rawId()];
               pp.push_back(it2->clone());
               ensembleRH[CSCId.rawId()]=pp;
		
    	}
    }// for roll of the segment
  }// for rec hits
  }// if ME1/1b
    
  else if(!(CSCId.station()==1 && CSCId.ring()==1 )) {
      std::vector<const CSCSegment* > ss_noME1b = ensembleS_noME1b[CSCId.rawId()];
      ss_noME1b.push_back(segm->clone());
      ensembleS_noME1b[CSCId.rawId()]=ss_noME1b;

  }
 std::cout<<"caso NO ME1b ensembleRH size: "<<ensembleRH[CSCId.rawId()].size()<<std::endl;// debug
}// for csc segments
    
for(auto enIt=ensembleS_noME1b.begin(); enIt != ensembleS_noME1b.end(); enIt++) {
    CSCDetId ch_id(enIt->first);
    if(ch_id.station()==1 && ch_id.ring()==1 )continue;
    std::vector<const GEMRecHit*> gemRecHits_noME1b;
	std::vector<const CSCSegment*> cscSegments_noME1b = ensembleS_noME1b[enIt->first];
    std::vector<GEMCSCSegment> segv2;

    for (unsigned int s=0; s<cscSegments_noME1b.size(); ++s){
    GEMCSCSegment tmp(cscSegments_noME1b[s], gemRecHits_noME1b, cscSegments_noME1b[s]->localPosition(), cscSegments_noME1b[s]->localDirection(), cscSegments_noME1b[s]->parametersError(), cscSegments_noME1b[s]->chi2());
    segv2.push_back(tmp);
       
    }
    
    CSCDetId mid(enIt->first);
    oc.put(mid, segv2.begin(), segv2.end());
}
    
for(auto enIt=ensembleRH.begin(); enIt != ensembleRH.end(); enIt++) {
    CSCDetId ch_id(enIt->first);
    if(!(ch_id.station()==1 && ch_id.ring()==1 ))continue;

    std::vector<const GEMRecHit*> gemRecHits;
	std::vector<const CSCSegment*> cscSegments = ensembleS[enIt->first];
    std::map<uint32_t,const GEMEtaPartition* > ens;
    const CSCChamber* cscChamber = cscgeom_->chamber(enIt->first);

   for(auto rechit = enIt->second.begin(); rechit != enIt->second.end(); rechit++) {
        gemRecHits.push_back(*rechit);
        ens[(*rechit)->gemId()]=gemgeom_->etaPartition((*rechit)->gemId());
    }
    GEMCSCSegmentAlgorithm::GEMCSCEnsamble ensamble(std::pair<const CSCChamber*, std::map<uint32_t,const GEMEtaPartition* > >(cscChamber,ens));

    std::vector<GEMCSCSegment> segv = algo->run(ensamble, cscSegments, gemRecHits);
    CSCDetId mid(enIt->first);
    LogDebug("GEMSegment|GEM") << "found " << segv.size() << " segments in chamber " << mid;
    
    // Add the segments to master collection
    oc.put(mid, segv.begin(), segv.end());
    }
}


void GEMCSCSegmentBuilder::setGeometry(const GEMGeometry* gemgeom, const CSCGeometry* cscgeom) {
	gemgeom_ = gemgeom;
	cscgeom_ = cscgeom;
}


