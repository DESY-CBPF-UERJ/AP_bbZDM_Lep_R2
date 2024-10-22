#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace TTJES{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTTJES() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_Njets", 0) );
    _cutFlow.insert(pair<string,double>("02_Nbjets", 0) );
    _cutFlow.insert(pair<string,double>("03_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("05_Selected", 0) );
    _cutFlow.insert(pair<string,double>("06_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    sys_regions = { }; // Use the same IDs produced by the Regions() function. Empty vector means that all events will be used.
    makeSysHist( "MET_pt", 90, 0., 600., "MET [GeV]", "Number of events");

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &TTJES::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "LepLep_mass", &LepLep_mass );
    HDF_insert( "Nbjets", &Nbjets );
    HDF_insert( "Njets30", &Njets30 );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TTJESRegion() {

    LeptonSelection();
    
    if( !((RecoLepID > 0) && (RecoLepID < 100)) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    JetSelection();
    METCorrection();
    
    if( !(Njets30 >= 1) ) return false;                                     
    _cutFlow.at("01_Njets") += evtWeight;
    
    if( !(Nbjets >= 1) ) return false;                                    
    _cutFlow.at("02_Nbjets") += evtWeight;
    
    Get_Leptonic_Info(true, true);
    
    if( !(LeadingLep_pt > 25) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("03_LeadingLep_Pt") += evtWeight; 
    
    Get_LepLep_Variables(true, true);
    
    if( !((LepLep_deltaM > 15) && (LepLep_mass > 20)) ) return false;          // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("04_LepLep_DM") += evtWeight; 
     
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
void HEPHero::TTJESSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //TTJES::variable1Name = 100;      [Example]

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
void HEPHero::TTJESSystematic() {

    FillSystematic( "MET_pt", MET_pt, evtWeight );
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTTJES() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
