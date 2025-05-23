#include "HEPHero.h"


//-------------------------------------------------------------------------
// Return boolean informing if the reco jet is a pileup jet or not
//-------------------------------------------------------------------------
bool HEPHero::PileupJet( int iJet ){
    bool isPileup = true;
    bool isSignal = false;

    for( unsigned int igenJet = 0; igenJet < nGenJet; ++igenJet ) {
        float DEta = fabs( GenJet_eta[igenJet] - Jet_eta[iJet] );
        float DPhi = fabs( GenJet_phi[igenJet] - Jet_phi[iJet] );
        if( DPhi > M_PI ) DPhi = 2*M_PI - DPhi;
        float DR = sqrt( DEta*DEta + DPhi*DPhi );
        if( DR < 0.4 ) isSignal = true;
    }
    if( isSignal ) isPileup = false;

    return isPileup;
}


//-------------------------------------------------------------------------
// HEM issue in 2018
// https://twiki.cern.ch/twiki/bin/view/CMS/SUSRecommendationsRun2Legacy#HEM_issue_in_2018
//-------------------------------------------------------------------------
void HEPHero::HEMissue(){

    HEM_issue_ele = false;
    HEM_issue_jet = false;
    HEM_issue_ele_v2 = false;
    HEM_issue_jet_v2 = false;
    HEM_issue_met = false;
    HEM_filter = true;

    for( unsigned int iselele = 0; iselele < selectedEle.size(); ++iselele ) {
        int iele = selectedEle.at(iselele);
        float eta = Electron_eta[iele];
        float phi = Electron_phi[iele];
        if( (eta > -3.0) && (eta < -1.3) && (phi > -1.57) && (phi < -0.87) ) HEM_issue_ele_v2 = true;
    }


    for( unsigned int iseljet = 0; iseljet < selectedJet.size(); ++iseljet ) {
        int ijet = selectedJet.at(iseljet);
        float eta = Jet_eta[ijet];
        float phi = Jet_phi[ijet];
        if( (eta > -3.0) && (eta < -1.3) && (phi > -1.57) && (phi < -0.87) ) HEM_issue_jet_v2 = true;
    }




    for( unsigned int iele = 0; iele < nElectron; ++iele ) {
        float eta = Electron_eta[iele];
        float phi = Electron_phi[iele];
        if( (eta > -3.0) && (eta < -1.3) && (phi > -1.57) && (phi < -0.87) ) HEM_issue_ele = true;
    }

    for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
        float eta = Jet_eta[ijet];
        float phi = Jet_phi[ijet];
        float pt = Jet_pt[ijet];
        if( (pt > 20) && (eta > -3.0) && (eta < -1.3) && (phi > -1.57) && (phi < -0.87) ) HEM_issue_jet = true;
    }


    if( (MET_phi > -1.57) && (MET_phi < -0.87) ) HEM_issue_met = true;


    if( (dataset_year == "18") && (HEM_issue_ele || HEM_issue_jet) ) HEM_filter = false;

}


//-------------------------------------------------------------------------
// MET  Filters
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#UL_data
//-------------------------------------------------------------------------
bool HEPHero::METFilters(){

    string dsName = _datasetName.substr(0,_datasetName.length()-5);
    string dsNameSignal = dsName.substr(0,15);


    bool filtered = false;
    bool filters;
    //if( (dataset_year == "18") && (dsNameSignal == "Signal_1000_600") ){
    //    filters =   Flag_goodVertices &&
    //                Flag_globalSuperTightHalo2016Filter &&
    //                Flag_HBHENoiseFilter &&
    //                Flag_HBHENoiseIsoFilter &&
    //                Flag_EcalDeadCellTriggerPrimitiveFilter &&
    //                Flag_BadPFMuonFilter &&
    //                Flag_eeBadScFilter;
    //}else{
    filters =   Flag_goodVertices &&
                Flag_globalSuperTightHalo2016Filter &&
                Flag_HBHENoiseFilter &&
                Flag_HBHENoiseIsoFilter &&
                Flag_EcalDeadCellTriggerPrimitiveFilter &&
                Flag_BadPFMuonFilter &&
                Flag_BadPFMuonDzFilter &&
                Flag_eeBadScFilter;


    if( dataset_year == "16" ){
        if( filters ) filtered = true;
    }else if( (dataset_year == "17") || (dataset_year == "18") ){
        if(filters && Flag_ecalBadCalibFilter) filtered = true;
    }else{
        std::cout << "Something is Wrong !!!" << std::endl;
        std::cout << "The dataset name does not have the year [16,17,18] or dataset is from another year" << std::endl;
    }

    return filtered ;
}


