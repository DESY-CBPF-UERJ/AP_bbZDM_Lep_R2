#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace ZZGen{

    
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupZZGen() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("N_Zdaughters", 0) ); 
    _cutFlow.insert(pair<string,double>("N_Zee", 0) ); 
    _cutFlow.insert(pair<string,double>("N_Zmumu", 0) );
    _cutFlow.insert(pair<string,double>("N_Ztautau", 0) );
    _cutFlow.insert(pair<string,double>("N_Znuenue", 0) );
    _cutFlow.insert(pair<string,double>("N_Znumunumu", 0) );
    _cutFlow.insert(pair<string,double>("N_Znutaunutau", 0) );
    _cutFlow.insert(pair<string,double>("N_Zothers", 0) );
    _cutFlow.insert(pair<string,double>("N_ZZ_4l", 0) );
    _cutFlow.insert(pair<string,double>("N_ZZ_2l2nu", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &ZZGen::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert("variable1NameInTheTree", &ZZGen::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::ZZGenRegion() {

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
void HEPHero::ZZGenSelection() {

    vector<int> daughters;
    for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
        if( (abs(GenPart_pdgId[ipart]) >= 11) && (abs(GenPart_pdgId[ipart]) <= 16) && (GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 23) ){ 
            _cutFlow.at("N_Zdaughters") += 1;
            daughters.push_back(ipart);
        }
    }

    if( daughters.size() == 4 ){
        int Z1_decay;
        if( (abs(GenPart_pdgId[daughters[0]]) == 11) && (abs(GenPart_pdgId[daughters[1]]) == 11) ){
            Z1_decay = 1;
            _cutFlow.at("N_Zee") += 1;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 13) && (abs(GenPart_pdgId[daughters[1]]) == 13) ){
            Z1_decay = 2;
            _cutFlow.at("N_Zmumu") += 1;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 15) && (abs(GenPart_pdgId[daughters[1]]) == 15) ){
            Z1_decay = 3;
            _cutFlow.at("N_Ztautau") += 1;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 12) && (abs(GenPart_pdgId[daughters[1]]) == 12) ){
            Z1_decay = 4;
            _cutFlow.at("N_Znuenue") += 1;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 14) && (abs(GenPart_pdgId[daughters[1]]) == 14) ){
            Z1_decay = 5;
            _cutFlow.at("N_Znumunumu") += 1;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 16) && (abs(GenPart_pdgId[daughters[1]]) == 16) ){
            Z1_decay = 6;
            _cutFlow.at("N_Znutaunutau") += 1;
        }else{
            Z1_decay = 0;
            _cutFlow.at("N_Zothers") += 1;
        }
        

        int Z2_decay;
        if( (abs(GenPart_pdgId[daughters[2]]) == 11) && (abs(GenPart_pdgId[daughters[3]]) == 11) ){
            Z2_decay = 1;
            _cutFlow.at("N_Zee") += 1;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 13) && (abs(GenPart_pdgId[daughters[3]]) == 13) ){
            Z2_decay = 2;
            _cutFlow.at("N_Zmumu") += 1;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 15) && (abs(GenPart_pdgId[daughters[3]]) == 15) ){
            Z2_decay = 3;
            _cutFlow.at("N_Ztautau") += 1;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 12) && (abs(GenPart_pdgId[daughters[3]]) == 12) ){
            Z2_decay = 4;
            _cutFlow.at("N_Znuenue") += 1;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 14) && (abs(GenPart_pdgId[daughters[3]]) == 14) ){
            Z2_decay = 5;
            _cutFlow.at("N_Znumunumu") += 1;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 16) && (abs(GenPart_pdgId[daughters[3]]) == 16) ){
            Z2_decay = 6;
            _cutFlow.at("N_Znutaunutau") += 1;
        }else{
            Z2_decay = 0;
            _cutFlow.at("N_Zothers") += 1;
        }


        if( (Z1_decay >= 1) && (Z1_decay <= 3) && (Z2_decay >= 1) && (Z2_decay <= 3) ){
            _cutFlow.at("N_ZZ_4l") += 1;
        }else if( ((Z1_decay >= 1) && (Z1_decay <= 3) && (Z2_decay >= 4) && (Z2_decay <= 6)) || ((Z1_decay >= 4) && (Z1_decay <= 6) && (Z2_decay >= 1) && (Z2_decay <= 3)) ){
            _cutFlow.at("N_ZZ_2l2nu") += 1;
        }
    }

   


    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //ZZGen::variable1Name = 100;      [Example]

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
void HEPHero::ZZGenSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishZZGen() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
