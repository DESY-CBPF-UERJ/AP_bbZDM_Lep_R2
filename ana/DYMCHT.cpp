#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace DYMCHT{

    float genHT; 
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupDYMCHT() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    /*
    makeHist( "ID_idx_0", 100, 0., 100., "ID", "Events" ); 
    makeHist( "ID_idx_1", 100, 0., 100., "ID", "Events" ); 
    makeHist( "ID_idx_2", 100, 0., 100., "ID", "Events" ); 
    makeHist( "ID_idx_3", 100, 0., 100., "ID", "Events" ); 
    makeHist( "ID_idx_4", 100, 0., 100., "ID", "Events" ); 
    makeHist( "ID_idx_5", 100, 0., 100., "ID", "Events" ); 
    makeHist( "ID_idx_6", 100, 0., 100., "ID", "Events" ); 
    makeHist( "ID_idx_7", 100, 0., 100., "ID", "Events" );
    makeHist( "Mother_idx_0", 10, 0., 10., "ID", "Events" ); 
    makeHist( "Mother_idx_1", 10, 0., 10., "ID", "Events" ); 
    makeHist( "Mother_idx_2", 10, 0., 10., "ID", "Events" ); 
    makeHist( "Mother_idx_3", 10, 0., 10., "ID", "Events" ); 
    makeHist( "Mother_idx_4", 10, 0., 10., "ID", "Events" ); 
    makeHist( "Mother_idx_5", 10, 0., 10., "ID", "Events" ); 
    makeHist( "Mother_idx_6", 10, 0., 10., "ID", "Events" ); 
    makeHist( "Mother_idx_7", 10, 0., 10., "ID", "Events" );
    makeHist( "Flags", 15, 0., 15., "Status", "Events" )
    */
    makeHist( "status", 100, 0., 100., "Status", "Partons" );
    makeHist( "flags", 100, 0., 100., "Flags", "Partons" );
    makeHist( "status_vs_pt", 100, 0., 100., 50, 0., 250., "Status",  "Pt [GeV]", "Partons", "COLZ" ); 

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("genHT", &DYMCHT::genHT ); 
    _outputTree->Branch("genWeight", &genWeight );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::DYMCHTRegion() {

    if( !DYJetsToLL_processing() ) return false;
    
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
void HEPHero::DYMCHTSelection() {


    DYMCHT::genHT = 0.;
    for( size_t ipart = 0; ipart < nGenPart; ++ipart ) {
        if( GenPart_status[ipart] != 23 ) continue;
        if( (GenPart_pdgId[ipart] != 21) && (abs(GenPart_pdgId[ipart]) > 6) ) continue;
        //int imother = GenPart_genPartIdxMother[ipart];
        //if( imother > 0 ) continue;
        // Replace mother by status
        //if( (GenPart_pdgId[imother] == 21) && (abs(GenPart_pdgId[imother]) <= 6) && (imother > 1) ) continue;
        //if( GenPart_statusFlags[ipart] != 9 ) continue;
        DYMCHT::genHT += GenPart_pt[ipart];
        //_histograms1D.at("flags").Fill( GenPart_statusFlags[ipart], 1 );
        //_histograms1D.at("status").Fill( GenPart_status[ipart], 1 );
        //_histograms2D.at("status_vs_pt").Fill( GenPart_status[ipart], GenPart_pt[ipart], 1 );
    }

    /*
    Handle<edm::View<reco::GenParticle> > pruned;
   if( !_isData ) {
     iEvent.getByToken(prunedGenToken_, pruned);
     GenLevel_HT = 0.;
     for( size_t i = 0; i < pruned->size(); ++i ) {
       if( (*pruned)[i].status() != 23 ) continue;
       if( (*pruned)[i].pdgId() != 21 && abs((*pruned)[i].pdgId()) > 6 ) continue;
       GenLevel_HT += (*pruned)[i].pt();
     }
   }
    */




    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //DYMCHT::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    /*
    _histograms1D.at("ID_idx_0").Fill( GenPart_pdgId[0], 1 );  
    _histograms1D.at("ID_idx_1").Fill( GenPart_pdgId[1], 1 ); 
    _histograms1D.at("ID_idx_2").Fill( GenPart_pdgId[2], 1 ); 
    _histograms1D.at("ID_idx_3").Fill( GenPart_pdgId[3], 1 ); 
    _histograms1D.at("ID_idx_4").Fill( GenPart_pdgId[4], 1 ); 
    _histograms1D.at("ID_idx_5").Fill( GenPart_pdgId[5], 1 ); 
    _histograms1D.at("ID_idx_6").Fill( GenPart_pdgId[6], 1 ); 
    _histograms1D.at("ID_idx_7").Fill( GenPart_pdgId[7], 1 );
    _histograms1D.at("Mother_idx_0").Fill( GenPart_genPartIdxMother[0], 1 );  
    _histograms1D.at("Mother_idx_1").Fill( GenPart_genPartIdxMother[1], 1 ); 
    _histograms1D.at("Mother_idx_2").Fill( GenPart_genPartIdxMother[2], 1 ); 
    _histograms1D.at("Mother_idx_3").Fill( GenPart_genPartIdxMother[3], 1 ); 
    _histograms1D.at("Mother_idx_4").Fill( GenPart_genPartIdxMother[4], 1 ); 
    _histograms1D.at("Mother_idx_5").Fill( GenPart_genPartIdxMother[5], 1 ); 
    _histograms1D.at("Mother_idx_6").Fill( GenPart_genPartIdxMother[6], 1 ); 
    _histograms1D.at("Mother_idx_7").Fill( GenPart_genPartIdxMother[7], 1 );
    */
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    _outputTree->Fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::DYMCHTSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishDYMCHT() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