//---------------------------------------------------------------------------------------------------------------
// Electron ID Correction
// Return weight associated to the identification of the two electrons
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetElectronWeight( string sysID ){

    float LeptonID_wgt = 1.;
    if( dataset_group != "Data" ){

        string year;
        if( dataset_year == "16" ){
            if( dataset_dti == 0 ){
                year = "2016preVFP";
            }else{
                year = "2016postVFP";
            }
        }else if( dataset_year == "17" ){
            year = "2017";
        }else if( dataset_year == "18" ){
            year = "2018";
        }

        string EleID_WP;
        if( ELECTRON_ID_WP == 0 ){
            EleID_WP = "Veto";
        }else if( ELECTRON_ID_WP == 1 ){
            EleID_WP = "Loose";
        }else if( ELECTRON_ID_WP == 2 ){
            EleID_WP = "Medium";
        }else if( ELECTRON_ID_WP == 3 ){
            EleID_WP = "Tight";
        }else if( ELECTRON_ID_WP == 4 ){
            EleID_WP = "wp90iso";
        }else if( ELECTRON_ID_WP == 5 ){
            EleID_WP = "wp80iso";
        }

        for( unsigned int iele = 0; iele < nElectron; ++iele ) {
            if( Electron_pt[iele] <= 10 ) continue;
            //if( Electron_pt[iele] >= 500 ) continue;

            float ele_pt = Electron_pt[iele];
            float ele_etaSC = Electron_eta[iele] + Electron_deltaEtaSC[iele];

            float RECO_SF;
            if( sysID == "cv" ){
                if( ele_pt <= 20     ) RECO_SF = electron_ID_corr->evaluate({year, "sf", "RecoBelow20", ele_etaSC, ele_pt});
                else if( ele_pt > 20 ) RECO_SF = electron_ID_corr->evaluate({year, "sf", "RecoAbove20", ele_etaSC, ele_pt});
            }else if( sysID == "down" ){
                if( ele_pt <= 20     ) RECO_SF = electron_ID_corr->evaluate({year, "sfdown", "RecoBelow20", ele_etaSC, ele_pt});
                else if( ele_pt > 20 ) RECO_SF = electron_ID_corr->evaluate({year, "sfdown", "RecoAbove20", ele_etaSC, ele_pt});
            }else if( sysID == "up" ){
                if( ele_pt <= 20     ) RECO_SF = electron_ID_corr->evaluate({year, "sfup", "RecoBelow20", ele_etaSC, ele_pt});
                else if( ele_pt > 20 ) RECO_SF = electron_ID_corr->evaluate({year, "sfup", "RecoAbove20", ele_etaSC, ele_pt});
            }
            LeptonID_wgt *= RECO_SF;

            if( !ElectronID( iele, ELECTRON_ID_WP ) ) continue;

            float ID_SF;
            if( sysID == "cv" ){
                ID_SF = electron_ID_corr->evaluate({year, "sf", EleID_WP, ele_etaSC, ele_pt});
            }else if( sysID == "down" ){
                ID_SF = electron_ID_corr->evaluate({year, "sfdown", EleID_WP, ele_etaSC, ele_pt});
            }else if( sysID == "up" ){
                ID_SF = electron_ID_corr->evaluate({year, "sfup", EleID_WP, ele_etaSC, ele_pt});
            }
            LeptonID_wgt *= ID_SF;
        }
    }

    return LeptonID_wgt;
}


//---------------------------------------------------------------------------------------------------------------
// Muon ID Correction
// Return weight associated to the identification of the muons
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetMuonWeight( string sysID ){

    float LeptonID_wgt = 1.;
    if( dataset_group != "Data" ){

        string year;
        if( dataset_year == "16" ){
            if( dataset_dti == 0 ){
                year = "2016preVFP";
            }else{
                year = "2016postVFP";
            }
        }else if( dataset_year == "17" ){
            year = "2017";
        }else if( dataset_year == "18" ){
            year = "2018";
        }

        for( unsigned int imu = 0; imu < nMuon; ++imu ) {
            if( Muon_pt[imu] <= 15 ) continue;
            if( abs(Muon_eta[imu]) >= 2.4 ) continue;

            float mu_pt = Muon_pt[imu];
            float mu_abseta = abs(Muon_eta[imu]);

            float RECO_SF;
            if( mu_pt < 200 ){
                if( sysID == "cv" ){
                    RECO_SF = muon_RECO_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "sf"});
                }else if( sysID == "down" ){
                    RECO_SF = muon_RECO_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "systdown"});
                }else if( sysID == "up" ){
                    RECO_SF = muon_RECO_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "systup"});
                }
                LeptonID_wgt *= RECO_SF;
            }

            if( !MuonID( imu, MUON_ID_WP ) ) continue;

            float ID_SF;
            if( sysID == "cv" ){
                ID_SF = muon_ID_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "sf"});
            }else if( sysID == "down" ){
                ID_SF = muon_ID_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "systdown"});
            }else if( sysID == "up" ){
                ID_SF = muon_ID_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "systup"});
            }
            LeptonID_wgt *= ID_SF;

            if( !MuonISO( imu, MUON_ISO_WP ) ) continue;

            float ISO_SF;
            if( sysID == "cv" ){
                ISO_SF = muon_ISO_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "sf"});
            }else if( sysID == "down" ){
                ISO_SF = muon_ISO_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "systdown"});
            }else if( sysID == "up" ){
                ISO_SF = muon_ISO_corr->evaluate({year+"_UL", mu_abseta, mu_pt, "systup"});
            }
            if( MUON_ISO_WP > 0 ) LeptonID_wgt *= ISO_SF;
        }
    }

    return LeptonID_wgt;
}


