#--------------------------------------------------------------------------------------------------
# Dataset files
#--------------------------------------------------------------------------------------------------
selection = "Test"
analysis = "AP_bbZDM_Lep_R2"
treeName = "Events"
LumiWeights = 1

analysis_parameters = {   
"JET_ETA_CUT                ": 2.4,
"JET_PT_CUT                 ": 20,      # GeV
"JET_ID_WP                  ": 6,       # 0-loose, 2-tight, 6-tightlepveto 
"JET_PUID_WP                ": 7,       # 0-fail, 1-loose, 3-medium, 7-tight (tight is the only correct WP in v9)
"JET_BTAG_WP                ": 3,       # DeepJet: 0-loose, 1-medium, 2-tight; DeepCSV: 3-loose, 4-medium, 5-tight; [Set up the efficiency maps as well]
"JET_LEP_DR_ISO_CUT         ": 0.4,
"ELECTRON_GAP_LOWER_CUT     ": 1.444,   # Lower absolute limit of barrel-endcap gap
"ELECTRON_GAP_UPPER_CUT     ": 1.566,   # Upper absolute limit of barrel-endcap gap
"ELECTRON_ETA_CUT           ": 2.5,
"ELECTRON_PT_CUT            ": 20,      # GeV  
"ELECTRON_LOW_PT_CUT        ": 15,      # GeV
"ELECTRON_ID_WP             ": 4,       # 0-veto, 1-loose, 2-medium, 3-tight, 4-WP90iso, 5-WP80iso
"MUON_ETA_CUT               ": 2.4, 
"MUON_PT_CUT                ": 20,      # GeV
"MUON_LOW_PT_CUT            ": 15,      # GeV
"MUON_ID_WP                 ": 1,       # 0-loose, 1-medium, 2-tight
"MUON_ISO_WP                ": 3,       # 0-none, 1-loose/looseID, 2-loose/mediumID, 3-tight/mediumID
#"LEPTON_DR_ISO_CUT          ": 0.3,
"LEADING_LEP_PT_CUT         ": 40,      # GeV
"LEPLEP_PT_CUT              ": 40,      # GeV
"LEPLEP_DR_CUT              ": 3.2,
"LEPLEP_DM_CUT              ": 25,      # GeV
"MET_CUT                    ": 65,      # GeV
"MET_DY_UPPER_CUT           ": 100,     # GeV
"MET_LEPLEP_DPHI_CUT        ": 0.8,
"MET_LEPLEP_MT_CUT          ": 90,      # GeV     
}

corrections = {  # 0-don't apply, 1-apply
"PILEUP_WGT                 ": 1, 
"ELECTRON_ID_WGT            ": 1,
"MUON_ID_WGT                ": 1,
"JET_PUID_WGT               ": 1,
"BTAG_WGT                   ": 1,
"TRIGGER_WGT                ": 1,
"PREFIRING_WGT              ": 1,
"JER_CORR                   ": 1,
"MET_XY_CORR                ": 1,
"MET_RECOIL_CORR            ": 1,
"TOP_PT_WGT                 ": 1,
"W_PT_WGT                   ": 1,
"MUON_ROC_CORR              ": 1,
#"VJETS_HT_WGT               ": 0,
}

lateral_systematics = { 
"CV":          [0,  1, [], []],   # [sys_source, sys_universe, processes_ID (empty -> all), subsources] 
#"JES":         [1,  54, [], ["AbsoluteMPFBias_fc", "AbsoluteScale_fc", "AbsoluteStat", "FlavorQCD_fc", "Fragmentation_fc", "PileUpDataMC_fc", "PileUpPtBB_fc", "PileUpPtEC1_fc", "PileUpPtEC2_fc", "PileUpPtHF_fc", "PileUpPtRef_fc", "RelativeFSR_fc", "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF_fc", "RelativePtBB_fc", "RelativePtEC1", "RelativePtEC2", "RelativePtHF_fc", "RelativeBal_fc", "RelativeSample", "RelativeStatEC", "RelativeStatFSR", "RelativeStatHF", "SinglePionECAL_fc", "SinglePionHCAL_fc", "TimePtEta"]],
#"JER":         [28,  2, [], []],
#"UncMET":      [29,  2, [], []],
#"Recoil":      [30,  6, ["02"], []],
#"JES":         [41,  2, [], ["Total"]],
}

