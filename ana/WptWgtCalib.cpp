#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace WptWgtCalib{

    float W_charge;
    float W_pt;
    float W_pt_wgt;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupWptWgtCalib() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &WptWgtCalib::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "W_charge", &WptWgtCalib::W_charge );
    HDF_insert( "W_pt", &WptWgtCalib::W_pt );
    HDF_insert( "W_pt_wgt", &WptWgtCalib::W_pt_wgt );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::WptWgtCalibRegion() {

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
void HEPHero::WptWgtCalibSelection() {


    vector<int> W_daughters;
    for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
        if( (abs(GenPart_pdgId[ipart]) >= 11) && (abs(GenPart_pdgId[ipart]) <= 16) && (abs(GenPart_pdgId[GenPart_genPartIdxMother[ipart]]) == 24) ){
            W_daughters.push_back(ipart);
        }
    }

    if( W_daughters.size() == 4 ){
        if( (abs(GenPart_pdgId[W_daughters[0]]) == 11) && (abs(GenPart_pdgId[W_daughters[1]]) == 11) ) W_daughters.erase(W_daughters.begin(),W_daughters.begin()+2);
        if( (abs(GenPart_pdgId[W_daughters[2]]) == 11) && (abs(GenPart_pdgId[W_daughters[3]]) == 11) ) W_daughters.erase(W_daughters.begin()+2,W_daughters.begin()+4);
    }


    WptWgtCalib::W_charge = -1.;
    if( (GenPart_pdgId[W_daughters[0]] == -11) || (GenPart_pdgId[W_daughters[0]] == -13) || (GenPart_pdgId[W_daughters[0]] == -15) || (GenPart_pdgId[W_daughters[1]] == -11) || (GenPart_pdgId[W_daughters[1]] == -13) || (GenPart_pdgId[W_daughters[1]] == -15) ){
        WptWgtCalib::W_charge = 1.;
    }

    TLorentzVector W_dau1;
    TLorentzVector W_dau2;
    W_dau1.SetPtEtaPhiM(GenPart_pt[W_daughters[0]], GenPart_eta[W_daughters[0]], GenPart_phi[W_daughters[0]], GenPart_mass[W_daughters[0]]);
    W_dau2.SetPtEtaPhiM(GenPart_pt[W_daughters[1]], GenPart_eta[W_daughters[1]], GenPart_phi[W_daughters[1]], GenPart_mass[W_daughters[1]]);

    TLorentzVector W_part = W_dau1 + W_dau2;

    WptWgtCalib::W_pt = W_part.Pt();

    vector<float> intervals;
    vector<float> k_factors;


    //---pT(W)-----------------------------------
    if( WptWgtCalib::W_charge > 0 ){
        intervals = {25, 35, 50, 70, 90, 110, 130, 160, 200, 30000};
        k_factors = {0.96773208, 0.99251315, 1.00993565, 1.02432459, 1.03293558, 1.05039964, 1.0413764, 1.07347302, 1.08221239};
    }else{
        intervals = {25, 35, 50, 70, 90, 110, 130, 160, 200, 30000};
        k_factors = {0.93163252, 0.98802125, 1.01016316, 1.03190894, 1.04062135, 1.07619106, 1.14103155, 1.13462869, 1.24623567};
    }

    int idx = -1;
    for (unsigned int i = 0; i < k_factors.size() ; i++){
        if ( WptWgtCalib::W_pt >= intervals[i] && WptWgtCalib::W_pt < intervals[i+1]  ){
            idx = i;
            break;
        }
    }

    WptWgtCalib::W_pt_wgt = 1.;
    if( idx >= 0 ) WptWgtCalib::W_pt_wgt = k_factors[idx];



    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //WptWgtCalib::variable1Name = 100;      [Example]

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
void HEPHero::WptWgtCalibSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishWptWgtCalib() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
