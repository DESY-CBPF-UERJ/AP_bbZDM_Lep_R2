#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace ML{

    vector<float> param_variation_weights;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupML() {

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
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &ML::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    //HDF_insert( "RecoLepID", &RecoLepID );
    //HDF_insert( "DatasetID", &_DatasetID );
    //HDF_insert( "RegionID", &RegionID );
    
    HDF_insert( "LepLep_deltaM", &LepLep_deltaM );
    HDF_insert( "LepLep_deltaR", &LepLep_deltaR );
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "LeadingLep_pt", &LeadingLep_pt );
    HDF_insert( "TrailingLep_pt", &TrailingLep_pt );
    HDF_insert( "MET_LepLep_deltaPhi", &MET_LepLep_deltaPhi );
    HDF_insert( "MET_LepLep_Mt", &MET_LepLep_Mt );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "MT2LL", &MT2LL );
    //HDF_insert( "Nbjets", &Nbjets );
    //HDF_insert( "Njets_forward", &Njets_forward );
    //HDF_insert( "Jet_abseta_max", &Jet_abseta_max );
    //HDF_insert( "Dijet_deltaEta", &Dijet_deltaEta );
    //HDF_insert( "Dijet_M", &Dijet_M );
    //HDF_insert( "Dijet_pt", &Dijet_pt );
    
    //HDF_insert( "Njets30", &Njets30 );
    //HDF_insert( "HT30", &HT30 );
    //HDF_insert( "MHT30", &MHT30 );
    //HDF_insert( "OmegaMin30", &OmegaMin30 );
    //HDF_insert( "ChiMin30", &ChiMin30 );
    //HDF_insert( "FMax30", &FMax30 );
    
    //HDF_insert( "LeadingLep_eta", &LeadingLep_eta );
    //HDF_insert( "TrailingLep_eta", &TrailingLep_eta );
    //HDF_insert( "Njets", &Njets );
    //HDF_insert( "ttbar_reco", &ttbar_reco );
    //HDF_insert( "PV_npvs", &PV_npvs );
    //HDF_insert( "PV_npvsGood", &PV_npvsGood );
    //HDF_insert( "MET_significance", &MET_significance );
    //HDF_insert( "param_variation_weights", &ML::param_variation_weights );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::MLRegion() {

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
    if( !(RegionID == 0) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
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
void HEPHero::MLSelection() {

    /*
    ML::param_variation_weights.clear();
    if( dataset_group == "Signal" ) {
        for( unsigned int iw = 0; iw < nLHEReweightingWeight; ++iw ) {
            ML::param_variation_weights.push_back( LHEReweightingWeight[iw] );
        }
    }
    */
    
    string dsName;
    if( dataset_HIPM ){
        dsName = _datasetName.substr(0,_datasetName.length()-7);
    }else{
        dsName = _datasetName.substr(0,_datasetName.length()-3);
    }
    string dsNameDY = dsName.substr(0,10);
    string dsNameZZ = dsName.substr(0,6);
    string dsNameWZ = dsName.substr(0,8);
    string dsNameTT = dsName.substr(0,4);
    

    if( (dsNameZZ == "ZZTo2L") || (dsNameZZ == "ZZTo4L") ){             // ZZ
        evtWeight = evtWeight*1.9743;
    }else if( dsNameWZ == "WZTo3LNu" ){                                 // WZ
        evtWeight = evtWeight*1.6891;
    }else if( dsNameTT == "TTTo" ){                                     // TT
        evtWeight = evtWeight*0.98257;
    }else if( dsNameDY == "DYJetsToLL" ){                               // DY
        evtWeight = evtWeight*1.0665;
    }

    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //ML::variable1Name = 100;      [Example]

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
void HEPHero::MLSystematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishML() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
