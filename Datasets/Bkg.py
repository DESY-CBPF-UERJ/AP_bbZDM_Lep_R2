analysis = "AP_EFT_ML_R2"
nano_version = 'v9'

#--------------------------------------------------------------------------------------------------
# Production ID:
# 00-09(Data), 10-19(MC-signal), 20-98(MC-bkg), 99(private sample)
#
# Data taking interval (DTI):
# 2016 DTIs = 0(with "HIPM"/"APV")("pre-VFP"), 1(without "HIPM"/"APV")("pos-VFP")
#--------------------------------------------------------------------------------------------------

paths = {}
paths["0_16"] = analysis+'/Datasets/Files/bkg_16/dti_0/'+nano_version+'/'
paths["1_16"] = analysis+'/Datasets/Files/bkg_16/dti_1/'+nano_version+'/'
paths["0_17"] = analysis+'/Datasets/Files/bkg_17/dti_0/'+nano_version+'/'
paths["0_18"] = analysis+'/Datasets/Files/bkg_18/dti_0/'+nano_version+'/'


# https://xsecdb-xsdb-official.app.cern.ch/
# Source[*] > > https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns
# Source[**] > > https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWG
# Source[new1] > > https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV
# Source[new2] > > https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
# Source[new3] > > https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SingleTopRefXsec
b_ds_info = { # [DatasetName, Production ID, PROC_XSEC[pb], XSEC_UNC[pb], XSEC_Accuracy]
"DYPt0To50": [
    ["DYJetsToLL_Pt-0To3",                  '20',       6077.22,            122.45,             '[new1]NNLO'],
    ["DYJetsToLL_PtZ-3To50",                '20',       1414.0,             0,                  '[new1]NNLO*'],
],

"DYPt50ToInf": [
    ["DYJetsToLL_PtZ-50To100",              '21',       378.97,             0,                  '[new1]NNLO*'],
    ["DYJetsToLL_PtZ-100To250",             '21',       93.1366,            0,                  '[new1]NNLO*'],
    ["DYJetsToLL_PtZ-250To400",             '21',       3.7093,             0,                  '[new1]NNLO*'],
    ["DYJetsToLL_PtZ-400To650",             '21',       0.5150,             0,                  '[new1]NNLO*'],
    ["DYJetsToLL_PtZ-650ToInf",             '21',       0.048934,           0,                  '[new1]NNLO*'],
],

"TTFullLep": [
    ["TTTo2L2Nu",                           '22',       88.51,              0,                  '[new2]NNLO+NNLL'],
],

"TTSemiLep": [
    ["TTToSemiLeptonic",                    '23',       366.29,             0,                  '[new2]NNLO+NNLL'],
],

"ST": [
    ["ST_tW_top",                           '24',       35.85,              (1.92,1.92),        '[new3]NLO'],
    ["ST_tW_antitop",                       '24',       35.85,              (1.92,1.92),        '[new3]NLO'],
    ["ST_t-channel_top",                    '24',       136.02,             (7.72,6.56),        '[new3]NLO'],
    ["ST_t-channel_antitop",                '24',       80.95,              (5.79,5.16),        '[new3]NLO'],
    ["ST_s-channel",                        '24',       10.32,              (0.20,0.18),        '[new3]NLO'],
],

"VZ": [
    ["ZZTo2L2Nu",                           '25',       0.9738,             0.0009971,          'NLO'],
    ["ZZTo4L",                              '25',       1.325,              0.00122,            'NLO'],
    ["WZTo3LNu",                            '25',       4.664,              0.004639,           'NLO'],
    #["ggToZZTo2e2mu",                       '25',       0.003292,           0.,                 'unknown'],
    #["ggToZZTo2e2nu",                       '25',       0.01773,            0.,                 'unknown'],
    #["ggToZZTo2e2tau",                      '25',       0.003294,           0.,                 'unknown'],
    #["ggToZZTo2mu2nu",                      '25',       0.01773,            0.,                 'unknown'],
    #["ggToZZTo2mu2tau",                     '25',       0.003294,           0.,                 'unknown'],
    #["ggToZZTo4e",                          '25',       0.001619,           0.,                 'unknown'],
    #["ggToZZTo4mu",                         '25',       0.001608,           0.,                 'unknown'],
    #["ggToZZTo4tau",                        '25',       0.001626,           0.,                 'unknown'],
],

"ResidualSM": [
    ["ZZTo2Q2L",                            '26',       3.676,              0,                  'unknown'],
    ["WZTo2Q2L",                            '26',       6.565,              0.01984,            'NLO'],
    ["WWTo2L2Nu",                           '26',       11.09,              0.00704,            'NLO'],
    ["ZZZ",                                 '26',       0.01476,            2.5e-06,            'unknown'],
    ["WZZ",                                 '26',       0.05709,            6.213e-05,          'unknown'],
    ["WWZ",                                 '26',       0.1707,             0.0001757,          'unknown'],
    ["WWW",                                 '26',       0.2158,             0.0002479,          'unknown'],
    ["TTWZ",                                '26',       0.002453,           1.623e-06,          'LO'],
    ["TTZZ",                                '26',       0.001386,           1.28e-06,           'LO'],
    ["TTWW",                                '26',       0.007003,           5.288e-06,          'LO'],
    ["TTWJetsToLNu",                        '26',       0.2161,             0.0008017,          'unknown'],
    ["TTWJetsToQQ",                         '26',       0.4377,             0.001622,           'unknown'],
    ["TTZToQQ",                             '26',       0.5104,             0.000697,           'unknown'],
    ["TTZToNuNu",                           '26',       0.1476,             0.0001971,          'unknown'],
    ["TTZToLL",                             '26',       0.05324,            0.0001121,          'unknown'],
    ["tZq_ll",                              '26',       0.07561,            0.0002088,          'unknown'],
    #["ZZ_Inclusive",                        '26',       12.17,              0.01966,            'unknown'],
    #["ZZ_Others",                           '26',       12.17,              0.01966,            'unknown'],
    #["WZ_Inclusive",                        '26',       27.59,              0.03993,            'unknown'],
    #["WZ_Others",                           '26',       27.59,              0.03993,            'unknown'],
    #["WW",                                  '26',       75.95,              0.1124,             'unknown'],
    #["TWZToLL_thad_Wlept",                  '26',       0.003004,           8.02e-06,           'unknown'],
    #["TWZToLL_tlept_Whad",                  '26',       0.003004,           8.02e-06,           'unknown'],
    #["TWZToLL_tlept_Wlept",                 '26',       0.001501,           4.009e-06,          'unknown'],
],
}


#----------------------------------------------------------------------------------------
# [DO NOT TOUCH THIS PART]
#----------------------------------------------------------------------------------------
b_ds = {}
for period in paths.keys():

    dti = period[0]
    year = period[-2:]

    for key in b_ds_info.keys():
        b_ds[key+"_"+period] = []
        for ds in b_ds_info[key]:
            list_temp = []
            list_temp.append(ds[0]+"_"+period)
            list_temp.append(ds[1]+year+dti)
            list_temp.append(paths[period]+ds[0]+".txt")
            list_temp.append(ds[2])
            list_temp.append(ds[3])
            b_ds[key+"_"+period].append(list_temp)

