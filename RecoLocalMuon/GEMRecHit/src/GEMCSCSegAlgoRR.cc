/**
 * \file GEMCSCSegAlgoMM.cc
 *
 *  \authors: Raffaella Radogna
 */

#include "GEMCSCSegAlgoRR.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

/////////////
// root include files
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
//////////////

/* Constructor
 *
 */
GEMCSCSegAlgoRR::GEMCSCSegAlgoRR(const edm::ParameterSet& ps) : GEMCSCSegmentAlgorithm(ps), myName("GEMCSCSegAlgoRR") {
	 
  debug                     = ps.getUntrackedParameter<bool>("GEMCSCDebug");
  minHitsPerSegment         = ps.getParameter<unsigned int>("minHitsPerSegment");
  preClustering             = ps.getParameter<bool>("preClustering");
  dXclusBoxMax              = ps.getParameter<double>("dXclusBoxMax");
  dYclusBoxMax              = ps.getParameter<double>("dYclusBoxMax");
  preClustering_useChaining = ps.getParameter<bool>("preClusteringUseChaining");
  dPhiChainBoxMax           = ps.getParameter<double>("dPhiChainBoxMax");
  dThetaChainBoxMax         = ps.getParameter<double>("dThetaChainBoxMax");
  dRChainBoxMax             = ps.getParameter<double>("dRChainBoxMax");
  maxRecHitsInCluster       = ps.getParameter<int>("maxRecHitsInCluster");
}


    
/* Destructor
 *
 */
GEMCSCSegAlgoRR::~GEMCSCSegAlgoRR() {
  }


std::vector<GEMCSCSegment> GEMCSCSegAlgoRR::run(GEMCSCEnsamble ensamble, const EnsambleCSCSegContainer& cscsegments, const EnsambleGEMHitContainer& rechits) {

  theEnsamble = ensamble;
    
  std::vector<GEMCSCSegment>          segments_temp;
  std::vector<GEMCSCSegment>          segments;
  EnsambleHitContainer                rechits_chain;
  ProtoSegments                       rechits_clusters; // this is a collection of groups of rechits
    
    
    for (unsigned int s=0; s<cscsegments.size(); ++s){
        rechits_chain = this->chainHitsToSegm(cscsegments[s], rechits );
          segments_temp.clear();
       if(rechits_chain.size()>cscsegments[s]->specificRecHits().size()){
           segments_temp = this->buildSegments(cscsegments[s], rechits_chain);}
       else {
           std::vector<const GEMRecHit*> gemRecHits_noGEMrh;

           GEMCSCSegment tmp(cscsegments[s], gemRecHits_noGEMrh, cscsegments[s]->localPosition(), cscsegments[s]->localDirection(), cscsegments[s]->parametersError(), cscsegments[s]->chi2());
               segments_temp.push_back(tmp);
       }
       
        // add the found subset of segments to the collection of all segments in this chamber:
        segments.insert( segments.end(), segments_temp.begin(), segments_temp.end() );
    }
 
    return segments;
  
  }
  

