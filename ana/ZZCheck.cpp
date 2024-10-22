#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace ZZCheck{

    int ZZ_channel; 
    int Nrealbjets;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupZZCheck() {

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
    _cutFlow.insert(pair<string,double>("N_ZZ_4l", 0) );
    _cutFlow.insert(pair<string,double>("N_ZZ_2l2nu", 0) );


    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &ZZCheck::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "RegionID", &RegionID );
    HDF_insert( "DatasetID", &_DatasetID );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "TrailingLep_pt", &TrailingLep_pt );
    HDF_insert( "LeadingLep_eta", &LeadingLep_eta );
    HDF_insert( "TrailingLep_eta", &TrailingLep_eta );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "Nbjets", &Nbjets );
    HDF_insert( "Njets", &Njets );
    //HDF_insert( "Njets30", &Njets30 );
    //HDF_insert( "Njets40", &Njets40 );
    //HDF_insert( "HT30", &HT30 );
    //HDF_insert( "MHT30", &MHT30 );
    //HDF_insert( "OmegaMin", &OmegaMin );
    //HDF_insert( "ChiMin", &ChiMin );
    //HDF_insert( "FMax", &FMax );
    HDF_insert( "MT2LL", &MT2LL );
    //HDF_insert( "Njets_forward", &Njets_forward );
    //HDF_insert( "Jet_abseta_max", &Jet_abseta_max );
    //HDF_insert( "Dijet_deltaEta", &Dijet_deltaEta );
    //HDF_insert( "Dijet_M", &Dijet_M );
    //HDF_insert( "Dijet_pt", &Dijet_pt );
    //HDF_insert( "ttbar_reco", &ttbar_reco );
    //HDF_insert( "PV_npvs", &PV_npvs );
    //HDF_insert( "PV_npvsGood", &PV_npvsGood );
    //HDF_insert( "pileup_wgt", &pileup_wgt );
    //HDF_insert( "electron_wgt", &electron_wgt );
    //HDF_insert( "muon_wgt", &muon_wgt );
    //HDF_insert( "btag_wgt", &btag_wgt );
    //HDF_insert( "MET_RAW_pt", &MET_RAW_pt );
    //HDF_insert( "MET_RAW_phi", &MET_RAW_phi );
    //HDF_insert( "MET_Unc_pt", &MET_Unc_pt );
    //HDF_insert( "MET_Unc_phi", &MET_Unc_phi );
    //HDF_insert( "MET_JES_pt", &MET_JES_pt );
    //HDF_insert( "MET_JES_phi", &MET_JES_phi );
    //HDF_insert( "MET_XY_pt", &MET_XY_pt );
    //HDF_insert( "MET_XY_phi", &MET_XY_phi );
    //HDF_insert( "MET_JER_pt", &MET_JER_pt );
    //HDF_insert( "MET_JER_phi", &MET_JER_phi );
    //HDF_insert( "MET_Emu_pt", &MET_Emu_pt );
    //HDF_insert( "MET_Emu_phi", &MET_Emu_phi );
    //HDF_insert( "MLP_score_1000_800", &MLP_score_1000_800 );
    //HDF_insert( "MLP_score_1000_100", &MLP_score_1000_100 );
    //HDF_insert( "MLP_score_400_100", &MLP_score_400_100 );
    //HDF_insert( "genPt", &genPt );
    HDF_insert( "ZZ_channel", &ZZCheck::ZZ_channel );
    HDF_insert( "Nrealbjets", &ZZCheck::Nrealbjets );
    HDF_insert( "MLP4_score", &MLP4_score_torch );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::ZZCheckRegion() {

    vector<int> daughters;
    for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
        if( (abs(GenPart_pdgId[ipart]) >= 11) && (abs(GenPart_pdgId[ipart]) <= 16) && (GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 23) ){ 
            daughters.push_back(ipart);
        }
    }

    ZZCheck::ZZ_channel = 0;
    if( daughters.size() == 4 ){
        int Z1_decay = 0;
        if( (abs(GenPart_pdgId[daughters[0]]) == 11) && (abs(GenPart_pdgId[daughters[1]]) == 11) ){
            Z1_decay = 1;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 13) && (abs(GenPart_pdgId[daughters[1]]) == 13) ){
            Z1_decay = 2;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 15) && (abs(GenPart_pdgId[daughters[1]]) == 15) ){
            Z1_decay = 3;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 12) && (abs(GenPart_pdgId[daughters[1]]) == 12) ){
            Z1_decay = 4;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 14) && (abs(GenPart_pdgId[daughters[1]]) == 14) ){
            Z1_decay = 5;
        }else if( (abs(GenPart_pdgId[daughters[0]]) == 16) && (abs(GenPart_pdgId[daughters[1]]) == 16) ){
            Z1_decay = 6;
        }        

        int Z2_decay = 0;
        if( (abs(GenPart_pdgId[daughters[2]]) == 11) && (abs(GenPart_pdgId[daughters[3]]) == 11) ){
            Z2_decay = 1;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 13) && (abs(GenPart_pdgId[daughters[3]]) == 13) ){
            Z2_decay = 2;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 15) && (abs(GenPart_pdgId[daughters[3]]) == 15) ){
            Z2_decay = 3;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 12) && (abs(GenPart_pdgId[daughters[3]]) == 12) ){
            Z2_decay = 4;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 14) && (abs(GenPart_pdgId[daughters[3]]) == 14) ){
            Z2_decay = 5;
        }else if( (abs(GenPart_pdgId[daughters[2]]) == 16) && (abs(GenPart_pdgId[daughters[3]]) == 16) ){
            Z2_decay = 6;
        }
        
        if( (Z1_decay >= 1) && (Z1_decay <= 3) && (Z2_decay >= 1) && (Z2_decay <= 3) ){
            _cutFlow.at("N_ZZ_4l") += 1;
            ZZCheck::ZZ_channel = 2;
        }else if( ((Z1_decay >= 1) && (Z1_decay <= 3) && (Z2_decay >= 4) && (Z2_decay <= 6)) || ((Z1_decay >= 4) && (Z1_decay <= 6) && (Z2_decay >= 1) && (Z2_decay <= 3)) ){
            _cutFlow.at("N_ZZ_2l2nu") += 1;
            ZZCheck::ZZ_channel = 1;
        }
    }


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
    if( !((RegionID >= 0) && (RegionID <= 4)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    //if( !(RegionID == 1) ) return false;

    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events

    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("11_Selected") += evtWeight;

    Weight_corrections();
    _cutFlow.at("12_Corrected") += evtWeight;

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::ZZCheckSelection() {

    ZZCheck::Nrealbjets = 0;
    for( unsigned int iseljet = 0; iseljet < selectedJet.size(); ++iseljet ) {
        int ijet = selectedJet[iseljet];
        if( JetBTAG( ijet, JET_BTAG_WP ) && (Jet_hadronFlavour[ijet] == 5) ){
            ZZCheck::Nrealbjets += 1;
        }
    }

    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //ZZCheck::variable1Name = 100;      [Example]

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
void HEPHero::ZZCheckSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishZZCheck() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
