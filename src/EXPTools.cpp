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


//---------------------------------------------------------------------------------------------
// JES uncertainty
// https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#JetCorUncertainties
// https://twiki.cern.ch/twiki/bin/view/CMS/IntroToJEC
// https://gitlab.cern.ch/cms-nanoAOD/jsonpog-integration/-/tree/master/POG/JME
// https://github.com/cms-jet/JECDatabase/blob/master/scripts/JERC2JSON/minimalDemo.py
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// JER correction
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// PDF Type
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Jet BTAG
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Jet GenJet match
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Electron ID
//---------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------
// Muon ID
//---------------------------------------------------------------------------------------------
bool HEPHero::MuonID( int iobj, int WP ){

    bool obj_selected = false;

    if(      WP == 0 ) obj_selected = Muon_looseId[iobj];
    else if( WP == 1 ) obj_selected = Muon_mediumId[iobj];
    else if( WP == 2 ) obj_selected = Muon_tightId[iobj];

    return obj_selected;
}


//---------------------------------------------------------------------------------------------
// Muon ISO
//---------------------------------------------------------------------------------------------
bool HEPHero::MuonISO( int iobj, int WP ){

    bool obj_selected = false;

    if(      WP == 0 ) obj_selected = true;
    else if( WP == 1 ) obj_selected = (Muon_pfIsoId[iobj] >= 2);
    else if( WP == 2 ) obj_selected = (Muon_pfIsoId[iobj] >= 2);
    else if( WP == 3 ) obj_selected = (Muon_pfIsoId[iobj] >= 4);

    return obj_selected;
}


//---------------------------------------------------------------------------------------------
// Lepton jet overlap
//---------------------------------------------------------------------------------------------
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