GEMCSCSegAlgoRR::EnsambleHitContainer
GEMCSCSegAlgoRR::chainHitsToSegm(const CSCSegment* cscsegment, const EnsambleGEMHitContainer & rechits){
  //ProtoSegments rechits_chains;
  //ProtoSegments seeds;

  std::vector <bool> usedCluster;

    EnsambleHitContainer rechits_chain;
    const CSCChamber* chamber = theEnsamble.first;
    auto segLP = cscsegment->localPosition();
    auto segLD = cscsegment->localDirection();
    auto cscrhs = cscsegment->specificRecHits();
    for (auto crh = cscrhs.begin(); crh!= cscrhs.end(); crh++){
        rechits_chain.push_back(crh->clone());
    }
    if(rechits.size()!=0){
        
        float Dphi_min_l1 = 999;
        float Dphi_min_l2 = 999;
        float Dtheta_min_l1 = 999;
        float Dtheta_min_l2 = 999;
        const GEMRecHit* gemrh_min_l1= rechits[0];
        const GEMRecHit* gemrh_min_l2= rechits[0];
        for(unsigned int i = 0; i < rechits.size(); ++i) {


            auto rhLP = rechits[i]->localPosition();
            const GEMEtaPartition* rhRef  = theEnsamble.second[rechits[i]->gemId()];
            auto rhGP = rhRef->toGlobal(rhLP);
            auto rhLP_inSegmRef = chamber->toLocal(rhGP);
            float xe  = segLP.x()+segLD.x()*rhLP_inSegmRef.z()/segLD.z();
            float ye  = segLP.y()+segLD.y()*rhLP_inSegmRef.z()/segLD.z();
            float ze = rhLP_inSegmRef.z();
            LocalPoint extrPoint(xe,ye,ze);
            
            auto rhGP_fromSegmRef = chamber->toGlobal(rhLP_inSegmRef);
            float phi_rh = rhGP_fromSegmRef.phi();
            float theta_rh = rhGP_fromSegmRef.theta();
            auto extrPoinGP_fromSegmRef = chamber->toGlobal(extrPoint);
            float phi_ext = extrPoinGP_fromSegmRef.phi();
            float theta_ext = extrPoinGP_fromSegmRef.theta();
            
           
            if (rechits[i]->gemId().layer()==1){
                float Dphi_l1 = fabs(phi_ext-phi_rh);
                float Dtheta_l1 = fabs(theta_ext-theta_rh);

                if (Dphi_l1 <= Dphi_min_l1){
                    Dphi_min_l1=Dphi_l1;
                    Dtheta_min_l1=Dtheta_l1;
                    gemrh_min_l1=rechits[i];}
                
            }
       

        
            if (rechits[i]->gemId().layer()==2){

                float Dphi_l2 = fabs(phi_ext-phi_rh);
                float Dtheta_l2 = fabs(theta_ext-theta_rh);
          
                if (Dphi_l2 <= Dphi_min_l2){
                    Dphi_min_l2=Dphi_l2;
                    Dtheta_min_l2=Dtheta_l2;
                    gemrh_min_l2=rechits[i];}
  
            }


        }
        bool phiRequirementOK_l1 = Dphi_min_l1 < dPhiChainBoxMax;
        bool thetaRequirementOK_l1 = Dtheta_min_l1 < dThetaChainBoxMax;
        
        if(phiRequirementOK_l1 && thetaRequirementOK_l1){

            rechits_chain.push_back(gemrh_min_l1->clone());
           
        
        }
        bool phiRequirementOK_l2 = Dphi_min_l2 < dPhiChainBoxMax;
        bool thetaRequirementOK_l2 = Dtheta_min_l2 < dThetaChainBoxMax;
        
        if(phiRequirementOK_l2 && thetaRequirementOK_l2){

            rechits_chain.push_back(gemrh_min_l2->clone());
            
        }
    }
    return rechits_chain;
}


std::vector<GEMCSCSegment> GEMCSCSegAlgoRR::buildSegments(const CSCSegment* cscsegment, const EnsambleHitContainer& rechits) {
    std::vector<GEMCSCSegment> gemcsc_segs;
    std::vector<const GEMRecHit*> gemrhs;
    proto_segment.clear();
 
  for (auto rh=rechits.begin(); rh!=rechits.end();rh++){
    proto_segment.push_back(*rh);
    const TrackingRecHit& hit = (**rh);
      if (DetId(hit.rawId()).subdetId() == MuonSubdetId::GEM ){
          gemrhs.push_back(dynamic_cast<const GEMRecHit*>(*rh));
      }
  }
   
  if (proto_segment.size() < minHitsPerSegment){
    return gemcsc_segs;
  }
  // The actual fit on all hit of the protosegments;
  this->doSlopesAndChi2();
  this->fillLocalDirection();
  AlgebraicSymMatrix protoErrors = this->calculateError();
  this->flipErrors( protoErrors );

  GEMCSCSegment tmp(cscsegment, gemrhs, protoIntercept, protoDirection, protoErrors, protoChi2);
  gemcsc_segs.push_back(tmp);
    
  return gemcsc_segs;
}

//Method doSlopesAndChi2
// fitSlopes() and  fillChiSquared() are always called one after the other 
// In fact the code is duplicated in the two functions (as we need 2 loops) - 
// it is much better to fix that at some point 
void GEMCSCSegAlgoRR::doSlopesAndChi2(){
  this->fitSlopes();
  this->fillChiSquared();
}
/* Method fitSlopes
 *
 * Perform a Least Square Fit on a segment as per SK algo
 *
 */

