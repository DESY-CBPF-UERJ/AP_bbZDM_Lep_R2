#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace DYMC{

    float type;
    float Z_pt;
    float Z_mass;
    
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupDYMC() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("Z_pt", &DYMC::Z_pt ); 
    _outputTree->Branch("Z_mass", &DYMC::Z_mass );
    _outputTree->Branch("type", &DYMC::type );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::DYMCRegion() {

    

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::DYMCSelection() {


    

    DYMC::Z_pt = -100;
    DYMC::Z_mass = -100;
    DYMC::type = 0;
    for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
        if( ((abs(GenPart_pdgId[ipart]) == 11) || (abs(GenPart_pdgId[ipart]) == 13) || (abs(GenPart_pdgId[ipart]) == 15)) &&
            ((GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 22) || (GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 23)) ){
            int boson_idx = GenPart_genPartIdxMother[ipart];
            DYMC::Z_pt = GenPart_pt[boson_idx];
            DYMC::Z_mass = GenPart_mass[boson_idx];
            DYMC::type = 1;
            break;
        }else if( ((abs(GenPart_pdgId[ipart]) == 11) || (abs(GenPart_pdgId[ipart]) == 13) || (abs(GenPart_pdgId[ipart]) == 15)) &&
            ((GenPart_genPartIdxMother[ipart] == 0) || (GenPart_genPartIdxMother[ipart] == 1)) ){
            TLorentzVector lep_1;
            TLorentzVector lep_2;
            lep_1.SetPtEtaPhiM(GenPart_pt[ipart], GenPart_eta[ipart], GenPart_phi[ipart], GenPart_mass[ipart]);
            lep_2.SetPtEtaPhiM(GenPart_pt[ipart+1], GenPart_eta[ipart+1], GenPart_phi[ipart+1], GenPart_mass[ipart+1]);
            DYMC::Z_pt = (lep_1+lep_2).Pt();
            DYMC::Z_mass = (lep_1+lep_2).M();
            DYMC::type = 2;
            break;
        }
    }








    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //DYMC::variable1Name = 100;      [Example]

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
void HEPHero::DYMCSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishDYMC() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
