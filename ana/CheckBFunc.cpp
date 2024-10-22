#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace CheckBFunc{

    int Nelectrons;
    int Nmuons;  
    float LeadingEle_eta;
    
    vector<int> Muon_highPtId;
    vector<int> Muon_pfIsoId;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupCheckBFunc() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("LeadingEle_eta", &CheckBFunc::LeadingEle_eta );
    _outputTree->Branch("Nelectrons", &CheckBFunc::Nelectrons ); 
    _outputTree->Branch("Nmuons", &CheckBFunc::Nmuons );
    _outputTree->Branch("Nbjets", &Nbjets );
    _outputTree->Branch("Njets", &Njets );
    _outputTree->Branch("Dijet_pt", &Dijet_pt );
    _outputTree->Branch("Muon_highPtId", &CheckBFunc::Muon_highPtId );
    _outputTree->Branch("Muon_pfIsoId", &CheckBFunc::Muon_pfIsoId );
    
    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::CheckBFuncRegion() {

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
void HEPHero::CheckBFuncSelection() {

    LeptonSelection();
    Get_LeadingAndTrailing_Lepton_Variables();
    
    CheckBFunc::Nelectrons = selectedEle.size();
    CheckBFunc::Nmuons = selectedMu.size();
    
    JetSelection();
    Get_Dijet_Variables();
    
    CheckBFunc::Muon_highPtId.clear();
    CheckBFunc::Muon_pfIsoId.clear();
    
    for( unsigned int imu = 0; imu < nMuon; ++imu ) {
        CheckBFunc::Muon_highPtId.push_back(Muon_highPtId[imu]);
        CheckBFunc::Muon_pfIsoId.push_back(Muon_pfIsoId[imu]);
    }
    
    CheckBFunc::LeadingEle_eta = 0;
    if( RecoLepID == 11 ){ 
        CheckBFunc::LeadingEle_eta = LeadingLep_eta;
        //_outputTree->Fill();
    }
    
    //for( unsigned int iseljet = 0; iseljet < selectedJet.size(); ++iseljet )
    //for( unsigned int iselele = 0; iselele < selectedEle.size(); ++iselele )
    

    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //CheckBFunc::variable1Name = 100;      [Example]

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
void HEPHero::CheckBFuncSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishCheckBFunc() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
