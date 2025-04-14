#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace CWRinfo{

    int TruthLepID;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupCWRinfo() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_Trigger", 0) );
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("06_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("07_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("08_MET_Filters", 0) );
    _cutFlow.insert(pair<string,double>("09_HEM_Filter", 0) );
    _cutFlow.insert(pair<string,double>("10_Njets", 0) );
    _cutFlow.insert(pair<string,double>("11_SR", 0) );
    _cutFlow.insert(pair<string,double>("12_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    makeHist( "TruthLepID", 10, 10., 20., "TruthLepID", "Events" );
    makeHist( "Trigger_before", 30, 0., 3., "LepLep_deltaR", "Events" );
    makeHist( "Trigger_after", 30, 0., 3., "LepLep_deltaR", "Events" );
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &CWRinfo::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "RegionID", &RegionID );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "TruthLepID", &CWRinfo::TruthLepID );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::CWRinfoRegion() {

    LeptonSelection();

    if( !(RecoLepID > 0) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoLepOS") += evtWeight;

    JetSelection();
    METCorrection();
    Get_Leptonic_Info(true, true);
    Get_LepLep_Variables(true, true);

    _histograms1D.at("Trigger_before").Fill( LepLep_deltaR, evtWeight );

    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("00_Trigger") += evtWeight;

    _histograms1D.at("Trigger_after").Fill( LepLep_deltaR, evtWeight );

    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("01_MET") += evtWeight;

    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("02_LeadingLep_Pt") += evtWeight;

    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("03_LepLep_DM") += evtWeight;

    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("04_LepLep_Pt") += evtWeight;

    if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Upper cut in LepLep Delta R
    _cutFlow.at("05_LepLep_DR") += evtWeight;

    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("06_MET_LepLep_DPhi") += evtWeight;

    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("07_MET_LepLep_Mt") += evtWeight;

    if( !METFilters() ) return false;                                           // Selected by MET filters
    _cutFlow.at("08_MET_Filters") += evtWeight;

    HEMissue();

    if( !HEM_filter ) return false;                                           // Selected by HEM filter
    _cutFlow.at("09_HEM_Filter") += evtWeight;

    if( !(Njets >= 1) ) return false;                                           //
    _cutFlow.at("10_Njets") += evtWeight;

    Get_ttbar_Variables();
    Signal_discriminators();

    Regions();
    if( !(RegionID == 0) ) return false;                   // 0=SR
    _cutFlow.at("11_SR") += evtWeight;

    Weight_corrections();
    _cutFlow.at("12_Corrected") += evtWeight;

    CWRinfo::TruthLepID = TruthLepID();

    _histograms1D.at("TruthLepID").Fill( CWRinfo::TruthLepID, evtWeight );


    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::CWRinfoSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //CWRinfo::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    //_outputTree->Fill();

    //======FILL THE OUTPUT HDF5 INFO===============================================================
    HDF_fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::CWRinfoSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishCWRinfo() {

    MakeEfficiencyPlot( _histograms1D.at("Trigger_after"), _histograms1D.at("Trigger_before"), "Trigger_eff_deltaR" );

    return;
}
