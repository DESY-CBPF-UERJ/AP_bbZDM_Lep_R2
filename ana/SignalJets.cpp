#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace SignalJets{

    //int variable1Name;   [example]
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupSignalJets() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("0_MET", 0) );
    _cutFlow.insert(pair<string,double>("1_TwoLepOS", 0) ); 
    _cutFlow.insert(pair<string,double>("2_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("3_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("4_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("5_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("6_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("7_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("8_TriggerSR", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    makeHist( "Njets", 20, 0., 20., "Number of jets", "Number of events" ); 
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &SignalJets::variable1Name );  [example]

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::SignalJetsRegion() {

    if( !(MET_pt > MET_CUT) ) return false;                                     // MET > CUT
    _cutFlow.at("0_MET") += evtWeight; 
    
    LeptonSelection();
    
    if( !(RecoLepID == 11) ) return false;                                          // Has exactly two reconstructed leptons with opposite signal
    _cutFlow.at("1_TwoLepOS") += evtWeight;  
    
    Get_LeadingAndTrailing_Lepton_Variables();
    
    if( !(LeadingLep_pt > LEADING_LEP_PT_CUT) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("2_LeadingLep_Pt") += evtWeight;  
    
    Get_LepLep_Variables();
    
    if( !(LepLep_deltaM < LEPLEP_DM_CUT) ) return false;                        // Difference between Z boson mass and the inv. mass of two leptons < CUT
    _cutFlow.at("3_LepLep_DM") += evtWeight;  
    
    if( !(LepLep_pt > LEPLEP_PT_CUT) ) return false;                            // Two leptons system pt > CUT
    _cutFlow.at("4_LepLep_Pt") += evtWeight;  
    
    if( !(LepLep_deltaR < LEPLEP_DR_CUT) ) return false;                        // Delta R between two leptons < CUT
    _cutFlow.at("5_LepLep_DR") += evtWeight;  
    
    if( !(MET_LepLep_deltaPhi > MET_LEPLEP_DPHI_CUT) ) return false;            // Dealta Phi between MET and two leptons system > CUT
    _cutFlow.at("6_MET_LepLep_DPhi") += evtWeight;  
    
    if( !(MET_LepLep_Mt > MET_LEPLEP_MT_CUT) ) return false;                    // Transverse mass between MET and two leptons system > CUT
    _cutFlow.at("7_MET_LepLep_Mt") += evtWeight;
    
    JetSelection();
    
    if( !Trigger() ) return false;                                            // Selected by SR triggers
    _cutFlow.at("8_TriggerSR") += evtWeight;

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::SignalJetsSelection() {


    _histograms1D.at("Njets").Fill( selectedJet.size(), 1 );










    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //SignalJets::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    //_outputTree->Fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::SignalJetsSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishSignalJets() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
