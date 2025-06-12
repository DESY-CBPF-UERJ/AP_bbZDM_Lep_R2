#include "HEPHero.h"

//---------------------------------------------------------------------------------------------
// Pre-Routines Setup
//---------------------------------------------------------------------------------------------
void HEPHero::PreRoutines() {
    
    //=============================================================================================
    // CMS SETUP
    //=============================================================================================

    //----OUTPUT INFO------------------------------------------------------------------------------
    _outputTree->Branch( "evtWeight", &evtWeight );
    HDF_insert( "evtWeight", &evtWeight );

    size_t pos_HT = _datasetName.find("_HT-");
    size_t pos_Pt = _datasetName.find("_Pt-");
    size_t pos_PtZ = _datasetName.find("_PtZ-");
    size_t pos_NJets = _datasetName.find("_NJets-");
    if( (pos_HT != string::npos) || (pos_Pt != string::npos) || (pos_PtZ != string::npos) || (pos_NJets != string::npos) ){
        _outputTree->Branch( "LHE_HT", &LHE_HT );
        _outputTree->Branch( "LHE_Vpt", &LHE_Vpt );
        //_outputTree->Branch( "LHE_Njets", &LHE_Njets );
        HDF_insert( "LHE_HT", &LHE_HT );
        HDF_insert( "LHE_Vpt", &LHE_Vpt );
        //HDF_insert( "LHE_Njets", &LHE_Njets );
    }


    /*
    TFile* vjets_file = new TFile("Metadata/QCD_NLO_k_factors_VJets/UL/scale_factors.root","READ");

    TH2D* vjets_h2d = (TH2D*) vjets_file->Get("dy_sf_ul");

    float mjj = 500.;
    float Vpt = 230.;

    int mjj_i = -1;
    int Vpt_j = -1;

    float imjj[9] = {200, 400, 600, 900, 1200, 1500, 2000, 2700, 3500};
    for( unsigned int i = 0; i < 8; ++i ) {
        if( (mjj >= imjj[i]) && (mjj < imjj[i+1]) ){
            mjj_i = i+1;
            break;
        }
    }

    float jVpt[10] = {200, 240, 260, 280, 300, 340, 400, 500, 740, 1000};
    for( unsigned int j = 0; j < 9; ++j ) {
        if( (Vpt >= jVpt[j]) && (Vpt < jVpt[j+1]) ){
            Vpt_j = j+1;
            break;
        }
    }

    if( (mjj_i > 0) && (Vpt_j > 0) ){
        float value_s = vjets_h2d->GetBinContent(mjj_i,Vpt_j) ;
        float err_s = vjets_h2d->GetBinError(mjj_i,Vpt_j);
        cout << "value_s " << value_s << " " << endl;
        cout << "err_s " << err_s << " " << endl;
    }
    */

    //----LUMI CERTIFICATE-------------------------------------------------------------------------
    lumi_certificate.ReadFile(certificate_file);

    //----PDF--------------------------------------------------------------------------------------
    PDFtype();

    //----JES--------------------------------------------------------------------------------------
    //if( dataset_group != "Data" ) JES_unc.ReadFile( JES_MC_file );

    //----JER--------------------------------------------------------------------------------------
    //if( apply_jer_corr ){
    //    if( dataset_group != "Data" ) jer_corr.ReadFiles(JER_file, JER_SF_file);
    //}

    //----ELECTRON ID------------------------------------------------------------------------------
    if( apply_electron_wgt ){
        auto electron_set = correction::CorrectionSet::from_file(electron_file.c_str());
        electron_ID_corr = electron_set->at("UL-Electron-ID-SF");
    }

    //----MUON ID----------------------------------------------------------------------------------
    if( apply_muon_wgt ){
        auto muon_set = correction::CorrectionSet::from_file(muon_file.c_str());

        muon_RECO_corr = muon_set->at("NUM_TrackerMuons_DEN_genTracks");

        string MuID_WP;
        if( MUON_ID_WP == 0 ){
            MuID_WP = "NUM_LooseID_DEN_TrackerMuons";
        }else if( MUON_ID_WP == 1 ){
            MuID_WP = "NUM_MediumID_DEN_TrackerMuons";
        }else if( MUON_ID_WP == 2 ){
            MuID_WP = "NUM_TightID_DEN_TrackerMuons";
        }
        muon_ID_corr = muon_set->at(MuID_WP);

        string MuISO_WP;
        if( MUON_ISO_WP == 0 ){
            MuISO_WP = "NUM_LooseRelIso_DEN_LooseID"; // dumb value, not used
        }else if( MUON_ISO_WP == 1 ){
            MuISO_WP = "NUM_LooseRelIso_DEN_LooseID";
        }else if( MUON_ISO_WP == 2 ){
            MuISO_WP = "NUM_LooseRelIso_DEN_MediumID";
        }else if( MUON_ISO_WP == 3 ){
            MuISO_WP = "NUM_TightRelIso_DEN_MediumID";
        }
        muon_ISO_corr = muon_set->at(MuISO_WP);
    }

    //----JET PU ID--------------------------------------------------------------------------------
    if( apply_jet_puid_wgt ){
        auto jet_puid_set = correction::CorrectionSet::from_file(jet_puid_file.c_str());
        jet_PUID_corr = jet_puid_set->at("PUJetID_eff");
    }

    //----JERC-------------------------------------------------------------------------------------
    auto jet_jerc_set = correction::CorrectionSet::from_file(jet_jerc_file.c_str());

    string jer_SF_corr_name;
    string jer_PtRes_corr_name;
    string jes_Unc_name;
    string jes_L1_name;
    string jes_L2_name;
    string jes_L3_name;
    string jes_Res_name;
    string jes_L1L2L3Res_name;

    if( dataset_year == "16" ){
        if( dataset_dti == 0 ){
            jer_SF_corr_name = "Summer20UL16APV_JRV3_MC_ScaleFactor_AK4PFchs";
            jer_PtRes_corr_name = "Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs";
            jes_L1_name = "Summer19UL16APV_V7_MC_L1FastJet_AK4PFchs";
            jes_L2_name = "Summer19UL16APV_V7_MC_L2Relative_AK4PFchs";
            jes_L3_name = "Summer19UL16APV_V7_MC_L3Absolute_AK4PFchs";
            jes_Res_name = "Summer19UL16APV_V7_MC_L2L3Residual_AK4PFchs";
            jes_L1L2L3Res_name = "Summer19UL16APV_V7_MC_L1L2L3Res_AK4PFchs";
            jes_Unc_name = "Summer19UL16APV_V7_MC_Total_AK4PFchs";
            if( _sysName_lateral == "JES" ) jes_Unc_name = "Summer19UL16APV_V7_MC_"+_SysSubSource+"_AK4PFchs";
        }else{
            jer_SF_corr_name = "Summer20UL16_JRV3_MC_ScaleFactor_AK4PFchs";
            jer_PtRes_corr_name = "Summer20UL16_JRV3_MC_PtResolution_AK4PFchs";
            jes_L1_name = "Summer19UL16_V7_MC_L1FastJet_AK4PFchs";
            jes_L2_name = "Summer19UL16_V7_MC_L2Relative_AK4PFchs";
            jes_L3_name = "Summer19UL16_V7_MC_L3Absolute_AK4PFchs";
            jes_Res_name = "Summer19UL16_V7_MC_L2L3Residual_AK4PFchs";
            jes_L1L2L3Res_name = "Summer19UL16_V7_MC_L1L2L3Res_AK4PFchs";
            jes_Unc_name = "Summer19UL16_V7_MC_Total_AK4PFchs";
            if( _sysName_lateral == "JES" ) jes_Unc_name = "Summer19UL16_V7_MC_"+_SysSubSource+"_AK4PFchs";
        }
    }else if( dataset_year == "17" ){
        jer_SF_corr_name = "Summer19UL17_JRV2_MC_ScaleFactor_AK4PFchs";
            jer_PtRes_corr_name = "Summer19UL17_JRV2_MC_PtResolution_AK4PFchs";
            jes_L1_name = "Summer19UL17_V5_MC_L1FastJet_AK4PFchs";
            jes_L2_name = "Summer19UL17_V5_MC_L2Relative_AK4PFchs";
            jes_L3_name = "Summer19UL17_V5_MC_L3Absolute_AK4PFchs";
            jes_Res_name = "Summer19UL17_V5_MC_L2L3Residual_AK4PFchs";
            jes_L1L2L3Res_name = "Summer19UL17_V5_MC_L1L2L3Res_AK4PFchs";
            jes_Unc_name = "Summer19UL17_V5_MC_Total_AK4PFchs";
            if( _sysName_lateral == "JES" ) jes_Unc_name = "Summer19UL17_V5_MC_"+_SysSubSource+"_AK4PFchs";
    }else if( dataset_year == "18" ){
        jer_SF_corr_name = "Summer19UL18_JRV2_MC_ScaleFactor_AK4PFchs";
            jer_PtRes_corr_name = "Summer19UL18_JRV2_MC_PtResolution_AK4PFchs";
            jes_L1_name = "Summer19UL18_V5_MC_L1FastJet_AK4PFchs";
            jes_L2_name = "Summer19UL18_V5_MC_L2Relative_AK4PFchs";
            jes_L3_name = "Summer19UL18_V5_MC_L3Absolute_AK4PFchs";
            jes_Res_name = "Summer19UL18_V5_MC_L2L3Residual_AK4PFchs";
            jes_L1L2L3Res_name = "Summer19UL18_V5_MC_L1L2L3Res_AK4PFchs";
            jes_Unc_name = "Summer19UL18_V5_MC_Total_AK4PFchs";
            if( _sysName_lateral == "JES" ) jes_Unc_name = "Summer19UL18_V5_MC_"+_SysSubSource+"_AK4PFchs";
    }

    if( apply_jer_corr ){
        jet_JER_SF_corr = jet_jerc_set->at(jer_SF_corr_name);
        jet_JER_PtRes_corr = jet_jerc_set->at(jer_PtRes_corr_name);
    }
    jet_JES_Unc = jet_jerc_set->at(jes_Unc_name);
    jet_JES_L1 = jet_jerc_set->at(jes_L1_name);
    jet_JES_L2 = jet_jerc_set->at(jes_L2_name);
    jet_JES_L3 = jet_jerc_set->at(jes_L3_name);
    jet_JES_Res = jet_jerc_set->at(jes_Res_name);
    //cout << "before" << endl;
    //jet_JES_L1L2L3Res = jet_jerc_set->at("compound_corrections").at(jes_L1L2L3Res_name);
    //cout << "after" << endl;

    /*
    shared_ptr<correction::Correction const> jet_JES_cout = jet_jerc_set->at("Summer19UL16APV_V7_MC_Total_AK4PFchs");
    double total_jes = jet_JES_cout->evaluate({1.3, 30.});
    cout << "Total " << total_jes << endl;
    */


    //----B TAGGING--------------------------------------------------------------------------------
    if( apply_btag_wgt ){

        string dsName = _datasetName.substr(0,_datasetName.length()-5);

        string dsNameDY = dsName.substr(0,10);
        if( dsNameDY == "DYJetsToLL" ) dsName = "DYJetsToLL";

        //string dsNameTop = dsName.substr(0,3);
        //if( dsNameTop == "TTT" or dsNameTop == "ST_" ) dsName = "Top";

        //string dsNameVV = dsName.substr(0,6);
        //if( dsNameVV == "WZTo3L" or dsNameVV == "ZZTo2L" or dsNameVV == "ZZTo4L" ) dsName = "VV";

        btag_eff.readFile(btag_eff_file);
        if( dataset_group != "Data" ) btag_eff.calib(dsName, "TTTo2L2Nu");

        // Choose btag algo
        // https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation106XUL18
        std::string btagAlgorithmMujets;
        std::string btagAlgorithmIncl;
        std::string btagAlgorithmComb;

        if (JET_BTAG_WP >= 0 and JET_BTAG_WP <= 2) {
            btagAlgorithmMujets = "deepJet_mujets";
            btagAlgorithmIncl = "deepJet_incl";
            btagAlgorithmComb = "deepJet_comb";
        }
        else if (JET_BTAG_WP >= 3 and JET_BTAG_WP <= 5) {
            btagAlgorithmMujets = "deepCSV_mujets";
            btagAlgorithmIncl = "deepCSV_incl";
            btagAlgorithmComb = "deepCSV_comb";
        }

        auto btag_set = correction::CorrectionSet::from_file(btag_SF_file.c_str());
        btag_bc_corr = btag_set->at(btagAlgorithmMujets.c_str());
        btag_udsg_corr = btag_set->at(btagAlgorithmIncl.c_str());
    }

    //----TRIGGER----------------------------------------------------------------------------------
    if( apply_trigger_wgt ){
        triggerSF.readFile(trigger_SF_file);
    }

    //----PILEUP-----------------------------------------------------------------------------------
    if( apply_pileup_wgt ){
        auto pileup_set = correction::CorrectionSet::from_file(pileup_file.c_str());
        string SetName = "Collisions" + dataset_year +"_UltraLegacy_goldenJSON";
        pileup_corr = pileup_set->at(SetName.c_str());
    }

    //----MUON ROCHESTER---------------------------------------------------------------------------
    if( apply_muon_roc_corr ){
        muon_roc_corr.Initialize(muon_roc_file);
    }

    //----MET RECOIL-------------------------------------------------------------------------------
    random_recoil_18.SetSeed(1);
    //if( apply_met_recoil_corr ){
    //    Z_recoil.ReadFile(Z_recoil_file.c_str());
    //}


    //=============================================================================================
    // ANALYSIS SETUP
    //=============================================================================================

    //----MACHINE LEARNING-------------------------------------------------------------------------
    MLP_keras.readFiles( model_keras_file, preprocessing_keras_file );
    MLP_torch.readFile( model_torch_file );
    MLP_onnx.readFile( model_onnx_file );


}


