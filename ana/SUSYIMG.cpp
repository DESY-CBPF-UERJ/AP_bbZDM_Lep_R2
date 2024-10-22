#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace SUSYIMG{

    vector<float> Jet_pt_2D;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupSUSYIMG() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_MET", 0) );
    _cutFlow.insert(pair<string,double>("01_MHT", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingJet_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_No_Leptons", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &SUSYIMG::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert("Jet_pt_2D", &SUSYIMG::Jet_pt_2D );

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::SUSYIMGRegion() {

    
    LeptonSelection();
    JetSelection();
    METCorrection();
    
    if( !(MET_pt > 200) ) return false;                                     // MET > CUT
    _cutFlow.at("00_MET") += evtWeight;  
    
    if( !(MHT > 180) ) return false;                                     // MHT > CUT
    _cutFlow.at("01_MHT") += evtWeight;
    
    if( !(LeadingJet_pt > 50) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("02_LeadingJet_Pt") += evtWeight; 
    
    if( !(Nleptons == 0) ) return false;                   // Leading lepton pt > CUT
    _cutFlow.at("03_No_Leptons") += evtWeight;
    
             
     
    bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::SUSYIMGSelection() {

    SUSYIMG::Jet_pt_2D.clear();
    for( int iabseta = 0; iabseta < 12; ++iabseta ){
        for( int idphi = 0; idphi < 16; ++idphi ){
            SUSYIMG::Jet_pt_2D.push_back( 0. );
        }
    }
    
    for( int iseljet = 0; iseljet < selectedJet.size(); ++iseljet ) {
        int ijet = selectedJet[iseljet];
        
        float jet_abseta = fabs(Jet_eta[ijet]);
        int iabseta = int(jet_abseta/(2.4/12));
        
        float jet_met_dphi = fabs(MET_phi - Jet_phi[ijet]);
        if( jet_met_dphi > M_PI ) jet_met_dphi = 2*M_PI - jet_met_dphi;
        int idphi = int(jet_met_dphi/(3.2/16));
        
        SUSYIMG::Jet_pt_2D[(iabseta*12)+idphi] += Jet_pt[ijet];
    }
    
    
    
    

    /*
    if args.check == 0:
        datasets = [["Signal_test_16",                  '1600000', "../splitSUSY_M1400_1300_ctau0p1.root", 0],]
    if args.check == 1:
        datasets = [["TTToSemiLeptonic_test_17",        '1700000', "../TTToSemiLeptonic.root",  0],]
    if args.check == 2:
        datasets = [["TTToSemiLeptonic_test_18",        '1800000', "../ZJetsToNuNu.root",  0],]
    */


    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //SUSYIMG::variable1Name = 100;      [Example]

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
void HEPHero::SUSYIMGSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishSUSYIMG() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
