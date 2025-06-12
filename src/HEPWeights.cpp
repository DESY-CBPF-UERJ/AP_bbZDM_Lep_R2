#include "HEPHero.h"


//---------------------------------------------------------------------------------------------
// Weight corrections
//---------------------------------------------------------------------------------------------
void HEPHero::Weight_corrections(){

    pileup_wgt = 1.;
    electron_wgt = 1.;
    muon_wgt = 1.;
    jet_puid_wgt = 1.;
    btag_wgt = 1.;
    trigger_wgt = 1.;
    prefiring_wgt = 1.;
    top_pt_wgt = 1.;
    w_pt_wgt = 1.;
    vjets_HT_wgt = 1.;

    if(dataset_group != "Data"){

        if( apply_pileup_wgt ){
            pileup_wgt = GetPileupWeight(Pileup_nTrueInt, "nominal");
            evtWeight *= pileup_wgt;
        }

        if( apply_electron_wgt ){
            electron_wgt = GetElectronWeight("cv");
            evtWeight *= electron_wgt;
        }

        if( apply_muon_wgt ){
            muon_wgt = GetMuonWeight("cv");
            evtWeight *= muon_wgt;
        }

        if( apply_jet_puid_wgt ){
            jet_puid_wgt = GetJetPUIDWeight("cv");
            evtWeight *= jet_puid_wgt;
        }

        if( apply_btag_wgt ){
            btag_wgt = GetBTagWeight("cv");
            evtWeight *= btag_wgt;
        }

        if( apply_trigger_wgt ){
            trigger_wgt = GetTriggerWeight("cv");
            evtWeight *= trigger_wgt;
        }

        if( apply_prefiring_wgt ){
            prefiring_wgt = GetPrefiringWeight("cv");
            evtWeight *= prefiring_wgt;
        }

        if( apply_top_pt_wgt ){
            top_pt_wgt = GetTopPtWeight();
            evtWeight *= top_pt_wgt;
        }

        if( apply_w_pt_wgt ){
            w_pt_wgt = GetWPtWeight();
            evtWeight *= w_pt_wgt;
        }

        if( apply_vjets_HT_wgt ){
            vjets_HT_wgt = GetVJetsHTWeight();
            evtWeight *= vjets_HT_wgt;
        }

    }
}


//---------------------------------------------------------------------------------------------
// Pileup Correction
// Return weight associated to the pileup effect
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Electron ID Correction
// Return weight associated to the identification of the two electrons
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Muon ID Correction
// Return weight associated to the identification of the muons
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Jet puID Correction
// Return weight associated to the jet pileup ID selection
// Jet pileup ID identify jets that are NOT pileup
// https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetIDUL#Data_MC_Efficiency_Scale_Factors
// https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetID#Efficiencies_and_data_MC_scale_f
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// BTagging Correction
// Return weight associated to the btagging selection
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Trigger Correction
// Return trigger scale factor
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Prefiring Correction
// Return prefiring scale factor
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// MCsamples processing
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// NLO -> NNLO k-factors for WZ (Valid for Gen Wpt > 25 GeV)
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Get V+Jets HT Weight (LO->NLO)
// It is recommended to be used in regions with (HT > 250 GeV). If DY the Zmass must be in the range: (80 < Zmass < 100)
//---------------------------------------------------------------------------------------------
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