vertical_systematics = {
"Pileup":      [50,   2,  [], []],
"EleID":       [51,   2,  [], []],
"MuID":        [52,  2,  [], []],
"JetPU":       [53,  2,  [], []],
"BTag":        [54,  8,  [], ["bc", "light", "bc_fc", "light_fc"]],
"Trig":        [58,  2,  [], []],
"PreFir":      [59,  2,  [], []],
"PDF":         [60,  100,  [], []],
"Scales":      [62,  7,  [], []],
"ISR":         [63,  2,  [], []],
"FSR":         [64,  2,  [], []],
"TopPt":       [65,  1,  [], []],
'WPt':         [66, 1, [], []],
#"TT1LXS":      [65,  2,  0],
#"TT2LXS":      [66,  2,  0],
#"DYXS":        [67,  2,  0],
}


#--------------------------------------------------------------------------------------------------
# Jobs setup
#--------------------------------------------------------------------------------------------------
NumMaxEvents = -1
NumFilesPerJob_Data = 1   
NumFilesPerJob_Signal = 50 
NumFilesPerJob_Bkg = 1


#--------------------------------------------------------------------------------------------------
# Dataset files
#--------------------------------------------------------------------------------------------------
periods = [
"0_16",
"1_16",
"0_17",
"0_18",
]

datasets = [
#"Data_Lep",
#"Data_MET",
#"Signal",
"DYPt50ToInf",
"DYPt0To50",
"TTFullLep",
"TTSemiLep",
"ST",
"VZ",
"ResidualSM"    
]


