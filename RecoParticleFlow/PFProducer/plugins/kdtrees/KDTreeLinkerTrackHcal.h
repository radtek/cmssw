#ifndef KDTreeLinkerTrackHcal_h
#define KDTreeLinkerTrackHcal_h

#include "RecoParticleFlow/PFProducer/interface/KDTreeLinkerBase.h"
#include "CommonTools/RecoAlgos/interface/KDTreeLinkerAlgo.h"
#include "CommonTools/RecoAlgos/interface/KDTreeLinkerTools.h"


// This class is used to find all links between Tracks and HCAL clusters
// using a KDTree algorithm.
// It is used in PFBlockAlgo.cc in the function links().
class KDTreeLinkerTrackHcal : public KDTreeLinkerBase
{
 public:
  KDTreeLinkerTrackHcal();
  ~KDTreeLinkerTrackHcal() override;
  
  // With this method, we create the list of psCluster that we want to link.
  void insertTargetElt(reco::PFBlockElement		*track) override;

  // Here, we create the list of hcalCluster that we want to link. From hcalCluster
  // and fraction, we will create a second list of rechits that will be used to
  // build the KDTree.
  void insertFieldClusterElt(reco::PFBlockElement	*hcalCluster) override;  

  // The KDTree building from rechits list.
  void buildTree() override;
  
  // Here we will iterate over all tracks. For each track intersection point with HCAL, 
  // we will search the closest rechits in the KDTree, from rechits we will find the 
  // hcalClusters and after that we will check the links between the track and 
  // all closest hcalClusters.  
  void searchLinks() override;
    
  // Here, we will store all PS/HCAL founded links in the PFBlockElement class
  // of each psCluster in the PFmultilinks field.
  void updatePFBlockEltWithLinks() override;
  
  // Here we free all allocated structures.
  void clear() override;
 
 private:
  // Data used by the KDTree algorithm : sets of Tracks and HCAL clusters.
  BlockEltSet		targetSet_;
  BlockEltSet		fieldClusterSet_;

  // Sets of rechits that compose the HCAL clusters. 
  RecHitSet		rechitsSet_;
  
  // Map of linked Track/HCAL clusters.
  BlockElt2BlockEltMap	cluster2TargetLinks_;

  // Map of the HCAL clusters associated to a rechit.
  RecHit2BlockEltMap	rechit2ClusterLinks_;
    
  // KD trees
  KDTreeLinkerAlgo<reco::PFRecHit const*>	tree_;

};

#endif /* !KDTreeLinkerTrackHcal_h */
