analysis = "AP_bbZDM_Lep_R2"
nano_version = 'v9'

#--------------------------------------------------------------------------------------------------
# Production ID:
# 00-09(Data), 10-19(MC-signal), 20-98(MC-bkg), 99(private sample)
#
# Data taking interval (DTI):
# 2016 DTIs = 0(with "HIPM"/"APV")("pre-VFP"), 1(without "HIPM"/"APV")("pos-VFP")
#--------------------------------------------------------------------------------------------------

paths = {}
paths["0_16"] = analysis+'/Datasets/Files/signal_16/dti_0/'+nano_version+'/'
paths["1_16"] = analysis+'/Datasets/Files/signal_16/dti_1/'+nano_version+'/'
paths["0_17"] = analysis+'/Datasets/Files/signal_17/dti_0/'+nano_version+'/'
paths["0_18"] = analysis+'/Datasets/Files/signal_18/dti_0/'+nano_version+'/'


s_ds_info = { # [DatasetName, Production ID, PROC_XSEC[pb], XSEC_UNC[pb], XSEC_Accuracy]
"Signal": [
    ["Signal_400_100",      '10',   0.04477874562,       0,             "unknown"],
    ["Signal_400_200",      '10',   0.03395530581,       0,             "unknown"],
    ["Signal_500_100",      '10',   0.02094114758,       0,             "unknown"],
    ["Signal_500_200",      '10',   0.01931976527,       0,             "unknown"],
    ["Signal_500_300",      '10',   0.01275722776,       0,             "unknown"],
    ["Signal_600_100",      '10',   0.00995470304,       0,             "unknown"],
    ["Signal_600_200",      '10',   0.00981176831,       0,             "unknown"],
    ["Signal_600_300",      '10',   0.00861876644,       0,             "unknown"],
    ["Signal_600_400",      '10',   0.00531642372,       0,             "unknown"],
    ["Signal_800_100",      '10',   0.00260945689,       0,             "unknown"],
    ["Signal_800_200",      '10',   0.00267848046,       0,             "unknown"],
    ["Signal_800_300",      '10',   0.00259129377,       0,             "unknown"],
    ["Signal_800_400",      '10',   0.00239304034,       0,             "unknown"],
    ["Signal_800_600",      '10',   0.00118851440,       0,             "unknown"],
    ["Signal_1000_100",     '10',   0.00081577775,       0,             "unknown"],
    ["Signal_1000_200",     '10',   0.00083496561,       0,             "unknown"],
    ["Signal_1000_300",     '10',   0.00081513973,       0,             "unknown"],
    ["Signal_1000_400",     '10',   0.00080103474,       0,             "unknown"],
    ["Signal_1000_600",     '10',   0.00071681937,       0,             "unknown"],
    ["Signal_1000_800",     '10',   0.00034042023,       0,             "unknown"],
    ##["Signal_200_100",    '99',   0.04477874562,       0,             "temporary"],
    ##["Signal_400_300",    '99',   0.04477874562,       0,             "temporary"],
    ##["Signal_500_400",    '99',   0.04477874562,       0,             "temporary"],
    ##["Signal_600_500",    '99',   0.04477874562,       0,             "temporary"],
    ##["Signal_800_700",    '99',   0.04477874562,       0,             "temporary"],
    ##["Signal_1000_900",   '99',   0.04477874562,       0,             "temporary"],
    ["Signal_1400_100",     '99',   0.04477874562,       0,             "temporary"],
    ["Signal_1400_400",     '99',   0.04477874562,       0,             "temporary"],
    ["Signal_1400_600",     '99',   0.04477874562,       0,             "temporary"],
    ["Signal_1400_1000",    '99',   0.04477874562,       0,             "temporary"],
    ["Signal_1400_1200",    '99',   0.04477874562,       0,             "temporary"],
    ##["Signal_1400_1300",  '99',   0.04477874562,       0,             "temporary"],
    ["Signal_2000_100",     '99',   0.04477874562,       0,             "temporary"],
    ["Signal_2000_400",     '99',   0.04477874562,       0,             "temporary"],
    ["Signal_2000_600",     '99',   0.04477874562,       0,             "temporary"],
    ["Signal_2000_1000",    '99',   0.04477874562,       0,             "temporary"],
    ["Signal_2000_1200",    '99',   0.04477874562,       0,             "temporary"],
    ["Signal_2000_1800",    '99',   0.04477874562,       0,             "temporary"],
    ##["Signal_2000_1900",  '99',   0.04477874562,       0,             "temporary"],
],
}


#----------------------------------------------------------------------------------------
# [DO NOT TOUCH THIS PART]
#----------------------------------------------------------------------------------------
s_ds = {}
for period in paths.keys():
    
    dti = period[0]
    year = period[-2:]

    for key in s_ds_info.keys():
        s_ds[key+"_"+period] = []
        for ds in s_ds_info[key]:
            list_temp = []
            list_temp.append(ds[0]+"_"+period)
            list_temp.append(ds[1]+year+dti)
            list_temp.append(paths[period]+ds[0]+".txt")
            list_temp.append(ds[2])
            list_temp.append(ds[3])
            s_ds[key+"_"+period].append(list_temp)

