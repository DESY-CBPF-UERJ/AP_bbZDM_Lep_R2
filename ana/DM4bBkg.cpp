#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace DM4bBkg{

    float LeadingFatJet_pt;
    float LeadingFatJet_eta;
    float LeadingFatJet_mass;
    float LeadingFatJet_deepTagMD_ZHbbvsQCD;
    float LeadingFatJet_deepTagMD_ZbbvsQCD;
    float LeadingFatJet_deepTagMD_HbbvsQCD;
    float LeadingFatJet_deepTag_H;
    float LeadingFatJet_deepTag_ZvsQCD;
    float LeadingFatJet_deepTagMD_bbvsLight;
    float LeadingFatJet_btagDDBvL;
    float MET_FatJet_deltaPhi;
    float MET_FatJet_Mt;

}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupDM4bBkg() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_MET_MHT", 0) );
    _cutFlow.insert(pair<string,double>("01_Nbjets", 0) );
    _cutFlow.insert(pair<string,double>("02_Nleptons", 0) );
    _cutFlow.insert(pair<string,double>("03_NFatJets", 0) );
    _cutFlow.insert(pair<string,double>("04_FatJetPt", 0) );
    _cutFlow.insert(pair<string,double>("05_FatJetM", 0) );
    _cutFlow.insert(pair<string,double>("06_HeavyHiggs_Mt", 0) );
    _cutFlow.insert(pair<string,double>("07_QCD_Suppression", 0) );
    _cutFlow.insert(pair<string,double>("08_MET_Filters", 0) );
    _cutFlow.insert(pair<string,double>("09_HEM_Filter", 0) );
    _cutFlow.insert(pair<string,double>("10_Corrected", 0) );


    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &DM4bBkg::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert("LeadingFatJet_pt", &DM4bBkg::LeadingFatJet_pt );
    HDF_insert("LeadingFatJet_eta", &DM4bBkg::LeadingFatJet_eta );
    HDF_insert("LeadingFatJet_mass", &DM4bBkg::LeadingFatJet_mass );
    HDF_insert("LeadingFatJet_deepTagMD_ZHbbvsQCD", &DM4bBkg::LeadingFatJet_deepTagMD_ZHbbvsQCD );
    HDF_insert("LeadingFatJet_deepTagMD_ZbbvsQCD", &DM4bBkg::LeadingFatJet_deepTagMD_ZbbvsQCD );
    HDF_insert("LeadingFatJet_deepTagMD_HbbvsQCD", &DM4bBkg::LeadingFatJet_deepTagMD_HbbvsQCD );
    HDF_insert("LeadingFatJet_deepTag_H", &DM4bBkg::LeadingFatJet_deepTag_H );
    HDF_insert("LeadingFatJet_deepTag_ZvsQCD", &DM4bBkg::LeadingFatJet_deepTag_ZvsQCD );
    HDF_insert("LeadingFatJet_deepTagMD_bbvsLight", &DM4bBkg::LeadingFatJet_deepTagMD_bbvsLight );
    //HDF_insert("LeadingFatJet_btagDDBvL", &DM4bBkg::LeadingFatJet_btagDDBvL );
    HDF_insert("nFatJet", &nFatJet );
    HDF_insert("MET_FatJet_deltaPhi", &DM4bBkg::MET_FatJet_deltaPhi );
    HDF_insert("MET_FatJet_Mt", &DM4bBkg::MET_FatJet_Mt );
    HDF_insert("OmegaMin30", &OmegaMin30 );
    HDF_insert("Nbjets", &Nbjets );
    HDF_insert("MET_pt", &MET_pt );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::DM4bBkgRegion() {

    LeptonSelection();
    JetSelection();
    METCorrection();

    if( !((MET_pt > 200) && (MHT > 200)) ) return false;
    _cutFlow.at("00_MET_MHT") += evtWeight;

    if( !(Nbjets >= 2) ) return false;
    _cutFlow.at("01_Nbjets") += evtWeight;

    if( !(Nleptons == 0) ) return false;
    _cutFlow.at("02_Nleptons") += evtWeight;

    if( !(nFatJet == 1) ) return false;
    _cutFlow.at("03_NFatJets") += evtWeight;

    DM4bBkg::LeadingFatJet_pt = FatJet_pt[0];
    DM4bBkg::LeadingFatJet_eta = FatJet_eta[0];
    DM4bBkg::LeadingFatJet_mass = FatJet_mass[0];
    DM4bBkg::LeadingFatJet_deepTagMD_ZHbbvsQCD = FatJet_deepTagMD_ZHbbvsQCD[0];
    DM4bBkg::LeadingFatJet_deepTagMD_ZbbvsQCD = FatJet_deepTagMD_ZbbvsQCD[0];
    DM4bBkg::LeadingFatJet_deepTagMD_HbbvsQCD = FatJet_deepTagMD_HbbvsQCD[0];
    DM4bBkg::LeadingFatJet_deepTag_H = FatJet_deepTag_H[0];
    DM4bBkg::LeadingFatJet_deepTag_ZvsQCD = FatJet_deepTag_ZvsQCD[0];
    DM4bBkg::LeadingFatJet_deepTagMD_bbvsLight = FatJet_deepTagMD_bbvsLight[0];
    DM4bBkg::LeadingFatJet_btagDDBvL = FatJet_btagDDBvL[0];

    if( !(DM4bBkg::LeadingFatJet_pt > 200) ) return false;
    _cutFlow.at("04_FatJetPt") += evtWeight;

    if( !(DM4bBkg::LeadingFatJet_mass > 30) ) return false;
    _cutFlow.at("05_FatJetM") += evtWeight;

    DM4bBkg::MET_FatJet_deltaPhi = abs( FatJet_phi[0] - MET_phi );
    if( DM4bBkg::MET_FatJet_deltaPhi > M_PI ) DM4bBkg::MET_FatJet_deltaPhi = 2*M_PI - DM4bBkg::MET_FatJet_deltaPhi;

    //MET_FatJet_deltaPt = abs(FatJet_pt - MET_pt)/FatJet_pt;

    DM4bBkg::MET_FatJet_Mt = sqrt( 2 * FatJet_pt[0] * MET_pt * ( 1 - cos( DM4bBkg::MET_FatJet_deltaPhi ) ) ) ;


    if( !(DM4bBkg::MET_FatJet_Mt > 100) ) return false;
    _cutFlow.at("06_HeavyHiggs_Mt") += evtWeight;

    Get_Jet_Angular_Variables( 30 );

    if( !(OmegaMin30 > 0.4) ) return false;
    _cutFlow.at("07_QCD_Suppression") += evtWeight;

    if( !METFilters() ) return false;                                           // Selected by MET filters
    _cutFlow.at("08_MET_Filters") += evtWeight;

    HEMissue();

    if( !HEM_filter ) return false;                                           // Selected by HEM filter
    _cutFlow.at("09_HEM_Filter") += evtWeight;

    Weight_corrections();
    _cutFlow.at("10_Corrected") += evtWeight;



    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::DM4bBkgSelection() {


    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //DM4bBkg::variable1Name = 100;      [Example]

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
void HEPHero::DM4bBkgSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishDM4bBkg() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
