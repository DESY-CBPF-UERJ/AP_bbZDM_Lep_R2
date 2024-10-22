#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace Trigger{

    bool  HLT_MET;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTrigger() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("06_Selected", 0) );

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

    _outputTree->Branch("HLT_Ele35_WPTight_Gsf", &HLT_Ele35_WPTight_Gsf );
    _outputTree->Branch("HLT_Ele23_Ele12_caloIdL_TrackIdL_IsoVL", &HLT_Ele23_Ele12_caloIdL_TrackIdL_IsoVL );
    _outputTree->Branch("HLT_Ele115_CaloIdVT_GsfTrkIdT", &HLT_Ele115_CaloIdVT_GsfTrkIdT );
    _outputTree->Branch("HLT_DoubleEle33_CaloIdL_MW", &HLT_DoubleEle33_CaloIdL_MW );
    _outputTree->Branch("HLT_MET", &Trigger::HLT_MET );
   
    
    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TriggerRegion() {

    
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
void HEPHero::TriggerSelection() {

    Trigger::HLT_MET = HLT_PFMET200_HBHECleaned || HLT_PFMET200_HBHE_BeamHaloCleaned || HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned || HLT_PFMET120_PFMHT120_IDTight || HLT_PFMET120_PFMHT120_IDTight_PFHT60 || HLT_PFMETNoMu120_PFMHTNoMu120_IDTight || HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 || HLT_PFHT500_PFMET100_PFMHT100_IDTight || HLT_PFHT700_PFMET85_PFMHT85_IDTight || HLT_PFHT800_PFMET75_PFMHT75_IDTight;    

    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //Trigger::variable1Name = 100;      [Example]

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
void HEPHero::TriggerSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTrigger() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
