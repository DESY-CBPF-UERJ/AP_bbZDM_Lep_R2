#include "HEPHero.h"

//-------------------------------------------------------------------------------------------------
// Description:
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define output variables
//-------------------------------------------------------------------------------------------------
namespace PUIDEffMap{

    int Jet_jetId;
    int Jet_truePU;
    float Jet_pt;
    float Jet_eta;
    int Jet_puId;
}


//-------------------------------------------------------------------------------------------------
// Define output derivatives
//-------------------------------------------------------------------------------------------------
void HEPHero::SetupPUIDEffMap() {

    //======SETUP CUTFLOW==========================================================================
    _cutFlow.insert(pair<string,double>("00_TwoLepOS", 0) );
    _cutFlow.insert(pair<string,double>("01_MET", 0) );
    _cutFlow.insert(pair<string,double>("02_LeadingLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("03_LepLep_DM", 0) );
    _cutFlow.insert(pair<string,double>("04_LepLep_Pt", 0) );
    _cutFlow.insert(pair<string,double>("05_LepLep_DR", 0) );
    _cutFlow.insert(pair<string,double>("06_MET_LepLep_DPhi", 0) );
    _cutFlow.insert(pair<string,double>("07_MET_LepLep_Mt", 0) );
    _cutFlow.insert(pair<string,double>("08_Selected", 0) );
    _cutFlow.insert(pair<string,double>("09_Corrected", 0) );

    //======SETUP HISTOGRAMS=======================================================================
    //makeHist( "histogram1DName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeHist( "histogram2DName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP SYSTEMATIC HISTOGRAMS============================================================
    //sys_regions = { 0, 1, 2 }; [example] // Choose regions as defined in RegionID. Empty vector means that all events will be used.
    //makeSysHist( "histogram1DSysName", 40, 0., 40., "xlabel", "ylabel" );   [example]
    //makeSysHist( "histogram2DSysName", 40, 0., 40., 100, 0., 50., "xlabel",  "ylabel", "zlabel", "COLZ" );   [example]

    //======SETUP OUTPUT BRANCHES==================================================================
    //_outputTree->Branch("variable1NameInTheTree", &PUIDEffMap::variable1Name );  [example]

    //======SETUP INFORMATION IN OUTPUT HDF5 FILE==================================================
    HDF_insert("EventPosition", &_EventPosition );
    HDF_insert("Jet_jetId", &PUIDEffMap::Jet_jetId);
    HDF_insert("Jet_truePU", &PUIDEffMap::Jet_truePU);
    HDF_insert("Jet_pt", &PUIDEffMap::Jet_pt);
    HDF_insert("Jet_eta", &PUIDEffMap::Jet_eta);
    HDF_insert("Jet_puId", &PUIDEffMap::Jet_puId);

    return;
}


//-------------------------------------------------------------------------------------------------
// Define the selection region
//-------------------------------------------------------------------------------------------------
bool HEPHero::PUIDEffMapRegion() {

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
    
    //Get_ttbar_Variables();
    
    //Regions();
    //if( !((RegionID >= 0) && (RegionID <= 4)) ) return false;                   // 0=SR, 1=DY-CR, 2=ttbar-CR, 3=WZ-CR, 4=ZZ-CR
    //if( !(RegionID == 1) ) return false;            
     
    //bool GoodEvent = lumi_certificate.GoodLumiSection( _datasetName, run, luminosityBlock );
    //if( !GoodEvent ) return false;                                              // Select only certified data events
    
    if( !METFilters() ) return false;                                           // Selected by MET filters
    
    if( !Trigger() ) return false;                                              // Selected by triggers
    _cutFlow.at("08_Selected") += evtWeight;
    
    Weight_corrections();
    _cutFlow.at("09_Corrected") += evtWeight;

    return true;
}


//-------------------------------------------------------------------------------------------------
// Write your analysis code here
//-------------------------------------------------------------------------------------------------
void HEPHero::PUIDEffMapSelection() {



    for (unsigned int ijet = 0; ijet < nJet; ++ijet) {
        if (
            Jet_pt[ijet] > JET_PT_CUT
            && Jet_jetId[ijet] >= JET_ID_WP
            && abs(Jet_eta[ijet]) < JET_ETA_CUT
        ) {
            PUIDEffMap::Jet_jetId = Jet_jetId[ijet];
            PUIDEffMap::Jet_truePU = 1;
            if( Jet_GenJet_match(ijet, 0.4) ) PUIDEffMap::Jet_truePU = 0;
            PUIDEffMap::Jet_pt = Jet_pt[ijet];
            PUIDEffMap::Jet_eta = Jet_eta[ijet];
            PUIDEffMap::Jet_puId = Jet_puId[ijet]; 
            HDF_fill();
        }
    }









    //======ASSIGN VALUES TO THE OUTPUT VARIABLES==================================================
    //PUIDEffMap::variable1Name = 100;      [Example]

    //======FILL THE HISTOGRAMS====================================================================
    //_histograms1D.at("histogram1DName").Fill( var, evtWeight );               [Example]
    //_histograms2D.at("histogram2DName").Fill( var1, var2, evtWeight );        [Example]

    //======FILL THE OUTPUT TREE===================================================================
    //_outputTree->Fill();

    //======FILL THE OUTPUT HDF5 INFO===============================================================
    //HDF_fill();

    return;
}


//-------------------------------------------------------------------------------------------------
// Produce systematic histograms
//-------------------------------------------------------------------------------------------------
void HEPHero::PUIDEffMapSystematic() {

    //FillSystematic( "histogram1DSysName", var, evtWeight );  [Example]
    //FillSystematic( "histogram2DSysName", var1, var2, evtWeight );  [Example]
}


//-------------------------------------------------------------------------------------------------
// Make efficiency plots
//-------------------------------------------------------------------------------------------------
void HEPHero::FinishPUIDEffMap() {

    //MakeEfficiencyPlot( _histograms1D.at("Matched_pt"), _histograms1D.at("all_pt"), "Match_pt" );   [example]

    return;
}
