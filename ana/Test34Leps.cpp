#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace Test34Leps{

    int Nelectrons;
    int Nmuons;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTest34Leps() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("RecoLepID", &RecoLepID ); 
    _outputTree->Branch("Nmuons", &Test34Leps::Nmuons ); 
    _outputTree->Branch("Nelectrons", &Test34Leps::Nelectrons ); 
    _outputTree->Branch("IdxLeadingLep", &IdxLeadingLep ); 
    _outputTree->Branch("IdxTrailingLep", &IdxTrailingLep ); 
    _outputTree->Branch("IdxThirdLep", &IdxThirdLep ); 
    _outputTree->Branch("IdxFourthLep", &IdxFourthLep ); 

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::Test34LepsRegion() {

    //-------------------------------------------------------------------------
    // Cut description
    //-------------------------------------------------------------------------
    //if( !(CutCondition) ) return false;           [Example]
    //_cutFlow.at("CutName") += evtWeight;          [Example]

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::Test34LepsSelection() {



    LeptonSelection();
    Get_LeadingAndTrailing_Lepton_Variables();
    
    Test34Leps::Nmuons = selectedMu.size();
    Test34Leps::Nelectrons = selectedEle.size();


    if( RecoLepID == 31301 ){
        cout << "Nelectrons = " <<  Test34Leps::Nelectrons << endl;
        cout << "Nmuons = " <<  Test34Leps::Nmuons << endl;
    }



    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //Test34Leps::variable1Name = 100;      [Example]

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
void HEPHero::Test34LepsSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTest34Leps() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
