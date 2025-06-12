#include "HEPHero.h"


//---------------------------------------------------------------------------------------------
// Get size of vertical systematic weights
// Keep the same order used in runSelection.py
//---------------------------------------------------------------------------------------------
void HEPHero::VerticalSysSizes( ){
    if( (_sysID_lateral == 0) && (dataset_group != "Data") ) {
        sys_vertical_size.clear();
        _inputTree->GetEntry(0);

        get_PDF_sfs = false;
        get_Scale_sfs = false;
        get_ISR_sfs = false;
        get_FSR_sfs = false;
        get_TopPt_sfs = false;
        get_WPt_sfs = false;
        get_Pileup_sfs = false;
        get_ElectronID_sfs = false;
        get_MuonID_sfs = false;
        get_JetPUID_sfs = false;
        get_BTag_sfs = false;
        get_Trig_sfs = false;
        get_PreFir_sfs = false;
        get_TT1LXS_sfs = false;
        get_TT2LXS_sfs = false;
        get_DYXS_sfs = false;

        for( int ivert = 0; ivert < _sysNames_vertical.size(); ++ivert ){
            string sysName = _sysNames_vertical.at(ivert);
            if( sysName == "PDF" ){
                sys_vertical_size.push_back(nLHEPdfWeight+1);
                get_PDF_sfs = true;
            }else if( sysName == "Scales" ){
                sys_vertical_size.push_back(7);
                get_Scale_sfs = true;
            }else if( sysName == "ISR" ){
                sys_vertical_size.push_back(2);
                get_ISR_sfs = true;
            }else if( sysName == "FSR" ){
                sys_vertical_size.push_back(2);
                get_FSR_sfs = true;
            }else if( sysName == "TopPt" ){
                sys_vertical_size.push_back(1);
                get_TopPt_sfs = true;
            }else if( sysName == "WPt" ){
                sys_vertical_size.push_back(1);
                get_WPt_sfs = true;
            }else if( sysName == "Pileup" ){
                sys_vertical_size.push_back(2);
                get_Pileup_sfs = true;
            }else if( sysName == "EleID" ){
                sys_vertical_size.push_back(2);
                get_ElectronID_sfs = true;
            }else if( sysName == "MuID" ){
                sys_vertical_size.push_back(2);
                get_MuonID_sfs = true;
            }else if( sysName == "Trig" ){
                sys_vertical_size.push_back(2);
                get_Trig_sfs = true;
            }else if( sysName == "PreFir" ){
                sys_vertical_size.push_back(2);
                get_PreFir_sfs = true;
            }else if( sysName == "JetPU" ){
                sys_vertical_size.push_back(2);
                get_JetPUID_sfs = true;
            }else if( sysName == "BTag" ){
                sys_vertical_size.push_back(8);
                get_BTag_sfs = true;
            }else if( sysName == "TT1LXS" ){
                sys_vertical_size.push_back(2);
                get_TT1LXS_sfs = true;
            }else if( sysName == "TT2LXS" ){
                sys_vertical_size.push_back(2);
                get_TT2LXS_sfs = true;
            }else if( sysName == "DYXS" ){
                sys_vertical_size.push_back(2);
                get_DYXS_sfs = true;
            }
        }
    }
}


