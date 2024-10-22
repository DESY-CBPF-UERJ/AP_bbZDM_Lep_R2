#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace Ele24{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupEle24() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("24&25", 0) ); 
    _cutFlow.insert(pair<string,double>("!24&!25", 0) ); 
    _cutFlow.insert(pair<string,double>("24&!25", 0) ); 
    _cutFlow.insert(pair<string,double>("!24&25", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    makeHist( "Ele24", 2, 0., 2., 2, 0., 2., "Ele24",  "Ele25", "Events", "COLZ" );

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &Ele24::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::Ele24Region() {

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
void HEPHero::Ele24Selection() {


    _histograms2D.at("Ele24").Fill( HLT_Ele24_eta2p1_WPLoose_Gsf, HLT_Ele25_eta2p1_WPLoose_Gsf, 1 );

    
    if( HLT_Ele24_eta2p1_WPLoose_Gsf && HLT_Ele25_eta2p1_WPLoose_Gsf ){
        _cutFlow.at("24&25") += 1;
    }else if( !HLT_Ele24_eta2p1_WPLoose_Gsf && !HLT_Ele25_eta2p1_WPLoose_Gsf ){
        _cutFlow.at("!24&!25") += 1;
    }else if( HLT_Ele24_eta2p1_WPLoose_Gsf && !HLT_Ele25_eta2p1_WPLoose_Gsf ){
        _cutFlow.at("24&!25") += 1;
    }else if( !HLT_Ele24_eta2p1_WPLoose_Gsf && HLT_Ele25_eta2p1_WPLoose_Gsf ){
        _cutFlow.at("!24&25") += 1;
    }    









    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //Ele24::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    //_outputTree->Fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::Ele24Systematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishEle24() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
