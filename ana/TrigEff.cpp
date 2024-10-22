#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace TrigEff{

    bool metFilters,GoodFirstPV;
    vector<float> lepton_scEta,jet_regFactor,lepton_relIso, lepton_IDSF,lepton_recoIsoSF,lepton_energyCorr;
    vector<int> lepton_isTight;
    float puSF;
    double Lep_leading_pt,Lep_subleading_pt,Lep_leading_eta,Lep_subleading_eta;
    bool Lep_triggers,Met_triggers;



    vector<float> lepton_pt;
    vector<float> lepton_eta;
    vector<float> lepton_phi;
    vector<int> lepton_e;
    vector<int> lepton_isMuon;
    vector<int> lep_charge;

    // vector<float> lepton_mu0_pt;
    // vector<float> lepton_mu1_pt;
    // vector<float> lepton_mu0_eta;
    // vector<float> lepton_mu1_eta;
    // vector<float> lepton_el0_pt;
    // vector<float> lepton_el1_pt;
    // vector<float> lepton_el0_eta;
    // vector<float> lepton_el1_eta;


    vector<float> v_muon_pt;
    vector<float> v_electron_pt;


    vector<float> v_jets_pt;
    vector<float> v_jets_eta;
    vector<float> v_jets_phi;
    vector<float> v_jets_m;

    

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTrigEff() {

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
    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &TrigEff::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert("variable1NameInTheTree", &TrigEff::variable1Name );  [example]

    // HDF_insert("Muon_pt",&Muon_pt);
    // HDF_insert("Muon_eta",&Muon_eta);
    // HDF_insert("Muon_charge",&Muon_charge);
    // HDF_insert("Electron_pt",&Electron_pt);
    // HDF_insert("Electron_eta",&Electron_eta);
    // HDF_insert("Electron_charge",&Electron_charge);
    
    HDF_insert("muon_pt",&TrigEff::v_muon_pt);
    HDF_insert("electron_pt",&TrigEff::v_electron_pt);



    HDF_insert("nMuon",&nMuon);
    HDF_insert("nElectron",&Nelectrons);
    HDF_insert("Lep_leading_pt", &TrigEff::Lep_leading_pt);                                                                                        
    HDF_insert("Lep_leading_eta",&TrigEff::Lep_leading_eta);
    HDF_insert("Lep_subleading_pt", &TrigEff::Lep_subleading_pt);                                                                                    
    HDF_insert("Lep_subleading_eta",&TrigEff::Lep_subleading_eta);
    HDF_insert("Lep_triggers", &TrigEff::Lep_triggers);                                                                                      
    HDF_insert("Met_triggers",&TrigEff::Met_triggers);
    HDF_insert("LepLep_deltaR",&LepLep_deltaR);
    HDF_insert("LeadingLep_pt", &LeadingLep_pt);
    HDF_insert("RecoLepID", &RecoLepID );
    HDF_insert("Njets", &Njets );
    HDF_insert("HLT_IsoMu24",&HLT_IsoMu24);
    HDF_insert("HLT_IsoTkMu24",&HLT_IsoTkMu24);
    HDF_insert("HLT_Mu50",&HLT_Mu50);
    HDF_insert("HLT_IsoMu27",&HLT_IsoMu27);
    HDF_insert("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
    HDF_insert("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
    HDF_insert("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL); 
    HDF_insert("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL",&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL); 
    HDF_insert("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8); 
    HDF_insert("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8); 
    HDF_insert("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ);
    HDF_insert("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    HDF_insert("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",&HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL);              
    HDF_insert("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",&HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL);
    HDF_insert("HLT_Ele27_WPTight_Gsf",&HLT_Ele27_WPTight_Gsf);
    HDF_insert("HLT_Ele115_CaloIdVT_GsfTrkIdT",&HLT_Ele115_CaloIdVT_GsfTrkIdT);
    HDF_insert("HLT_Ele32_WPTight_Gsf",&HLT_Ele32_WPTight_Gsf);
    HDF_insert("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ); 
    HDF_insert("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL",&HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL);
    HDF_insert("HLT_DoubleEle33_CaloIdL_MW",&HLT_DoubleEle33_CaloIdL_MW); 
    HDF_insert("HLT_PFMET300",&HLT_PFMET300);   
    HDF_insert("HLT_MET200",&HLT_MET200);
    HDF_insert("HLT_PFHT300_PFMET110",&HLT_PFHT300_PFMET110);
    HDF_insert("HLT_PFMET170_HBHECleaned",&HLT_PFMET170_HBHECleaned);
    HDF_insert("HLT_PFMET120_PFMHT120_IDTight",&HLT_PFMET120_PFMHT120_IDTight);
    HDF_insert("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight",&HLT_PFMETNoMu120_PFMHTNoMu120_IDTight);
    HDF_insert("HLT_PFMET200_HBHECleaned",&HLT_PFMET200_HBHECleaned);
    HDF_insert("HLT_PFMET200_HBHE_BeamHaloCleaned",&HLT_PFMET200_HBHE_BeamHaloCleaned); 
    HDF_insert("HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned",&HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned); 
    HDF_insert("HLT_PFMET120_PFMHT120_IDTight_PFHT60",&HLT_PFMET120_PFMHT120_IDTight_PFHT60); 
    HDF_insert("HLT_PFHT500_PFMET100_PFMHT100_IDTight",&HLT_PFHT500_PFMET100_PFMHT100_IDTight); 
    HDF_insert("HLT_PFHT700_PFMET85_PFMHT85_IDTight",&HLT_PFHT700_PFMET85_PFMHT85_IDTight); 
    HDF_insert("HLT_PFHT800_PFMET75_PFMHT75_IDTight",&HLT_PFHT800_PFMET75_PFMHT75_IDTight); 
    HDF_insert("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60",&HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60);

    // NOVOS TRIGGERS

    





    HDF_insert("run_", &run );
    HDF_insert("lumi_", &luminosityBlock );
    // HDF_insert("evt_",&event); 
    HDF_insert("numPVs_", &PV_npvs);
    HDF_insert("puSF_",&TrigEff::puSF);
    HDF_insert("PassMETFilters_",&TrigEff::metFilters);
    HDF_insert("GoodFirstPV_",&TrigEff::GoodFirstPV);
    HDF_insert("genWeight_", &genWeight );
    HDF_insert("jet_pt_", &TrigEff::v_jets_pt );
    HDF_insert("jet_eta_", &TrigEff::v_jets_eta );
    HDF_insert("jet_phi_", &TrigEff::v_jets_phi );
    HDF_insert("jet_m_", &TrigEff::v_jets_m );
    HDF_insert("jet_regFactor",&TrigEff::jet_regFactor);
    HDF_insert("lepton_pt_",&TrigEff::lepton_pt);
    HDF_insert("lepton_eta_",&TrigEff::lepton_eta);
    HDF_insert("lepton_phi_",&TrigEff::lepton_phi);
    HDF_insert("lepton_e_",&TrigEff::lepton_e);
    HDF_insert("lepton_scEta_",&TrigEff::lepton_scEta);
    HDF_insert("lepton_isMuon_",&TrigEff::lepton_isMuon);
    HDF_insert("lepton_isTight_",&TrigEff::lepton_isTight);
    HDF_insert("lepton_charge_",&TrigEff::lep_charge);
    HDF_insert("lepton_relIso",&TrigEff::lepton_relIso);
    HDF_insert("lepton_IDSF_",&TrigEff::lepton_IDSF);
    HDF_insert("lepton_recoIsoSF_",&TrigEff::lepton_recoIsoSF);
    HDF_insert("lepton_energyCorr_",&TrigEff::lepton_energyCorr);
    HDF_insert("MET_", &MET_pt );
    HDF_insert("MET_phi_", &MET_phi );
    HDF_insert("MET_Type1xy_",&MET_pt);
    HDF_insert("MET_Type1xy_phi_",&MET_phi);
    HDF_insert("MET_Type1xy_sync_",&MET_pt);
    HDF_insert("MET_Type1xy_phi_sync_",&MET_phi);

    HDF_insert("HLT_Ele35_WPTight_Gsf",&HLT_Ele35_WPTight_Gsf);





    _outputTree->Branch("muon_pt",&TrigEff::v_muon_pt);


    _outputTree->Branch("electron_pt",&TrigEff::v_electron_pt);


    _outputTree->Branch("HLT_Ele35_WPTight_Gsf",&HLT_Ele35_WPTight_Gsf);
    _outputTree->Branch("nMuon",&Nmuons);                                                                                        
    _outputTree->Branch("nElectron",&Nelectrons);
    _outputTree->Branch("Lep_leading_pt", &TrigEff::Lep_leading_pt);                                                                                        
    _outputTree->Branch("Lep_leading_eta",&TrigEff::Lep_leading_eta);
    _outputTree->Branch("Lep_subleading_pt", &TrigEff::Lep_subleading_pt);                                                                                    
    _outputTree->Branch("Lep_subleading_eta",&TrigEff::Lep_subleading_eta);
    _outputTree->Branch("Lep_triggers", &TrigEff::Lep_triggers);                                                                                      
    _outputTree->Branch("Met_triggers",&TrigEff::Met_triggers);
    _outputTree->Branch("LepLep_deltaR",&LepLep_deltaR);
    _outputTree->Branch("LeadingLep_pt", &LeadingLep_pt);
    _outputTree->Branch("RecoLepID", &RecoLepID );
    _outputTree->Branch("Njets", &Njets );
    _outputTree->Branch("HLT_IsoMu24",&HLT_IsoMu24);
    _outputTree->Branch("HLT_IsoTkMu24",&HLT_IsoTkMu24);
    _outputTree->Branch("HLT_Mu50",&HLT_Mu50);
    _outputTree->Branch("HLT_IsoMu27",&HLT_IsoMu27);
    _outputTree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ);
    _outputTree->Branch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ",&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
    _outputTree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL); 
    _outputTree->Branch("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL",&HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL); 
    _outputTree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8); 
    _outputTree->Branch("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8",&HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8); 
    _outputTree->Branch("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ);
    _outputTree->Branch("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    _outputTree->Branch("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL",&HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL);              
    _outputTree->Branch("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL",&HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL);
    _outputTree->Branch("HLT_Ele27_WPTight_Gsf",&HLT_Ele27_WPTight_Gsf);
    _outputTree->Branch("HLT_Ele115_CaloIdVT_GsfTrkIdT",&HLT_Ele115_CaloIdVT_GsfTrkIdT);
    _outputTree->Branch("HLT_Ele32_WPTight_Gsf",&HLT_Ele32_WPTight_Gsf);
    _outputTree->Branch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ",&HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ); 
    _outputTree->Branch("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL",&HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL);
    _outputTree->Branch("HLT_DoubleEle33_CaloIdL_MW",&HLT_DoubleEle33_CaloIdL_MW); 
    _outputTree->Branch("HLT_PFMET300",&HLT_PFMET300);   
    _outputTree->Branch("HLT_MET200",&HLT_MET200);
    _outputTree->Branch("HLT_PFHT300_PFMET110",&HLT_PFHT300_PFMET110);
    _outputTree->Branch("HLT_PFMET170_HBHECleaned",&HLT_PFMET170_HBHECleaned);
    _outputTree->Branch("HLT_PFMET120_PFMHT120_IDTight",&HLT_PFMET120_PFMHT120_IDTight);
    _outputTree->Branch("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight",&HLT_PFMETNoMu120_PFMHTNoMu120_IDTight);
    _outputTree->Branch("HLT_PFMET200_HBHECleaned",&HLT_PFMET200_HBHECleaned);
    _outputTree->Branch("HLT_PFMET200_HBHE_BeamHaloCleaned",&HLT_PFMET200_HBHE_BeamHaloCleaned); 
    _outputTree->Branch("HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned",&HLT_PFMETTypeOne200_HBHE_BeamHaloCleaned); 
    _outputTree->Branch("HLT_PFMET120_PFMHT120_IDTight_PFHT60",&HLT_PFMET120_PFMHT120_IDTight_PFHT60); 
    _outputTree->Branch("HLT_PFHT500_PFMET100_PFMHT100_IDTight",&HLT_PFHT500_PFMET100_PFMHT100_IDTight); 
    _outputTree->Branch("HLT_PFHT700_PFMET85_PFMHT85_IDTight",&HLT_PFHT700_PFMET85_PFMHT85_IDTight); 
    _outputTree->Branch("HLT_PFHT800_PFMET75_PFMHT75_IDTight",&HLT_PFHT800_PFMET75_PFMHT75_IDTight); 
    _outputTree->Branch("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60",&HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60);
    _outputTree->Branch("run_", &run );
    _outputTree->Branch("lumi_", &luminosityBlock );
    _outputTree->Branch("evt_",&event); 
    _outputTree->Branch("numPVs_", &PV_npvs);
    _outputTree->Branch("puSF_",&TrigEff::puSF);
    _outputTree->Branch("PassMETFilters_",&TrigEff::metFilters);
    _outputTree->Branch("GoodFirstPV_",&TrigEff::GoodFirstPV);
    _outputTree->Branch("genWeight_", &genWeight );
    _outputTree->Branch("jet_pt_", &TrigEff::v_jets_pt );
    _outputTree->Branch("jet_eta_", &TrigEff::v_jets_eta );
    _outputTree->Branch("jet_phi_", &TrigEff::v_jets_phi );
    _outputTree->Branch("jet_m_", &TrigEff::v_jets_m );
    _outputTree->Branch("jet_regFactor",&TrigEff::jet_regFactor);
    _outputTree->Branch("lepton_pt_",&TrigEff::lepton_pt);
    _outputTree->Branch("lepton_eta_",&TrigEff::lepton_eta);
    _outputTree->Branch("lepton_phi_",&TrigEff::lepton_phi);
    _outputTree->Branch("lepton_e_",&TrigEff::lepton_e);
    _outputTree->Branch("lepton_scEta_",&TrigEff::lepton_scEta);
    _outputTree->Branch("lepton_isMuon_",&TrigEff::lepton_isMuon);
    _outputTree->Branch("lepton_isTight_",&TrigEff::lepton_isTight);
    _outputTree->Branch("lepton_charge_",&TrigEff::lep_charge);
    _outputTree->Branch("lepton_relIso",&TrigEff::lepton_relIso);
    _outputTree->Branch("lepton_IDSF_",&TrigEff::lepton_IDSF);
    _outputTree->Branch("lepton_recoIsoSF_",&TrigEff::lepton_recoIsoSF);
    _outputTree->Branch("lepton_energyCorr_",&TrigEff::lepton_energyCorr);
    _outputTree->Branch("MET_", &MET_pt );
    _outputTree->Branch("MET_phi_", &MET_phi );
    _outputTree->Branch("MET_Type1xy_",&MET_pt);
    _outputTree->Branch("MET_Type1xy_phi_",&MET_phi);
    _outputTree->Branch("MET_Type1xy_sync_",&MET_pt);
    _outputTree->Branch("MET_Type1xy_phi_sync_",&MET_phi);
    // _outputTree->Branch("lepton_mu0_pt",&TrigEff::lepton_mu0_pt);
    // _outputTree->Branch("lepton_mu0_eta",&TrigEff::lepton_mu0_eta);
    // _outputTree->Branch("lepton_mu1_pt",&TrigEff::lepton_mu1_pt);
    // _outputTree->Branch("lepton_mu1_eta",&TrigEff::lepton_mu1_eta);
    // _outputTree->Branch("lepton_el0_pt",&TrigEff::lepton_el0_pt);
    // _outputTree->Branch("lepton_el0_eta",&TrigEff::lepton_el0_eta);
    // _outputTree->Branch("lepton_el1_pt",&TrigEff::lepton_el1_pt);
    // _outputTree->Branch("lepton_el1_eta",&TrigEff::lepton_el1_eta);

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TrigEffRegion() {
    
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
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    //if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("08_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("09_Corrected") += evtWeight;

    return true;
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
void HEPHero::TrigEffSelection() {




   TrigEff::lepton_scEta.clear();
    TrigEff::lepton_isTight.clear();
    TrigEff::lepton_relIso.clear();
    TrigEff::lepton_IDSF.clear();
    TrigEff::lepton_recoIsoSF.clear();
    TrigEff::lepton_energyCorr.clear();
    TrigEff::lepton_pt.clear();
    TrigEff::lepton_eta.clear();
    TrigEff::lepton_phi.clear();
    TrigEff::lepton_e.clear();
    TrigEff::lepton_isMuon.clear();
    TrigEff::lep_charge.clear();


    // TrigEff::lepton_mu0_pt.clear();
    // TrigEff::lepton_mu1_pt.clear();
    // TrigEff::lepton_mu0_eta.clear();
    // TrigEff::lepton_mu1_eta.clear();
    // TrigEff::lepton_el0_pt.clear();
    // TrigEff::lepton_el1_pt.clear();
    // TrigEff::lepton_el0_eta.clear();
    // TrigEff::lepton_el1_eta.clear();


    TrigEff::v_muon_pt.clear();
    TrigEff::v_electron_pt.clear();


    TrigEff::jet_regFactor.clear();
    TrigEff::v_jets_pt.clear();
    TrigEff::v_jets_phi.clear();
    TrigEff::v_jets_eta.clear();
    TrigEff::v_jets_m.clear();   
    
    TLorentzVector lep_1;
    TLorentzVector lep_2;

    lep_1.Clear();
    lep_2.Clear();


    TrigEff::Lep_triggers = Trigger();
    TrigEff::Met_triggers = HLT_MET;
    //(HLT_PFMET300 || HLT_MET200 || HLT_PFHT300_PFMET110 || HLT_PFMET170_HBHECleaned || HLT_PFMET120_PFMHT120_IDTight || HLT_PFMETNoMu120_PFMHTNoMu120_IDTight); 2016



    if((RecoLepID == 11) || ((RecoLepID > 31100) && (RecoLepID <= 31199)) || ((RecoLepID > 41100) && (RecoLepID <= 41199))){
        lep_1.SetPtEtaPhiM(Electron_pt[IdxLeadingLep], Electron_eta[IdxLeadingLep], Electron_phi[IdxLeadingLep], Electron_pdg_mass);
        lep_2.SetPtEtaPhiM(Electron_pt[IdxTrailingLep], Electron_eta[IdxTrailingLep], Electron_phi[IdxTrailingLep], Electron_pdg_mass);
        TrigEff::lepton_isMuon.push_back(0);
        TrigEff::lepton_isMuon.push_back(0);
        TrigEff::lep_charge.push_back(Electron_charge[IdxLeadingLep]);
        TrigEff::lep_charge.push_back(Electron_charge[IdxTrailingLep]);

        TrigEff::Lep_leading_pt = Electron_pt[IdxLeadingLep];
        TrigEff::Lep_subleading_pt = Electron_pt[IdxTrailingLep];
        TrigEff::Lep_leading_eta = Electron_eta[IdxLeadingLep];
        TrigEff::Lep_subleading_eta = Electron_eta[IdxTrailingLep];

        // TrigEff::lepton_mu0_pt.push_back( -1);
        // TrigEff::lepton_mu1_pt.push_back(-1);
        // TrigEff::lepton_mu0_eta.push_back(-99);
        // TrigEff::lepton_mu1_eta.push_back(-99);
        // TrigEff::lepton_el0_pt.push_back( Electron_pt[IdxLeadingLep]);
        // TrigEff::lepton_el1_pt.push_back(Electron_pt[IdxTrailingLep]);
        // TrigEff::lepton_el0_eta.push_back( Electron_eta[IdxLeadingLep]);
        // TrigEff::lepton_el1_eta.push_back(Electron_eta[IdxTrailingLep]);





    }
    else if((RecoLepID == 13) || ((RecoLepID > 31300) && (RecoLepID <= 31399)) || ((RecoLepID > 41300) && (RecoLepID <= 41399))){
        lep_1.SetPtEtaPhiM(Muon_pt[IdxLeadingLep], Muon_eta[IdxLeadingLep], Muon_phi[IdxLeadingLep], Muon_pdg_mass);
        lep_2.SetPtEtaPhiM(Muon_pt[IdxTrailingLep], Muon_eta[IdxTrailingLep], Muon_phi[IdxTrailingLep], Muon_pdg_mass);
        TrigEff::lepton_isMuon.push_back(1);
        TrigEff::lepton_isMuon.push_back(1);
        TrigEff::lep_charge.push_back(Muon_charge[IdxLeadingLep]);
        TrigEff::lep_charge.push_back(Muon_charge[IdxTrailingLep]);

        TrigEff::Lep_leading_pt = Muon_pt[IdxLeadingLep];
        TrigEff::Lep_subleading_pt = Muon_pt[IdxTrailingLep];
        TrigEff::Lep_leading_eta = Muon_eta[IdxLeadingLep];
        TrigEff::Lep_subleading_eta = Muon_eta[IdxTrailingLep];

        // TrigEff::lepton_mu0_pt.push_back(Muon_pt[IdxLeadingLep]);
        // TrigEff::lepton_mu1_pt.push_back(Muon_pt[IdxTrailingLep]);
        // TrigEff::lepton_mu0_eta.push_back(Muon_eta[IdxLeadingLep]);
        // TrigEff::lepton_mu1_eta.push_back(Muon_eta[IdxTrailingLep]);
        // TrigEff::lepton_el0_pt.push_back(-1.);
        // TrigEff::lepton_el1_pt.push_back( -1.);
        // TrigEff::lepton_el0_eta.push_back(-99);
        // TrigEff::lepton_el1_eta.push_back(-99);

    }
    else if(RecoLepID == 1113){
        lep_1.SetPtEtaPhiM(Electron_pt[IdxLeadingLep], Electron_eta[IdxLeadingLep], Electron_phi[IdxLeadingLep], Electron_pdg_mass);
        lep_2.SetPtEtaPhiM(Muon_pt[IdxTrailingLep], Muon_eta[IdxTrailingLep], Muon_phi[IdxTrailingLep], Muon_pdg_mass);
        TrigEff::lepton_isMuon.push_back(0);
        TrigEff::lepton_isMuon.push_back(1);
        TrigEff::lep_charge.push_back(Electron_charge[IdxLeadingLep]);
        TrigEff::lep_charge.push_back(Muon_charge[IdxTrailingLep]);

        TrigEff::Lep_leading_pt = Electron_pt[IdxLeadingLep];
        TrigEff::Lep_subleading_pt = Muon_pt[IdxTrailingLep];
        TrigEff::Lep_leading_eta = Electron_eta[IdxLeadingLep];
        TrigEff::Lep_subleading_eta = Muon_eta[IdxTrailingLep];



        // TrigEff::lepton_mu0_pt.push_back(Muon_pt[IdxTrailingLep]);
        // TrigEff::lepton_mu1_pt.push_back(-1);
        // TrigEff::lepton_mu0_eta.push_back(Muon_eta[IdxTrailingLep]);
        // TrigEff::lepton_mu1_eta.push_back( -99.);
        // TrigEff::lepton_el0_pt.push_back( Electron_pt[IdxLeadingLep]);
        // TrigEff::lepton_el1_pt.push_back( -1.);
        // TrigEff::lepton_el0_eta.push_back( Electron_eta[IdxLeadingLep]);
        // TrigEff::lepton_el1_eta.push_back( -99.);
    }
    else if(RecoLepID == 1311){
        lep_1.SetPtEtaPhiM(Muon_pt[IdxLeadingLep], Muon_eta[IdxLeadingLep], Muon_phi[IdxLeadingLep], Muon_pdg_mass);
        lep_2.SetPtEtaPhiM(Electron_pt[IdxTrailingLep], Electron_eta[IdxTrailingLep], Electron_phi[IdxTrailingLep], Electron_pdg_mass);
        TrigEff::lepton_isMuon.push_back(1);
        TrigEff::lepton_isMuon.push_back(0);
        TrigEff::lep_charge.push_back(Muon_charge[IdxLeadingLep]);
        TrigEff::lep_charge.push_back(Electron_charge[IdxTrailingLep]);

        TrigEff::Lep_leading_pt = Muon_pt[IdxLeadingLep];
        TrigEff::Lep_subleading_pt = Electron_pt[IdxTrailingLep];
        TrigEff::Lep_leading_eta = Muon_eta[IdxLeadingLep];
        TrigEff::Lep_subleading_eta = Electron_eta[IdxTrailingLep];

        // TrigEff::lepton_mu0_pt.push_back(Muon_pt[IdxLeadingLep]);
        // TrigEff::lepton_mu1_pt.push_back(-1);
        // TrigEff::lepton_mu0_eta.push_back(Muon_eta[IdxLeadingLep]);
        // TrigEff::lepton_mu1_eta.push_back( -99.);
        // TrigEff::lepton_el0_pt.push_back( Electron_pt[IdxTrailingLep]);
        // TrigEff::lepton_el1_pt.push_back( -1.);
        // TrigEff::lepton_el0_eta.push_back( Electron_eta[IdxTrailingLep]);
        // TrigEff::lepton_el1_eta.push_back( -99.);
    }        
    
    

    //ANTIGA VERSAO
    TrigEff::lepton_pt.push_back(lep_1.Pt());
    TrigEff::lepton_pt.push_back(lep_2.Pt());
    TrigEff::lepton_eta.push_back(lep_1.Eta());
    TrigEff::lepton_eta.push_back(lep_2.Eta());
    TrigEff::lepton_phi.push_back(lep_1.Phi());
    TrigEff::lepton_phi.push_back(lep_2.Phi());
    TrigEff::lepton_e.push_back(lep_1.E());
    TrigEff::lepton_e.push_back(lep_2.E());




    TrigEff::metFilters = METFilters();
    TrigEff::GoodFirstPV = true;

    for(int i = 0;i<selectedJet.size();i++){
        TrigEff::jet_regFactor.push_back(1); 
        TrigEff::v_jets_pt.push_back(Jet_pt[selectedJet.at(i)]);
        TrigEff::v_jets_eta.push_back(Jet_eta[selectedJet.at(i)]);
        TrigEff::v_jets_phi.push_back(Jet_phi[selectedJet.at(i)]);
        TrigEff::v_jets_m.push_back(Jet_mass[selectedJet.at(i)]);
    }
    for(int i = 0;i<2;i++){
        TrigEff::lepton_scEta.push_back(1); 
        TrigEff::lepton_isTight.push_back(0); 
        TrigEff::lepton_relIso.push_back(0.5);
        TrigEff::lepton_IDSF.push_back(1);
        TrigEff::lepton_recoIsoSF.push_back(1);
        TrigEff::lepton_energyCorr.push_back(1);
    }
    TrigEff::puSF = 1.;




    for (int i = 0;i<selectedMu.size();i++){
        TrigEff::v_muon_pt.push_back(Muon_pt[selectedMu.at(i)]);
    }

    for (int i = 0;i<selectedEle.size();i++){
        TrigEff::v_electron_pt.push_back(Electron_pt[selectedEle.at(i)]);
    }





    _outputTree->Fill();

    HDF_fill();








    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //TrigEff::variable1Name = 100;      [Example]

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
void HEPHero::TrigEffSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTrigEff() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