//---------------------------------------------------------------------------------------------
// Vertical systematics
// Keep the same order used in runSelection.py
//---------------------------------------------------------------------------------------------
void HEPHero::VerticalSys(){
    if( (_sysID_lateral == 0) && (dataset_group != "Data") ) {
        sys_vertical_sfs.clear();

        //-----------------------------------------------------------------------------------
        // https://arxiv.org/pdf/1510.03865.pdf
        // https://cms-pdmv.gitbook.io/project/mccontact/info-for-mc-production-for-ultra-legacy-campaigns-2016-2017-2018
        if( get_PDF_sfs ){
            vector<float> PDF_sfs;

            if( nLHEPdfWeight > 0 ){

                for( int iuniv = 0; iuniv < nLHEPdfWeight; ++iuniv ) {
                    PDF_sfs.push_back(LHEPdfWeight[iuniv]);
                }

                if( PDF_TYPE == "mc" ) PDF_sfs.push_back(2);
                if( PDF_TYPE == "hessian" ) PDF_sfs.push_back(1);

                //cout << "PDF size " << PDF_sfs.size() << endl;

            }else{
                PDF_sfs.push_back(3);
                cout << "No PDF weights!" << endl;
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("PDF", PDF_sfs));
        }

        //-----------------------------------------------------------------------------------
        /*
        if( get_AlphaS_sfs ){
            vector<float> AlphaS_sfs;

            if( nLHEPdfWeight == 33 || nLHEPdfWeight == 103 ){
                float unc = (LHEPdfWeight[nLHEPdfWeight-1] - LHEPdfWeight[nLHEPdfWeight-2])/2;

                AlphaS_sfs.push_back(1-unc);          // DOWN
                AlphaS_sfs.push_back(1+unc);          // UP
            }else{
                AlphaS_sfs.push_back(1);              // DOWN
                AlphaS_sfs.push_back(1);              // UP
                //cout << "No alpha_s weights!" << endl;
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("AlphaS", AlphaS_sfs));
        }
        */
        //-----------------------------------------------------------------------------------
        if( get_Scale_sfs ){
            vector<float> Scale_sfs;
            if( nLHEScaleWeight == 9 ){
                for( int iuniv = 0; iuniv < 9; ++iuniv ) {
                    if( (iuniv != 2) && (iuniv != 6) ) Scale_sfs.push_back(LHEScaleWeight[iuniv]);
                }
            }else{
                for( int iuniv = 0; iuniv < 7; ++iuniv ) {
                    Scale_sfs.push_back(1);
                }
                //cout << "No renormalization and factorization scale weights!" << endl;
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("Scales", Scale_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_ISR_sfs ){
            vector<float> ISR_sfs;
            if( nPSWeight == 4 ){
                ISR_sfs.push_back(PSWeight[2]);    // DOWN
                ISR_sfs.push_back(PSWeight[0]);    // UP
            }else{
                ISR_sfs.push_back(1);              // DOWN
                ISR_sfs.push_back(1);              // UP
                //cout << "No PS weights for ISR!" << endl;
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("ISR", ISR_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_FSR_sfs ){
            vector<float> FSR_sfs;
            if( nPSWeight == 4 ){
                FSR_sfs.push_back(PSWeight[3]);    // DOWN
                FSR_sfs.push_back(PSWeight[1]);    // UP
            }else{
                FSR_sfs.push_back(1);              // DOWN
                FSR_sfs.push_back(1);              // UP
                //cout << "No PS weights for FSR!" << endl;
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("FSR", FSR_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_TopPt_sfs ){
            vector<float> TopPt_sfs;
            TopPt_sfs.push_back(1./top_pt_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("TopPt", TopPt_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_WPt_sfs ){
            vector<float> WPt_sfs;
            WPt_sfs.push_back(1./w_pt_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("WPt", WPt_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_Pileup_sfs ){
            vector<float> Pileup_sfs;
            double pileup_wgt_down = GetPileupWeight(Pileup_nTrueInt, "down");
            double pileup_wgt_up = GetPileupWeight(Pileup_nTrueInt, "up");
            Pileup_sfs.push_back(pileup_wgt_down/pileup_wgt);
            Pileup_sfs.push_back(pileup_wgt_up/pileup_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("Pileup", Pileup_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_ElectronID_sfs ){
            vector<float> ElectronID_sfs;
            double electron_wgt_down = GetElectronWeight("down");
            double electron_wgt_up = GetElectronWeight("up");
            ElectronID_sfs.push_back(electron_wgt_down/electron_wgt);
            ElectronID_sfs.push_back(electron_wgt_up/electron_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("EleID", ElectronID_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_MuonID_sfs ){
            vector<float> MuonID_sfs;
            double muon_wgt_down = GetMuonWeight("down");
            double muon_wgt_up = GetMuonWeight("up");
            MuonID_sfs.push_back(muon_wgt_down/muon_wgt);
            MuonID_sfs.push_back(muon_wgt_up/muon_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("MuID", MuonID_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_JetPUID_sfs ){
            vector<float> JetPUID_sfs;
            double jet_puid_wgt_down = GetJetPUIDWeight("down");
            double jet_puid_wgt_up = GetJetPUIDWeight("up");
            JetPUID_sfs.push_back(jet_puid_wgt_down/jet_puid_wgt);
            JetPUID_sfs.push_back(jet_puid_wgt_up/jet_puid_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("JetPU", JetPUID_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_BTag_sfs ){
            vector<float> BTag_sfs;

            double btag_bc_wgt_down = GetBTagWeight("down", "bc", "uncorrelated");
            double btag_bc_wgt_up = GetBTagWeight("up", "bc", "uncorrelated");
            BTag_sfs.push_back(btag_bc_wgt_down/btag_wgt);
            BTag_sfs.push_back(btag_bc_wgt_up/btag_wgt);

            double btag_light_wgt_down = GetBTagWeight("down", "light", "uncorrelated");
            double btag_light_wgt_up = GetBTagWeight("up", "light", "uncorrelated");
            BTag_sfs.push_back(btag_light_wgt_down/btag_wgt);
            BTag_sfs.push_back(btag_light_wgt_up/btag_wgt);

            double btag_bc_corr_wgt_down = GetBTagWeight("down", "bc", "correlated");
            double btag_bc_corr_wgt_up = GetBTagWeight("up", "bc", "correlated");
            BTag_sfs.push_back(btag_bc_corr_wgt_down/btag_wgt);
            BTag_sfs.push_back(btag_bc_corr_wgt_up/btag_wgt);

            double btag_light_corr_wgt_down = GetBTagWeight("down", "light", "correlated");
            double btag_light_corr_wgt_up = GetBTagWeight("up", "light", "correlated");
            BTag_sfs.push_back(btag_light_corr_wgt_down/btag_wgt);
            BTag_sfs.push_back(btag_light_corr_wgt_up/btag_wgt);

            sys_vertical_sfs.insert(pair<string, vector<float>>("BTag", BTag_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_Trig_sfs ){
            vector<float> Trig_sfs;
            double trig_wgt_down = GetTriggerWeight("down");
            double trig_wgt_up = GetTriggerWeight("up");
            Trig_sfs.push_back(trig_wgt_down/trigger_wgt);
            Trig_sfs.push_back(trig_wgt_up/trigger_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("Trig", Trig_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_PreFir_sfs ){
            vector<float> PreFir_sfs;
            double prefir_wgt_down = GetPrefiringWeight("down");
            double prefir_wgt_up = GetPrefiringWeight("up");
            PreFir_sfs.push_back(prefir_wgt_down/prefiring_wgt);
            PreFir_sfs.push_back(prefir_wgt_up/prefiring_wgt);
            sys_vertical_sfs.insert(pair<string, vector<float>>("PreFir", PreFir_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_TT1LXS_sfs ){
            vector<float> XS_sfs;
            if( _datasetName.length() > 6 ){
                if( _datasetName.substr(0,6) == "TTToSe" ){
                    double PROC_XSEC_DOWN = PROC_XSEC - PROC_XSEC_UNC_DOWN;
                    double PROC_XSEC_UP = PROC_XSEC + PROC_XSEC_UNC_UP;
                    XS_sfs.push_back(PROC_XSEC_DOWN/PROC_XSEC);
                    XS_sfs.push_back(PROC_XSEC_UP/PROC_XSEC);
                }else{
                    XS_sfs.push_back(1);
                    XS_sfs.push_back(1);
                }
            }else{
                XS_sfs.push_back(1);
                XS_sfs.push_back(1);
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("TT1LXS", XS_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_TT2LXS_sfs ){
            vector<float> XS_sfs;
            if( _datasetName.length() > 6 ){
                if( _datasetName.substr(0,6) == "TTTo2L" ){
                    double PROC_XSEC_DOWN = PROC_XSEC - PROC_XSEC_UNC_DOWN;
                    double PROC_XSEC_UP = PROC_XSEC + PROC_XSEC_UNC_UP;
                    XS_sfs.push_back(PROC_XSEC_DOWN/PROC_XSEC);
                    XS_sfs.push_back(PROC_XSEC_UP/PROC_XSEC);
                }else{
                    XS_sfs.push_back(1);
                    XS_sfs.push_back(1);
                }
            }else{
                XS_sfs.push_back(1);
                XS_sfs.push_back(1);
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("TT2LXS", XS_sfs));
        }

        //-----------------------------------------------------------------------------------
        if( get_DYXS_sfs ){
            vector<float> XS_sfs;
            if( _datasetName.length() > 6 ){
                if( _datasetName.substr(0,6) == "DYJets" ){
                    double PROC_XSEC_DOWN = PROC_XSEC - PROC_XSEC_UNC_DOWN;
                    double PROC_XSEC_UP = PROC_XSEC + PROC_XSEC_UNC_UP;
                    XS_sfs.push_back(PROC_XSEC_DOWN/PROC_XSEC);
                    XS_sfs.push_back(PROC_XSEC_UP/PROC_XSEC);
                }else{
                    XS_sfs.push_back(1);
                    XS_sfs.push_back(1);
                }
            }else{
                XS_sfs.push_back(1);
                XS_sfs.push_back(1);
            }
            sys_vertical_sfs.insert(pair<string, vector<float>>("DYXS", XS_sfs));
        }
    }
}