//---------------------------------------------------------------------------------------------------------------
// Jet puID Correction
// Return weight associated to the jet pileup ID selection
// Jet pileup ID identify jets that are NOT pileup
// https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetIDUL#Data_MC_Efficiency_Scale_Factors
// https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetID#Efficiencies_and_data_MC_scale_f
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetJetPUIDWeight( string sysID ){

    double puid_weight = 1.;
    if( dataset_group != "Data" ){

        string WP;
        if( JET_PUID_WP == 1 ){
            WP = "L";
        }else if( JET_PUID_WP == 3 ){
            WP = "M";
        }else if( JET_PUID_WP == 7 ){
            WP = "T";
        }

        //double P_MC = 1;
        //double P_DATA = 1;
        for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
            if( abs(Jet_eta[ijet]) >= 5.0 ) continue;
            if( Jet_pt[ijet] < 20 ) continue;
            if( Jet_pt[ijet] >= 50 ) continue;
            if( Jet_puId[ijet] < JET_PUID_WP ) continue;
            //if( !Jet_GenJet_match(ijet, 0.4) ) continue;
            if( Jet_genJetIdx[ijet] < 0 ) continue;

            float jet_pt = Jet_pt[ijet];
            float jet_eta = Jet_eta[ijet];

            //double eff = jet_PUID_corr->evaluate({jet_eta, jet_pt, "MCEff", WP});

            double SF;
            if( sysID == "cv" ){
                SF = jet_PUID_corr->evaluate({jet_eta, jet_pt, "nom", WP});
            }else if( sysID == "down" ){
                SF = jet_PUID_corr->evaluate({jet_eta, jet_pt, "down", WP});
            }else if( sysID == "up" ){
                SF = jet_PUID_corr->evaluate({jet_eta, jet_pt, "up", WP});
            }
            if( JET_PUID_WP > 0 ) puid_weight *= SF;
            //if( JET_PUID_WP > 0 ){
            //    P_MC *= eff;
            //    P_DATA *= eff*SF;
            //}
        }
        //if( P_MC > 0 ){
        //    puid_weight = P_DATA/P_MC;
        //}else{
        //    puid_weight = 1.;
        //}
    }

    return puid_weight;
}


