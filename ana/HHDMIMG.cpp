#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace HHDMIMG{

    vector<float> Jet_pt_2D;
    vector<float> Bjet_pt_2D;
    vector<float> Leptons_pt_2D;
    vector<float> LepLep_pt_2D;
    vector<float> LepLep_deltaM_2D;
    
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupHHDMIMG() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    //_cutFlow.insert(pair<string,double>("01_Dilep_IsoDR", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("06_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("07_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("08_MET_Filters", 0) );
    _cutFlow.insert(pair<string,double>("09_HEM_Filter", 0) );
    _cutFlow.insert(pair<string,double>("10_Njets", 0) );
    _cutFlow.insert(pair<string,double>("11_Selected", 0) );
    _cutFlow.insert(pair<string,double>("12_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &HHDMIMG::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "TrailingLep_pt", &TrailingLep_pt );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "MT2LL", &MT2LL );
    
    HDF_insert("Jet_pt_2D", &HHDMIMG::Jet_pt_2D );
    HDF_insert("Bjet_pt_2D", &HHDMIMG::Bjet_pt_2D );
    HDF_insert("Leptons_pt_2D", &HHDMIMG::Leptons_pt_2D );
    HDF_insert("LepLep_pt_2D", &HHDMIMG::LepLep_pt_2D );
    HDF_insert("LepLep_deltaM_2D", &HHDMIMG::LepLep_deltaM_2D );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::HHDMIMGRegion() {

    LeptonSelection();
    
    if( !(RecoLepID > 0) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    //if( !(Min_dilep_deltaR > LEPTON_DR_ISO_CUT) ) return false;                 // Minimum Delta R among leptons > CUT
    //_cutFlow.at("01_Dilep_IsoDR") += evtWeight;
    
    JetSelection();
    METCorrection();
    
    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("01_MET") += evtWeight;  
    
    Get_Leptonic_Info(true, true);
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("02_LeadingLep_Pt") += evtWeight; 
    
    Get_LepLep_Variables(true, true);
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("03_LepLep_DM") += evtWeight; 
    
    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("04_LepLep_Pt") += evtWeight;
    
    if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Upper cut in LepLep Delta R 
    _cutFlow.at("05_LepLep_DR") += evtWeight; 
    
    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("06_MET_LepLep_DPhi") += evtWeight;  
    
    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("07_MET_LepLep_Mt") += evtWeight;
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    _cutFlow.at("08_MET_Filters") += evtWeight;
    
    HEMissue();
    
    if( !HEM_filter ) return false;                                           // Selected by HEM filter
    _cutFlow.at("09_HEM_Filter") += evtWeight;
    
    if( !(Njets >= 1) ) return false;                                           // 
    _cutFlow.at("10_Njets") += evtWeight;
    
    Get_ttbar_Variables();
    Signal_discriminators();
    
    Regions();
    if( !(RegionID == 0) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    //if( !(RegionID == 1) ) return false;            
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("11_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("12_Corrected") += evtWeight;

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::HHDMIMGSelection() {

    HHDMIMG::Jet_pt_2D.clear();
    HHDMIMG::Bjet_pt_2D.clear();
    HHDMIMG::Leptons_pt_2D.clear();
    HHDMIMG::LepLep_pt_2D.clear();
    HHDMIMG::LepLep_deltaM_2D.clear();
    for( int iabseta = 0; iabseta < 12; ++iabseta ){
        for( int idphi = 0; idphi < 16; ++idphi ){
            HHDMIMG::Jet_pt_2D.push_back( 0. );
            HHDMIMG::Bjet_pt_2D.push_back( 0. );
            HHDMIMG::Leptons_pt_2D.push_back( 0. );
            HHDMIMG::LepLep_pt_2D.push_back( 0. );
            HHDMIMG::LepLep_deltaM_2D.push_back( 0. );
        }
    }
    
    for( int iseljet = 0; iseljet < selectedJet.size(); ++iseljet ) {
        int ijet = selectedJet[iseljet];
        
        float jet_abseta = fabs(Jet_eta[ijet]);
        int iabseta = int(jet_abseta/(2.4/12));
        
        float jet_met_dphi = fabs(MET_phi - Jet_phi[ijet]);
        if( jet_met_dphi > M_PI ) jet_met_dphi = 2*M_PI - jet_met_dphi;
        int idphi = int(jet_met_dphi/(3.2/16));
        
        
        HHDMIMG::Jet_pt_2D[(iabseta*12)+idphi] += Jet_pt[ijet];
        //Fill_vector(HHDMIMG::Jet_pt_2D, (iabseta*12)+idphi, Jet_pt[ijet])
        //https://math.stackexchange.com/questions/2971315/how-do-i-combine-standard-deviations-of-two-groups
        
        
        if( JetBTAG( ijet, JET_BTAG_WP ) ) HHDMIMG::Bjet_pt_2D[(iabseta*12)+idphi] += Jet_pt[ijet];
    }
    
    
    for( int isellep = 0; isellep < selectedEle.size(); ++isellep ) {
        int ilep = selectedEle[isellep];
        
        float lep_abseta = fabs(Electron_eta[ilep]);
        int iabseta = int(lep_abseta/(2.4/12));
        
        float lep_met_dphi = fabs(MET_phi - Electron_phi[ilep]);
        if( lep_met_dphi > M_PI ) lep_met_dphi = 2*M_PI - lep_met_dphi;
        int idphi = int(lep_met_dphi/(3.2/16));
        
        HHDMIMG::Leptons_pt_2D[(iabseta*12)+idphi] += Electron_pt[ilep];
    }
    
    for( int isellep = 0; isellep < selectedMu.size(); ++isellep ) {
        int ilep = selectedMu[isellep];
        
        float lep_abseta = fabs(Muon_eta[ilep]);
        int iabseta = int(lep_abseta/(2.4/12));
        
        float lep_met_dphi = fabs(MET_phi - Muon_phi[ilep]);
        if( lep_met_dphi > M_PI ) lep_met_dphi = 2*M_PI - lep_met_dphi;
        int idphi = int(lep_met_dphi/(3.2/16));
        
        HHDMIMG::Leptons_pt_2D[(iabseta*12)+idphi] += Muon_pt[ilep];
    }
    
        
    float Z_abseta = fabs(LepLep_eta);
    int iabseta = int(Z_abseta/(2.4/12));
    
    float Z_met_dphi = fabs(MET_phi - LepLep_phi);
    if( Z_met_dphi > M_PI ) Z_met_dphi = 2*M_PI - Z_met_dphi;
    int idphi = int(Z_met_dphi/(3.2/16));
    
    HHDMIMG::LepLep_pt_2D[(iabseta*12)+idphi] += LepLep_pt;
    HHDMIMG::LepLep_deltaM_2D[(iabseta*12)+idphi] += LepLep_deltaM;
    

   

    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //HHDMIMG::variable1Name = 100;      [Example]

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
void HEPHero::HHDMIMGSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishHHDMIMG() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
