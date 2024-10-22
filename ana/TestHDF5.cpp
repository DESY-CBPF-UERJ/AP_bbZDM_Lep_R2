#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace TestHDF5{

    vector<int> Electrons_id;
    vector<float> Electrons_pt;
    vector<double> Electrons_eta;
    vector<int> Electrons_isPFcand;
    vector<int> SV_ntracks;
    vector<int> Muons_id;
    vector<float> Empty_vec;
    float missed_var;
    unsigned char test_char;
    unsigned int test_int;
    
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupTestHDF5() {

    //======SETUP CUTFLOW==========================================================================
    //_cutFlow.insert(pair<string,double>("CutName", 0) );   [example]

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Define regions, same as in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogramSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]

    //======SETUP BRANCHES IN OUTPUT ROOT FILE=====================================================
    _outputTree->Branch( "LepLep_pt", &LepLep_pt );
    _outputTree->Branch( "Njets", &Njets );
    _outputTree->Branch( "MET_pt", &MET_pt );
    _outputTree->Branch( "PV_npvs", &PV_npvs );
    _outputTree->Branch( "Electrons_id", &TestHDF5::Electrons_id );
    _outputTree->Branch( "Electrons_pt", &TestHDF5::Electrons_pt );
    _outputTree->Branch( "Electrons_eta", &TestHDF5::Electrons_eta );
    _outputTree->Branch( "Muons_id", &TestHDF5::Muons_id );
    
    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert( "LepLep_pt", &LepLep_pt );
    HDF_insert( "Njets", &Njets );
    HDF_insert( "MET_pt", &MET_pt );
    HDF_insert( "PV_npvs", &PV_npvs );
    HDF_insert( "Electrons_id", &TestHDF5::Electrons_id );
    HDF_insert( "Electrons_pt", &TestHDF5::Electrons_pt );
    HDF_insert( "Electrons_eta", &TestHDF5::Electrons_eta );
    HDF_insert( "Electrons_isPFcand", &TestHDF5::Electrons_isPFcand );
    HDF_insert( "SV_ntracks", &TestHDF5::SV_ntracks );
    HDF_insert( "Muons_id", &TestHDF5::Muons_id );
    HDF_insert( "Empty_vec", &TestHDF5::Empty_vec );
    HDF_insert( "missed_var", &TestHDF5::missed_var );
    HDF_insert( "nElectron", &nElectron );
    //HDF_insert( "HLT_IsoMu27", &HLT_IsoMu27 );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::TestHDF5Region() {

    LeptonSelection();
    JetSelection();
    METCorrection();
    Get_Leptonic_Info(true, true);
    Get_LepLep_Variables(true, true);
    
    //-------------------------------------------------------------------------
    // Cut description
    //-------------------------------------------------------------------------
    //if( !(CutCondition) ) return false;           [Example]
    //_cutFlow.at("CutName") += evtWeight;          [Example]

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::TestHDF5Selection() {

    TestHDF5::Electrons_pt.clear();
    TestHDF5::Electrons_eta.clear();
    TestHDF5::Electrons_id.clear();
    TestHDF5::Electrons_isPFcand.clear();
    TestHDF5::SV_ntracks.clear();
    TestHDF5::Muons_id.clear();
    TestHDF5::Empty_vec.clear();

    for( unsigned int iele = 0; iele < nElectron; ++iele ) {
        TestHDF5::Electrons_pt.push_back(Electron_pt[iele]);
        TestHDF5::Electrons_eta.push_back(Electron_eta[iele]);
        TestHDF5::Electrons_id.push_back(Electron_pdgId[iele]);
        TestHDF5::Electrons_isPFcand.push_back(Electron_isPFcand[iele]);
    }
    
    for( unsigned int isv = 0; isv < nSV; ++isv ) {
        TestHDF5::SV_ntracks.push_back(SV_ntracks[isv]);
        //cout << SV_ntracks[isv] << endl;
    }
    
    for( unsigned int imu = 0; imu < nMuon; ++imu ) {
        TestHDF5::Muons_id.push_back(Muon_pdgId[imu]);
    }
    

    //H5Easy::dump(*HDF_file, "electronPt", TestHDF5::electronPt);



    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //TestHDF5::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    _outputTree->Fill();
    
    //======FILL THE OUTPUT HDF5 INFO===============================================================
    HDF_fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::TestHDF5Systematic() {

    //FillSystematic( "histogramSysName", var, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishTestHDF5() {
    
    // let's create a dataset of native integer with the size of the vector 'data'
    //HighFive::DataSet dataset1 = HDF_file->createDataSet<float>("electronPt",  HighFive::DataSpace::From(TestHDF5::electronPt_array));
    //HighFive::DataSet dataset2 = HDF_file->createDataSet<vector<float>>("vectors/ele_pt",  HighFive::DataSpace::From(TestHDF5::ele_pt_array));

    // let's write our vector of int to the HDF5 dataset
    //dataset1.write(TestHDF5::electronPt_array);
    //dataset2.write(TestHDF5::ele_pt_array);
    
    /*
    HDF_file->createDataSet("electronPt", TestHDF5::electronPt_array);
    HDF_file->createDataSet("v2/electronPt", TestHDF5::electronPt_array);
    HDF_file->createDataSet("my_array1", TestHDF5::my_array1);
    //HDF_file->createDataSet("vectors/ele_pt", TestHDF5::ele_pt_array);
    
    // Define the size of our dataset: 2x6
    long unsigned int n = 2;
    long unsigned int m = 6;
    std::vector<size_t> dims{n, m};

    // Create the dataset
    HighFive::DataSet dataset = HDF_file->createDataSet<double>("vectors/ele_pt", HighFive::DataSpace(dims));
    */
    //double data[2][6] = {{1.1, 2.2, 3.3, 4.4, 5.5, 6.6}, {11.11, 12.12, 13.13, 14.14, 15.15, 16.16}};

    // write it
    //dataset.write(data);
    
    

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
