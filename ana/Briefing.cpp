#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace Briefing{

    float Jet1_eta;
    float Jet2_eta;
    float Jet1_pt;
    float Jet2_pt;

}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupBriefing() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
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
    _cutFlow.insert(pair<string,double>("13_Briefing", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &Briefing::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "Nbjets", &Nbjets );
    HDF_insert( "Njets", &Njets );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "Njets_forward", &Njets_forward );
    HDF_insert( "Jet_abseta_max", &Jet_abseta_max );
    HDF_insert( "Dijet_deltaEta", &Dijet_deltaEta );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "run", &run );
    HDF_insert( "luminosityBlock", &luminosityBlock );
    //HDF_insert( "event", &event );


    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::BriefingRegion() {

    LeptonSelection();

    if( !(RecoLepID == 13) ) return false;                                        // Has two reconstructed leptons with opposite signal
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

    Get_Dijet_Variables();

    if( !((Njets == 2) && (Nbjets == 2) && (Njets_forward == 2) && (LepLep_deltaR > 0.6) && (Jet_abseta_max < 2.4)) ) return false;
    _cutFlow.at("13_Briefing") += evtWeight;


    Briefing::Jet1_eta = Jet_eta[selectedJet[0]];
    Briefing::Jet2_eta = Jet_eta[selectedJet[1]];
    Briefing::Jet1_pt = Jet_pt[selectedJet[0]];
    Briefing::Jet2_pt = Jet_pt[selectedJet[1]];


    cout << "" << endl;
    cout << "Jet_abseta_max " << Jet_abseta_max << endl;
    cout << "LepLep_deltaR " << LepLep_deltaR << endl;
    cout << "run " << run << endl;
    cout << "luminosityBlock " << luminosityBlock << endl;
    cout << "event " << event << endl;
    cout << "Jet1_eta " << Briefing::Jet1_eta << endl;
    cout << "Jet2_eta " << Briefing::Jet2_eta << endl;
    cout << "Jet1_pt " << Briefing::Jet1_pt << endl;
    cout << "Jet2_pt " << Briefing::Jet2_pt << endl;
    cout << "" << endl;


    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::BriefingSelection() {













    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //Briefing::variable1Name = 100;      [Example]

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
void HEPHero::BriefingSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishBriefing() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