void GEMCSCSegAlgoRR::fitSlopes() {

  CLHEP::HepMatrix M(4,4,0);
  CLHEP::HepVector B(4,0);
  const CSCChamber* ens = theEnsamble.first;
  for (auto ih = proto_segment.begin(); ih != proto_segment.end(); ++ih) {
    const RecHit2DLocalPos& hit = (**ih);
    GlobalPoint gp;
    
    if (DetId(hit.rawId()).subdetId() == MuonSubdetId::GEM ){
        const GEMEtaPartition* roll  = theEnsamble.second[hit.rawId()];
        gp = roll->toGlobal(hit.localPosition());
    }
      
    else if (DetId(hit.rawId()).subdetId() == MuonSubdetId::CSC ){
        const CSCRecHit2D& hit = (*dynamic_cast<const CSCRecHit2D*>(*ih));
        const CSCLayer* layer = ens->layer(CSCDetId(hit.rawId()).layer());
        gp = layer->toGlobal(hit.localPosition());
    }
      
    LocalPoint  lp         = ens->toLocal(gp);
    // ptc: Local position of hit w.r.t. chamber
    double u = lp.x();
    double v = lp.y();
    double z = lp.z();
    // ptc: Covariance matrix of local errors
    CLHEP::HepMatrix IC(2,2);
    IC(1,1) = hit.localPositionError().xx();
    IC(1,2) = hit.localPositionError().xy();
    IC(2,2) = hit.localPositionError().yy();
    IC(2,1) = IC(1,2); // since Cov is symmetric
    // ptc: Invert covariance matrix (and trap if it fails!)
    int ierr = 0;
    IC.invert(ierr); // inverts in place
    if (ierr != 0) {
      LogDebug("GEMCSCSegment|GEMCSC") << "GEMCSCSegment::fitSlopes: failed to invert covariance matrix=\n" << IC;      
            std::cout<< "GEMCSCSegment::fitSlopes: failed to invert covariance matrix=\n" << IC << "\n"<<std::endl;
    }
    
    M(1,1) += IC(1,1);
    M(1,2) += IC(1,2);
    M(1,3) += IC(1,1) * z;
    M(1,4) += IC(1,2) * z;
    B(1)   += u * IC(1,1) + v * IC(1,2);
    
    M(2,1) += IC(2,1);
    M(2,2) += IC(2,2);
    M(2,3) += IC(2,1) * z;
    M(2,4) += IC(2,2) * z;
    B(2)   += u * IC(2,1) + v * IC(2,2);
    
    M(3,1) += IC(1,1) * z;
    M(3,2) += IC(1,2) * z;
    M(3,3) += IC(1,1) * z * z;
    M(3,4) += IC(1,2) * z * z;
    B(3)   += ( u * IC(1,1) + v * IC(1,2) ) * z;
    
    M(4,1) += IC(2,1) * z;
    M(4,2) += IC(2,2) * z;
    M(4,3) += IC(2,1) * z * z;
    M(4,4) += IC(2,2) * z * z;
    B(4)   += ( u * IC(2,1) + v * IC(2,2) ) * z;
  }
  CLHEP::HepVector p = solve(M, B);
  // Update member variables
  // Note that origin has local z = 0
  protoIntercept = LocalPoint(p(1), p(2), 0.);
  protoSlope_u = p(3);
  protoSlope_v = p(4);
    
}
/* Method fillChiSquared
 *
 * Determine Chi^2 for the proto wire segment
 *
 */
void GEMCSCSegAlgoRR::fillChiSquared() {
  double chsq = 0.;
  const CSCChamber* ens = theEnsamble.first;
    
  for (auto ih = proto_segment.begin(); ih != proto_segment.end(); ++ih) {
    const RecHit2DLocalPos& hit = (**ih);
    GlobalPoint gp;
      
    if (DetId(hit.rawId()).subdetId() == MuonSubdetId::GEM ){
          const GEMEtaPartition* roll  = theEnsamble.second[hit.rawId()];
          gp = roll->toGlobal(hit.localPosition());
    }
      
    else if (DetId(hit.rawId()).subdetId() == MuonSubdetId::CSC ){
          const CSCRecHit2D& hit = (*dynamic_cast<const CSCRecHit2D*>(*ih));
          const CSCLayer* layer = ens->layer(CSCDetId(hit.rawId()).layer());
          gp = layer->toGlobal(hit.localPosition());
    }

    LocalPoint  lp         = ens->toLocal(gp); 
    // ptc: Local position of hit w.r.t. chamber
    double u = lp.x();
    double v = lp.y();
    double z = lp.z();
    
    double du = protoIntercept.x() + protoSlope_u * z - u;
    double dv = protoIntercept.y() + protoSlope_v * z - v;


    CLHEP::HepMatrix IC(2,2);
    IC(1,1) = hit.localPositionError().xx();
    //    IC(1,1) = hit.localPositionError().xx();
    IC(1,2) = hit.localPositionError().xy();
    IC(2,2) = hit.localPositionError().yy();
    IC(2,1) = IC(1,2);
    
    // Invert covariance matrix
    int ierr = 0;
    IC.invert(ierr);
    if (ierr != 0) {
      LogDebug("GEMCSCSegment|GEMCSC") << "GEMCSCSegment::fillChiSquared: failed to invert covariance matrix=\n" << IC;
             std::cout << "GEMCSCSegment::fillChiSquared: failed to invert covariance matrix=\n" << IC << "\n";
      
    }
    
    chsq += du*du*IC(1,1) + 2.*du*dv*IC(1,2) + dv*dv*IC(2,2);
  }

  protoChi2 = chsq;
  protoNDF = 2.*proto_segment.size() - 4;
    
}
/* fillLocalDirection
 *
 */
