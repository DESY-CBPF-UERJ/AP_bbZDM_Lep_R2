#include "HEPAnalysis.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace METXY{

    double MET_x;
    double MET_y;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPAnalysis::SetupMETXY() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoMuonsOS", 0) );
    _cutFlow.insert(pair<string,double>("01_NoExtraLeptons", 0) );
    _cutFlow.insert(pair<string,double>("02_Pt_ratio", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_deltaPhi", 0) );
    _cutFlow.insert(pair<string,double>("05_Selected", 0) );
    _cutFlow.insert(pair<string,double>("06_Corrected", 0) );
    

    //======SETUP HISTOGRAMS=======================================================================
    makeHist( "NoExtraLeptons_before", 10, 0., 10., "NvetoLeptons", "Events" );  
    makeHist( "NoExtraLeptons_after", 10, 0., 10., "NvetoLeptons", "Events" ); 
    makeHist( "LeadingLep_pt-TrailingLep_pt_before", 50, 0., 2., "LeadingLep_pt/TrailingLep_pt", "Events" );  
    makeHist( "LeadingLep_pt-TrailingLep_pt_after", 50, 0., 2., "LeadingLep_pt/TrailingLep_pt", "Events" );
    makeHist( "TrailingLep_pt-LeadingLep_pt_before", 50, 0., 2., "TrailingLep_pt/LeadingLep_pt", "Events" );  
    makeHist( "TrailingLep_pt-LeadingLep_pt_after", 50, 0., 2., "TrailingLep_pt/LeadingLep_pt", "Events" );
    makeHist( "LepLep_DM_before", 50, 0., 50., "LepLep_deltaM", "Events" );  
    makeHist( "LepLep_DM_after", 50, 0., 50., "LepLep_deltaM", "Events" );
    makeHist( "LepLep_deltaPhi_before", 40, 0., 4., "LepLep_deltaPhi", "Events" );  
    makeHist( "LepLep_deltaPhi_after", 40, 0., 4., "LepLep_deltaPhi", "Events" );

    
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &METXY::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "pileup_wgt", &pileup_wgt );
    HDF_insert( "muon_wgt", &muon_wgt );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "MET_phi", &MET_phi );
    HDF_insert( "MET_x", &METXY::MET_x );
    HDF_insert( "MET_y", &METXY::MET_y );
    HDF_insert( "NPVs", &PV_npvs );
    HDF_insert( "NGoodPVs", &PV_npvsGood );
    
    

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPAnalysis::METXYRegion() {

    LeptonSelection();
    
    if( !(RecoLepID == 13) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoMuonsOS") += evtWeight;
    
    _histograms1D.at("NoExtraLeptons_before").Fill( NvetoLeptons, evtWeight );
    if( !(NvetoLeptons == 0) ) return false;
    _histograms1D.at("NoExtraLeptons_after").Fill( NvetoLeptons, evtWeight );
    _cutFlow.at("01_NoExtraLeptons") += evtWeight;
 
    Get_Leptonic_Info(true, true);
    
    _histograms1D.at("LeadingLep_pt-TrailingLep_pt_before").Fill( LeadingLep_pt/TrailingLep_pt, evtWeight );
    _histograms1D.at("TrailingLep_pt-LeadingLep_pt_before").Fill( TrailingLep_pt/LeadingLep_pt, evtWeight );
    if( !(LeadingLep_pt/TrailingLep_pt < 1.2) ) return false;
    if( !(TrailingLep_pt/LeadingLep_pt > 0.8) ) return false;
    _histograms1D.at("LeadingLep_pt-TrailingLep_pt_after").Fill( LeadingLep_pt/TrailingLep_pt, evtWeight );
    _histograms1D.at("TrailingLep_pt-LeadingLep_pt_after").Fill( TrailingLep_pt/LeadingLep_pt, evtWeight );
    _cutFlow.at("02_Pt_ratio") += evtWeight;
    
    Get_LepLep_Variables(true, true);
    
    _histograms1D.at("LepLep_DM_before").Fill( LepLep_deltaM, evtWeight );
    if( !(LepLep_deltaM < 20) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _histograms1D.at("LepLep_DM_after").Fill( LepLep_deltaM, evtWeight );
    _cutFlow.at("03_LepLep_DM") += evtWeight;
    
    _histograms1D.at("LepLep_deltaPhi_before").Fill( LepLep_deltaPhi, evtWeight );
    if( !(LepLep_deltaPhi > 2.8) ) return false;
    _histograms1D.at("LepLep_deltaPhi_after").Fill( LepLep_deltaPhi, evtWeight );
    _cutFlow.at("04_LepLep_deltaPhi") += evtWeight;
    
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("05_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("06_Corrected") += evtWeight;
    

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPAnalysis::METXYSelection() {

    METXY::MET_x = MET_pt*cos( MET_phi );
    METXY::MET_y = MET_pt*sin( MET_phi );

    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //METXY::variable1Name = 100;      [Example]

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
void HEPAnalysis::METXYSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPAnalysis::FinishMETXY() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
