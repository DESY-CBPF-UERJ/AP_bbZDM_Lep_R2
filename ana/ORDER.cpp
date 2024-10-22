#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace ORDER{

    int run_signal;
    int lumi_signal;
    int event_signal;

    int run_old = 0;
    int lumi_old = 0;
    int event_old = 0;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupORDER() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    makeHist( "run_signal", 4, -2., 2., "Signal", "Events" );
    makeHist( "lumi_signal", 4, -2., 2., "Signal", "Events" );
    makeHist( "event_signal", 4, -2., 2., "Signal", "Events" );
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &ORDER::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert("run_signal", &ORDER::run_signal );
    HDF_insert("lumi_signal", &ORDER::lumi_signal );
    HDF_insert("event_signal", &ORDER::event_signal );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::ORDERRegion() {

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
void HEPHero::ORDERSelection() {

    ORDER::run_signal = run - ORDER::run_old;
    ORDER::lumi_signal = luminosityBlock - ORDER::lumi_old;
    ORDER::event_signal = event - ORDER::event_old;

    if( ORDER::run_signal > 0 ){
        ORDER::run_signal = 1;
    }else if( ORDER::run_signal < 0 ){
        ORDER::run_signal = -1;
    }

    if( ORDER::lumi_signal > 0 ){
        ORDER::lumi_signal = 1;
    }else if( ORDER::lumi_signal < 0 ){
        ORDER::lumi_signal = -1;
    }

    if( ORDER::event_signal > 0 ){
        ORDER::event_signal = 1;
    }else if( ORDER::event_signal < 0 ){
        ORDER::event_signal = -1;
    }


    _histograms1D.at("run_signal").Fill( ORDER::run_signal, 1. );
    _histograms1D.at("lumi_signal").Fill( ORDER::lumi_signal, 1. );
    _histograms1D.at("event_signal").Fill( ORDER::event_signal, 1. );



    ORDER::run_old = run;
    ORDER::lumi_old = luminosityBlock;
    ORDER::event_old = event;




    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //ORDER::variable1Name = 100;      [Example]

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
void HEPHero::ORDERSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishORDER() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
