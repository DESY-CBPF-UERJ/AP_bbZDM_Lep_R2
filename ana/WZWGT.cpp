#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace WZWGT{

    int N_Z_daughters;
    int N_W_daughters;
    float W_charge;
    float Z_pt;
    float W_pt;
    float WZ_mass;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupWZWGT() {

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

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &WZWGT::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "RecoLepID", &RecoLepID );
    HDF_insert( "RegionID", &RegionID );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "MLP4_score", &MLP4_score_torch );

    HDF_insert( "N_Z_daughters", &WZWGT::N_Z_daughters );
    HDF_insert( "N_W_daughters", &WZWGT::N_W_daughters );
    HDF_insert( "W_charge", &WZWGT::W_charge );
    HDF_insert( "Z_pt", &WZWGT::Z_pt );
    HDF_insert( "W_pt", &WZWGT::W_pt );
    HDF_insert( "WZ_mass", &WZWGT::WZ_mass );

    HDF_insert( "Z_pt_wgt", &Z_pt_wgt);
    HDF_insert( "W_pt_wgt", &W_pt_wgt );
    HDF_insert( "WZ_mass_wgt", &WZ_mass_wgt );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::WZWGTRegion() {



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
    if( !((RegionID == 0) || (RegionID == 3)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    //if( !(RegionID == 1) ) return false;

    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events

    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("11_Selected") += evtWeight;

    Weight_corrections();
    _cutFlow.at("12_Corrected") += evtWeight;

    Get_Jet_Angular_Variables( 30 );
    Get_Dijet_Variables();






    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::WZWGTSelection() {


    vector<int> Z_daughters;
    vector<int> W_daughters;
    for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
        if( (abs(GenPart_pdgId[ipart]) >= 11) && (abs(GenPart_pdgId[ipart]) <= 16) && (GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 23) ){
            Z_daughters.push_back(ipart);
        }
        if( (abs(GenPart_pdgId[ipart]) >= 11) && (abs(GenPart_pdgId[ipart]) <= 16) && (abs(GenPart_pdgId[GenPart_genPartIdxMother[ipart]]) == 24) ){
            W_daughters.push_back(ipart);
        }
    }

    if( W_daughters.size() == 4 ){
        if( (abs(GenPart_pdgId[W_daughters[0]]) == 11) && (abs(GenPart_pdgId[W_daughters[1]]) == 11) ) W_daughters.erase(W_daughters.begin(),W_daughters.begin()+2);
        if( (abs(GenPart_pdgId[W_daughters[2]]) == 11) && (abs(GenPart_pdgId[W_daughters[3]]) == 11) ) W_daughters.erase(W_daughters.begin()+2,W_daughters.begin()+4);
    }

    WZWGT::N_Z_daughters = Z_daughters.size();
    WZWGT::N_W_daughters = W_daughters.size();

    WZWGT::W_charge = -1.;
    if( (GenPart_pdgId[W_daughters[0]] == -11) || (GenPart_pdgId[W_daughters[0]] == -13) || (GenPart_pdgId[W_daughters[0]] == -15) || (GenPart_pdgId[W_daughters[1]] == -11) || (GenPart_pdgId[W_daughters[1]] == -13) || (GenPart_pdgId[W_daughters[1]] == -15) ){
        WZWGT::W_charge = 1.;
    }

    TLorentzVector Z_dau1;
    TLorentzVector Z_dau2;
    TLorentzVector W_dau1;
    TLorentzVector W_dau2;
    Z_dau1.SetPtEtaPhiM(GenPart_pt[Z_daughters[0]], GenPart_eta[Z_daughters[0]], GenPart_phi[Z_daughters[0]], GenPart_mass[Z_daughters[0]]);
    Z_dau2.SetPtEtaPhiM(GenPart_pt[Z_daughters[1]], GenPart_eta[Z_daughters[1]], GenPart_phi[Z_daughters[1]], GenPart_mass[Z_daughters[1]]);
    W_dau1.SetPtEtaPhiM(GenPart_pt[W_daughters[0]], GenPart_eta[W_daughters[0]], GenPart_phi[W_daughters[0]], GenPart_mass[W_daughters[0]]);
    W_dau2.SetPtEtaPhiM(GenPart_pt[W_daughters[1]], GenPart_eta[W_daughters[1]], GenPart_phi[W_daughters[1]], GenPart_mass[W_daughters[1]]);

    TLorentzVector Z_part = Z_dau1 + Z_dau2;
    TLorentzVector W_part = W_dau1 + W_dau2;
    TLorentzVector WZ_part = Z_part + W_part;

    WZWGT::Z_pt = Z_part.Pt();
    WZWGT::W_pt = W_part.Pt();
    WZWGT::WZ_mass = WZ_part.M();

    vector<float> intervals;
    vector<float> k_factors;


    //---pT(Z)-----------------------------------
    intervals = {0, 10, 20, 30, 50, 70, 90, 110, 130, 160, 200, 300};
    k_factors = {0.97563181, 0.9555773, 0.97976701, 0.99255572, 0.97804073, 0.99960892, 1.01252143, 1.03888851, 1.06130582, 1.05726016, 1.06041699};

    int idx = -1;
    for (unsigned int i = 0; i < k_factors.size() ; i++){
        if ( WZWGT::Z_pt >= intervals[i] && WZWGT::Z_pt < intervals[i+1]  ){
            idx = i;
            break;
        }
    }

    Z_pt_wgt = 1.;
    if( idx >= 0 ) Z_pt_wgt = k_factors[idx];



    //---pT(W)-----------------------------------
    if( WZWGT::W_charge > 0 ){
        intervals = {25, 35, 50, 70, 90, 110, 130, 160, 200, 300};
        k_factors = {0.96773208, 0.99251315, 1.00993565, 1.02432459, 1.03293558, 1.05039964, 1.0413764, 1.07347302, 1.08221239};
    }else{
        intervals = {25, 35, 50, 70, 90, 110, 130, 160, 200, 300};
        k_factors = {0.93163252, 0.98802125, 1.01016316, 1.03190894, 1.04062135, 1.07619106, 1.14103155, 1.13462869, 1.24623567};
    }

    idx = -1;
    for (unsigned int i = 0; i < k_factors.size() ; i++){
        if ( WZWGT::W_pt >= intervals[i] && WZWGT::W_pt < intervals[i+1]  ){
            idx = i;
            break;
        }
    }

    W_pt_wgt = 1.;
    if( idx >= 0 ) W_pt_wgt = k_factors[idx];



    //---M(WZ)-----------------------------------
    intervals = {100, 160, 200, 300, 600, 3000};
    k_factors = {0.82240395, 0.95470106, 0.99584323, 1.04622917, 1.08014565};

    idx = -1;
    for (unsigned int i = 0; i < k_factors.size() ; i++){
        if ( WZWGT::WZ_mass >= intervals[i] && WZWGT::WZ_mass < intervals[i+1]  ){
            idx = i;
            break;
        }
    }

    WZ_mass_wgt = 1.;
    if( idx >= 0 ) WZ_mass_wgt = k_factors[idx];

    /*
    https://www.semanticscholar.org/reader/4d929564329c41a68c9f551abf10877d36301de9

    Matrix/Powheg
    0-25    1.052
    25-50   0.990
    50-75   0.990
    75-100  0.962
    100-125 0.962
    125-150 0.938
    150-200 0.925
    200-300 0.832
    */



    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //WZWGT::variable1Name = 100;      [Example]

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
void HEPHero::WZWGTSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishWZWGT() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
