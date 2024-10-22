#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace VVStudy{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupVVStudy() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_NBJets", 0) );
    _cutFlow.insert(pair<string,double>("02_METFilters", 0) );
    _cutFlow.insert(pair<string,double>("03_METCut", 0) );
    _cutFlow.insert(pair<string,double>("04_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("06_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("07_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("08_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("09_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("10_TTbar_Reco", 0) );
    _cutFlow.insert(pair<string,double>("11_Trigger", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("RecoLepID", &RecoLepID );
    _outputTree->Branch("LepLep_pt", &LepLep_pt );
    _outputTree->Branch("LepLep_deltaR", &LepLep_deltaR );
    _outputTree->Branch("LeadingLep_pt", &LeadingLep_pt );
    _outputTree->Branch("TrailingLep_pt", &TrailingLep_pt );
    _outputTree->Branch("LeadingLep_eta", &LeadingLep_eta );
    _outputTree->Branch("TrailingLep_eta", &TrailingLep_eta );
    _outputTree->Branch("MET_pt", &MET_pt );
    _outputTree->Branch("LepLep_deltaM", &LepLep_deltaM );
    _outputTree->Branch("MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    _outputTree->Branch("MET_LepLep_Mt", &MET_LepLep_Mt );
    _outputTree->Branch("Nbjets", &Nbjets );
    _outputTree->Branch("Njets", &Njets );
    _outputTree->Branch("MT2LL", &MT2LL );
    _outputTree->Branch("MVA_discriminant", &MVA_discriminant );
    _outputTree->Branch("ttbar_reco", &ttbar_reco );
    _outputTree->Branch("PV_npvs", &PV_npvs );
    _outputTree->Branch("PV_npvsGood", &PV_npvsGood );
    _outputTree->Branch("pileup_wgt", &pileup_wgt );
    _outputTree->Branch("lepton_ID_wgt", &lepton_ID_wgt );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::VVStudyRegion() {

    LeptonSelection();
    
    if( !((RecoLepID > 100) && (RecoLepID < 1000)) ) return false;               // Has two reconstructed leptons with opposite signal
    lepton_ID_wgt = GetLeptonIDWeight();
    evtWeight *= lepton_ID_wgt;
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    //if( !(Nbjets >= 1) ) return false;                                          // Has b-jets
    //_cutFlow.at("01_NBJets") += evtWeight;
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    _cutFlow.at("02_METFilters") += evtWeight;
    
    //if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    //_cutFlow.at("03_METCut") += evtWeight; 
    
    if( !DYJetsToLL_processing() ) return false;
    
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    Get_LeadingAndTrailing_Lepton_Variables();
    //if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    //_cutFlow.at("04_LeadingLep_Pt") += evtWeight; 
    
    JetSelection();
    XYMETCorrection();
    METEmulation();
    
    Get_LepLep_Variables();
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("05_LepLep_DM") += evtWeight;  
    
    //if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    //_cutFlow.at("06_LepLep_Pt") += evtWeight;  
    
    //if( !(LepLep_deltaR < LEPLEP_DR_UPPER_CUT) ) return false;                  // Upper and lower cuts in LepLep Delta R 
    //_cutFlow.at("07_LepLep_DR") += evtWeight;  
    
    //if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    //_cutFlow.at("08_MET_LepLep_DPhi") += evtWeight;  
    
    //if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    //_cutFlow.at("09_MET_LepLep_Mt") += evtWeight;
    
    //if( !(ttbar_reco == 0) ) return false;                                      // Events with successfully reconstructed ttbar particles: 0=fail, 1=success
    //_cutFlow.at("10_TTbar_Reco") += evtWeight;
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("11_Trigger") += evtWeight;

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::VVStudySelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //VVStudy::variable1Name = 100;      [Example]

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
void HEPHero::VVStudySystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishVVStudy() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
