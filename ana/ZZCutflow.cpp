#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace ZZCutflow{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupZZCutflow() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LL_deltaR", 0) );
    _cutFlow.insert(pair<string,double>("04_MET_LL_Mt", 0) );
    _cutFlow.insert(pair<string,double>("05_LL_DM", 0) );
    _cutFlow.insert(pair<string,double>("06_LL_pt", 0) );
    _cutFlow.insert(pair<string,double>("07_MET_LL_deltaPhi", 0) );
    _cutFlow.insert(pair<string,double>("08_Nbjets", 0) );
    _cutFlow.insert(pair<string,double>("09_Selected", 0) );
    _cutFlow.insert(pair<string,double>("10_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &ZZCutflow::variable1Name );  [example]
    _outputTree->Branch( "RecoLepID", &RecoLepID );
    _outputTree->Branch( "RegionID", &RegionID );
    _outputTree->Branch( "DatasetID", &_DatasetID );
    _outputTree->Branch( "LepLep_pt", &LepLep_pt );
    _outputTree->Branch( "LepLep_deltaR", &LepLep_deltaR );
    _outputTree->Branch( "LeadingLep_pt", &LeadingLep_pt );
    _outputTree->Branch( "TrailingLep_pt", &TrailingLep_pt );
    _outputTree->Branch( "LeadingLep_eta", &LeadingLep_eta );
    _outputTree->Branch( "TrailingLep_eta", &TrailingLep_eta );
    _outputTree->Branch( "MET_pt", &MET_pt );
    _outputTree->Branch( "LepLep_deltaM", &LepLep_deltaM );
    _outputTree->Branch( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    _outputTree->Branch( "MET_LepLep_Mt", &MET_LepLep_Mt );
    _outputTree->Branch( "Nbjets", &Nbjets );
    _outputTree->Branch( "Njets", &Njets );
    _outputTree->Branch( "Njets30", &Njets30 );
    _outputTree->Branch( "Njets40", &Njets40 );
    _outputTree->Branch( "HT30", &HT30 );
    _outputTree->Branch( "MHT30", &MHT30 );
    _outputTree->Branch( "OmegaMin", &OmegaMin );
    _outputTree->Branch( "ChiMin", &ChiMin );
    _outputTree->Branch( "FMax", &FMax );
    _outputTree->Branch( "MT2LL", &MT2LL );
    _outputTree->Branch( "Njets_forward", &Njets_forward );
    _outputTree->Branch( "Jet_abseta_max", &Jet_abseta_max );
    _outputTree->Branch( "Dijet_deltaEta", &Dijet_deltaEta );
    _outputTree->Branch( "Dijet_M", &Dijet_M );
    _outputTree->Branch( "Dijet_pt", &Dijet_pt );
    _outputTree->Branch( "ttbar_reco", &ttbar_reco );
    _outputTree->Branch( "PV_npvs", &PV_npvs );
    _outputTree->Branch( "PV_npvsGood", &PV_npvsGood );
    _outputTree->Branch( "pileup_wgt", &pileup_wgt );
    _outputTree->Branch( "electron_wgt", &electron_wgt );
    _outputTree->Branch( "muon_wgt", &muon_wgt );
    _outputTree->Branch( "btag_wgt", &btag_wgt );
    _outputTree->Branch( "MET_RAW_pt", &MET_RAW_pt );
    _outputTree->Branch( "MET_RAW_phi", &MET_RAW_phi );
    _outputTree->Branch( "MET_Unc_pt", &MET_Unc_pt );
    _outputTree->Branch( "MET_Unc_phi", &MET_Unc_phi );
    _outputTree->Branch( "MET_JES_pt", &MET_JES_pt );
    _outputTree->Branch( "MET_JES_phi", &MET_JES_phi );
    _outputTree->Branch( "MET_XY_pt", &MET_XY_pt );
    _outputTree->Branch( "MET_XY_phi", &MET_XY_phi );
    _outputTree->Branch( "MET_JER_pt", &MET_JER_pt );
    _outputTree->Branch( "MET_JER_phi", &MET_JER_phi );
    _outputTree->Branch( "MET_Emu_pt", &MET_Emu_pt );
    _outputTree->Branch( "MET_Emu_phi", &MET_Emu_phi );
    //_outputTree->Branch( "MLP_score_1000_800", &MLP_score_1000_800 );
    //_outputTree->Branch( "MLP_score_1000_100", &MLP_score_1000_100 );
    //_outputTree->Branch( "MLP_score_400_100", &MLP_score_400_100 );
    _outputTree->Branch( "genPt", &genPt );
    //_outputTree->Branch( "param_variation_weights", &SysTest::param_variation_weights );
    _outputTree->Branch( "VVCR_LeadingLep_pt", &VVCR_LeadingLep_pt );

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert("variable1NameInTheTree", &ZZCutflow::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::ZZCutflowRegion() {

    LeptonSelection();
    
    if( !(RecoLepID > 40000) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    
    JetSelection();
    METCorrection();
    
    bool MET_selected = false;
    if( (RecoLepID < 10000) && (MET_pt > MET_CUT) ) MET_selected = true;        
    if( (RecoLepID > 10000) && (MET_pt > MET_BASE_CUT) ) MET_selected = true;
    if( !MET_selected ) return false;                                           // MET selection
    _cutFlow.at("01_MET") += evtWeight; 
    
    Get_Leptonic_Info(true, true);
    
    bool LeadingLepPt_selected = false;
    if( (RecoLepID < 10000) && (LeadingLep_pt > LEADING_LEP_PT_CUT) ) LeadingLepPt_selected = true;        
    if( (RecoLepID > 10000) && (LeadingLep_pt > LEADING_LEP_PT_BASE_CUT) ) LeadingLepPt_selected = true;
    if( !LeadingLepPt_selected ) return false;                                  // Leading lepton pt selection
    _cutFlow.at("02_LeadingLep_Pt") += evtWeight; 
    
    Get_LepLep_Variables(true, true);
    
    bool LepLep_deltaR_selected = false;
    if( (RecoLepID < 10000) && (LepLep_deltaR < LEPLEP_DR_CUT) ) LepLep_deltaR_selected = true;        
    if( (RecoLepID > 10000) && (LepLep_deltaR < LEPLEP_DR_BASE_CUT) ) LepLep_deltaR_selected = true;
    if( !LepLep_deltaR_selected ) return false;                                 // LepLep deltaR selection
    _cutFlow.at("03_LL_deltaR") += evtWeight;
    
    bool MET_LepLep_Mt_selected = false;
    if( (RecoLepID < 10000) && (MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) MET_LepLep_Mt_selected = true;        
    if( (RecoLepID > 10000) ) MET_LepLep_Mt_selected = true;
    if( !MET_LepLep_Mt_selected ) return false;                                 // LepLep pt selection
    _cutFlow.at("04_MET_LL_Mt") += evtWeight;
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("05_LL_DM") += evtWeight; 
    
    bool LepLep_pt_selected = false;
    if( (RecoLepID < 10000) && (LepLep_pt > LEPLEP_PT_CUT) ) LepLep_pt_selected = true;        
    if( (RecoLepID > 10000) && (LepLep_pt > LEPLEP_PT_BASE_CUT) ) LepLep_pt_selected = true;
    if( !LepLep_pt_selected ) return false;                                     // LepLep pt selection
    _cutFlow.at("06_LL_pt") += evtWeight;
    
    bool MET_LepLep_deltaPhi_selected = false;
    if( (RecoLepID < 10000) && (MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) MET_LepLep_deltaPhi_selected = true;        
    if( (RecoLepID > 10000) ) MET_LepLep_deltaPhi_selected = true;
    if( !MET_LepLep_deltaPhi_selected ) return false;                           // LepLep pt selection
    _cutFlow.at("07_MET_LL_deltaPhi") += evtWeight;
    
    bool Nbjets_selection = false;
    if( (RecoLepID < 1000) ) Nbjets_selection = true;
    if( (RecoLepID > 1000) && (RecoLepID < 10000) && (Nbjets >= 2) ) Nbjets_selection = true;
    if( (RecoLepID > 10000) ) Nbjets_selection = true;
    //if( (RecoLepID > 10000) && (Nbjets == 0) ) Nbjets_selection = true;
    if( !Nbjets_selection ) return false;                                       // Nbjets selection
    _cutFlow.at("08_Nbjets") += evtWeight;
    
    Get_ttbar_Variables();
    
    Regions();
    if( !(RegionID == 4) ) return false;                                        // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("09_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("10_Corrected") += evtWeight;
    
    Get_Jet_Angular_Variables( );
    Get_Dijet_Variables();
    Signal_discriminators();
    
    
    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::ZZCutflowSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //ZZCutflow::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    _outputTree->Fill();

    //======FILL THE OUTPUT HDF5 INFO===============================================================
    //HDF_fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::ZZCutflowSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishZZCutflow() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
