#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace LepJet{

    int Njets_medium;
    int Njets_tight;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupLepJet() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_2to4Lep", 0) );
    _cutFlow.insert(pair<string,double>("01_Selected", 0) );
    _cutFlow.insert(pair<string,double>("02_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &LepJet::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "RegionID", &RegionID );
    //HDF_insert( "DatasetID", &_DatasetID );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LepLep_phi", &LepLep_phi );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "TrailingLep_pt", &TrailingLep_pt );
    //HDF_insert( "LeadingLep_eta", &LeadingLep_eta );
    //HDF_insert( "TrailingLep_eta", &TrailingLep_eta );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "MET_phi", &MET_phi );
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "Nbjets", &Nbjets );
    HDF_insert( "Njets", &Njets );
    //HDF_insert( "Njets30", &Njets30 );
    //HDF_insert( "Njets40", &Njets40 );
    //HDF_insert( "HT30", &HT30 );
    //HDF_insert( "MHT30", &MHT30 );
    //HDF_insert( "OmegaMin", &OmegaMin );
    //HDF_insert( "ChiMin", &ChiMin );
    //HDF_insert( "FMax", &FMax );
    //HDF_insert( "MT2LL", &MT2LL );
    //HDF_insert( "Njets_forward", &Njets_forward );
    //HDF_insert( "Jet_abseta_max", &Jet_abseta_max );
    //HDF_insert( "Dijet_deltaEta", &Dijet_deltaEta );
    //HDF_insert( "Dijet_M", &Dijet_M );
    //HDF_insert( "Dijet_pt", &Dijet_pt );
    //HDF_insert( "ttbar_reco", &ttbar_reco );
    //HDF_insert( "PV_npvs", &PV_npvs );
    //HDF_insert( "PV_npvsGood", &PV_npvsGood );
    HDF_insert( "pileup_wgt", &pileup_wgt );
    HDF_insert( "electron_wgt", &electron_wgt );
    HDF_insert( "muon_wgt", &muon_wgt );
    HDF_insert( "btag_wgt", &btag_wgt );
    HDF_insert( "trigger_wgt", &trigger_wgt );
    HDF_insert( "prefiring_wgt", &prefiring_wgt );
    HDF_insert( "top_pt_wgt", &top_pt_wgt );
    HDF_insert( "jet_puid_wgt", &jet_puid_wgt );
    //HDF_insert( "MET_RAW_pt", &MET_RAW_pt );
    //HDF_insert( "MET_RAW_phi", &MET_RAW_phi );
    //HDF_insert( "MET_Unc_pt", &MET_Unc_pt );
    //HDF_insert( "MET_Unc_phi", &MET_Unc_phi );
    //HDF_insert( "MET_JES_pt", &MET_JES_pt );
    //HDF_insert( "MET_JES_phi", &MET_JES_phi );
    //HDF_insert( "MET_XY_pt", &MET_XY_pt );
    //HDF_insert( "MET_XY_phi", &MET_XY_phi );
    //HDF_insert( "MET_RECOIL_pt", &MET_RECOIL_pt );
    //HDF_insert( "MET_RECOIL_phi", &MET_RECOIL_phi );
    //HDF_insert( "MET_JER_pt", &MET_JER_pt );
    //HDF_insert( "MET_JER_phi", &MET_JER_phi );
    //HDF_insert( "MET_Emu_pt", &MET_Emu_pt );
    //HDF_insert( "MET_Emu_phi", &MET_Emu_phi );
    //HDF_insert( "MLP_score_keras", &MLP_score_keras );
    //HDF_insert( "MLP_score_torch", &MLP_score_torch );
    //HDF_insert( "param_variation_weights", &Test::param_variation_weights );
    HDF_insert( "MET_Lep3_Mt", &MET_Lep3_Mt );
    HDF_insert( "Lep3Lep4_M", &Lep3Lep4_M );
    //HDF_insert( "MET_significance", &MET_significance );
    //HDF_insert( "Nleptons", &Nleptons );
    //HDF_insert( "NleptonsLowPt", &NleptonsLowPt );
    //HDF_insert( "Nelectrons", &Nelectrons );
    HDF_insert( "Nmuons", &Nmuons );
    HDF_insert( "ttbar_reco", &ttbar_reco );
    
    HDF_insert( "Njets", &Njets );
    HDF_insert( "Njets_medium", &LepJet::Njets_medium );
    HDF_insert( "Njets_tight", &LepJet::Njets_medium );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::LepJetRegion() {

    LeptonSelection();
    
    if( !((Nleptons >= 2) && (Nleptons <= 4)) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_2to4Lep") += evtWeight;
    
    //if( !(Min_dilep_deltaR > LEPTON_DR_ISO_CUT) ) return false;                 // Minimum Delta R among leptons > CUT
    //_cutFlow.at("01_Dilep_IsoDR") += evtWeight;
    
    JetSelection();
    METCorrection();
    
    Get_Leptonic_Info(true, true);
    Get_LepLep_Variables(true, true);
    
    if( Nleptons == 2 ){
        if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
        
        if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
        
        if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
        
        if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
        
        if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Upper cut in LepLep Delta R 
        
        if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
        
        if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    }
    
    Get_ttbar_Variables();
    
    //Regions();
    //if( !((RegionID >= 0) && (RegionID <= 4)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    //if( !(RegionID == 1) ) return false;            
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("01_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("02_Corrected") += evtWeight;
    
    
    
    //Get_Jet_Angular_Variables( 30 );
    //Get_Dijet_Variables();
    //Signal_discriminators();
    //HEMissue();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::LepJetSelection() {

    /*
    vector<bool> Jet_LepOverlap_medium;
    vector<bool> Jet_LepOverlap_tight;
    for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
        Jet_LepOverlap_medium.push_back(false);
        Jet_LepOverlap_tight.push_back(false);
    }
    
    for( unsigned int iele = 0; iele < nElectron; ++iele ) {
        int iele = selectedEle.at(iselEle);  
                
        float drMin = 99999.;
        int JetID = -1;
        for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
            double deta = fabs(Electron_eta[iele] - Jet_eta[ijet]);
            double dphi = fabs(Electron_phi[iele] - Jet_phi[ijet]);
            if( dphi > M_PI ) dphi = 2*M_PI - dphi;
            double dr = sqrt( deta*deta + dphi*dphi );
            if( dr < drMin ){
                drMin = dr;
                JetID = ijet;
            }
        }
        
        if( !ElectronID( iele, 4 ) ) continue;
        if( (drMin < deltaR_cut) && (JetID >= 0) ) Jet_LepOverlap_medium[JetID] = true;
        if( !ElectronID( iele, 5 ) ) continue;
        if( (drMin < deltaR_cut) && (JetID >= 0) ) Jet_LepOverlap_tight[JetID] = true;
    }
    
    for( unsigned int imu = 0; imu < nMuon; ++imu ) {
        int imu = selectedMu.at(iselMu);
        
        float drMin = 99999.;
        int JetID = -1;
        for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
            double deta = fabs(Muon_eta[imu] - Jet_eta[ijet]);
            double dphi = fabs(Muon_phi[imu] - Jet_phi[ijet]);
            if( dphi > M_PI ) dphi = 2*M_PI - dphi;
            double dr = sqrt( deta*deta + dphi*dphi );
            if( dr < drMin ){
                drMin = dr;
                JetID = ijet;
            }
        }
        
        if( !MuonID( imu, 1 ) ) continue;
        if( (drMin < deltaR_cut) && (JetID >= 0) ) Jet_LepOverlap_medium[JetID] = true;
        if( !MuonID( imu, 2 ) ) continue;
        if( (drMin < deltaR_cut) && (JetID >= 0) ) Jet_LepOverlap_tight[JetID] = true;
    }
    
    
    LepJet::Njets_medium = 0;
    LepJet::Njets_tight = 0;
    for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
        
        if( Jet_pt[ijet] <= JET_PT_CUT ) continue;
        if( Jet_jetId[ijet] < JET_ID_WP ) continue;
        if( (Jet_pt[ijet] < 50) && (Jet_puId[ijet] < JET_PUID_WP) ) continue;
        if( abs(Jet_eta[ijet]) >= JET_ETA_CUT ) continue;
        
        if( Jet_LepOverlap_medium[ijet] ) continue;
        Njets_medium += 1;
        if( Jet_LepOverlap_tight[ijet] ) continue;
        Njets_tight += 1;
    }


    


    */






    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //LepJet::variable1Name = 100;      [Example]

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
void HEPHero::LepJetSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishLepJet() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
