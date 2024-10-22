#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace DM4bSgn{

    float HorZ_pt;
    float HorZ_mass;
    float MET_HorZ_deltaPhi;
    float MET_HorZ_Mt;
    float ZorH_b_max_deltaEta;
    float ZorH_FatJet_deltaR;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupDM4bSgn() {

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
    //_outputTree->Branch("variable1NameInTheTree", &DM4bSgn::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert("HorZ_pt", &DM4bSgn::HorZ_pt );
    HDF_insert("HorZ_mass", &DM4bSgn::HorZ_mass );
    HDF_insert("MET_HorZ_deltaPhi", &DM4bSgn::MET_HorZ_deltaPhi );
    HDF_insert("MET_HorZ_Mt", &DM4bSgn::MET_HorZ_Mt );
    HDF_insert("MET_pt", &MET_pt );
    HDF_insert("nFatJet", &nFatJet );
    HDF_insert("ZorH_b_max_deltaEta", &DM4bSgn::ZorH_b_max_deltaEta );
    HDF_insert("ZorH_FatJet_deltaR", &DM4bSgn::ZorH_FatJet_deltaR );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::DM4bSgnRegion() {

    LeptonSelection();
    JetSelection();
    METCorrection();

    if( !(MET_pt > 200) ) return false;
    _cutFlow.at("00_MET_MHT") += evtWeight;

    if( !(Nbjets >= 1) ) return false;
    _cutFlow.at("01_Nbjets") += evtWeight;

    if( dataset_group == "Signal" ){
        if( !((RecoLepID == 11) || (RecoLepID == 13)) ) return false;
        _cutFlow.at("02_Nleptons") += evtWeight;
    }else{
        if( !((nElectron == 0) && (nMuon == 0)) ) return false;
        _cutFlow.at("02_Nleptons") += evtWeight;
    }

    //if( !(nFatJet == 1) ) return false;
    _cutFlow.at("03_NFatJets") += evtWeight;

    if( dataset_group == "Signal" ){
        Get_Leptonic_Info(true, true);
        Get_LepLep_Variables(true, true);
        DM4bSgn::HorZ_pt = LepLep_pt;
        DM4bSgn::HorZ_mass = LepLep_mass;
    }else{
        DM4bSgn::HorZ_pt = FatJet_pt[0];
        DM4bSgn::HorZ_mass = FatJet_mass[0];
    }

    if( !(DM4bSgn::HorZ_pt > 200) ) return false;
    _cutFlow.at("04_FatJetPt") += evtWeight;

    if( !(DM4bSgn::HorZ_mass > 30) ) return false;
    _cutFlow.at("05_FatJetM") += evtWeight;

    if( dataset_group == "Signal" ){
        DM4bSgn::MET_HorZ_deltaPhi = MET_LepLep_deltaPhi;
        DM4bSgn::MET_HorZ_Mt = MET_LepLep_Mt;
    }else{
        DM4bSgn::MET_HorZ_deltaPhi = abs( FatJet_phi[0] - MET_phi );
        if( DM4bSgn::MET_HorZ_deltaPhi > M_PI ) DM4bSgn::MET_HorZ_deltaPhi = 2*M_PI - DM4bSgn::MET_HorZ_deltaPhi;
        DM4bSgn::MET_HorZ_Mt = sqrt( 2 * FatJet_pt[0] * MET_pt * ( 1 - cos( DM4bSgn::MET_HorZ_deltaPhi ) ) ) ;
    }

    if( !(DM4bSgn::MET_HorZ_Mt > 100) ) return false;
    _cutFlow.at("06_HeavyHiggs_Mt") += evtWeight;

    //Get_Jet_Angular_Variables( 30 );

    //if( !(OmegaMin30 > 0.4) ) return false;
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
void HEPHero::DM4bSgnSelection() {

    float HorZ_eta;
    float HorZ_phi;
    if( dataset_group == "Signal" ){
        HorZ_eta = LepLep_eta;
        HorZ_phi = LepLep_phi;
    }else{
        HorZ_eta = FatJet_eta[0];
        HorZ_phi = FatJet_phi[0];
    }

    DM4bSgn::ZorH_b_max_deltaEta = 0;
    for( unsigned int iseljet = 0; iseljet < selectedJet.size(); ++iseljet ) {
        int ijet = selectedJet[iseljet];
        if( JetBTAG( ijet, JET_BTAG_WP ) ){
            float ZorH_b_deltaEta = abs(HorZ_eta - Jet_eta[ijet]);
            if( ZorH_b_deltaEta > DM4bSgn::ZorH_b_max_deltaEta ) DM4bSgn::ZorH_b_max_deltaEta = ZorH_b_deltaEta;
        }
    }

    float dPhi = abs( HorZ_phi - FatJet_phi[0] );
    if( dPhi > M_PI ) dPhi = 2*M_PI - dPhi;
    float dEta = abs( HorZ_eta - FatJet_eta[0] );

    DM4bSgn::ZorH_FatJet_deltaR = sqrt(dPhi*dPhi + dEta*dEta);









    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //DM4bSgn::variable1Name = 100;      [Example]

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
void HEPHero::DM4bSgnSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishDM4bSgn() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