#--------------------------------------------------------------------------------------------------
# Metadata
#--------------------------------------------------------------------------------------------------
metadata = {       
"NN_prep_keras_X_XX         ": analysis+"/Metadata/ML/Keras/preprocessing.json",
"NN_model_keras_X_XX        ": analysis+"/Metadata/ML/Keras/NN_4_100_elu_adam/model.json",
"NN_model_torch_0_16        ": analysis+"/Metadata/ML/Torch/DeepCSV/2016preVFP/model_scripted.pt",
"NN_model_torch_1_16        ": analysis+"/Metadata/ML/Torch/DeepCSV/2016postVFP/model_scripted.pt",
"NN_model_torch_0_17        ": analysis+"/Metadata/ML/Torch/DeepCSV/2017/model_scripted.pt",
"NN_model_torch_0_18        ": analysis+"/Metadata/ML/Torch/DeepCSV/2018/model_scripted.pt",
"lumi_certificate_X_16      ": analysis+"/Metadata/certificates/Cert_271036-284044_13TeV_Legacy2016_Collisions16.json",
"lumi_certificate_X_17      ": analysis+"/Metadata/certificates/Cert_294927-306462_13TeV_UL2017_Collisions17.json",
"lumi_certificate_X_18      ": analysis+"/Metadata/certificates/Cert_314472-325175_13TeV_Legacy2018_Collisions18.json",
"pdf_type_X_XX              ": analysis+"/Metadata/PDF/pdf_type.json",
"pileup_0_16                ": analysis+"/Metadata/POG/LUM/2016preVFP_UL/puWeights.json.gz",
"pileup_1_16                ": analysis+"/Metadata/POG/LUM/2016postVFP_UL/puWeights.json.gz",
"pileup_0_17                ": analysis+"/Metadata/POG/LUM/2017_UL/puWeights.json.gz",
"pileup_0_18                ": analysis+"/Metadata/POG/LUM/2018_UL/puWeights.json.gz",
"electron_0_16              ": analysis+"/Metadata/POG/EGM/2016preVFP_UL/electron.json.gz",
"electron_1_16              ": analysis+"/Metadata/POG/EGM/2016postVFP_UL/electron.json.gz",
"electron_0_17              ": analysis+"/Metadata/POG/EGM/2017_UL/electron.json.gz",
"electron_0_18              ": analysis+"/Metadata/POG/EGM/2018_UL/electron.json.gz",
"muon_0_16                  ": analysis+"/Metadata/POG/MUO/2016preVFP_UL/muon_Z.json.gz",
"muon_1_16                  ": analysis+"/Metadata/POG/MUO/2016postVFP_UL/muon_Z.json.gz",
"muon_0_17                  ": analysis+"/Metadata/POG/MUO/2017_UL/muon_Z.json.gz",
"muon_0_18                  ": analysis+"/Metadata/POG/MUO/2018_UL/muon_Z.json.gz",
"btag_SF_0_16               ": analysis+"/Metadata/POG/BTV/2016preVFP_UL/btagging.json.gz",
"btag_SF_1_16               ": analysis+"/Metadata/POG/BTV/2016postVFP_UL/btagging.json.gz",
"btag_SF_0_17               ": analysis+"/Metadata/POG/BTV/2017_UL/btagging.json.gz",
"btag_SF_0_18               ": analysis+"/Metadata/POG/BTV/2018_UL/btagging.json.gz",
"btag_eff_0_16              ": analysis+"/Metadata/btag_eff/DeepCSVLoose/2016preVFP.json",
"btag_eff_1_16              ": analysis+"/Metadata/btag_eff/DeepCSVLoose/2016postVFP.json",
"btag_eff_0_17              ": analysis+"/Metadata/btag_eff/DeepCSVLoose/2017.json",
"btag_eff_0_18              ": analysis+"/Metadata/btag_eff/DeepCSVLoose/2018.json",
"trigger_X_16               ": analysis+"/Metadata/trigger/SF_2016_ttbar.json",
"trigger_X_17               ": analysis+"/Metadata/trigger/SF_2017_ttbar.json",
"trigger_X_18               ": analysis+"/Metadata/trigger/SF_2018_ttbar.json",
"JES_MC_0_16                ": analysis+"/Metadata/JES/JES_MC_16_preVFP.txt",
"JES_MC_1_16                ": analysis+"/Metadata/JES/JES_MC_16_postVFP.txt",
"JES_MC_0_17                ": analysis+"/Metadata/JES/JES_MC_17.txt",
"JES_MC_0_18                ": analysis+"/Metadata/JES/JES_MC_18.txt",
"JER_MC_0_16                ": analysis+"/Metadata/JER/Summer20UL16APV_JRV3_MC/Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs.txt",
"JER_SF_MC_0_16             ": analysis+"/Metadata/JER/Summer20UL16APV_JRV3_MC/Summer20UL16APV_JRV3_MC_SF_AK4PFchs.txt",
"JER_MC_1_16                ": analysis+"/Metadata/JER/Summer20UL16_JRV3_MC/Summer20UL16_JRV3_MC_PtResolution_AK4PFchs.txt",
"JER_SF_MC_1_16             ": analysis+"/Metadata/JER/Summer20UL16_JRV3_MC/Summer20UL16_JRV3_MC_SF_AK4PFchs.txt",
"JER_MC_0_17                ": analysis+"/Metadata/JER/Summer19UL17_JRV2_MC/Summer19UL17_JRV2_MC_PtResolution_AK4PFchs.txt",
"JER_SF_MC_0_17             ": analysis+"/Metadata/JER/Summer19UL17_JRV2_MC/Summer19UL17_JRV2_MC_SF_AK4PFchs.txt",
"JER_MC_0_18                ": analysis+"/Metadata/JER/Summer19UL18_JRV2_MC/Summer19UL18_JRV2_MC_PtResolution_AK4PFchs.txt",
"JER_SF_MC_0_18             ": analysis+"/Metadata/JER/Summer19UL18_JRV2_MC/Summer19UL18_JRV2_MC_SF_AK4PFchs.txt",
"jet_puID_0_16              ": analysis+"/Metadata/POG/JME/2016preVFP_UL/jmar.json.gz",
"jet_puID_1_16              ": analysis+"/Metadata/POG/JME/2016postVFP_UL/jmar.json.gz",
"jet_puID_0_17              ": analysis+"/Metadata/POG/JME/2017_UL/jmar.json.gz",
"jet_puID_0_18              ": analysis+"/Metadata/POG/JME/2018_UL/jmar.json.gz",
"JERC_0_16                  ": analysis+"/Metadata/POG/JME/2016preVFP_UL/jet_jerc.json.gz",
"JERC_1_16                  ": analysis+"/Metadata/POG/JME/2016postVFP_UL/jet_jerc.json.gz",
"JERC_0_17                  ": analysis+"/Metadata/POG/JME/2017_UL/jet_jerc.json.gz",
"JERC_0_18                  ": analysis+"/Metadata/POG/JME/2018_UL/jet_jerc.json.gz",
"mu_RoccoR_0_16             ": analysis+"/Metadata/mu_Rochester/RoccoR2016aUL.txt",
"mu_RoccoR_1_16             ": analysis+"/Metadata/mu_Rochester/RoccoR2016bUL.txt",
"mu_RoccoR_0_17             ": analysis+"/Metadata/mu_Rochester/RoccoR2017UL.txt",
"mu_RoccoR_0_18             ": analysis+"/Metadata/mu_Rochester/RoccoR2018UL.txt",
"Z_recoil_X_16              ": analysis+"/Metadata/boson_recoil/Z/TypeI-PFMet_Run2016_legacy.root",
"Z_recoil_X_17              ": analysis+"/Metadata/boson_recoil/Z/Type1_PFMET_2017.root",
"Z_recoil_X_18              ": analysis+"/Metadata/boson_recoil/Z/TypeI-PFMet_Run2018.root",
}


#--------------------------------------------------------------------------------------------------
# Plots
#--------------------------------------------------------------------------------------------------
Get_Image_in_EPS = 0
Get_Image_in_PNG = 1
Get_Image_in_PDF = 0
