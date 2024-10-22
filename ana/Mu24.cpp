#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace Mu24{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupMu24() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("Iso&IsoTk", 0) ); 
    _cutFlow.insert(pair<string,double>("!Iso&!IsoTk", 0) ); 
    _cutFlow.insert(pair<string,double>("Iso&!IsoTk", 0) ); 
    _cutFlow.insert(pair<string,double>("!Iso&IsoTk", 0) ); 

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    makeHist( "Mu24", 2, 0., 2., 2, 0., 2., "IsoMu24",  "IsoTkMu24", "Events", "COLZ" );

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &Mu24::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::Mu24Region() {

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
void HEPHero::Mu24Selection() {




    _histograms2D.at("Mu24").Fill( HLT_IsoMu24, HLT_IsoTkMu24, 1 );

    
    if( HLT_IsoMu24 && HLT_IsoTkMu24 ){
        _cutFlow.at("Iso&IsoTk") += 1;
    }else if( !HLT_IsoMu24 && !HLT_IsoTkMu24 ){
        _cutFlow.at("!Iso&!IsoTk") += 1;
    }else if( HLT_IsoMu24 && !HLT_IsoTkMu24 ){
        _cutFlow.at("Iso&!IsoTk") += 1;
    }else if( !HLT_IsoMu24 && HLT_IsoTkMu24 ){
        _cutFlow.at("!Iso&IsoTk") += 1;
    }





    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //Mu24::variable1Name = 100;      [Example]

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
void HEPHero::Mu24Systematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishMu24() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
