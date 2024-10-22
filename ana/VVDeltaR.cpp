#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace VVDeltaR{

    float LepJet_deltaR;
    bool Has_overlap_0p4;
    bool Event_info;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupVVDeltaR() {

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
    //_outputTree->Branch("variable1NameInTheTree", &VVDeltaR::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );

    HDF_insert( "Nbjets", &Nbjets );
    HDF_insert( "Njets", &Njets );
    
    HDF_insert( "MET_Lep3_Mt", &MET_Lep3_Mt );
    HDF_insert( "Lep3Lep4_M", &Lep3Lep4_M );
    HDF_insert( "Nmuons", &Nmuons );
    HDF_insert( "ttbar_reco", &ttbar_reco );
    
    HDF_insert( "LepJet_deltaR", &VVDeltaR::LepJet_deltaR );
    HDF_insert( "Has_overlap_0p4", &VVDeltaR::Has_overlap_0p4 );
    
    HDF_insert( "HEM_issue_ele", &HEM_issue_ele );
    HDF_insert( "HEM_issue_jet", &HEM_issue_jet );
    HDF_insert( "HEM_issue_ele_v2", &HEM_issue_ele_v2 );
    HDF_insert( "HEM_issue_jet_v2", &HEM_issue_jet_v2 );
    HDF_insert( "HEM_issue_met", &HEM_issue_met );
    
    HDF_insert( "Event_info", &VVDeltaR::Event_info );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::VVDeltaRRegion() {

    LeptonSelection();
    
    if( !((Nleptons >= 2) && (Nleptons <= 4)) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_2to4Lep") += evtWeight;
    
    //if( !(Min_dilep_deltaR > LEPTON_DR_ISO_CUT) ) return false;                 // Minimum Delta R among leptons > CUT
    //_cutFlow.at("01_Dilep_IsoDR") += evtWeight;
    
    JetSelection();
    METCorrection();
    
    Get_Leptonic_Info(true, true);
    Get_LepLep_Variables(true, true);
    
    
    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    
    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    
    if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Upper cut in LepLep Delta R 
    
    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    
    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    
    if( !(Njets >= 1) ) return false;                   
    
    
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
    HEMissue();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::VVDeltaRSelection() {


    VVDeltaR::Has_overlap_0p4 = false;
    
    for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
        
        if( Jet_pt[ijet] <= JET_PT_CUT ) continue;
        if( Jet_jetId[ijet] < JET_ID_WP ) continue;
        if( (Jet_pt[ijet] < 50) && (Jet_puId[ijet] < JET_PUID_WP) ) continue;
        if( abs(Jet_eta[ijet]) >= JET_ETA_CUT ) continue;
        
        //for( unsigned int iele = 0; iele < nElectron; ++iele ) {
        for( unsigned int iselEle = 0; iselEle < selectedEle.size(); ++iselEle ) {
            int iele = selectedEle.at(iselEle);    
            double deta = fabs(Electron_eta[iele] - Jet_eta[ijet]);
            double dphi = fabs(Electron_phi[iele] - Jet_phi[ijet]);
            if( dphi > M_PI ) dphi = 2*M_PI - dphi;
            double dr = sqrt( deta*deta + dphi*dphi );
            if( dr < 0.4 ) VVDeltaR::Has_overlap_0p4 = true;
        }
        //for( unsigned int imu = 0; imu < nMuon; ++imu ) {
        for( unsigned int iselMu = 0; iselMu < selectedMu.size(); ++iselMu ) {
            int imu = selectedMu.at(iselMu);
            double deta = fabs(Muon_eta[imu] - Jet_eta[ijet]);
            double dphi = fabs(Muon_phi[imu] - Jet_phi[ijet]);
            if( dphi > M_PI ) dphi = 2*M_PI - dphi;
            double dr = sqrt( deta*deta + dphi*dphi );
            if( dr < 0.4 ) VVDeltaR::Has_overlap_0p4 = true;
        }
    }
    
    
    
    
    VVDeltaR::Event_info = true;
    
    for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
        
        if( Jet_pt[ijet] <= JET_PT_CUT ) continue;
        if( Jet_jetId[ijet] < JET_ID_WP ) continue;
        if( (Jet_pt[ijet] < 50) && (Jet_puId[ijet] < JET_PUID_WP) ) continue;
        if( abs(Jet_eta[ijet]) >= JET_ETA_CUT ) continue;
        
        double drMin = 10000;
        
        //for( unsigned int iele = 0; iele < nElectron; ++iele ) {
        for( unsigned int iselEle = 0; iselEle < selectedEle.size(); ++iselEle ) {
            int iele = selectedEle.at(iselEle);    
            double deta = fabs(Electron_eta[iele] - Jet_eta[ijet]);
            double dphi = fabs(Electron_phi[iele] - Jet_phi[ijet]);
            if( dphi > M_PI ) dphi = 2*M_PI - dphi;
            double dr = sqrt( deta*deta + dphi*dphi );
            if( dr < drMin ) drMin = dr;
        }
        //for( unsigned int imu = 0; imu < nMuon; ++imu ) {
        for( unsigned int iselMu = 0; iselMu < selectedMu.size(); ++iselMu ) {
            int imu = selectedMu.at(iselMu);
            double deta = fabs(Muon_eta[imu] - Jet_eta[ijet]);
            double dphi = fabs(Muon_phi[imu] - Jet_phi[ijet]);
            if( dphi > M_PI ) dphi = 2*M_PI - dphi;
            double dr = sqrt( deta*deta + dphi*dphi );
            if( dr < drMin ) drMin = dr;
        }
        
        VVDeltaR::LepJet_deltaR = drMin; 
        
        HDF_fill();
        
        VVDeltaR::Event_info = false;
        
    }






    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //VVDeltaR::variable1Name = 100;      [Example]

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
void HEPHero::VVDeltaRSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishVVDeltaR() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
