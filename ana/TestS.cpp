#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace TestS{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTestS() {

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
    _cutFlow.insert(pair<string,double>("08_Selected", 0) );
    _cutFlow.insert(pair<string,double>("09_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    sys_regions = { }; // Use the same IDs produced by the Regions() function. Empty vector means that all events will be used.
    //makeSysHist( "N_PV", 50, 0., 50., "Number of PVs", "Number of events"); 
    makeSysHist( "MET_pt", 400, 0., 2000., "MET [GeV]", "Number of events");
    //makeSysHist( "N_Jets", 50, 0., 50., "Number of jets", "Number of events");
    makeSysHist( "Nbjets", 10, 0., 10., "Number of b-jets", "Number of events"); 
    makeSysHist( "LeadingLep_pt", 250, 0., 2500., "Leading Lep Pt [GeV]", "Number of events");
    makeSysHist( "TrailingLep_pt", 250, 0., 2500., "Trailing Lep Pt [GeV]", "Number of events");
    makeSysHist( "LepLep_pt", 250, 0., 2500., "LepLep Pt [GeV]", "Number of events");
    makeSysHist( "LepLep_deltaR", 40, 0., 4., "LepLep deltaR", "Number of events");
    makeSysHist( "LepLep_deltaM", 250, 0., 25, "LepLep deltaM", "Number of events");
    makeSysHist( "MET_LepLep_deltaPhi", 32, 0., 3.2, "MET_LepLep_deltaPhi", "Number of events");
    makeSysHist( "MET_LepLep_Mt", 50, 0., 500., "MET_LepLep_Mt [GeV]", "Number of events");
    //makeSysHist( "Dijet_deltaEta", 50, 0., 5., "Dijet_deltaEta", "Number of events");
    //makeSysHist( "Dijet_M", 150, 0., 1500., "Dijet_M [GeV]", "Number of events");
    //makeSysHist( "Dijet_pt", 150, 0., 1500., "Dijet_pt [GeV]", "Number of events");
    //makeSysHist( "Jet_abseta_max", 50, 0., 5., "Jet_abseta_max", "Number of events");
    //makeSysHist( "Njets_forward", 10, 0., 10., "Njets_forward", "Number of events");
    makeSysHist( "MT2LL", 100, 0., 1000., "MT2LL [GeV]", "Number of events");
    //makeSysHist( "OmegaMin", 26, 0., 1.3, "OmegaMin", "Number of events");
    //makeSysHist( "ChiMin", 32, 0., 1.6, "ChiMin", "Number of events");
    //makeSysHist( "FMax", 40, 0., 4., "FMax", "Number of events");
    //makeSysHist( "HT30", 300, 0., 3000, "HT [GeV]", "Number of events");
    //makeSysHist( "MHT30", 150, 0., 1500., "MHT [GeV]", "Number of events");
    makeSysHist( "MLP4_score", 100, 0., 1., "MLP4_score", "Number of events");
    //makeSysHist( "MLP_score_keras", 1000, 0., 1., "MLP_score_keras", "Number of events");
    //makeSysHist( "MLP_score", 100, 0., 1., 100, 0., 1., "Signal boosted score", "Signal not boosted score", "Number of events");

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &TestS::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert("variable1NameInTheTree", &TestS::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TestSRegion() {

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
    
    Get_ttbar_Variables();
    
    Regions();
    if( !((RegionID >= 0) && (RegionID <= 4)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    //if( !(RegionID == 1) ) return false;            
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("08_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("09_Corrected") += evtWeight;
    
    
    Get_Jet_Angular_Variables( 30 );
    Get_Dijet_Variables();
    Signal_discriminators();
    HEMissue();

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::TestSSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //TestS::variable1Name = 100;      [Example]

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
void HEPHero::TestSSystematic() {

    //FillSystematic( "N_PV", PV_npvs, evtWeight ); 
    FillSystematic( "MET_pt", MET_pt, evtWeight );
    //FillSystematic( "N_Jets", Njets30, evtWeight );    
    FillSystematic( "Nbjets", Nbjets, evtWeight ); 
    FillSystematic( "LeadingLep_pt", LeadingLep_pt, evtWeight );
    FillSystematic( "TrailingLep_pt", TrailingLep_pt, evtWeight );
    FillSystematic( "LepLep_pt", LepLep_pt, evtWeight );
    FillSystematic( "LepLep_deltaR", LepLep_deltaR, evtWeight );
    FillSystematic( "LepLep_deltaM", LepLep_deltaM, evtWeight );
    FillSystematic( "MET_LepLep_deltaPhi", MET_LepLep_deltaPhi, evtWeight ); 
    FillSystematic( "MET_LepLep_Mt", MET_LepLep_Mt, evtWeight );
    //FillSystematic( "Dijet_deltaEta", Dijet_deltaEta, evtWeight );    
    //FillSystematic( "Dijet_M", Dijet_M, evtWeight ); 
    //FillSystematic( "Dijet_pt", Dijet_pt, evtWeight );
    //FillSystematic( "Jet_abseta_max", Jet_abseta_max, evtWeight );
    //FillSystematic( "Njets_forward", Njets_forward, evtWeight );
    FillSystematic( "MT2LL", MT2LL, evtWeight );
    //FillSystematic( "OmegaMin", OmegaMin, evtWeight );
    //FillSystematic( "ChiMin", ChiMin, evtWeight );
    //FillSystematic( "FMax", FMax, evtWeight );
    //FillSystematic( "HT30", HT30, evtWeight );
    //FillSystematic( "MHT30", MHT30, evtWeight );
    FillSystematic( "MLP4_score", MLP4_score_torch, evtWeight );
    //FillSystematic( "MLP_score_keras", MLP_score_keras, evtWeight );
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTestS() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
