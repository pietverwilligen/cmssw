
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
#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 


GEMCSCSegmentBuilder::GEMCSCSegmentBuilder(const edm::ParameterSet& ps) : gemgeom_(0), cscgeom_(0) {

    // Algo name
    std::string algoName = ps.getParameter<std::string>("algo_name");
        
    LogDebug("GEMCSCSegment|GEMCSC")<< "GEMCSCSegmentBuilder algorithm name: " << algoName;
    std::cout<<"[GEMCSCSegmentBuilder ::constructor] --> GEMCSCSegmentBuilder algorithm name: "<<algoName<<std::endl;

    // SegAlgo parameter set 	  
    edm::ParameterSet segAlgoPSet = ps.getParameter<edm::ParameterSet>("algo_psets");
 
    // Ask factory to build this algorithm, giving it appropriate ParameterSet
    algo = GEMCSCSegmentBuilderPluginFactory::get()->create(algoName, segAlgoPSet);

}

GEMCSCSegmentBuilder::~GEMCSCSegmentBuilder() {
    delete algo;
}

void GEMCSCSegmentBuilder::setSetup(const GEMGeometry* gemGeo, const CSCGeometry* cscGeo) {
  // Debug
  std::cout<<"[GEMCSCSegmentBuilder::setSetup]"<<std::endl;
  // ------------------------------------------------------
  for (TrackingGeometry::DetContainer::const_iterator it=gemGeo->dets().begin();it<gemGeo->dets().end();it++){
    if(dynamic_cast< const GEMChamber* >( *it ) != 0 ){
      const GEMChamber* ch = dynamic_cast< const GEMChamber* >( *it );
      std::vector< const GEMEtaPartition*> rolls = (ch->etaPartitions());
      for(std::vector<const GEMEtaPartition*>::const_iterator r = rolls.begin();r != rolls.end(); ++r){
        GEMDetId gemId = (*r)->id();
        int region=gemId.region();
        if(region!=0){
          int station=gemId.station();
          int ring=gemId.ring();
          int gemchamber=gemId.chamber();
          int layer=gemId.layer();
          int cscring=ring;
          int cscstation=station;
          int cscchamber = gemchamber;
          int csclayer = layer;
          CSCStationIndex ind(region,cscstation,cscring,cscchamber,csclayer);
	  std::set<GEMDetId> myrolls;
          if (rollstoreCSC.find(ind)!=rollstoreCSC.end()) myrolls=rollstoreCSC[ind];
          myrolls.insert(gemId);
          rollstoreCSC[ind]=myrolls;
	  // Print out for checking
	  // std::cout<<"CSC Station Index :: ["<<region<<","<<cscstation<<","<<cscring<<","<<cscchamber<<","<<csclayer<<"] has following GEM rolls: ";
	  // for(std::set<int>::iterator it=myrolls.begin(); it!=myrolls.end(); ++it) { std::cout<<"GEM Id: "<<*it; }
	  // std::cout<<" "<<std::endl;
        }
      }
    }
  }
}