//---------------------------------------------------------------------------------------------
// On fly Routines Setup [run before genWeight count]
//---------------------------------------------------------------------------------------------
bool HEPHero::RunRoutines() {
    

    //======SUM THE GENERATOR WEIGHTS================================================
    SumGenWeights += genWeight;

    if( !MC_processing() ) return false;

    
    return true;
}


//---------------------------------------------------------------------------------------------
// MCsamples processing
//---------------------------------------------------------------------------------------------
bool HEPHero::MC_processing(){

    bool pass_cut = true;
    string dsName = _datasetName.substr(0,_datasetName.length()-5);

    /*
    //================================================================================================================================
    if( (dsName == "DYJetsToLL_Pt-0To65") || (dsName == "DYJetsToLL_Pt-50To100") || (dsName == "DYJetsToLL_Pt-100To250") ){
        genPt = 0;
        float pt_cut = 65;
        float boson_pt = -1;
        for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
            if( ((abs(GenPart_pdgId[ipart]) == 11) || (abs(GenPart_pdgId[ipart]) == 13) || (abs(GenPart_pdgId[ipart]) == 15)) &&
                ((GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 22) || (GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 23)) ){
                int boson_idx = GenPart_genPartIdxMother[ipart];
                boson_pt = GenPart_pt[boson_idx];
                break;
            }else if( ((abs(GenPart_pdgId[ipart]) == 11) || (abs(GenPart_pdgId[ipart]) == 13) || (abs(GenPart_pdgId[ipart]) == 15)) &&
                ((GenPart_genPartIdxMother[ipart] == 0) || (GenPart_genPartIdxMother[ipart] == 1)) ){
                TLorentzVector lep1;
                TLorentzVector lep2;
                lep1.SetPtEtaPhiM(GenPart_pt[ipart], GenPart_eta[ipart], GenPart_phi[ipart], GenPart_mass[ipart]);
                lep2.SetPtEtaPhiM(GenPart_pt[ipart+1], GenPart_eta[ipart+1], GenPart_phi[ipart+1], GenPart_mass[ipart+1]);
                boson_pt = (lep1+lep2).Pt();
                break;
            }
        }
        if( (dsName == "DYJetsToLL_Pt-0To65") && (boson_pt > pt_cut) ) pass_cut = false;
        if( (dsName == "DYJetsToLL_Pt-50To100") && (boson_pt <= pt_cut) ) pass_cut = false;
        if( (dsName == "DYJetsToLL_Pt-100To250") && (boson_pt <= pt_cut) ) pass_cut = false;
        genPt = boson_pt;
    }

    //================================================================================================================================
    string dsName13 = dsName.substr(0,13);
    if( dsName13 == "DYJetsToLL_HT" ){
        genHT = 0.;
        for( size_t ipart = 0; ipart < nGenPart; ++ipart ) {
            if( GenPart_status[ipart] != 23 ) continue;
            if( (GenPart_pdgId[ipart] != 21) && (abs(GenPart_pdgId[ipart]) > 6) ) continue;
            genHT += GenPart_pt[ipart];
        }
    }
    */
    //================================================================================================================================
    if( dsName == "DYJetsToLL_Pt-0To3" ){

        if( LHE_Vpt >= 3 ) pass_cut = false;

    }

    if( dsName == "DYJetsToLL_PtZ-3To50" ){

        if( LHE_Vpt < 3 ) pass_cut = false;

    }

    //================================================================================================================================
    if( dsName == "ZZ_Others" ){

        vector<int> daughters;
        for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
            if( (abs(GenPart_pdgId[ipart]) >= 11) && (abs(GenPart_pdgId[ipart]) <= 16) && (GenPart_pdgId[GenPart_genPartIdxMother[ipart]] == 23) ){
                daughters.push_back(ipart);
            }
            if( daughters.size() == 4 ) break;
        }

        if( daughters.size() == 4 ){
            int Z1_decay;
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

            int Z2_decay;
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
                pass_cut = false;
            }else if( ((Z1_decay >= 1) && (Z1_decay <= 3) && (Z2_decay >= 4) && (Z2_decay <= 6)) || ((Z1_decay >= 4) && (Z1_decay <= 6) && (Z2_decay >= 1) && (Z2_decay <= 3)) ){
                pass_cut = false;
            }
        }
    }

    //================================================================================================================================
    if( dsName == "WZ_Others" ){

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
            if( ((abs(GenPart_pdgId[W_daughters[0]]) == 11) && (abs(GenPart_pdgId[W_daughters[1]]) == 11)) || ((abs(GenPart_pdgId[W_daughters[0]]) == 13) && (abs(GenPart_pdgId[W_daughters[1]]) == 13)) ) W_daughters.erase(W_daughters.begin(),W_daughters.begin()+2);
        }

        if( (Z_daughters.size() == 2) && (W_daughters.size() == 2) ){
            int Z_decay = 0;
            if( (abs(GenPart_pdgId[Z_daughters[0]]) == 11) && (abs(GenPart_pdgId[Z_daughters[1]]) == 11) ){
                Z_decay = 1;
            }else if( (abs(GenPart_pdgId[Z_daughters[0]]) == 13) && (abs(GenPart_pdgId[Z_daughters[1]]) == 13) ){
                Z_decay = 2;
            }else if( (abs(GenPart_pdgId[Z_daughters[0]]) == 15) && (abs(GenPart_pdgId[Z_daughters[1]]) == 15) ){
                Z_decay = 3;
            }

            int W_decay = 0;
            if( ((abs(GenPart_pdgId[W_daughters[0]]) == 11) && (abs(GenPart_pdgId[W_daughters[1]]) == 12)) || ((abs(GenPart_pdgId[W_daughters[0]]) == 12) && (abs(GenPart_pdgId[W_daughters[1]]) == 11)) ){
                W_decay = 1;
            }else if( ((abs(GenPart_pdgId[W_daughters[0]]) == 13) && (abs(GenPart_pdgId[W_daughters[1]]) == 14)) || ((abs(GenPart_pdgId[W_daughters[0]]) == 14) && (abs(GenPart_pdgId[W_daughters[1]]) == 13)) ){
                W_decay = 2;
            }else if( ((abs(GenPart_pdgId[W_daughters[0]]) == 15) && (abs(GenPart_pdgId[W_daughters[1]]) == 16)) || ((abs(GenPart_pdgId[W_daughters[0]]) == 16) && (abs(GenPart_pdgId[W_daughters[1]]) == 15)) ){
                W_decay = 3;
            }

            if( (Z_decay >= 1) && (Z_decay <= 3) && (W_decay >= 1) && (W_decay <= 3) ){
                pass_cut = false;
            }
        }
    }


    return pass_cut;
}

    
