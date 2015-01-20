#ifndef xAODAnaHelpers_TrackPlotsClass_H
#define xAODAnaHelpers_TrackPlotsClass_H

#include "xAODAnaHelpers/ManageHists.h"
//#include "xAODTracking/TrackParticle.h"

namespace xAOD {
#ifndef XAODTRACKING_TRACKPARTICLECONTAINER_H
  class TrackParticleContainer;
#endif
#ifndef XAODTRACKING_TRACKPARTICLE_H
  class TrackParticle;
#endif
#ifndef XAODTRACKING_XAODPRIMITIVES_H
  class SummaryType;
#endif
}

//class SummaryType;

class TrackPlotsClass : public ManageHists
{

  private:
    
    // bools to control which histograms are filled
    bool m_fillKinematic;     //!
    bool m_fillClean;         //!
    bool m_fillEnergy;        //!
    
    bool m_fillResolution;    //!

    // Histograms
    TH1F* m_trk_Pt              ; //!
    TH1F* m_trk_Pt_l   		; //!
    TH1F* m_trk_Eta    		; //!
    TH1F* m_trk_Phi    		; //!
    TH1F* m_trk_dO     		; //!
    TH1F* m_trk_z0     		; //!
    TH1F* m_trk_chi2Prob	; //!
    TH1F* m_trk_charge		; //!
    TH1F* m_trk_d0_l        	; //!
    TH1F* m_trk_d0Err       	; //!
    TH1F* m_trk_d0Sig       	; //!
    TH1F* m_trk_z0Err           ; //!
    TH1F* m_trk_z0_l            ; //!
    TH1F* m_trk_z0Sig           ; //!
    TH1F* m_trk_z0sinT          ; //!
    TH1F* m_trk_z0SigsinT       ; //!
    TH1F* m_trk_chi2Prob_l      ; //!
    TH1F* m_trk_chi2ndof   	; //!
    TH1F* m_trk_chi2ndof_l 	; //!
    TH1F* m_trk_nSi        	; //!
    TH1F* m_trk_nSiAndDead 	; //!
    TH1F* m_trk_nSiDead    	; //!
    TH1F* m_trk_nSCT       	; //!
    TH1F* m_trk_nPix       	; //!
    TH1F* m_trk_nPixHoles  	; //!
    TH1F* m_trk_nBL        	; //!
    TH1F* m_trk_phiErr   	; //!
    TH1F* m_trk_thetaErr 	; //!
    TH1F* m_trk_qOpErr   	; //!
    TH1F* m_trk_mc_prob     	; //!
    TH1F* m_trk_mc_barcode  	; //!
    TH1F* m_trk_mc_barcode_s	; //!
    TH1F* m_trk_eta_vl     	; //!
    TH1F* m_trk_z0_vl      	; //!
    TH1F* m_trk_d0_vl      	; //!
    TH1F* m_trk_pt_ss      	; //!
    TH1F* m_trk_phiManyBins     ; //!


  public:

    TrackPlotsClass();
    TrackPlotsClass(std::string name, int detailLevel);
    TrackPlotsClass(std::string name, std::string detailStr);

    EL::StatusCode BookHistograms(EL::Worker* wk = 0);
    void FillHistograms( const xAOD::TrackParticleContainer* trks,  float eventWeight );
    void FillHistograms( const xAOD::TrackParticle* trk,            float eventWeight );

};


#endif
