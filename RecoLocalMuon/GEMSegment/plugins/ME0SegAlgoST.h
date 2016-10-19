#ifndef ME0Segment_ME0SegAlgoST_h
#define ME0Segment_ME0SegAlgoST_h

/**
 * \class ME0SegAlgoST is derived by CSCSegAlgoST
 *     from  S. Stoynev  - NWU
 *           I. Bloch    - FNAL
 *           E. James    - FNAL
 *           A. Sakharov - WSU (extensive revision to handle weird segments)
 *           T. Cox      - UC Davis (struggling to handle this monster)
 * This algorithm is based on the Minimum Spanning Tree (ST) approach 
 * for building endcap muon track segments out of the rechit's in a ME0Chamber.<BR>
 *
 * A ME0Segment is a RecSegment4D, and is built from
 * ME0RecHit objects, each of which is a RecHit2DLocalPos. <BR>
 *
 * This builds segments consisting of at least 3 hits.
 * Segments can share a common rechit, but only one. 
 * Check how to deal with more shared hits...
 * 
 *  \authors M. Maggi  - Bari
 *
 */

#include <RecoLocalMuon/GEMSegment/plugins/ME0SegmentAlgorithmBase.h>
#include <DataFormats/GEMRecHit/interface/ME0RecHit.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <deque>
#include <vector>

class ME0SegAlgoST : public ME0SegmentAlgorithmBase {


public:

  /// Typedefs

  typedef std::vector<const ME0RecHit*> EnsembleHitContainer;
  typedef std::vector < EnsembleHitContainer > ProtoSegments;
  typedef std::deque<bool> BoolContainer;

  /// Constructor
  explicit ME0SegAlgoST(const edm::ParameterSet& ps);

  /// Destructor
  virtual ~ME0SegAlgoST();

  /**
   * Build track segments in this chamber (this is where the actual
   * segment-building algorithm hides.)
   */
  std::vector<ME0Segment> buildSegments(const ME0Ensemble& ensemble, const EnsembleHitContainer& rechits);

  /**
   * Build track segments in this chamber (this is where the actual
   * segment-building algorithm hides.)
   */
  //  std::vector<ME0Segment> buildSegments2(const ChamberHitContainer& rechits);

  /**
   * Build segments for all desired groups of hits
   */
  std::vector<ME0Segment> run(const ME0Ensemble& ensemble, const EnsembleHitContainer& rechits); 

  /**
   * Build groups of rechits that are separated in x and y to save time on the segment finding
   */
  ProtoSegments clusterHits(const ME0Ensemble& ensemble, const EnsembleHitContainer & rechits);



  /**
   * Remove bad hits from found segments based not only on chi2, but also on charge and 
   * further "low level" chamber information.
   */
  std::vector< ME0Segment > prune_bad_hits(const ME0Ensemble& ensemble, std::vector< ME0Segment > & segments);

private:

  // Retrieve pset
  const edm::ParameterSet& pset(void) const { return ps_;}

  // Adjust covariance matrix?
  bool adjustCovariance(void) { return adjustCovariance_;}

  /// Utility functions 
  double theWeight(double coordinate_1, double coordinate_2, double coordinate_3, float layer_1, float layer_2, float layer_3);

  void ChooseSegments(void);

  // Return the segment with the smallest weight
  void ChooseSegments2a(ProtoSegments & best_segments, int best_seg);
  // Version of ChooseSegments for the case without fake hits
  void ChooseSegments2(int best_seg);

  // Choose routine with reduce nr of loops
  void ChooseSegments3(int best_seg);
  void ChooseSegments3(ProtoSegments & best_segments, std::vector< float > & best_weight, int best_seg);
  //

  void dumpSegment( const ME0Segment& seg ) const;

  // Member variables
  const std::string myName_; 
  const edm::ParameterSet ps_;

  ProtoSegments GoodSegments;

  EnsembleHitContainer PAhits_onLayer[6];
  EnsembleHitContainer Psegments_hits;

  ProtoSegments Psegments;
  ProtoSegments Psegments_noLx;
  ProtoSegments Psegments_noL1;
  ProtoSegments Psegments_noL2;
  ProtoSegments Psegments_noL3;
  ProtoSegments Psegments_noL4;
  ProtoSegments Psegments_noL5;
  ProtoSegments Psegments_noL6;
  ProtoSegments chosen_Psegments;
  std::vector< float > weight_A;
  std::vector< float > weight_noLx_A;
  std::vector< float > weight_noL1_A;
  std::vector< float > weight_noL2_A;
  std::vector< float > weight_noL3_A;
  std::vector< float > weight_noL4_A;
  std::vector< float > weight_noL5_A;
  std::vector< float > weight_noL6_A;
  std::vector< float > chosen_weight_A;
  std::vector< float > curv_A;
  std::vector< float > curv_noL1_A;
  std::vector< float > curv_noL2_A;
  std::vector< float > curv_noL3_A;
  std::vector< float > curv_noL4_A;
  std::vector< float > curv_noL5_A;
  std::vector< float > curv_noL6_A;
  std::vector< float > weight_B;
  std::vector< float > weight_noL1_B;
  std::vector< float > weight_noL2_B;
  std::vector< float > weight_noL3_B;
  std::vector< float > weight_noL4_B;
  std::vector< float > weight_noL5_B;
  std::vector< float > weight_noL6_B;

  EnsembleHitContainer protoSegment;

  // input from .cfi file
  bool    debug;
  //  int     minLayersApart;
  //  double  nSigmaFromSegment;
  int     minHitsPerSegment;
  //  int     muonsPerChamberMax;
  //  double  chi2Max;
  double  dXclusBoxMax;
  double  dYclusBoxMax;
  int     maxRecHitsInCluster;
  bool    preClustering;
  bool    Pruning;
  bool    BrutePruning;
  double  BPMinImprovement;
  bool    onlyBestSegment;

  double  hitDropLimit4Hits;
  double  hitDropLimit5Hits;
  double  hitDropLimit6Hits;

  float a_yweightPenaltyThreshold[5][5];

  double  yweightPenaltyThreshold;
  double  yweightPenalty;

  double  curvePenaltyThreshold;
  double  curvePenalty;


  bool adjustCovariance_;       /// Flag whether to 'improve' covariance matrix

  bool condpass1, condpass2;

  double chi2Norm_3D_;           /// Chi^2 normalization for the initial fit

  bool prePrun_;                 /// Allow to prune a (rechit in a) segment in segment buld method
                                 /// once it passed through Chi^2-X and  chi2uCorrection is big.
  double prePrunLimit_;          /// The upper limit of protoChiUCorrection to apply prePrun

};

#endif
