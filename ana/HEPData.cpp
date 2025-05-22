#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace HEPData{

    bool trigger;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupHEPData() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_Trigger", 0) );
    _cutFlow.insert(pair<string,double>("01_MET_Filters", 0) );
    _cutFlow.insert(pair<string,double>("02_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("03_MET", 0) );
    _cutFlow.insert(pair<string,double>("04_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("06_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("07_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("08_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("09_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("10_Njets", 0) );
    _cutFlow.insert(pair<string,double>("11_Nbjets", 0) );
    _cutFlow.insert(pair<string,double>("12_TwoLepOSSF", 0) );
    _cutFlow.insert(pair<string,double>("13_No_ttbar_Reco", 0) );
    _cutFlow.insert(pair<string,double>("14_Corrected", 0) );


    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &HEPData::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "trigger", &HEPData::trigger );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "Nbjets", &Nbjets );
    HDF_insert( "Njets", &Njets );
    HDF_insert( "ttbar_reco", &ttbar_reco );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::HEPDataRegion() {

    LeptonSelection();
    JetSelection();
    METCorrection();


    HEPData::trigger = Trigger();

    //if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("00_Trigger") += evtWeight;

    //if( !METFilters() ) return false;                                           // Selected by MET filters
    //_cutFlow.at("01_MET_Filters") += evtWeight;

    //if( !(RecoLepID > 0) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("02_TwoLepOS") += evtWeight;

    //if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("03_MET") += evtWeight;

    Get_Leptonic_Info(true, true);

    //if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("04_LeadingLep_Pt") += evtWeight;

    Get_LepLep_Variables(true, true);

    //if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("05_LepLep_Pt") += evtWeight;

    //if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("06_LepLep_DM") += evtWeight;

    //if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Upper cut in LepLep Delta R
    _cutFlow.at("07_LepLep_DR") += evtWeight;

    //if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("08_MET_LepLep_DPhi") += evtWeight;

    //if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("09_MET_LepLep_Mt") += evtWeight;

    //if( !(Njets >= 1) ) return false;                                           //
    _cutFlow.at("10_Njets") += evtWeight;

    //if( !(Nbjets >= 1) ) return false;                                           //
    _cutFlow.at("11_Nbjets") += evtWeight;

    //if( !(RecoLepID < 100) ) return false;                                           //
    _cutFlow.at("12_TwoLepOSSF") += evtWeight;

    Get_ttbar_Variables();

    //if( !(ttbar_reco == 0) ) return false;                                           //
    _cutFlow.at("13_No_ttbar_Reco") += evtWeight;

    Weight_corrections();

    _cutFlow.at("14_Corrected") += evtWeight;

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::HEPDataSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //HEPData::variable1Name = 100;      [Example]

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
void HEPHero::HEPDataSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishHEPData() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
