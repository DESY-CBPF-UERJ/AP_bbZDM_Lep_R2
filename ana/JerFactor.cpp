#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace JerFactor{

    float diff_jer_wgt;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupJerFactor() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch( "diff_jer_wgt", &JerFactor::diff_jer_wgt );

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert("variable1NameInTheTree", &JerFactor::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::JerFactorRegion() {

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
void HEPHero::JerFactorSelection() {




    if( apply_jer_corr && (dataset_group != "Data") ){
        
        for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
            TLorentzVector JetLV_before(0.,0.,0.,0.);
            JetLV_before.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
            
            float jet_pt = Jet_pt[ijet];
            float jet_eta = Jet_eta[ijet];
            float genjet_pt = GenJet_pt[Jet_genJetIdx[ijet]];
            int   genjet_idx = Jet_genJetIdx[ijet];
            
            jer_corr.SetVariablesandMatching( { {"JetPt",jet_pt}, {"JetEta",jet_eta}, {"Rho",fixedGridRhoFastjetAll}, {"GenJetPt", (genjet_idx>=0) ? genjet_pt : 0.} }, (genjet_idx>=0) ? true : false );
            double jer_factor_old = jer_corr.GetCorrection("nominal");
        
            //if( abs(Jet_eta[ijet]) >= 4.7 ) continue;
            double jer_PtRes = jet_JER_PtRes_corr->evaluate({jet_eta, jet_pt, fixedGridRhoFastjetAll});
            double jer_SF = jet_JER_SF_corr->evaluate({jet_eta, "nom"});
            
            bool isMatched = (genjet_idx>=0) ? true : false;
            double jer_factor;
            if( isMatched ){ 
                jer_factor = 1. + (jer_SF-1.)*(jet_pt - genjet_pt)/jet_pt;
            }else {
                TRandom random;
                //random.SetSeed();
                jer_factor = 1. + random.Gaus(0.,jer_PtRes)*TMath::Sqrt(TMath::Max(pow(jer_SF,2)-1.,0.));
            }
            
            bool isMatched_new = false;
            if( Jet_GenJet_match(ijet, 0.2) && (abs(jet_pt-genjet_pt) < 3*jer_PtRes*jet_pt) ) isMatched_new = true;
            
            double jer_factor_new;
            if( isMatched_new ){ 
                jer_factor_new = 1. + (jer_SF-1.)*(jet_pt - genjet_pt)/jet_pt;
            }else {
                TRandom random;
                //random.SetSeed();
                jer_factor_new = 1. + random.Gaus(0.,jer_PtRes)*TMath::Sqrt(TMath::Max(pow(jer_SF,2)-1.,0.));
            }
            
            JerFactor::diff_jer_wgt = abs(jer_factor - jer_factor_new);
            
            _outputTree->Fill();

        }
    }






    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //JerFactor::variable1Name = 100;      [Example]

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
void HEPHero::JerFactorSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishJerFactor() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