void GEMCSCSegmentBuilder::build(const GEMRecHitCollection* recHits, const CSCSegmentCollection* cscsegments, const edm::EventSetup& setup, GEMCSCSegmentCollection& oc) {
  	
    LogDebug("GEMCSCSegment|GEMCSC")<< "Total number of rechits in this event: " << recHits->size()<< " Total number of csc segments" << cscsegments->size();
    std::cout<<"[GEMCSCSegmentBuilder :: build] --> Total number of rechits in this event: " << recHits->size()<< " Total number of csc segments" << cscsegments->size()<<std::endl;

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
      // Search for Matches between GEM Roll and CSC Chamber
      if(CSCId.station()==1 && (CSCId.ring()==1 || CSCId.ring()==4)) {   // ring=1 ==> ME1/1b; ring=4 ==> ME1/1a
	std::cout<<"[GEMCSCSegmentBuilder :: build] --> looping over ME1/1 CSC segments :: ME1/1 Segment in "<<CSCId<<std::endl;
        std::vector<const CSCSegment* > ss = ensembleS[CSCId.rawId()];
        ss.push_back(segm->clone());
        ensembleS[CSCId.rawId()]=ss;
    
        int cscRegion = CSCId.endcap();
        int gemRegion = 1; if(cscRegion==2) gemRegion= -1; // +/- Endcaps :: CSC Endcaps are numbered 1,2; GEM Endcaps are numbered +1, -1
	// int cscRing = CSCId.ring();
	int gemRing = 1;                                   // this we can hardcode, only GEMs in Ring 1
        int cscStation = CSCId.station();
        int gemStation = cscStation;
	int cscChamber = CSCId.chamber();
        // int gemChamber = cscChamber;
	// Continue cleanup and printout from here
        // ObjectMapCSC* TheObjectCSC = ObjectMapCSC::GetInstance(setup);
        // ObjectMapCSC* TheObjectCSC = ObjectMapCSC::GetInstance(gemGeo, cscGeo);

        // CSCStationIndex theindex(gemRegion,gemStation,gemRing,gemChamber,1); // n rolls in layer1 == n rolls in layer 2
	// check gemRegion = 1 and gemRegion = -1 ==> dont we have to use CSCRegion ??? ==> no, it is ok!
	/*
	CSCStationIndex theindex0(-1,gemStation,gemRing,gemChamber,1);  std::set<GEMDetId> rollsForThisCSC0 = TheObjectCSC->GetInstance(setup)->GetRolls(theindex0);        
	CSCStationIndex theindex1(+1,gemStation,gemRing,gemChamber,1);  std::set<GEMDetId> rollsForThisCSC1 = TheObjectCSC->GetInstance(setup)->GetRolls(theindex1);        
	CSCStationIndex theindex2(+2,gemStation,gemRing,gemChamber,1);  std::set<GEMDetId> rollsForThisCSC2 = TheObjectCSC->GetInstance(setup)->GetRolls(theindex2);        
	std::cout<<"Test CSC Index :: re:"<<-1<<" st:"<<gemStation<<" ri:"<<gemRing<<" ch:"<<gemChamber<<" index = "<<&theindex0; 
	for (std::set<GEMDetId>::iterator iteraRoll = rollsForThisCSC0.begin();iteraRoll != rollsForThisCSC0.end(); ++iteraRoll){ std::cout<<" id: "<<*iteraRoll; } std::cout<<""<<std::endl;
	std::cout<<"Test CSC Index :: re:"<<+1<<" st:"<<gemStation<<" ri:"<<gemRing<<" ch:"<<gemChamber<<" index = "<<&theindex1;
	for (std::set<GEMDetId>::iterator iteraRoll = rollsForThisCSC1.begin();iteraRoll != rollsForThisCSC1.end(); ++iteraRoll){ std::cout<<" id: "<<*iteraRoll; } std::cout<<""<<std::endl;
	std::cout<<"Test CSC Index :: re:"<<+2<<" st:"<<gemStation<<" ri:"<<gemRing<<" ch:"<<gemChamber<<" index = "<<&theindex2;
	for (std::set<GEMDetId>::iterator iteraRoll = rollsForThisCSC2.begin();iteraRoll != rollsForThisCSC2.end(); ++iteraRoll){ std::cout<<" id: "<<*iteraRoll; } std::cout<<""<<std::endl;
	/*/

	int gem1stChamber = cscChamber;
	int gem2ndChamber = gem1stChamber+1; if(gem2ndChamber>36) gem2ndChamber-=36;
	int gem3rdChamber = gem1stChamber-1; if(gem2ndChamber<1)  gem2ndChamber+=36;

	// Make something smart that you are also looking for hits in overlap region
        CSCStationIndex index11(gemRegion,gemStation,gemRing,gem1stChamber,1);         CSCStationIndex index12(gemRegion,gemStation,gemRing,gem1stChamber,2); 
        CSCStationIndex index21(gemRegion,gemStation,gemRing,gem2ndChamber,1);         CSCStationIndex index22(gemRegion,gemStation,gemRing,gem2ndChamber,2); 
        CSCStationIndex index31(gemRegion,gemStation,gemRing,gem3rdChamber,1);         CSCStationIndex index32(gemRegion,gemStation,gemRing,gem3rdChamber,2); 
	std::vector<CSCStationIndex> indexvector; 
	indexvector.push_back(index11); indexvector.push_back(index12); indexvector.push_back(index21); 
	indexvector.push_back(index22); indexvector.push_back(index31); indexvector.push_back(index32); 

        // std::set<GEMDetId> rollsForThisCSC = TheObjectCSC->GetInstance(setup)->GetRolls(theindex);        
	std::cout<<"indexvector size = "<<indexvector.size()<<std::endl;
	std::vector<GEMDetId> rollsForThisCSCvector; 
	for(unsigned int i=0; i<indexvector.size(); ++i) {
	  // std::set<GEMDetId> rollsForThisCSC = TheObjectCSC->GetInstance(setup)->GetRolls(indexvector[i]); // in original file this one is not filled .... come mai?
	  std::set<GEMDetId> rollsForThisCSC = rollstoreCSC[indexvector[i]]; 
	  std::cout<<"gemrolls size = "<<rollsForThisCSC.size()<<std::endl; // <<" more information :: setup = "<<&setup<<" TheObjectCSC = "<<&TheObjectCSC<<" --> GetInstance(setup) = "<<(TheObjectCSC->GetInstance(setup))<<std::endl;
	  for (std::set<GEMDetId>::iterator itRoll = rollsForThisCSC.begin();itRoll != rollsForThisCSC.end(); ++itRoll){rollsForThisCSCvector.push_back(*itRoll);}
	}


	// Loop over GEM Rechits
	std::cout<<"Start Loop over GEM Rechits :: size = "<<recHits->size()<<" and number of Rolls for this CSC :: "<<rollsForThisCSCvector.size()<<std::endl;
        for(GEMRecHitCollection::const_iterator it2 = recHits->begin(); it2 != recHits->end(); ++it2) {
	  // std::cout<<"GEM Rechit found in "<<it2->gemId()<<std::endl;
	  // Loop over GEM rolls being pointed by a CSC segment and look for a match
	  // for (std::set<GEMDetId>::iterator iteraRoll = rollsForThisCSC.begin();iteraRoll != rollsForThisCSC.end(); ++iteraRoll){
	  for (std::vector<GEMDetId>::iterator iteraRoll = rollsForThisCSCvector.begin();iteraRoll != rollsForThisCSCvector.end(); ++iteraRoll){
	    const GEMEtaPartition* rollasociated = gemGeo->etaPartition(*iteraRoll);
	    GEMDetId gemIdfromSegm = rollasociated->id();
	    // GEMDetId gemIdfromCSC(*iteraRoll);
	    // std::cout<<"GEM Id :: "<<gemIdfromCSC<<" with RawId: "<<gemIdfromCSC.rawId()<<std::endl;
	    // if(it2->gemId().roll()== gemIdfromSegm.roll() && it2->gemId().chamber()==CSCId.chamber() && ((it2->gemId().region()==1 && CSCId.endcap()==1 )||(it2->gemId().region()==-1 && CSCId.endcap()==2 ))){ //NEW match chamber && endcap
	    if(it2->gemId().rawId() == gemIdfromSegm.rawId()) { 
	      std::vector<GEMRecHit* > pp = ensembleRH[CSCId.rawId()];
	      pp.push_back(it2->clone());
	      ensembleRH[CSCId.rawId()]=pp;
	      std::cout<<"GEM Rechit in "<<it2->gemId()<< "["<<it2->gemId().rawId()<<"] added to CSC segment found in "<<CSCId<<" ["<<CSCId.rawId()<<"]"<<std::endl;	      
	    }
	  }// for roll of the segment
	}// for rec hits
    }// if ME1/1b
    
  else if(!(CSCId.station()==1 && CSCId.ring()==1 )) {
      std::vector<const CSCSegment* > ss_noME1b = ensembleS_noME1b[CSCId.rawId()];
      ss_noME1b.push_back(segm->clone());
      ensembleS_noME1b[CSCId.rawId()]=ss_noME1b;

  }
  // std::cout<<"caso NO ME1b ensembleRH size: "<<ensembleRH[CSCId.rawId()].size()<<std::endl;// debug
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


