#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace WZGen{

    
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupWZGen() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("N_Zdaughters", 0) ); 
    _cutFlow.insert(pair<string,double>("N_Zee", 0) ); 
    _cutFlow.insert(pair<string,double>("N_Zmumu", 0) );
    _cutFlow.insert(pair<string,double>("N_Ztautau", 0) );
    _cutFlow.insert(pair<string,double>("N_Znuenue", 0) );
    _cutFlow.insert(pair<string,double>("N_Znumunumu", 0) );
    _cutFlow.insert(pair<string,double>("N_Znutaunutau", 0) );
    _cutFlow.insert(pair<string,double>("N_Zothers", 0) );
    
    _cutFlow.insert(pair<string,double>("N_Wdaughters", 0) ); 
    _cutFlow.insert(pair<string,double>("N_Wenue", 0) ); 
    _cutFlow.insert(pair<string,double>("N_Wmunumu", 0) );
    _cutFlow.insert(pair<string,double>("N_Wtaunutau", 0) );
    _cutFlow.insert(pair<string,double>("N_Wothers", 0) );
    
    _cutFlow.insert(pair<string,double>("N_WZ_3l1nu", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &WZGen::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert("variable1NameInTheTree", &WZGen::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::WZGenRegion() {

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
void HEPHero::WZGenSelection() {


    vector<int> Z_daughters;
    vector<int> W_daughters;
    for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
        if( (GenPart_pdgId[ipart] != 23) && (GenPart_pdgId[ipart] != 22) && (GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 23) ){ 
            _cutFlow.at("N_Zdaughters") += 1;
            Z_daughters.push_back(ipart);
        }
        if( (abs(GenPart_pdgId[ipart]) != 24) && (GenPart_pdgId[ipart] != 22) && (abs(GenPart_pdgId[GenPart_genPartIdxMother[ipart]]) == 24) ){ 
            _cutFlow.at("N_Wdaughters") += 1;
            W_daughters.push_back(ipart);
        }
    }

    if( (W_daughters.size() > 2) ){
        for( unsigned int idau = 0; idau < W_daughters.size(); ++idau ) {
            int ipart = W_daughters[idau];
            cout << GenPart_pdgId[ipart] << " ";
        }
        cout << endl;
    }
    
    if( (Z_daughters.size() == 2) && (W_daughters.size() == 2) ){
    //if( (Z_daughters.size() == 2) ){
        int Z_decay;
        if( (abs(GenPart_pdgId[Z_daughters[0]]) == 11) && (abs(GenPart_pdgId[Z_daughters[1]]) == 11) ){
            Z_decay = 1;
            _cutFlow.at("N_Zee") += 1;
        }else if( (abs(GenPart_pdgId[Z_daughters[0]]) == 13) && (abs(GenPart_pdgId[Z_daughters[1]]) == 13) ){
            Z_decay = 2;
            _cutFlow.at("N_Zmumu") += 1;
        }else if( (abs(GenPart_pdgId[Z_daughters[0]]) == 15) && (abs(GenPart_pdgId[Z_daughters[1]]) == 15) ){
            Z_decay = 3;
            _cutFlow.at("N_Ztautau") += 1;
        }else if( (abs(GenPart_pdgId[Z_daughters[0]]) == 12) && (abs(GenPart_pdgId[Z_daughters[1]]) == 12) ){
            Z_decay = 4;
            _cutFlow.at("N_Znuenue") += 1;
        }else if( (abs(GenPart_pdgId[Z_daughters[0]]) == 14) && (abs(GenPart_pdgId[Z_daughters[1]]) == 14) ){
            Z_decay = 5;
            _cutFlow.at("N_Znumunumu") += 1;
        }else if( (abs(GenPart_pdgId[Z_daughters[0]]) == 16) && (abs(GenPart_pdgId[Z_daughters[1]]) == 16) ){
            Z_decay = 6;
            _cutFlow.at("N_Znutaunutau") += 1;
        }else{
            Z_decay = 0;
            _cutFlow.at("N_Zothers") += 1;
        }
        
        
        int W_decay;
        if( ((abs(GenPart_pdgId[W_daughters[0]]) == 11) && (abs(GenPart_pdgId[W_daughters[1]]) == 12)) || ((abs(GenPart_pdgId[W_daughters[0]]) == 12) && (abs(GenPart_pdgId[W_daughters[1]]) == 11)) ){
            W_decay = 1;
            _cutFlow.at("N_Wenue") += 1;
        }else if( ((abs(GenPart_pdgId[W_daughters[0]]) == 13) && (abs(GenPart_pdgId[W_daughters[1]]) == 14)) || ((abs(GenPart_pdgId[W_daughters[0]]) == 14) && (abs(GenPart_pdgId[W_daughters[1]]) == 13)) ){
            W_decay = 2;
            _cutFlow.at("N_Wmunumu") += 1;
        }else if( ((abs(GenPart_pdgId[W_daughters[0]]) == 15) && (abs(GenPart_pdgId[W_daughters[1]]) == 16)) || ((abs(GenPart_pdgId[W_daughters[0]]) == 16) && (abs(GenPart_pdgId[W_daughters[1]]) == 15)) ){
            W_decay = 3;
            _cutFlow.at("N_Wtaunutau") += 1;
        }else{
            W_decay = 0;
            _cutFlow.at("N_Wothers") += 1;
        }


        if( (Z_decay >= 1) && (Z_decay <= 3) && (W_decay >= 1) && (W_decay <= 3) ){
            _cutFlow.at("N_WZ_3l1nu") += 1;
        }
        
    }










    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //WZGen::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    //_outputTree->Fill();

    //======FILL THE OUTPUT HDF5 INFO===============================================================
    //HDF_fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::WZGenSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishWZGen() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
