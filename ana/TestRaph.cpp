#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace TestRaph{
    
    int Njets_loose;
    float MinDeltaR_leadingLep_jet_loose;
    float MinDeltaR_trailingLep_jet_loose;
    int Njets_tight;
    float MinDeltaR_leadingLep_jet_tight;
    float MinDeltaR_trailingLep_jet_tight;
    int Njets_tightlepveto;
    float MinDeltaR_leadingLep_jet_tightlepveto;
    float MinDeltaR_trailingLep_jet_tightlepveto;
    
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTestRaph() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("0_MET", 0) );
    _cutFlow.insert(pair<string,double>("1_METFilters", 0) );
    _cutFlow.insert(pair<string,double>("2_TwoLepOS", 0) ); 
    _cutFlow.insert(pair<string,double>("3_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("4_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("5_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("6_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("7_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("8_MET_LepLep_Mt", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("RecoLepID", &RecoLepID );
    _outputTree->Branch("LepLep_pt", &LepLep_pt );
    _outputTree->Branch("LepLep_phi", &LepLep_phi );
    _outputTree->Branch("LepLep_mass", &LepLep_mass );
    _outputTree->Branch("LepLep_daltaR", &LepLep_deltaR );
    _outputTree->Branch("LeadingLep_pt", &LeadingLep_pt );
    _outputTree->Branch("LeadingLep_eta", &LeadingLep_eta );
    _outputTree->Branch("TrailingLep_pt", &TrailingLep_pt );
    _outputTree->Branch("TrailingLep_eta", &TrailingLep_eta );
    _outputTree->Branch("MET_pt", &MET_pt );
    _outputTree->Branch("LepLep_deltaM", &LepLep_deltaM );
    _outputTree->Branch("MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    _outputTree->Branch("MET_LepLep_Mt", &MET_LepLep_Mt );
    _outputTree->Branch("Nbjets", &Nbjets );
    _outputTree->Branch("Nleptons", &Nleptons );

    _outputTree->Branch("Njets_loose", &TestRaph::Njets_loose );
    _outputTree->Branch("MinDeltaR_leadingLep_jet_loose", &TestRaph::MinDeltaR_leadingLep_jet_loose );
    _outputTree->Branch("MinDeltaR_trailingLep_jet_loose", &TestRaph::MinDeltaR_trailingLep_jet_loose );

    _outputTree->Branch("Njets_tight", &TestRaph::Njets_tight );
    _outputTree->Branch("MinDeltaR_leadingLep_jet_tight", &TestRaph::MinDeltaR_leadingLep_jet_tight );
    _outputTree->Branch("MinDeltaR_trailingLep_jet_tight", &TestRaph::MinDeltaR_trailingLep_jet_tight );

    _outputTree->Branch("Njets_tightlepveto", &TestRaph::Njets_tightlepveto );
    _outputTree->Branch("MinDeltaR_leadingLep_jet_tightlepveto", &TestRaph::MinDeltaR_leadingLep_jet_tightlepveto );
    _outputTree->Branch("MinDeltaR_trailingLep_jet_tightlepveto", &TestRaph::MinDeltaR_trailingLep_jet_tightlepveto );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TestRaphRegion() {

    //-------------------------------------------------------------------------
    // Cut description
    //-------------------------------------------------------------------------
    if( !(MET_pt > MET_CUT) ) return false; // MET > CUT
    _cutFlow.at("0_MET") += evtWeight;

    //if( !METFilters() ) return false;   // Selected MET Filters
    //_cutFlow.at("1_METFilters") += evtWeight;
    
    LeptonSelection();
    if( !(RecoLepID > 0) ) return false;                                        // Has exactly two reconstructed leptons with opposite signal
    _cutFlow.at("2_TwoLepOS") += evtWeight;  
    
    Get_LeadingAndTrailing_Lepton_Variables();
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("3_LeadingLep_Pt") += evtWeight;  
    
    Get_LepLep_Variables();
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("4_LepLep_DM") += evtWeight;  
    
    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("5_LepLep_Pt") += evtWeight;  
    
    if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Delta R between two leptons < CUT
    _cutFlow.at("6_LepLep_DR") += evtWeight;  
    
    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("7_MET_LepLep_DPhi") += evtWeight;  
    
    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("8_MET_LepLep_Mt") += evtWeight;

    // if( !TriggerSR() ) return false;    // Selected by SR triggers
    // _cutFlow.at("9_TriggerSR") += evtWeight;
    
    JetSelection();
    //GetVetoLeptons();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::TestRaphSelection() {

    float deltaR1 = 50;
    float deltaR2 = 50;
    float deltaR3 = 50;
    float deltaR4 = 50;
    float deltaR5 = 50;
    float deltaR6 = 50;

    std::vector<int> selectedJet_loose ;
    std::vector<int> selectedJet_tight ;
    std::vector<int> selectedJet_tightlepveto ;

    selectedJet_loose.clear();
    selectedJet_tight.clear();
    selectedJet_tightlepveto.clear();

    TestRaph::Njets_loose = 0;
    TestRaph::Njets_tight = 0;
    TestRaph::Njets_tightlepveto = 0;
    // fill vector for each jet ID
    for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
        if( abs(Jet_eta[ijet]) >= 2.4 ) continue;
        if( Jet_pt[ijet] <= 20 ) continue;

        if( Jet_jetId[ijet] >= 0 ){                        // changed -> Jet_jetId[ijet] >= 0
            TestRaph::Njets_loose += 1;
            selectedJet_loose.push_back(ijet);
        }
        if( Jet_jetId[ijet] >= 2 ){                        // changed -> Jet_jetId[ijet] >= 2
            TestRaph::Njets_tight += 1;
            selectedJet_tight.push_back(ijet);
        }
        if( Jet_jetId[ijet] >= 6 ){                        // changed -> Jet_jetId[ijet] >= 6
            TestRaph::Njets_tightlepveto += 1;
            selectedJet_tightlepveto.push_back(ijet);
        }  
    } 



    // -----------------------------------------
    
 
    for( unsigned int idjet = 0; idjet < selectedJet_loose.size(); ++idjet ) {

        int ijet = selectedJet_loose[idjet];

        float Delta_eta = 0.;
        float Delta_phi = 0.;
        //---------------------------------------
        // Min DeltaR Leading Lep - Jet
        //---------------------------------------
        // For Electron
        if( RecoLepID == 11 ){
            Delta_eta = abs( Electron_eta[IdxLeadingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Electron_phi[IdxLeadingLep] - Jet_phi[ijet] );
        }
        // For muon
        else if( RecoLepID == 13 ){
            Delta_eta = abs( Muon_eta[IdxLeadingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Muon_phi[IdxLeadingLep] - Jet_phi[ijet] );
        }
                
        if( Delta_phi > M_PI ) Delta_phi = 2*M_PI - Delta_phi;
        float deltaR_test = sqrt( pow(Delta_phi,2) + pow(Delta_eta,2) );

        
        if ( deltaR_test < deltaR1 ){
            deltaR1 = deltaR_test ;
        }

        //---------------------------------------
        // Min DeltaR Trailing Lep - Jet
        //---------------------------------------
        // For Electron
        if( RecoLepID == 11 ){
            Delta_eta = abs( Electron_eta[IdxTrailingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Electron_phi[IdxTrailingLep] - Jet_phi[ijet] );
        }
        // For muon
        else if( RecoLepID == 13 ){
            Delta_eta = abs( Muon_eta[IdxTrailingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Muon_phi[IdxTrailingLep] - Jet_phi[ijet] );
        }
                
        if( Delta_phi > M_PI ) Delta_phi = 2*M_PI - Delta_phi;
        float deltaR_test2 = sqrt( pow(Delta_phi,2) + pow(Delta_eta,2) );

        
        if ( deltaR_test2 < deltaR2 ){
            deltaR2 = deltaR_test2 ;
        }
    }

    //varaiveis declaradas no "".h"
    TestRaph::MinDeltaR_leadingLep_jet_loose = deltaR1;
    TestRaph::MinDeltaR_trailingLep_jet_loose = deltaR2;
    
    
    for( unsigned int idjet = 0; idjet < selectedJet_tight.size(); ++idjet ) {

        int ijet = selectedJet_tight[idjet];

        float Delta_eta = 0.;
        float Delta_phi = 0.;
        //---------------------------------------
        // Min DeltaR Leading Lep - Jet
        //---------------------------------------
        // For Electron
        if( RecoLepID == 11 ){
            Delta_eta = abs( Electron_eta[IdxLeadingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Electron_phi[IdxLeadingLep] - Jet_phi[ijet] );
        }
        // For muon
        else if( RecoLepID == 13 ){
            Delta_eta = abs( Muon_eta[IdxLeadingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Muon_phi[IdxLeadingLep] - Jet_phi[ijet] );
        }
                
        if( Delta_phi > M_PI ) Delta_phi = 2*M_PI - Delta_phi;
        float deltaR_test = sqrt( pow(Delta_phi,2) + pow(Delta_eta,2) );

        
        if ( deltaR_test < deltaR3 ){
            deltaR3 = deltaR_test ;
        }

        //---------------------------------------
        // Min DeltaR Trailing Lep - Jet
        //---------------------------------------
        // For Electron
        if( RecoLepID == 11 ){
            Delta_eta = abs( Electron_eta[IdxTrailingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Electron_phi[IdxTrailingLep] - Jet_phi[ijet] );
        }
        // For muon
        else if( RecoLepID == 13 ){
            Delta_eta = abs( Muon_eta[IdxTrailingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Muon_phi[IdxTrailingLep] - Jet_phi[ijet] );
        }
                
        if( Delta_phi > M_PI ) Delta_phi = 2*M_PI - Delta_phi;
        float deltaR_test2 = sqrt( pow(Delta_phi,2) + pow(Delta_eta,2) );

        
        if ( deltaR_test2 < deltaR4 ){
            deltaR4 = deltaR_test2 ;
        }
    }

    TestRaph::MinDeltaR_leadingLep_jet_tight = deltaR3;
    TestRaph::MinDeltaR_trailingLep_jet_tight = deltaR4;
    
    for( unsigned int idjet = 0; idjet < selectedJet_tightlepveto.size(); ++idjet ) {

        int ijet = selectedJet_tightlepveto[idjet];

        float Delta_eta = 0.;
        float Delta_phi = 0.;
        //---------------------------------------
        // Min DeltaR Leading Lep - Jet
        //---------------------------------------
        // For Electron
        if( RecoLepID == 11 ){
            Delta_eta = abs( Electron_eta[IdxLeadingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Electron_phi[IdxLeadingLep] - Jet_phi[ijet] );
        }
        // For muon
        else if( RecoLepID == 13 ){
            Delta_eta = abs( Muon_eta[IdxLeadingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Muon_phi[IdxLeadingLep] - Jet_phi[ijet] );
        }
                
        if( Delta_phi > M_PI ) Delta_phi = 2*M_PI - Delta_phi;
        float deltaR_test = sqrt( pow(Delta_phi,2) + pow(Delta_eta,2) );

        
        if ( deltaR_test < deltaR5 ){
            deltaR5 = deltaR_test ;
        }

        //---------------------------------------
        // Min DeltaR Trailing Lep - Jet
        //---------------------------------------
        // For Electron
        if( RecoLepID == 11 ){
            Delta_eta = abs( Electron_eta[IdxTrailingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Electron_phi[IdxTrailingLep] - Jet_phi[ijet] );
        }
        // For muon
        else if( RecoLepID == 13 ){
            Delta_eta = abs( Muon_eta[IdxTrailingLep] - Jet_eta[ijet] );
            Delta_phi = abs( Muon_phi[IdxTrailingLep] - Jet_phi[ijet] );
        }
                
        if( Delta_phi > M_PI ) Delta_phi = 2*M_PI - Delta_phi;
        float deltaR_test2 = sqrt( pow(Delta_phi,2) + pow(Delta_eta,2) );

        
        if ( deltaR_test2 < deltaR6 ){
            deltaR6 = deltaR_test2 ;
        }
    }

    TestRaph::MinDeltaR_leadingLep_jet_tightlepveto = deltaR5;
    TestRaph::MinDeltaR_trailingLep_jet_tightlepveto = deltaR6;






    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //TestRaph::variable1Name = 100;      [Example]

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
void HEPHero::TestRaphSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTestRaph() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
