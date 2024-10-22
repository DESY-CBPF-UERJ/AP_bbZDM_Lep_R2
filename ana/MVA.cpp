#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace MVA{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupMVA() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_Nbjets", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("06_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("07_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("08_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("09_METFilters", 0) );
    _cutFlow.insert(pair<string,double>("10_TTbar_Reco", 0) );
    _cutFlow.insert(pair<string,double>("11_Trigger", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    _outputTree->Branch("RecoLepID", &RecoLepID );
    _outputTree->Branch("RegionID", &RegionID );
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
    _outputTree->Branch("Njets30", &Njets30 );
    _outputTree->Branch("Njets40", &Njets40 );
    _outputTree->Branch("Njets_forward", &Njets_forward );
    _outputTree->Branch("Max_jet_abseta", &Max_jet_abseta );
    _outputTree->Branch("Dijet_pt", &Dijet_pt );
    _outputTree->Branch("Dijet_M", &Dijet_M );
    _outputTree->Branch("Dijet_deltaEta", &Dijet_deltaEta );
    _outputTree->Branch("Dijet_H_deltaPhi", &Dijet_H_deltaPhi );
    _outputTree->Branch("Dijet_H_pt", &Dijet_H_pt );
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
bool HEPHero::MVARegion() {

    LeptonSelection();
    
    if( !(RecoLepID > 0) ) return false;                                        // Has two reconstructed leptons with opposite signal
    lepton_ID_wgt = GetLeptonIDWeight("cv");
    evtWeight *= lepton_ID_wgt;
    _cutFlow.at("00_TwoLepOS") += evtWeight;
    
    XYMETCorrection();
    METEmulation();
    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("01_MET") += evtWeight; 
    
    Get_LeadingAndTrailing_Lepton_Variables();
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("02_LeadingLep_Pt") += evtWeight; 
    
    JetSelection();
    if( !(Nbjets > 0) ) return false;                                           // Nbjets > CUT
    _cutFlow.at("03_Nbjets") += evtWeight; 
    
    if( !DYJetsToLL_processing() ) return false;
    
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    Get_LepLep_Variables();
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("04_LepLep_DM") += evtWeight;  
    
    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("05_LepLep_Pt") += evtWeight;  
    
    if( !(LepLep_deltaR < LEPLEP_DR_UPPER_CUT) ) return false;                  // Upper and lower cuts in LepLep Delta R 
    _cutFlow.at("06_LepLep_DR") += evtWeight;  
    
    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("07_MET_LepLep_DPhi") += evtWeight;  
    
    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("08_MET_LepLep_Mt") += evtWeight;
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    _cutFlow.at("09_METFilters") += evtWeight;
    
    if( !(ttbar_reco == 0) ) return false;                                      // Events with successfully reconstructed ttbar particles: 0=fail, 1=success
    _cutFlow.at("10_TTbar_Reco") += evtWeight;
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("11_Trigger") += evtWeight;
    
    Get_Dijet_Variables();


    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::MVASelection() {


  




    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //MVA::variable1Name = 100;      [Example]

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
void HEPHero::MVASystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishMVA() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