//---------------------------------------------------------------------------------------------------------------
// BTagging Correction
// Return weight associated to the btagging selection
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetBTagWeight( string sysID, string sysFlavor, string sysType ){

    double btag_weight = 1.;
    if( dataset_group != "Data" ){

        string WP;
        if(      (JET_BTAG_WP == 0) || (JET_BTAG_WP == 3) ) WP = "L";
        else if( (JET_BTAG_WP == 1) || (JET_BTAG_WP == 4) ) WP = "M";
        else if( (JET_BTAG_WP == 2) || (JET_BTAG_WP == 5) ) WP = "T";

        double P_MC = 1;
        double P_DATA = 1;
        //for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
            //if( Jet_pt[ijet] <= 20 ) continue;
            //if( abs(Jet_eta[ijet]) >= 2.4 ) continue;
        for( unsigned int iselJet = 0; iselJet < selectedJet.size(); ++iselJet ) {
            int ijet = selectedJet[iselJet];

            std::string Jet_flavour_str;
            if (Jet_hadronFlavour[ijet] == 5) {         // B
                Jet_flavour_str = "b";
            } else if (Jet_hadronFlavour[ijet] == 4) {  // C
                Jet_flavour_str = "c";
            } else if (Jet_hadronFlavour[ijet] == 0) {  // UDSG
                Jet_flavour_str = "udsg";
            }

            double eff = btag_eff.getEfficiency( Jet_flavour_str, Jet_eta[ijet], Jet_pt[ijet] );

            double SF;

            if( (Jet_flavour_str == "b") || (Jet_flavour_str == "c") ){
                if( (sysID == "cv") || (sysFlavor == "light") ){
                    SF = btag_bc_corr->evaluate({"central", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "down") && (sysType == "uncorrelated") ){
                    SF = btag_bc_corr->evaluate({"down_uncorrelated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "down") && (sysType == "correlated") ){
                    SF = btag_bc_corr->evaluate({"down_correlated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "up") && (sysType == "uncorrelated") ){
                    SF = btag_bc_corr->evaluate({"up_uncorrelated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "up") && (sysType == "correlated") ){
                    SF = btag_bc_corr->evaluate({"up_correlated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }
            }else if( Jet_flavour_str == "udsg" ){
                if( (sysID == "cv") || (sysFlavor == "bc") ){
                    SF = btag_udsg_corr->evaluate({"central", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "down") && (sysType == "uncorrelated") ){
                    SF = btag_udsg_corr->evaluate({"down_uncorrelated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "down") && (sysType == "correlated") ){
                    SF = btag_udsg_corr->evaluate({"down_correlated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "up") && (sysType == "uncorrelated") ){
                    SF = btag_udsg_corr->evaluate({"up_uncorrelated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }else if( (sysID == "up") && (sysType == "correlated") ){
                    SF = btag_udsg_corr->evaluate({"up_correlated", WP, Jet_hadronFlavour[ijet], abs(Jet_eta[ijet]), Jet_pt[ijet]});
                }
            }

            if( JetBTAG( ijet, JET_BTAG_WP ) ){
                P_MC *= eff;
                P_DATA *= eff*SF;
            }else{
                P_MC *= 1-eff;
                P_DATA *= 1-eff*SF;
            }
        }
        if( P_MC > 0 ){
            btag_weight = P_DATA/P_MC;
        }else{
            btag_weight = 1.;
        }
    }

    return btag_weight;
}


//---------------------------------------------------------------------------------------------------------------
// Pileup Correction
// Return weight associated to the pileup effect
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetPileupWeight( float Pileup_nTrueInt, string sysType ){

    double pileup_weight = 1.;

    string dsName = _datasetName.substr(0,_datasetName.length()-5);
    string dsNameDY = dsName.substr(0,10);

    if( (dsNameDY == "DYJetsToLL") && (sysType != "nominal") ){

        float syst_SF;
        if( sysType == "up" ){
            vector<float> SF_APV_16 = {1.027, 1.067};
            vector<float> SF_16 = {1.030, 1.064};
            vector<float> SF_17 = {1.028, 1.066};
            vector<float> SF_18 = {1.032, 1.069};
            if( (dataset_year == "16") && (dataset_dti == 0) ){
                if( NPUjets == 0 ){
                    syst_SF = SF_APV_16[0];
                }else{
                    syst_SF = SF_APV_16[1];
                }
            }else if( (dataset_year == "16") && (dataset_dti == 1) ){
                if( NPUjets == 0 ){
                    syst_SF = SF_16[0];
                }else{
                    syst_SF = SF_16[1];
                }
            }else if( dataset_year == "17" ){
                if( NPUjets == 0 ){
                    syst_SF = SF_17[0];
                }else{
                    syst_SF = SF_17[1];
                }
            }else if( dataset_year == "18" ){
                if( NPUjets == 0 ){
                    syst_SF = SF_18[0];
                }else{
                    syst_SF = SF_18[1];
                }
            }
            pileup_weight = pileup_corr->evaluate({Pileup_nTrueInt, "nominal"})*syst_SF;
        }else if( sysType == "down" ){
            vector<float> SF_APV_16 = {0.971, 0.934};
            vector<float> SF_16 = {0.970, 0.940};
            vector<float> SF_17 = {0.972, 0.934};
            vector<float> SF_18 = {0.968, 0.934};
            if( (dataset_year == "16") && (dataset_dti == 0) ){
                if( NPUjets == 0 ){
                    syst_SF = SF_APV_16[0];
                }else{
                    syst_SF = SF_APV_16[1];
                }
            }else if( (dataset_year == "16") && (dataset_dti == 1) ){
                if( NPUjets == 0 ){
                    syst_SF = SF_16[0];
                }else{
                    syst_SF = SF_16[1];
                }
            }else if( dataset_year == "17" ){
                if( NPUjets == 0 ){
                    syst_SF = SF_17[0];
                }else{
                    syst_SF = SF_17[1];
                }
            }else if( dataset_year == "18" ){
                if( NPUjets == 0 ){
                    syst_SF = SF_18[0];
                }else{
                    syst_SF = SF_18[1];
                }
            }
            pileup_weight = pileup_corr->evaluate({Pileup_nTrueInt, "nominal"})*syst_SF;
        }
    }else{
        pileup_weight = pileup_corr->evaluate({Pileup_nTrueInt, sysType});
    }

    //pileup_weight = pileup_corr->evaluate({Pileup_nTrueInt, sysType});

    return pileup_weight;
}


//---------------------------------------------------------------------------------------------------------------
// Trigger Correction
// Return trigger scale factor
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetTriggerWeight( string sysID ){

    double trigger_weight = 1.;
    if( dataset_group != "Data" and Trigger() ){

        string sample;
        if( (RecoLepID == 11) || ((RecoLepID > 31100) && (RecoLepID <= 31199)) || ((RecoLepID > 41100) && (RecoLepID <= 41199)) ){
            sample = "ElEl";
        }else if( (RecoLepID == 13) || ((RecoLepID > 31300) && (RecoLepID <= 31399)) || ((RecoLepID > 41300) && (RecoLepID <= 41399)) ){
            sample = "MuMu";
        }else if( (RecoLepID > 1000) && (RecoLepID < 1999) ){
            sample = "ElMu";
        }

        trigger_weight = triggerSF.getSF(sample, LeadingLep_pt, TrailingLep_pt);

        double trigger_weight_UncSyst = triggerSF.getSFErrorSys(sample, LeadingLep_pt, TrailingLep_pt);
        if( sysID == "down" ){
            double trigger_weight_UncStatDown = triggerSF.getSFErrorLow(sample, LeadingLep_pt, TrailingLep_pt);
            double trigger_unc = sqrt(pow(trigger_weight_UncSyst,2) + pow(trigger_weight_UncStatDown,2));
            trigger_weight = trigger_weight - trigger_unc;
        }else if( sysID == "up" ){
            double trigger_weight_UncSystUp = triggerSF.getSFErrorUp(sample, LeadingLep_pt, TrailingLep_pt);
            double trigger_unc = sqrt(pow(trigger_weight_UncSyst,2) + pow(trigger_weight_UncSystUp,2));
            trigger_weight = trigger_weight + trigger_unc;
        }

    }
    if( trigger_weight == 0 ) trigger_weight = 1;

    return trigger_weight;
}


//---------------------------------------------------------------------------------------------------------------
// Prefiring Correction
// Return prefiring scale factor
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetPrefiringWeight( string sysID ){

    double prefiring_weight = 1.;
    if( (dataset_group != "Data") && (dataset_year != "18") ){
        if( sysID == "cv" ){
            prefiring_weight = L1PreFiringWeight_Nom;
        }else if( sysID == "down" ){
            prefiring_weight = L1PreFiringWeight_Dn;
        }else if( sysID == "up" ){
            prefiring_weight = L1PreFiringWeight_Up;
        }
    }

    return prefiring_weight;
}


//---------------------------------------------------------------------------------------------------------------
// JES uncertainty
// https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#JetCorUncertainties
// https://twiki.cern.ch/twiki/bin/view/CMS/IntroToJEC
// https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/master/POG/JME
// https://github.com/cms-jet/JECDatabase/blob/master/scripts/JERC2JSON/minimalDemo.py
//---------------------------------------------------------------------------------------------------------------
void HEPHero::JESvariation(){

    //if( (_sysName_lateral == "JES") && (dataset_group != "Data") ){
    if( ((_sysName_lateral == "JES") || ((_sysName_lateral == "Recoil") && (_Universe > 3)))  && (dataset_group != "Data") ){
        METCorrectionFromJES.SetXYZT(0.,0.,0.,0.);

        for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
            TLorentzVector JetLV_before;
            JetLV_before.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);

            float jet_raw_pt = Jet_pt[ijet]*(1-Jet_rawFactor[ijet]);
            float jet_pt = Jet_pt[ijet];
            float jet_eta = Jet_eta[ijet];
            float jet_area = Jet_area[ijet];
            float jet_pt_nomuon = Jet_pt[ijet]*(1-Jet_muonSubtrFactor[ijet]);
            float jet_EmEF = Jet_neEmEF[ijet] + Jet_chEmEF[ijet];

            if( jet_raw_pt <= 10 ) continue;
            if( jet_eta >= 5.2 ) continue;

            //float pt_unc = JES_unc.getUnc( jet_eta, jet_pt );
            float pt_unc = jet_JES_Unc->evaluate({jet_eta, jet_pt});

            /*
            float facL1 = jet_JES_L1->evaluate({jet_area, jet_eta, jet_raw_pt, fixedGridRhoFastjetAll});
            float facL2 = jet_JES_L2->evaluate({jet_eta, jet_raw_pt});
            float facL3 = jet_JES_L3->evaluate({jet_eta, jet_raw_pt});
            float facRes = jet_JES_Res->evaluate({jet_eta, jet_raw_pt});
            //float facL1L2L3Res = jet_JES_L1L2L3Res->evaluate({jet_area, jet_eta, jet_raw_pt, fixedGridRhoFastjetAll});

            float jet_pt_new = jet_raw_pt*facL1*facL2*facL3;

            cout << " " << endl;
            cout << "diff raw-new = " << abs(jet_raw_pt - jet_pt_new)/jet_pt << endl;
            cout << "diff std-new = " << abs(jet_pt - jet_pt_new)/jet_pt << endl;
            cout << "diff std-newRes = " << abs(jet_pt - jet_pt_new*facRes)/jet_pt << endl;
            //cout << "diff std-newCom = " << abs(jet_pt - jet_raw_pt*facL1L2L3Res)/jet_pt << endl;
            */

            if( _Universe % 2 == 0 ){
                Jet_pt[ijet] = jet_pt*(1. - pt_unc);
            }else{
                Jet_pt[ijet] = jet_pt*(1. + pt_unc);
            }

            // Cut according to MissingETRun2Corrections Twiki
            if( jet_pt_nomuon <= 15 ) continue;
            if( jet_EmEF >= 0.9 ) continue;

            TLorentzVector JetLV_after;
            JetLV_after.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);

            METCorrectionFromJES -= JetLV_after - JetLV_before;
        }


        for( unsigned int ijet = 0; ijet < nCorrT1METJet; ++ijet ) {
            TLorentzVector JetLV_before;
            JetLV_before.SetPtEtaPhiM(CorrT1METJet_rawPt[ijet],CorrT1METJet_eta[ijet],CorrT1METJet_phi[ijet],0);

            float jet_raw_pt = CorrT1METJet_rawPt[ijet];
            float jet_eta = CorrT1METJet_eta[ijet];
            float jet_area = CorrT1METJet_area[ijet];

            if( jet_raw_pt <= 10 ) continue;
            if( jet_eta >= 5.2 ) continue;

            float facL1 = jet_JES_L1->evaluate({jet_area, jet_eta, jet_raw_pt, fixedGridRhoFastjetAll});
            float facL2 = jet_JES_L2->evaluate({jet_eta, jet_raw_pt});
            float facL3 = jet_JES_L3->evaluate({jet_eta, jet_raw_pt});

            float jet_pt = jet_raw_pt*facL1*facL2*facL3;

            //float pt_unc = JES_unc.getUnc( jet_eta, jet_pt );
            float pt_unc = jet_JES_Unc->evaluate({jet_eta, jet_pt});


            if( _Universe % 2 == 0 ){
                jet_pt = jet_pt*(1 - pt_unc);
            }else{
                jet_pt = jet_pt*(1 + pt_unc);
            }

            float jet_pt_nomuon = jet_pt*(1-CorrT1METJet_muonSubtrFactor[ijet]);
            float jet_EmEF = 0.;

            // Cut according to MissingETRun2Corrections Twiki
            if( jet_pt_nomuon <= 15 ) continue;
            if( jet_EmEF >= 0.9 ) continue;

            TLorentzVector JetLV_after;
            JetLV_after.SetPtEtaPhiM(jet_pt,CorrT1METJet_eta[ijet],CorrT1METJet_phi[ijet],0);

            METCorrectionFromJES -= JetLV_after - JetLV_before;
        }

    }
}


//---------------------------------------------------------------------------------------------------------------
// JER correction
//---------------------------------------------------------------------------------------------------------------
void HEPHero::JERvariation(){

    if( apply_jer_corr && (dataset_group != "Data") ){

        //string sys_var = "nominal";
        string sys_var = "nom";
        if( (_sysName_lateral == "JER") && (_Universe == 0) ) sys_var = "down";
        if( (_sysName_lateral == "JER") && (_Universe == 1) ) sys_var = "up";

        METCorrectionFromJER.SetXYZT(0.,0.,0.,0.);
        for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
            TLorentzVector JetLV_before(0.,0.,0.,0.);
            JetLV_before.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);

            float jet_pt = Jet_pt[ijet];
            float jet_eta = Jet_eta[ijet];
            float genjet_pt = GenJet_pt[Jet_genJetIdx[ijet]];
            int   genjet_idx = Jet_genJetIdx[ijet];

            /*
            jer_corr.SetVariablesandMatching( { {"JetPt",jet_pt}, {"JetEta",jet_eta}, {"Rho",fixedGridRhoFastjetAll}, {"GenJetPt", (genjet_idx>=0) ? genjet_pt : 0.} }, (genjet_idx>=0) ? true : false );
            double jer_factor = jer_corr.GetCorrection(sys_var);
            */


            //if( abs(Jet_eta[ijet]) >= 4.7 ) continue;
            double jer_PtRes = jet_JER_PtRes_corr->evaluate({jet_eta, jet_pt, fixedGridRhoFastjetAll});
            double jer_SF = jet_JER_SF_corr->evaluate({jet_eta, sys_var});

            bool isMatched = false;
            if( Jet_GenJet_match(ijet, 0.2) && (abs(jet_pt-genjet_pt) < 3*jer_PtRes*jet_pt) ) isMatched = true;
            //bool isMatched = (genjet_idx>=0) ? true : false;
            double jer_factor;
            if( isMatched ){
                jer_factor = 1. + (jer_SF-1.)*(jet_pt - genjet_pt)/jet_pt;
            }else {
                TRandom random;
                random.SetSeed(2);
                jer_factor = 1. + random.Gaus(0.,jer_PtRes)*TMath::Sqrt(TMath::Max(pow(jer_SF,2)-1.,0.));
            }


            TLorentzVector JetLV_after = JetLV_before*jer_factor;

            Jet_pt[ijet] = JetLV_after.Pt();
            Jet_mass[ijet] = JetLV_after.M();
            Jet_eta[ijet] = JetLV_after.Eta();
            Jet_phi[ijet] = JetLV_after.Phi();

            float jet_pt_nomuon = Jet_pt[ijet]*(1-Jet_muonSubtrFactor[ijet]);
            float jet_EmEF = Jet_neEmEF[ijet] + Jet_chEmEF[ijet];

            // Cut according to MissingETRun2Corrections Twiki
            if( jet_pt_nomuon <= 15 ) continue;
            if( jet_EmEF >= 0.9 ) continue;

            METCorrectionFromJER -= JetLV_after - JetLV_before;
        }
    }
}


//---------------------------------------------------------------------------------------------------------------
// PDF Type
//---------------------------------------------------------------------------------------------------------------
void HEPHero::PDFtype(){

    if( (_sysID_lateral == 0) && (dataset_group != "Data") ){
        rapidjson::Document pdf_list;
        FILE *fp_pdf = fopen(PDF_file.c_str(), "r");
        char buf_pdf[0XFFFF];
        rapidjson::FileReadStream input_pdf(fp_pdf, buf_pdf, sizeof(buf_pdf));
        pdf_list.ParseStream(input_pdf);

        TObjArray *branches_list = _inputTree->GetListOfBranches();
        bool hasPDFbranch = false;
        for( unsigned int ibr = 0; ibr < branches_list[1].GetSize(); ++ibr ) {
            if( strcmp( branches_list[1][ibr][0].GetName(), "nLHEPdfWeight" ) == 0 ) hasPDFbranch = true;
        }

        string title;
        if( hasPDFbranch ){
            title = _inputTree->GetBranch("LHEPdfWeight")->GetTitle();
        }else{
            title = "Unknown";
            cout << "No branch named LHEPdfWeight!" << endl;
        }

        string delimiter = "LHA IDs ";
        size_t pos = title.find(delimiter);
        string title_end = title.erase(0, pos + delimiter.length());

        delimiter = " - ";
        pos = title_end.find(delimiter);
        string LHAID = title_end.substr(0, pos);

        if( pdf_list.HasMember(LHAID.c_str()) ){
            PDF_TYPE = pdf_list[LHAID.c_str()].GetString();
            cout << "PDF_TYPE " << PDF_TYPE << endl;
        }else{
            PDF_TYPE = "unknown";
            cout << "LHA ID unknown!" << endl;
        }
    }
}


//---------------------------------------------------------------------------------------------------------------
// Jet BTAG
//---------------------------------------------------------------------------------------------------------------
bool HEPHero::JetBTAG( int iobj, int WP ){

    bool obj_selected = false;
    float BTAG_CUT;

    // DeepJet
    if( dataset_year == "16"){
        if( dataset_dti == 0 ){
            if(      WP == 0 ) BTAG_CUT = 0.0508;   // loose
            else if( WP == 1 ) BTAG_CUT = 0.2598;   // medium
            else if( WP == 2 ) BTAG_CUT = 0.6502;   // tight
        }else{
            if(      WP == 0 ) BTAG_CUT = 0.0480;   // loose
            else if( WP == 1 ) BTAG_CUT = 0.2489;   // medium
            else if( WP == 2 ) BTAG_CUT = 0.6377;   // tight
        }
    }else if( dataset_year == "17" ){
        if(      WP == 0 ) BTAG_CUT = 0.0532;   // loose
        else if( WP == 1 ) BTAG_CUT = 0.3040;   // medium
        else if( WP == 2 ) BTAG_CUT = 0.7476;   // tight
    }else if( dataset_year == "18" ){
        if(      WP == 0 ) BTAG_CUT = 0.0490;   // loose
        else if( WP == 1 ) BTAG_CUT = 0.2783;   // medium
        else if( WP == 2 ) BTAG_CUT = 0.7100;   // tight
    }

    // DeepCSV
    if( dataset_year == "16" ){
        if( dataset_dti == 0 ){
            if(      WP == 3 ) BTAG_CUT = 0.2027;   // loose
            else if( WP == 4 ) BTAG_CUT = 0.6001;   // medium
            else if( WP == 5 ) BTAG_CUT = 0.8819;   // tight
        }else{
            if(      WP == 3 ) BTAG_CUT = 0.1918;   // loose
            else if( WP == 4 ) BTAG_CUT = 0.5847;   // medium
            else if( WP == 5 ) BTAG_CUT = 0.8767;   // tight
        }
    }else if( dataset_year == "17" ){
        if(      WP == 3 ) BTAG_CUT = 0.1355;   // loose
        else if( WP == 4 ) BTAG_CUT = 0.4506;   // medium
        else if( WP == 5 ) BTAG_CUT = 0.7738;   // tight
    }else if( dataset_year == "18" ){
        if(      WP == 3 ) BTAG_CUT = 0.1208;   // loose
        else if( WP == 4 ) BTAG_CUT = 0.4168;   // medium
        else if( WP == 5 ) BTAG_CUT = 0.7665;   // tight
    }

    if(      WP >= 0 and WP <=2 ) obj_selected = (Jet_btagDeepFlavB[iobj] > BTAG_CUT);    // DeepJet
    else if( WP >= 3 and WP <=5 ) obj_selected = (Jet_btagDeepB[iobj] > BTAG_CUT);        // DeepCSV

    return obj_selected;
}


//---------------------------------------------------------------------------------------------------------------
// Jet GenJet match
//---------------------------------------------------------------------------------------------------------------
bool HEPHero::Jet_GenJet_match( int ijet, float deltaR_cut ){

    bool match = false;
    double drMin = 10000;

    for( unsigned int igenJet = 0; igenJet < nGenJet; ++igenJet ) {
        double deta = fabs(GenJet_eta[igenJet] - Jet_eta[ijet]);
        double dphi = fabs(GenJet_phi[igenJet] - Jet_phi[ijet]);
        if( dphi > M_PI ) dphi = 2*M_PI - dphi;
        double dr = sqrt( deta*deta + dphi*dphi );
        if( dr < drMin ) drMin = dr;
    }

    if( drMin < deltaR_cut ) match = true;

    return match;
}


//---------------------------------------------------------------------------------------------------------------
// Electron ID
//---------------------------------------------------------------------------------------------------------------
bool HEPHero::ElectronID( int iobj, int WP ){

    bool obj_selected = false;

    if(      WP == 0 ) obj_selected = (Electron_cutBased[iobj] >= 1);
    else if( WP == 1 ) obj_selected = (Electron_cutBased[iobj] >= 2);
    else if( WP == 2 ) obj_selected = (Electron_cutBased[iobj] >= 3);
    else if( WP == 3 ) obj_selected = (Electron_cutBased[iobj] >= 4);
    else if( WP == 4 ) obj_selected = Electron_mvaFall17V2Iso_WP90[iobj];
    else if( WP == 5 ) obj_selected = Electron_mvaFall17V2Iso_WP80[iobj];

    return obj_selected;
}


//---------------------------------------------------------------------------------------------------------------
// Muon ID
//---------------------------------------------------------------------------------------------------------------
bool HEPHero::MuonID( int iobj, int WP ){

    bool obj_selected = false;

    if(      WP == 0 ) obj_selected = Muon_looseId[iobj];
    else if( WP == 1 ) obj_selected = Muon_mediumId[iobj];
    else if( WP == 2 ) obj_selected = Muon_tightId[iobj];

    return obj_selected;
}


//---------------------------------------------------------------------------------------------------------------
// Muon ISO
//---------------------------------------------------------------------------------------------------------------
bool HEPHero::MuonISO( int iobj, int WP ){

    bool obj_selected = false;

    if(      WP == 0 ) obj_selected = true;
    else if( WP == 1 ) obj_selected = (Muon_pfIsoId[iobj] >= 2);
    else if( WP == 2 ) obj_selected = (Muon_pfIsoId[iobj] >= 2);
    else if( WP == 3 ) obj_selected = (Muon_pfIsoId[iobj] >= 4);

    return obj_selected;
}


//---------------------------------------------------------------------------------------------------------------
// Lepton jet overlap
//---------------------------------------------------------------------------------------------------------------
bool HEPHero::Lep_jet_overlap( int ilep, string type ){

    bool overlap = false;
    double drMin = 10000;

    for( unsigned int ijet = 0; ijet < nJet; ++ijet ) {
        if( Jet_pt[ijet] <= 20 ) continue;
        if( Jet_jetId[ijet] < 2 ) continue;  // Test different jet IDs
        double deta;
        double dphi;
        if( type == "muon" ){
            deta = fabs(Muon_eta[ilep] - Jet_eta[ijet]);
            dphi = fabs(Muon_phi[ilep] - Jet_phi[ijet]);
        }else if( type == "electron" ){
            deta = fabs(Electron_eta[ilep] - Jet_eta[ijet]);
            dphi = fabs(Electron_phi[ilep] - Jet_phi[ijet]);
        }
        if( dphi > M_PI ) dphi = 2*M_PI - dphi;
        double dr = sqrt( deta*deta + dphi*dphi );
        if( dr < drMin ) drMin = dr;
    }
    if( drMin < 0.4 ) overlap = true;

    return overlap;
}


//---------------------------------------------------------------------------------------------------------------
// MCsamples processing
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetTopPtWeight(){

    float pt_weight = 1.;
    string dsName4 = _datasetName.substr(0,4);
    if( (dsName4 == "TTTo") && (dataset_group != "Data") ){
        std::map<int, Float_t> pdgId_pt_map;
        for(int iPart=0; iPart<nGenPart; iPart++){

            if( pdgId_pt_map.size()==2 ) break;

            if( (GenPart_pdgId[iPart] == 6) && (GenPart_statusFlags[iPart] & 256) && (GenPart_statusFlags[iPart] & 8192) ){
                pdgId_pt_map[6] = GenPart_pt[iPart];
            }else if( (GenPart_pdgId[iPart] == -6) && (GenPart_statusFlags[iPart] & 256) && (GenPart_statusFlags[iPart] & 8192) ){
                pdgId_pt_map[-6] = GenPart_pt[iPart];
            }else{
                continue;
            }

        }

        if( pdgId_pt_map.size()==2 ){

            double top_sf = 0.103*TMath::Exp(-0.0118*pdgId_pt_map[6])-0.000134*pdgId_pt_map[6]+0.973;
            double antitop_sf = 0.103*TMath::Exp(-0.0118*pdgId_pt_map[-6])-0.000134*pdgId_pt_map[-6]+0.973;

            pt_weight *= TMath::Sqrt(top_sf*antitop_sf);

        }else{
            std::cout << "Warning: Particle level ttbar system not found, not applying the top-pt reweighting correction ..." << std::endl;
        }
    }

    return pt_weight;
}



//---------------------------------------------------------------------------------------------------------------
// NLO -> NNLO k-factors for WZ (Valid for Gen Wpt > 25 GeV)
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetWPtWeight(){

    float pt_weight = 1.;
    string dsName6 = _datasetName.substr(0,6);
    if( (dsName6 == "WZTo3L") && (dataset_group != "Data") ){

        vector<int> W_daughters;
        for( unsigned int ipart = 0; ipart < nGenPart; ++ipart ) {
            if( (abs(GenPart_pdgId[ipart]) >= 11) && (abs(GenPart_pdgId[ipart]) <= 16) && (abs(GenPart_pdgId[GenPart_genPartIdxMother[ipart]]) == 24) ){
                W_daughters.push_back(ipart);
            }
        }

        if( W_daughters.size() == 4 ){
            if( (abs(GenPart_pdgId[W_daughters[0]]) == 11) && (abs(GenPart_pdgId[W_daughters[1]]) == 11) ) W_daughters.erase(W_daughters.begin(),W_daughters.begin()+2);
            if( (abs(GenPart_pdgId[W_daughters[2]]) == 11) && (abs(GenPart_pdgId[W_daughters[3]]) == 11) ) W_daughters.erase(W_daughters.begin()+2,W_daughters.begin()+4);
        }

        float W_charge = -1.;
        if( (GenPart_pdgId[W_daughters[0]] == -11) || (GenPart_pdgId[W_daughters[0]] == -13) || (GenPart_pdgId[W_daughters[0]] == -15) || (GenPart_pdgId[W_daughters[1]] == -11) || (GenPart_pdgId[W_daughters[1]] == -13) || (GenPart_pdgId[W_daughters[1]] == -15) ){
            W_charge = 1.;
        }

        TLorentzVector W_dau1;
        TLorentzVector W_dau2;
        W_dau1.SetPtEtaPhiM(GenPart_pt[W_daughters[0]], GenPart_eta[W_daughters[0]], GenPart_phi[W_daughters[0]], GenPart_mass[W_daughters[0]]);
        W_dau2.SetPtEtaPhiM(GenPart_pt[W_daughters[1]], GenPart_eta[W_daughters[1]], GenPart_phi[W_daughters[1]], GenPart_mass[W_daughters[1]]);

        TLorentzVector W_part = W_dau1 + W_dau2;
        float W_pt = W_part.Pt();

        vector<float> intervals = {0, 35, 50, 70, 90, 110, 130, 160, 200, 30000}; //{25, 35, 50, 70, 90, 110, 130, 160, 200, 300};
        vector<float> k_factors;
        if( W_charge > 0 ){
            k_factors = {0.96773208, 0.99251315, 1.00993565, 1.02432459, 1.03293558, 1.05039964, 1.0413764, 1.07347302, 1.08221239};
        }else{
            k_factors = {0.93163252, 0.98802125, 1.01016316, 1.03190894, 1.04062135, 1.07619106, 1.14103155, 1.13462869, 1.24623567};
        }

        int idx = -1;
        for (unsigned int i = 0; i < k_factors.size() ; i++){
            if ( W_pt >= intervals[i] && W_pt < intervals[i+1]  ){
                idx = i;
                break;
            }
        }
        if( idx >= 0 ) pt_weight = k_factors[idx];

    }

    return pt_weight;
}



//---------------------------------------------------------------------------------------------------------------
// Get V+Jets HT Weight (LO->NLO)
// It is recommended to be used in regions with (HT > 250 GeV). If DY the Zmass must be in the range: (80 < Zmass < 100)
//---------------------------------------------------------------------------------------------------------------
float HEPHero::GetVJetsHTWeight(){

    float HT_weight = 1.;
    string dsName13 = _datasetName.substr(0,13);
    string dsName14 = _datasetName.substr(0,14);
    if( (dsName13 == "DYJetsToLL_HT") || (dsName13 == "WJetsToLNu_HT") || (dsName14 == "ZJetsToNuNu_HT") ){

        vector<float> genHT_ranges = {100., 120., 140., 160., 180., 200., 220., 240., 260., 280., 300., 320., 340., 360., 380., 400., 450., 500., 550., 600., 650., 700., 750., 800., 900., 1000., 1000000.};

        vector<float> factors = {1.28356649, 1.25595964, 1.22291551, 1.18181696, 1.14899496, 1.1088571, 1.0725011, 1.05669841, 1.033511, 1.01575615, 0.99106104, 0.96863749, 0.94938954, 0.93980183, 0.92809302, 0.89480215, 0.86860013, 0.83442312, 0.80850022, 0.80742663, 0.78754158, 0.78368212, 0.75381177, 0.77197091, 0.7433579, 0.7460261};

        int idx = -1;
        for (unsigned int i = 0; i < factors.size() ; i++){
            if ( LHE_HT >= genHT_ranges[i] && LHE_HT < genHT_ranges[i+1]  ){
                idx = i;
                break;
            }
        }
        if( idx >= 0 ) HT_weight = factors[idx];

    }

    return HT_weight;
}




