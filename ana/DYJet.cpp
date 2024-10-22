#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace DYJet{

    float Jet_LepLep_deltaR;
    float Jet_area;
    float Jet_chEmEF;
    float Jet_chHEF;
    float Jet_eta;
    float Jet_mass;
    float Jet_neHEF;
    float Jet_pt;
    int   Jet_nConstituents;
    bool  Jet_matched;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupDYJet() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("0_ISR_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("1_ISR_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("2_ISR_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("3_ISR_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("4g_ISR_Jets", 0) ); 
    
    _cutFlow.insert(pair<string,double>("0_RECO_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("1_RECO_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("2_RECO_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("3_RECO_Jets", 0) ); 
    _cutFlow.insert(pair<string,double>("4g_RECO_Jets", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &DYJet::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert("Jet_LepLep_deltaR", &DYJet::Jet_LepLep_deltaR );
    HDF_insert("Jet_area", &DYJet::Jet_area );
    HDF_insert("Jet_eta", &DYJet::Jet_eta );
    HDF_insert("Jet_pt", &DYJet::Jet_pt );
    HDF_insert("Jet_mass", &DYJet::Jet_mass );
    HDF_insert("Jet_chEmEF", &DYJet::Jet_chEmEF );
    HDF_insert("Jet_chHEF", &DYJet::Jet_chHEF );
    HDF_insert("Jet_neHEF", &DYJet::Jet_neHEF );
    HDF_insert("Jet_nConstituents", &DYJet::Jet_nConstituents );
    HDF_insert("Jet_matched", &DYJet::Jet_matched );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::DYJetRegion() {

    //-------------------------------------------------------------------------
    // Cut description
    //-------------------------------------------------------------------------
    //if( !(CutCondition) ) return false;           [Example]
    //_cutFlow.at("CutName") += evtWeight;          [Example]
    
    LeptonSelection();
    JetSelection();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::DYJetSelection() {

    
    float Num_ISR_parts = 0;
    vector<int> ISR_part;
    for( size_t ipart = 0; ipart < nGenPart; ++ipart ) {
        //cout << "ipart: " << ipart << ", PDG_ID: " << GenPart_pdgId[ipart] << ", Mother_ID: " << GenPart_genPartIdxMother[ipart] << endl;
        if( GenPart_genPartIdxMother[ipart] != 0 ) continue;
        if( (GenPart_pdgId[ipart] != 21) && (abs(GenPart_pdgId[ipart]) > 6) ) continue;
        Num_ISR_parts += 1;
        ISR_part.push_back(ipart);
    }
    //cout << "  " << endl;
    //cout << Num_ISR_parts << endl;
    
    if( Num_ISR_parts == 0 ){
        _cutFlow.at("0_ISR_Jets") += 1;
    }else if( Num_ISR_parts == 1 ){
        _cutFlow.at("1_ISR_Jets") += 1;
    }else if( Num_ISR_parts == 2 ){
        _cutFlow.at("2_ISR_Jets") += 1;
    }else if( Num_ISR_parts == 3 ){
        _cutFlow.at("3_ISR_Jets") += 1;
    }else if( Num_ISR_parts >= 4 ){
        _cutFlow.at("4g_ISR_Jets") += 1;
    }

    //==============================================================================================
    
    
    if( (ISR_part.size() == 0) || (ISR_part.size() >= 4) ) return;
    
    vector<int> ISR_matched;
    vector<int> Jet_matched;
    float DR_cut = 0.4;
    
    //for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
    for( unsigned int iselJet = 0; iselJet < selectedJet.size(); ++iselJet ) { 
        int ijet = selectedJet[iselJet];
        
        int part_matched = -1;
        float DR_min = 9999999.;
        
        bool part0_is_matched = false;
        for( unsigned int imat = 0; imat < ISR_matched.size() ; ++imat ) {
            if( ISR_part[0] == ISR_matched[imat] ) part0_is_matched = true;
        }
        if( !part0_is_matched ){
            int ipart = ISR_part[0];
            float DEta = Jet_eta[ijet] - GenPart_eta[ipart];
            float DPhi = Jet_phi[ijet] - GenPart_phi[ipart];
            if( DPhi > M_PI ) DPhi = 2*M_PI - DPhi;
            float DR = sqrt(pow(DEta,2) + pow(DPhi,2));
            if( DR < DR_cut ){ 
                part_matched = ipart;
                DR_min = DR;
            }
        }
        
        if( ISR_part.size() >= 2 ){
            bool part1_is_matched = false;
            for( unsigned int imat = 0; imat < ISR_matched.size() ; ++imat ) {
                if( ISR_part[1] == ISR_matched[imat] ) part1_is_matched = true;
            }
            if( !part1_is_matched ){
                int ipart = ISR_part[1];
                float DEta = Jet_eta[ijet] - GenPart_eta[ipart];
                float DPhi = Jet_phi[ijet] - GenPart_phi[ipart];
                if( DPhi > M_PI ) DPhi = 2*M_PI - DPhi;
                float DR = sqrt(pow(DEta,2) + pow(DPhi,2));
                if( (DR < DR_cut) && (DR < DR_min) ){ 
                    part_matched = ipart;
                    DR_min = DR;
                }
            }
        }
        
        if( ISR_part.size() == 3 ){
            bool part2_is_matched = false;
            for( unsigned int imat = 0; imat < ISR_matched.size() ; ++imat ) {
                if( ISR_part[2] == ISR_matched[imat] ) part2_is_matched = true;
            }
            if( !part2_is_matched ){
                int ipart = ISR_part[2];
                float DEta = Jet_eta[ijet] - GenPart_eta[ipart];
                float DPhi = Jet_phi[ijet] - GenPart_phi[ipart];
                if( DPhi > M_PI ) DPhi = 2*M_PI - DPhi;
                float DR = sqrt(pow(DEta,2) + pow(DPhi,2));
                if( (DR < DR_cut) && (DR < DR_min) ){ 
                    part_matched = ipart;
                    DR_min = DR;
                }
            }
        }
        
        if( part_matched >= 0 ){ 
            ISR_matched.push_back(part_matched);
            Jet_matched.push_back(ijet);
        }
        if( ISR_matched.size() == ISR_part.size() ) break;
        
    }
    
    if( Jet_matched.size() == 0 ){
        _cutFlow.at("0_RECO_Jets") += 1;
    }else if( Jet_matched.size() == 1 ){
        _cutFlow.at("1_RECO_Jets") += 1;
    }else if( Jet_matched.size() == 2 ){
        _cutFlow.at("2_RECO_Jets") += 1;
    }else if( Jet_matched.size() == 3 ){
        _cutFlow.at("3_RECO_Jets") += 1;
    }else if( Jet_matched.size() >= 4 ){
        _cutFlow.at("4g_RECO_Jets") += 1;
    }
    

    //==============================================================================================
    
    if( !((RecoLepID > 0) && (RecoLepID < 100)) ) return;                      // Has two reconstructed leptons with opposite signal
    
    METCorrection();
    Get_Leptonic_Info(true, true);
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return;                   // Leading lepton pt > CUT
    
    Get_LepLep_Variables(true, true);
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    
    if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return;                        // Upper cut in LepLep Delta R 
    
    if( !METFilters() ) return;                                           // Selected by MET filters
    
    if( !Trigger() ) return;                                              // Selected by triggers
    
    Weight_corrections();
    
    
    vector<bool> isMatchedJet;
    for( unsigned int iselJet = 0; iselJet < selectedJet.size(); ++iselJet ) {
        bool isMatched = false;
        for( unsigned int imatJet = 0; imatJet < Jet_matched.size() ; ++imatJet ) {
            if( selectedJet[iselJet] == Jet_matched[imatJet] ) isMatched = true;
        }
        if( isMatched ){
            isMatchedJet.push_back(true);
        }else{
            isMatchedJet.push_back(false);
        }
    }
    
    
    for( unsigned int iselJet = 0; iselJet < selectedJet.size(); ++iselJet ) {
        int ijet = selectedJet[iselJet];
        
        float DEta = Jet_eta[ijet] - LepLep_eta;
        float DPhi = Jet_phi[ijet] - LepLep_phi;
        if( DPhi > M_PI ) DPhi = 2*M_PI - DPhi;
        DYJet::Jet_LepLep_deltaR = sqrt(pow(DEta,2) + pow(DPhi,2));        
        DYJet::Jet_area = Jet_area[ijet];
        DYJet::Jet_eta = Jet_eta[ijet];
        DYJet::Jet_mass = Jet_mass[ijet];
        DYJet::Jet_pt = Jet_pt[ijet];
        DYJet::Jet_chEmEF = Jet_chEmEF[ijet];
        DYJet::Jet_chHEF = Jet_chHEF[ijet];
        DYJet::Jet_neHEF = Jet_neHEF[ijet];
        DYJet::Jet_nConstituents = Jet_nConstituents[ijet];
        DYJet::Jet_matched = isMatchedJet[iselJet];
        
        HDF_fill();
    }
    
    
    

    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //DYJet::variable1Name = 100;      [Example]

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
void HEPHero::DYJetSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishDYJet() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
