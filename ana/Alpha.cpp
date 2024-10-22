#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace Alpha{

    bool HLT_MET;
    bool HLT_SingleEle;
    bool HLT_DoubleEle;
    bool HLT_SingleMu; 
    bool HLT_DoubleMu;
    bool HLT_EleMu;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupAlpha() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("0_MET", 0) );
    _cutFlow.insert(pair<string,double>("1_TwoLepOS", 0) ); 
    _cutFlow.insert(pair<string,double>("2_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("3_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("4_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("5_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("6_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("7_MET_LepLep_Mt", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("RecoLepID", &RecoLepID );
    _outputTree->Branch("LepLep_pt", &LepLep_pt );
    _outputTree->Branch("LepLep_phi", &LepLep_phi );
    _outputTree->Branch("LepLep_mass", &LepLep_mass );
    _outputTree->Branch("LepLep_deltaR", &LepLep_deltaR );
    _outputTree->Branch("LeadingLep_pt", &LeadingLep_pt );
    _outputTree->Branch("MET_pt", &MET_pt );
    _outputTree->Branch("LepLep_deltaM", &LepLep_deltaM );
    _outputTree->Branch("MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    _outputTree->Branch("MET_LepLep_Mt", &MET_LepLep_Mt );
    _outputTree->Branch("Nbjets", &Nbjets );
    _outputTree->Branch("Nleptons", &Nleptons );
    _outputTree->Branch("TrailingLep_pt", &TrailingLep_pt );
    _outputTree->Branch("TrailingLep_eta", &TrailingLep_eta );
    _outputTree->Branch("LeadingLep_eta", &LeadingLep_eta );

    _outputTree->Branch("HLT_MET", &Alpha::HLT_MET );
    _outputTree->Branch("HLT_SingleEle", &Alpha::HLT_SingleEle );
    _outputTree->Branch("HLT_DoubleEle", &Alpha::HLT_DoubleEle );
    _outputTree->Branch("HLT_SingleMu", &Alpha::HLT_SingleMu );
    _outputTree->Branch("HLT_DoubleMu", &Alpha::HLT_DoubleMu );
    _outputTree->Branch("HLT_EleMu", &Alpha::HLT_EleMu );
    
    _outputTree->Branch("HLT_Ele115_CaloIdVT_GsfTrkIdT", &HLT_Ele115_CaloIdVT_GsfTrkIdT );
    _outputTree->Branch("HLT_Ele27_eta2p1_WPLoose_Gsf", &HLT_Ele27_eta2p1_WPLoose_Gsf );
    _outputTree->Branch("HLT_Ele25_eta2p1_WPTight_Gsf", &HLT_Ele25_eta2p1_WPTight_Gsf );
    _outputTree->Branch("HLT_Ele35_WPLoose_Gsf", &HLT_Ele35_WPLoose_Gsf );
    _outputTree->Branch("Electron_mvaIso_WP90_leading", &Electron_mvaFall17V2Iso_WP90[IdxLeadingLep] );
    _outputTree->Branch("Electron_mvaIso_WP90_trailing", &Electron_mvaFall17V2Iso_WP90[IdxTrailingLep] );
    
    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::AlphaRegion() {

    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("0_MET") += evtWeight; 
    
    LeptonSelection();
    
    if( !(RecoLepID > 0) ) return false;                                        // Has exactly two reconstructed leptons with opposite signal
    _cutFlow.at("1_TwoLepOS") += evtWeight;  
    
    Get_LeadingAndTrailing_Lepton_Variables();
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("2_LeadingLep_Pt") += evtWeight;  
    
    Get_LepLep_Variables();
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("3_LepLep_DM") += evtWeight;  
    
    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("4_LepLep_Pt") += evtWeight;  
    
    if( !(LepLep_deltaR < 2.2) ) return false;                        // Delta R between two leptons < CUT
    _cutFlow.at("5_LepLep_DR") += evtWeight;  
    
    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("6_MET_LepLep_DPhi") += evtWeight;  
    
    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("7_MET_LepLep_Mt") += evtWeight;
    
    JetSelection();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::AlphaSelection() {


    Alpha::HLT_MET = HLT_PFMET300 || HLT_MET200 || HLT_PFHT300_PFMET110 || HLT_PFMET170_HBHECleaned || HLT_PFMET120_PFMHT120_IDTight || HLT_PFMETNoMu120_PFMHTNoMu120_IDTight;

    string data_era = _datasetName.substr(_datasetName.length()-4,1);
        
    Alpha::HLT_SingleEle = HLT_Ele27_WPTight_Gsf;
            
    Alpha::HLT_DoubleEle = HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
            
    Alpha::HLT_SingleMu = HLT_IsoMu24 || HLT_IsoTkMu24; 
            
    if( (_datasetName.substr(0,4) == "Data") && (data_era == "H") ){
        Alpha::HLT_DoubleMu = HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ  || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ;
    }else{
        Alpha::HLT_DoubleMu = HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL  || HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL; // HLT_Mu30_TkMu11;
    }
            
    if( (_datasetName.substr(0,4) == "Data") && (data_era == "H") ){
        Alpha::HLT_EleMu = HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
    }else{
        Alpha::HLT_EleMu = HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL || HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
    }

    
    


    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //Alpha::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    _outputTree->Fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::AlphaSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishAlpha() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