void GEMCSCSegAlgoRR::fillLocalDirection() {
  // Always enforce direction of segment to point from IP outwards
  // (Incorrect for particles not coming from IP, of course.)
  
  double dxdz = protoSlope_u;
  double dydz = protoSlope_v;
  double dz   = 1./sqrt(1. + dxdz*dxdz + dydz*dydz);
  double dx   = dz*dxdz;
  double dy   = dz*dydz;
  LocalVector localDir(dx,dy,dz);
  
  // localDir may need sign flip to ensure it points outward from IP
  // ptc: Examine its direction and origin in global z: to point outward
  // the localDir should always have same sign as global z...
  const CSCChamber* ens = theEnsamble.first;

  double globalZpos    = ( ens->toGlobal( protoIntercept ) ).z();
  double globalZdir    = ( ens->toGlobal( localDir ) ).z();
  double directionSign = globalZpos * globalZdir;
  protoDirection       = (directionSign * localDir).unit();
}

/* weightMatrix
 *   
 */
AlgebraicSymMatrix GEMCSCSegAlgoRR::weightMatrix() {
  
  std::vector<const RecHit2DLocalPos*>::const_iterator it;
  int nhits = proto_segment.size();
  AlgebraicSymMatrix matrix(2*nhits, 0);
  int row = 0;
  
  for (it = proto_segment.begin(); it != proto_segment.end(); ++it) {
    
    const RecHit2DLocalPos& hit = (**it);
    ++row;
    matrix(row, row)   = protoChiUCorrection*hit.localPositionError().xx();
    matrix(row, row+1) = hit.localPositionError().xy();
    ++row;
    matrix(row, row-1) = hit.localPositionError().xy();
    matrix(row, row)   = hit.localPositionError().yy();
  }
  int ierr;
  matrix.invert(ierr);
  return matrix;
}


/* derivativeMatrix
 *
 */
CLHEP::HepMatrix GEMCSCSegAlgoRR::derivativeMatrix(){
  
  int nhits = proto_segment.size();
  CLHEP::HepMatrix matrix(2*nhits, 4);
  int row = 0;
  
  const CSCChamber* ens = theEnsamble.first;

  for (auto ih = proto_segment.begin(); ih != proto_segment.end(); ++ih) {
    const RecHit2DLocalPos& hit = (**ih);
      
      GlobalPoint gp;
      
      if (DetId(hit.rawId()).subdetId() == MuonSubdetId::GEM ){
          const GEMEtaPartition* roll  = theEnsamble.second[hit.rawId()];
          gp = roll->toGlobal(hit.localPosition());
      }
      
      else if (DetId(hit.rawId()).subdetId() == MuonSubdetId::CSC ){
          const CSCRecHit2D& hit = (*dynamic_cast<const CSCRecHit2D*>(*ih));
          const CSCLayer* layer = ens->layer(CSCDetId(hit.rawId()).layer());
          gp = layer->toGlobal(hit.localPosition());
      }
      
      LocalPoint  lp         = ens->toLocal(gp);

    float z = lp.z();
    ++row;
    matrix(row, 1) = 1.;
    matrix(row, 3) = z;
    ++row;
    matrix(row, 2) = 1.;
    matrix(row, 4) = z;
  }
  return matrix;
}

/* calculateError*/
AlgebraicSymMatrix GEMCSCSegAlgoRR::calculateError(){


  AlgebraicSymMatrix weights = this->weightMatrix();
  AlgebraicMatrix A = this->derivativeMatrix();
  // (AT W A)^-1
  // from http://www.phys.ufl.edu/~avery/fitting.html, part I                                                                                                            
  int ierr;
  AlgebraicSymMatrix result = weights.similarityT(A);
  result.invert(ierr);
    if (ierr != 0) {
    
        
    }
  // blithely assuming the inverting never fails...                                                                                                                      
  return result;
}

void GEMCSCSegAlgoRR::flipErrors( AlgebraicSymMatrix& a ) { 
        
  AlgebraicSymMatrix hold( a ); 
    
  // errors on slopes into upper left 
  a(1,1) = hold(3,3); 
  a(1,2) = hold(3,4); 
  a(2,1) = hold(4,3); 
  a(2,2) = hold(4,4); 
    
  // errors on positions into lower right 
  a(3,3) = hold(1,1); 
  a(3,4) = hold(1,2); 
  a(4,3) = hold(2,1); 
  a(4,4) = hold(2,2); 
    
  // must also interchange off-diagonal elements of off-diagonal 2x2 submatrices
  a(4,1) = hold(2,3);
  a(3,2) = hold(1,4);
  a(2,3) = hold(4,1); // = hold(1,4)
  a(1,4) = hold(3,2); // = hold(2,3)
} 


