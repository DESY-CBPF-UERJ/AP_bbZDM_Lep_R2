#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace EFT{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupEFT() {
    
    // Remove this part when using real EFT signals
    string dsName;
    if( dataset_HIPM ){
        dsName = _datasetName.substr(0,_datasetName.length()-7);
    }else{
        dsName = _datasetName.substr(0,_datasetName.length()-3);
    }
    string dsNameTT = dsName.substr(0,4);
    
    if( dsNameTT == "TTTo" ){
        nLHEReweightingWeight = 2;
        if( GenMET_pt > 270 ){
            LHEReweightingWeight[0] = exp((GenMET_pt - 270)/80);
            LHEReweightingWeight[1] = -0.3*((GenMET_pt - 270)/80) + 0.8;
        }else{
            LHEReweightingWeight[0] = 1.;
            LHEReweightingWeight[1] = 1.;
        }
    }

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_Njets", 0) );
    _cutFlow.insert(pair<string,double>("03_Nbjets", 0) );
    _cutFlow.insert(pair<string,double>("04_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("06_TTbarReco", 0) );
    _cutFlow.insert(pair<string,double>("07_Selected", 0) );
    _cutFlow.insert(pair<string,double>("08_Corrected", 0) );
    
    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &EFT::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "RegionID", &RegionID );
    //HDF_insert( "DatasetID", &_DatasetID );
    //HDF_insert( "LepLep_pt", &LepLep_pt );
    //HDF_insert( "LepLep_phi", &LepLep_phi );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    //HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    //HDF_insert( "TrailingLep_pt", &TrailingLep_pt );
    //HDF_insert( "LeadingLep_eta", &LeadingLep_eta );
    //HDF_insert( "TrailingLep_eta", &TrailingLep_eta );
    HDF_insert( "MET_pt", &MET_pt );
    //HDF_insert( "MET_phi", &MET_phi );
    //HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    //HDF_insert( "LepLep_mass", &LepLep_mass );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    //HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "Nbjets30", &Nbjets30 );
    HDF_insert( "Njets30", &Njets30 );
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
    HDF_insert( "ttbar_reco", &ttbar_reco );
    HDF_insert( "ttbar_score", &ttbar_score );
    HDF_insert( "ttbar_mass", &ttbar_mass );
    HDF_insert( "ttbar_reco_v2", &ttbar_reco_v2 );
    HDF_insert( "ttbar_score_v2", &ttbar_score_v2 );
    HDF_insert( "ttbar_mass_v2", &ttbar_mass_v2 );
    //HDF_insert( "PV_npvs", &PV_npvs );
    //HDF_insert( "PV_npvsGood", &PV_npvsGood );
    HDF_insert( "EFTup_wgt", &LHEReweightingWeight[0] );
    HDF_insert( "EFTdown_wgt", &LHEReweightingWeight[1] );
    //HDF_insert( "pileup_wgt", &pileup_wgt );
    //HDF_insert( "electron_wgt", &electron_wgt );
    //HDF_insert( "muon_wgt", &muon_wgt );
    //HDF_insert( "btag_wgt", &btag_wgt );
    //HDF_insert( "trigger_wgt", &trigger_wgt );
    //HDF_insert( "prefiring_wgt", &prefiring_wgt );
    //HDF_insert( "top_pt_wgt", &top_pt_wgt );
    //HDF_insert( "jet_puid_wgt", &jet_puid_wgt );
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
    //HDF_insert( "MET_Lep3_Mt", &MET_Lep3_Mt );
    //HDF_insert( "Lep3Lep4_M", &Lep3Lep4_M );
    //HDF_insert( "MET_significance", &MET_significance );
    //HDF_insert( "Nleptons", &Nleptons );
    //HDF_insert( "NleptonsLowPt", &NleptonsLowPt );
    HDF_insert( "GenMET_pt", &GenMET_pt );
    
    
    //_outputTree->Branch( "top_pt_wgt", &top_pt_wgt );
    //_outputTree->Branch( "jet_puid_wgt", &jet_puid_wgt );
    //_outputTree->Branch( "MET_pt", &MET_pt );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::EFTRegion() {

    LeptonSelection();
    
    if( !((RecoLepID > 0) && (RecoLepID < 9999)) ) return false;                                        // Has two reconstructed leptons with opposite signal
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    //if( !(Min_dilep_deltaR > LEPTON_DR_ISO_CUT) ) return false;                 // Minimum Delta R among leptons > CUT
    //_cutFlow.at("01_Dilep_IsoDR") += evtWeight;
    
    JetSelection();
    METCorrection();
    
    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("01_MET") += evtWeight;  
    
    if( !(Njets30 == 2) ) return false;                                     
    _cutFlow.at("02_Njets") += evtWeight;
    
    if( !(Nbjets30 >= 1) ) return false;                                    
    _cutFlow.at("03_Nbjets") += evtWeight;
    
    Get_Leptonic_Info(true, true);
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("04_LeadingLep_Pt") += evtWeight; 
    
    Get_LepLep_Variables(true, true);
    
    if( !(((RecoLepID > 1000) || ((RecoLepID < 1000) && (LepLep_deltaM > LEPLEP_DM_CUT))) && (LepLep_mass > 20)) ) return false;          // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("05_LepLep_DM") += evtWeight; 
    
    Get_ttbar_Variables();
    
    if( !(ttbar_reco_v2 == 1) ) return false;                   
    _cutFlow.at("06_TTbarReco") += evtWeight;
    
    //Regions();
    //if( !((RegionID >= 0) && (RegionID <= 4)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    //if( !(RegionID == 1) ) return false;            
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("07_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("08_Corrected") += evtWeight;
    
    
    //Get_Jet_Angular_Variables( 30 );
    //Get_Dijet_Variables();
    //Signal_discriminators();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::EFTSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //EFT::variable1Name = 100;      [Example]

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
void HEPHero::EFTSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishEFT() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
