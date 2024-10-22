#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace btaggingCR{
    int Nbjets_loose_deepcsv;
    int Nbjets_medium_deepcsv;
    int Nbjets_tight_deepcsv;
    int Nbjets_loose_deepjet;
    int Nbjets_medium_deepjet;
    int Nbjets_tight_deepjet;
    int Nbjets30_loose_deepcsv;
    int Nbjets30_medium_deepcsv;
    int Nbjets30_tight_deepcsv;
    int Nbjets30_loose_deepjet;
    int Nbjets30_medium_deepjet;
    int Nbjets30_tight_deepjet;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupbtaggingCR() {

    // Setup cutflow
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_Selected", 0) );

    // Setup output branches
    _outputTree->Branch("RegionID", &RegionID);
    _outputTree->Branch("lepton_ID_wgt", &lepton_ID_wgt);
    _outputTree->Branch("pileup_wgt", &pileup_wgt);
    _outputTree->Branch("Njets", &Njets);
    _outputTree->Branch("Njets30", &Njets30);
    _outputTree->Branch("Nbjets_loose_deepcsv", &btaggingCR::Nbjets_loose_deepcsv);
    _outputTree->Branch("Nbjets_medium_deepcsv", &btaggingCR::Nbjets_medium_deepcsv);
    _outputTree->Branch("Nbjets_tight_deepcsv", &btaggingCR::Nbjets_tight_deepcsv);
    _outputTree->Branch("Nbjets_loose_deepjet", &btaggingCR::Nbjets_loose_deepjet);
    _outputTree->Branch("Nbjets_medium_deepjet", &btaggingCR::Nbjets_medium_deepjet);
    _outputTree->Branch("Nbjets_tight_deepjet", &btaggingCR::Nbjets_tight_deepjet);
    _outputTree->Branch("Nbjets30_loose_deepcsv", &btaggingCR::Nbjets30_loose_deepcsv);
    _outputTree->Branch("Nbjets30_medium_deepcsv", &btaggingCR::Nbjets30_medium_deepcsv);
    _outputTree->Branch("Nbjets30_tight_deepcsv", &btaggingCR::Nbjets30_tight_deepcsv);
    _outputTree->Branch("Nbjets30_loose_deepjet", &btaggingCR::Nbjets30_loose_deepjet);
    _outputTree->Branch("Nbjets30_medium_deepjet", &btaggingCR::Nbjets30_medium_deepjet);
    _outputTree->Branch("Nbjets30_tight_deepjet", &btaggingCR::Nbjets30_tight_deepjet);

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::btaggingCRRegion() {

    LeptonSelection();
    
    if( !(RecoLepID > 0) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    XYMETCorrection();
    METEmulation();
    if( !(MET_pt > MET_BASE_CUT) ) return false;                                 // MET > BASE_CUT
    _cutFlow.at("01_MET") += evtWeight; 
    
    Get_LeadingAndTrailing_Lepton_Variables();
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_BASE_CUT) ) return false;               // Leading lepton pt > BASE_CUT
    _cutFlow.at("02_LeadingLep_Pt") += evtWeight; 
    
    if( !DYJetsToLL_processing() ) return false;
    
    JetSelection();
    Get_LepLep_Variables();
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("03_LepLep_DM") += evtWeight;  
    
    Regions();
    if( !(RegionID == 1 || RegionID == 2) ) return false;                                        // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("04_Selected") += evtWeight; 
    
    //Get_Jet_Angular_Variables( );
    Weight_corrections();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::btaggingCRSelection() {

    float BTAG_DEEPCSV_LOOSE_CUT;
    float BTAG_DEEPCSV_MEDIUM_CUT;
    float BTAG_DEEPCSV_TIGHT_CUT;
    float BTAG_DEEPJET_LOOSE_CUT;
    float BTAG_DEEPJET_MEDIUM_CUT;
    float BTAG_DEEPJET_TIGHT_CUT;

    if( dataset_year == "16" ){
        BTAG_DEEPCSV_LOOSE_CUT = 0.2217;
        BTAG_DEEPCSV_MEDIUM_CUT = 0.6321;
        BTAG_DEEPCSV_TIGHT_CUT = 0.8953;
        BTAG_DEEPJET_LOOSE_CUT = 0.0614;
        BTAG_DEEPJET_MEDIUM_CUT = 0.3093;
        BTAG_DEEPJET_TIGHT_CUT = 0.7221;
    }else if( dataset_year == "17" ){
        BTAG_DEEPCSV_LOOSE_CUT = 0.1355;
        BTAG_DEEPCSV_MEDIUM_CUT = 0.4506;
        BTAG_DEEPCSV_TIGHT_CUT = 0.7738;
        BTAG_DEEPJET_LOOSE_CUT = 0.0532;
        BTAG_DEEPJET_MEDIUM_CUT = 0.3040;
        BTAG_DEEPJET_TIGHT_CUT = 0.7476;
    }else if( dataset_year == "18" ){
        BTAG_DEEPCSV_LOOSE_CUT = 0.1208;
        BTAG_DEEPCSV_MEDIUM_CUT = 0.4168;
        BTAG_DEEPCSV_TIGHT_CUT = 0.7665;
        BTAG_DEEPJET_LOOSE_CUT = 0.0490;
        BTAG_DEEPJET_MEDIUM_CUT = 0.2783;
        BTAG_DEEPJET_TIGHT_CUT = 0.7100;
    }

    bool bTagged_loose_deepcsv;
    bool bTagged_medium_deepcsv;
    bool bTagged_tight_deepcsv;
    bool bTagged_loose_deepjet;
    bool bTagged_medium_deepjet;
    bool bTagged_tight_deepjet;

    btaggingCR::Nbjets_loose_deepcsv = 0;
    btaggingCR::Nbjets_medium_deepcsv = 0;
    btaggingCR::Nbjets_tight_deepcsv = 0;
    btaggingCR::Nbjets_loose_deepjet = 0;
    btaggingCR::Nbjets_medium_deepjet = 0;
    btaggingCR::Nbjets_tight_deepjet = 0;
    btaggingCR::Nbjets30_loose_deepcsv = 0;
    btaggingCR::Nbjets30_medium_deepcsv = 0;
    btaggingCR::Nbjets30_tight_deepcsv = 0;
    btaggingCR::Nbjets30_loose_deepjet = 0;
    btaggingCR::Nbjets30_medium_deepjet = 0;
    btaggingCR::Nbjets30_tight_deepjet = 0;

    // Only selected jets
    // * Jet_pt > 20
    // * Jet_jetId >= 6 (TightLepVeto)
    // * |Jet_eta| <= 2.4
    for (unsigned int ijet = 0; ijet < nJet; ++ijet) {
        if (
            Jet_pt[ijet] > JET_PT_CUT
            && Jet_jetId[ijet] >= JET_ID
            && abs(Jet_eta[ijet]) < JET_ETA_CUT
        ) {
            bTagged_loose_deepcsv = Jet_btagDeepB[ijet] > BTAG_DEEPCSV_LOOSE_CUT;
            bTagged_medium_deepcsv = Jet_btagDeepB[ijet] > BTAG_DEEPCSV_MEDIUM_CUT;
            bTagged_tight_deepcsv = Jet_btagDeepB[ijet] > BTAG_DEEPCSV_TIGHT_CUT;
            bTagged_loose_deepjet = Jet_btagDeepFlavB[ijet] > BTAG_DEEPJET_LOOSE_CUT;
            bTagged_medium_deepjet = Jet_btagDeepFlavB[ijet] > BTAG_DEEPJET_MEDIUM_CUT;
            bTagged_tight_deepjet = Jet_btagDeepFlavB[ijet] > BTAG_DEEPJET_TIGHT_CUT;

            if (bTagged_loose_deepcsv) btaggingCR::Nbjets_loose_deepcsv += 1;
            if (bTagged_medium_deepcsv) btaggingCR::Nbjets_medium_deepcsv += 1;
            if (bTagged_tight_deepcsv) btaggingCR::Nbjets_tight_deepcsv += 1;
            if (bTagged_loose_deepjet) btaggingCR::Nbjets_loose_deepjet += 1;
            if (bTagged_medium_deepjet) btaggingCR::Nbjets_medium_deepjet += 1;
            if (bTagged_tight_deepjet) btaggingCR::Nbjets_tight_deepjet += 1;

            if (bTagged_loose_deepcsv && Jet_pt[ijet] > 30) btaggingCR::Nbjets30_loose_deepcsv += 1;
            if (bTagged_medium_deepcsv && Jet_pt[ijet] > 30) btaggingCR::Nbjets30_medium_deepcsv += 1;
            if (bTagged_tight_deepcsv && Jet_pt[ijet] > 30) btaggingCR::Nbjets30_tight_deepcsv += 1;
            if (bTagged_loose_deepjet && Jet_pt[ijet] > 30) btaggingCR::Nbjets30_loose_deepjet += 1;
            if (bTagged_medium_deepjet && Jet_pt[ijet] > 30) btaggingCR::Nbjets30_medium_deepjet += 1;
            if (bTagged_tight_deepjet && Jet_pt[ijet] > 30) btaggingCR::Nbjets30_tight_deepjet += 1;

            _outputTree->Fill();
        }
    }

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::btaggingCRSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishbtaggingCR() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
