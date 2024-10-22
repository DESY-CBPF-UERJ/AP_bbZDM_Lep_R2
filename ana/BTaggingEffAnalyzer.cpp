#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace BTaggingEffAnalyzer{
    bool bTagged;
    float jetDiscr;
    float jetPt;
    float jetEta;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupBTaggingEffAnalyzer() {

    // Setup cutflow
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0));
    _cutFlow.insert(pair<string,double>("01_MET", 0));
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0));
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0));
    _cutFlow.insert(pair<string,double>("04_LepLep_Pt", 0));
    _cutFlow.insert(pair<string,double>("05_LepLep_DR", 0));
    _cutFlow.insert(pair<string,double>("06_Selected", 0));

    // Setup output branches
    _outputTree->Branch("RecoLepID", &RecoLepID);
    _outputTree->Branch("RegionID", &RegionID);
    _outputTree->Branch("bTagged", &BTaggingEffAnalyzer::bTagged);
    _outputTree->Branch("jetDiscr", &BTaggingEffAnalyzer::jetDiscr);
    _outputTree->Branch("jetPt", &BTaggingEffAnalyzer::jetPt);
    _outputTree->Branch("jetEta", &BTaggingEffAnalyzer::jetEta);

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::BTaggingEffAnalyzerRegion() {

    LeptonSelection();
    
    if( !(RecoLepID > 0) ) return false;                                        // Has two reconstructed leptons with opposite signal
    lepton_ID_wgt = GetLeptonIDWeight("cv");
    evtWeight *= lepton_ID_wgt;
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
    
    if( !(LepLep_pt > LEPLEP_PT_BASE_CUT) ) return false;                       // Two leptons system pt > BASE_CUT
    _cutFlow.at("04_LepLep_Pt") += evtWeight;
    
    if( !(LepLep_deltaR < LEPLEP_DR_BASE_CUT) ) return false;                   // Upper cut in LepLep Delta R 
    _cutFlow.at("05_LepLep_DR") += evtWeight; 
    
    Regions();
    if( !(RegionID >= 0) ) return false;                                        // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("06_Selected") += evtWeight;

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::BTaggingEffAnalyzerSelection() {

    /*
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
            BTaggingEffAnalyzer::bTagged = Jet_btagDeepFlavB[ijet] > BTAG_WP;
            BTaggingEffAnalyzer::jetDiscr = Jet_btagDeepFlavB[ijet];
            BTaggingEffAnalyzer::jetPt = Jet_pt[ijet];
            BTaggingEffAnalyzer::jetEta = Jet_eta[ijet];
            _outputTree->Fill();
        }
    }
    */

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::BTaggingEffAnalyzerSystematic() {

}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishBTaggingEffAnalyzer() {
    return;
}
