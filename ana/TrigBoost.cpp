#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace TrigBoost{

    bool  HLT_LEPTONS;
    float Ele_pt_0;
    float Ele_pt_1;
    float Mu_pt_0;
    float Mu_pt_1;
    float Ele_eta_0;
    float Ele_eta_1;
    float Mu_eta_0;
    float Mu_eta_1;
    float EleEle_deltaR;
    float MuMu_deltaR;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTrigBoost() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("06_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("07_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("08_Selected", 0) );
    _cutFlow.insert(pair<string,double>("09_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &TrigBoost::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "RegionID", &RegionID );
    HDF_insert( "DatasetID", &_DatasetID );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "TrailingLep_pt", &TrailingLep_pt );
    HDF_insert( "LeadingLep_eta", &LeadingLep_eta );
    HDF_insert( "TrailingLep_eta", &TrailingLep_eta );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "MET_RAW_pt", &MET_RAW_pt );
    
    HDF_insert( "HLT_LEPTONS", &TrigBoost::HLT_LEPTONS );
    HDF_insert( "Ele_pt_0", &TrigBoost::Ele_pt_0 );
    HDF_insert( "Ele_pt_1", &TrigBoost::Ele_pt_1 );
    HDF_insert( "Mu_pt_0", &TrigBoost::Mu_pt_0 );
    HDF_insert( "Mu_pt_1", &TrigBoost::Mu_pt_1 );
    HDF_insert( "Ele_eta_0", &TrigBoost::Ele_eta_0 );
    HDF_insert( "Ele_eta_1", &TrigBoost::Ele_eta_1 );
    HDF_insert( "Mu_eta_0", &TrigBoost::Mu_eta_0 );
    HDF_insert( "Mu_eta_1", &TrigBoost::Mu_eta_1 );
    HDF_insert( "EleEle_deltaR", &TrigBoost::EleEle_deltaR );
    HDF_insert( "MuMu_deltaR", &TrigBoost::MuMu_deltaR );    

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TrigBoostRegion() {

    LeptonSelection();
    
    if( !(RecoLepID > 0) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    JetSelection();
    METCorrection();
    
    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("01_MET") += evtWeight;  
    
    Get_Leptonic_Info(true, true);
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("02_LeadingLep_Pt") += evtWeight; 
    
    Get_LepLep_Variables(true, true);
    
    //if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("03_LepLep_DM") += evtWeight; 
    
    //if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("04_LepLep_Pt") += evtWeight;
    
    //if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Upper cut in LepLep Delta R 
    _cutFlow.at("05_LepLep_DR") += evtWeight; 
    
    //if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("06_MET_LepLep_DPhi") += evtWeight;  
    
    //if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("07_MET_LepLep_Mt") += evtWeight;
    
    //Get_ttbar_Variables();
    
    //Regions();
    //if( !((RegionID >= 0) && (RegionID <= 4)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    TrigBoost::HLT_LEPTONS = Trigger();
    if( !HLT_MET ) return false;                                              // Selected by triggers
    _cutFlow.at("08_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("09_Corrected") += evtWeight;
    
    //Signal_discriminators();
    //Get_Jet_Angular_Variables( );
    //Get_Dijet_Variables();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::TrigBoostSelection() {

    TrigBoost::Ele_pt_0 = Electron_pt[0];
    TrigBoost::Ele_pt_1 = Electron_pt[1];
    TrigBoost::Mu_pt_0 = Muon_pt[0];
    TrigBoost::Mu_pt_1 = Muon_pt[1];
    TrigBoost::Ele_eta_0 = Electron_eta[0];
    TrigBoost::Ele_eta_1 = Electron_eta[1];
    TrigBoost::Mu_eta_0 = Muon_eta[0];
    TrigBoost::Mu_eta_1 = Muon_eta[1];
    
    TLorentzVector Ele_0;
    TLorentzVector Ele_1;
    Ele_0.SetPtEtaPhiM(Electron_pt[0], Electron_eta[0], Electron_phi[0], Electron_pdg_mass);
    Ele_1.SetPtEtaPhiM(Electron_pt[1], Electron_eta[1], Electron_phi[1], Electron_pdg_mass);
    
    TLorentzVector Mu_0;
    TLorentzVector Mu_1;
    Mu_0.SetPtEtaPhiM(Muon_pt[0], Muon_eta[0], Muon_phi[0], Muon_pdg_mass);
    Mu_1.SetPtEtaPhiM(Muon_pt[1], Muon_eta[1], Muon_phi[1], Muon_pdg_mass);
    
    TrigBoost::EleEle_deltaR = Ele_0.DeltaR( Ele_1 );
    TrigBoost::MuMu_deltaR = Mu_0.DeltaR( Mu_1 );


    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //TrigBoost::variable1Name = 100;      [Example]

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
void HEPHero::TrigBoostSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTrigBoost() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
