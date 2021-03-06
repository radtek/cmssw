//=========================================================================
// EtaDependentPileup.h
//
// 2-d interpolation table appropriate for pile-up calculations
//
// I. Volobouev
// June 2011
//=========================================================================

#ifndef RecoJets_FFTJetAlgorithms_EtaDependentPileup_h
#define RecoJets_FFTJetAlgorithms_EtaDependentPileup_h

#include "RecoJets/FFTJetAlgorithms/interface/AbsPileupCalculator.h"
#include "fftjet/LinearInterpolator2d.hh"

namespace fftjetcms {
  class EtaDependentPileup : public AbsPileupCalculator {
  public:
    EtaDependentPileup(const fftjet::LinearInterpolator2d& i, double inputRhoFactor, double outputRhoFactor);

    inline ~EtaDependentPileup() override {}

    double operator()(double eta, double phi, const reco::FFTJetPileupSummary& summary) const override;

    inline bool isPhiDependent() const override { return false; }

  private:
    fftjet::LinearInterpolator2d interp_;
    double inputRhoFactor_;
    double outputRhoFactor_;
    double etaMin_;
    double etaMax_;
    double rhoMin_;
    double rhoMax_;
    double rhoStep_;
  };
}  // namespace fftjetcms

#endif  // RecoJets_FFTJetAlgorithms_EtaDependentPileup_h
