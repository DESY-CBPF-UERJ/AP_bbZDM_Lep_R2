#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace XCheck{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupXCheck() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_AllEvents", 0) );
    _cutFlow.insert(pair<string,double>("01_GoodLumi", 0) );
    _cutFlow.insert(pair<string,double>("02_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("03_Trigger", 0) );
    _cutFlow.insert(pair<string,double>("04_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("06_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("07_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("08_METFilter", 0) );
    _cutFlow.insert(pair<string,double>("09_MET", 0) );
    _cutFlow.insert(pair<string,double>("10_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("11_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("12_DYCR", 0) );
    

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &XCheck::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "RegionID", &RegionID );
    HDF_insert( "DatasetID", &_DatasetID );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "TrailingLep_pt", &TrailingLep_pt );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "Nbjets", &Nbjets );
    HDF_insert( "Njets", &Njets );
    HDF_insert( "Njets30", &Njets30 );
    HDF_insert( "Njets40", &Njets40 );
    HDF_insert( "HT30", &HT30 );
    HDF_insert( "MHT30", &MHT30 );
    HDF_insert( "PV_npvs", &PV_npvs );
    HDF_insert( "PV_npvsGood", &PV_npvsGood );
    HDF_insert( "pileup_wgt", &pileup_wgt );
    HDF_insert( "electron_wgt", &electron_wgt );
    HDF_insert( "muon_wgt", &muon_wgt );
    HDF_insert( "btag_wgt", &btag_wgt );
    HDF_insert( "MET_RAW_pt", &MET_RAW_pt );
    HDF_insert( "MET_XY_pt", &MET_XY_pt );
    HDF_insert( "MET_JER_pt", &MET_JER_pt );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::XCheckRegion() {

    _cutFlow.at("00_AllEvents") += evtWeight;
    
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    _cutFlow.at("01_GoodLumi") += evtWeight;
    
    LeptonSelection();
    
    if( !((RecoLepID > 0) && (RecoLepID < 10000)) ) return false;               // Has two main leptons & zero extra leptons
    _cutFlow.at("02_TwoLepOS") += evtWeight;
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("03_Trigger") += evtWeight;
    
    JetSelection();
    METCorrection();
    Get_Leptonic_Info(true, true);
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("04_LeadingLep_Pt") += evtWeight; 
    
    Get_LepLep_Variables(true, true);
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("05_LepLep_DM") += evtWeight; 
    
    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("06_LepLep_Pt") += evtWeight;
    
    if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Upper cut in LepLep Delta R 
    _cutFlow.at("07_LepLep_DR") += evtWeight;
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    _cutFlow.at("08_METFilter") += evtWeight;
    
    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("09_MET") += evtWeight;
    
    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("10_MET_LepLep_DPhi") += evtWeight;  
    
    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("11_MET_LepLep_Mt") += evtWeight;
    
    Weight_corrections();
    
    if( !((RecoLepID < 100) && (Nbjets == 0) && (MET_pt < 100)) ) return false;        
    _cutFlow.at("12_DYCR") += evtWeight;
    
    
    
    
    /*
    
    Regions();
    if( !((RegionID >= 1) && (RegionID <= 2)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    
    Weight_corrections();
    _cutFlow.at("09_Corrected") += evtWeight;
    */

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::XCheckSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //XCheck::variable1Name = 100;      [Example]

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
void HEPHero::XCheckSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishXCheck() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
