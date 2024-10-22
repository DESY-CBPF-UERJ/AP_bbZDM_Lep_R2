#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace CheckPUSyst{

    float Pileup_sfs_down;
    float Pileup_sfs_up;
    float LeptonID_sfs_down;
    float LeptonID_sfs_up;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupCheckPUSyst() {
    
    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_Selected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("Pileup_sfs_down", &CheckPUSyst::Pileup_sfs_down );
    _outputTree->Branch("Pileup_sfs_up", &CheckPUSyst::Pileup_sfs_up );
    _outputTree->Branch("pileup_wgt", &pileup_wgt );
    
    _outputTree->Branch("LeptonID_sfs_down", &CheckPUSyst::LeptonID_sfs_down );
    _outputTree->Branch("LeptonID_sfs_up", &CheckPUSyst::LeptonID_sfs_up );
    _outputTree->Branch("lepton_ID_wgt", &lepton_ID_wgt );
    
    

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::CheckPUSystRegion() {

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
    if( !((RegionID >= 0) && (RegionID < 3)) ) return false;                    // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("04_Selected") += evtWeight; 
    

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::CheckPUSystSelection() {


    LeptonSelection();
    
    pileup_wgt = pileup_corr->evaluate({Pileup_nTrueInt, "nominal"});
    lepton_ID_wgt = GetLeptonIDWeight("cv");
    
    double pileup_wgt_down = pileup_corr->evaluate({Pileup_nTrueInt, "down"});
    double pileup_wgt_up = pileup_corr->evaluate({Pileup_nTrueInt, "up"});
    CheckPUSyst::Pileup_sfs_down = pileup_wgt_down/pileup_wgt;
    CheckPUSyst::Pileup_sfs_up = pileup_wgt_up/pileup_wgt;
    
    double lepton_ID_wgt_down = GetLeptonIDWeight("down");
    double lepton_ID_wgt_up = GetLeptonIDWeight("up");
    CheckPUSyst::LeptonID_sfs_down = lepton_ID_wgt_down/lepton_ID_wgt;
    CheckPUSyst::LeptonID_sfs_up = lepton_ID_wgt_up/lepton_ID_wgt;










    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //CheckPUSyst::variable1Name = 100;      [Example]

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
void HEPHero::CheckPUSystSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishCheckPUSyst() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
