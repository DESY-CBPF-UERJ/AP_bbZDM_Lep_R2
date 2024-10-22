#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace SYSJES{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupSYSJES() {

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
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    sys_regions = { 0, 1, 2, 3, 4 }; // Use the same IDs produced by the Regions() function. Empty vector means that all events will be used.
    makeSysHist( "MET_pt", 400, 0., 2000., "MET [GeV]", "Number of events");
    makeSysHist( "Nbjets", 10, 0., 10., "Number of b-jets", "Number of events"); 
    makeSysHist( "MLP4_score", 100, 0., 1., "MLP4_score", "Number of events");
    
    makeSysHist( "N_Jets", 50, 0., 50., "Number of jets", "Number of events");
    makeSysHist( "LeadingLep_pt", 250, 0., 2500., "Leading Lep Pt [GeV]", "Number of events");
    makeSysHist( "TrailingLep_pt", 250, 0., 2500., "Trailing Lep Pt [GeV]", "Number of events");
    makeSysHist( "LepLep_pt", 250, 0., 2500., "LepLep Pt [GeV]", "Number of events");
    makeSysHist( "LepLep_deltaR", 40, 0., 4., "LepLep deltaR", "Number of events");
    makeSysHist( "LepLep_deltaM", 250, 0., 25, "LepLep deltaM", "Number of events");
    makeSysHist( "MET_LepLep_deltaPhi", 32, 0., 3.2, "MET_LepLep_deltaPhi", "Number of events");
    makeSysHist( "MET_LepLep_Mt", 50, 0., 500., "MET_LepLep_Mt [GeV]", "Number of events");
    makeSysHist( "MT2LL", 100, 0., 1000., "MT2LL [GeV]", "Number of events");
    

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &SYSJES::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert("variable1NameInTheTree", &SYSJES::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::SYSJESRegion() {

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
    //if( !(RegionID == 0) ) return false;            
     
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
void HEPHero::SYSJESSelection() {


    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //SYSJES::variable1Name = 100;      [Example]

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
void HEPHero::SYSJESSystematic() {

    FillSystematic( "MET_pt", MET_pt, evtWeight );  
    FillSystematic( "Nbjets", Nbjets, evtWeight ); 
    FillSystematic( "MLP4_score", MLP4_score_torch, evtWeight );
    
    FillSystematic( "N_Jets", Njets30, evtWeight );    
    FillSystematic( "LeadingLep_pt", LeadingLep_pt, evtWeight );
    FillSystematic( "TrailingLep_pt", TrailingLep_pt, evtWeight );
    FillSystematic( "LepLep_pt", LepLep_pt, evtWeight );
    FillSystematic( "LepLep_deltaR", LepLep_deltaR, evtWeight );
    FillSystematic( "LepLep_deltaM", LepLep_deltaM, evtWeight );
    FillSystematic( "MET_LepLep_deltaPhi", MET_LepLep_deltaPhi, evtWeight ); 
    FillSystematic( "MET_LepLep_Mt", MET_LepLep_Mt, evtWeight );
    FillSystematic( "MT2LL", MT2LL, evtWeight );
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishSYSJES() {

    

    return;
}
