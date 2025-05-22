analysis = "AP_bbZDM_Lep_R2"
nano_version = 'v9'
path_0_16 = analysis+'/Datasets/Files/signal_16/dti_0/'+nano_version+'/'
path_1_16 = analysis+'/Datasets/Files/signal_16/dti_1/'+nano_version+'/'
path_0_17 = analysis+'/Datasets/Files/signal_17/dti_0/'+nano_version+'/'
path_0_18 = analysis+'/Datasets/Files/signal_18/dti_0/'+nano_version+'/'

#----------------------------------------------------------------------------------------
# ID digits:
# 1st-2nd = 16(2016),17(2017),18(2018)                      # Year
# 3th-4th = 00(Data),01(MC-signal),02-13(MC-bkg)            # Group
# 5th-6th = 00(none),...                                    # Bkg -> Process
# 5th-6th = 00(none),11(250_30),12(250_40),55(1250_100)     # Signal -> Signal point
# 5th-6th = 00(none),01(A),02(B),03(C)                      # Data -> Era
# 7th     = 0,1,2,...                                       # Data taking interval (DTI)

# 2016 DTIs = 0(with "HIPM"/"APV")("pre-VFP"), 1(without "HIPM"/"APV")("pos-VFP")
#----------------------------------------------------------------------------------------


periods = ["0_16", "1_16", "0_17", "0_18"]
paths = [path_0_16, path_1_16, path_0_17, path_0_18]

for period,path in zip(periods,paths):
    
    dti = period[0]
    year = period[-2:]
        
    Signal = [
        ["Signal_400_100_"+period]           + [year+'0100'+dti, path+"Signal_400_100.txt"],
        ["Signal_400_200_"+period]           + [year+'0101'+dti, path+"Signal_400_200.txt"],
        ["Signal_500_100_"+period]           + [year+'0102'+dti, path+"Signal_500_100.txt"],
        ["Signal_500_200_"+period]           + [year+'0103'+dti, path+"Signal_500_200.txt"],
        ["Signal_500_300_"+period]           + [year+'0104'+dti, path+"Signal_500_300.txt"],
        ["Signal_600_100_"+period]           + [year+'0105'+dti, path+"Signal_600_100.txt"],
        ["Signal_600_200_"+period]           + [year+'0106'+dti, path+"Signal_600_200.txt"],
        ["Signal_600_300_"+period]           + [year+'0107'+dti, path+"Signal_600_300.txt"],
        ["Signal_600_400_"+period]           + [year+'0108'+dti, path+"Signal_600_400.txt"],
        ["Signal_800_100_"+period]           + [year+'0109'+dti, path+"Signal_800_100.txt"],
        ["Signal_800_200_"+period]           + [year+'0110'+dti, path+"Signal_800_200.txt"],
        ["Signal_800_300_"+period]           + [year+'0111'+dti, path+"Signal_800_300.txt"],
        ["Signal_800_400_"+period]           + [year+'0112'+dti, path+"Signal_800_400.txt"],
        ["Signal_800_600_"+period]           + [year+'0113'+dti, path+"Signal_800_600.txt"],
        ["Signal_1000_100_"+period]          + [year+'0114'+dti, path+"Signal_1000_100.txt"],
        ["Signal_1000_200_"+period]          + [year+'0115'+dti, path+"Signal_1000_200.txt"],
        ["Signal_1000_300_"+period]          + [year+'0116'+dti, path+"Signal_1000_300.txt"],
        ["Signal_1000_400_"+period]          + [year+'0117'+dti, path+"Signal_1000_400.txt"],
        ["Signal_1000_600_"+period]          + [year+'0118'+dti, path+"Signal_1000_600.txt"],
        ["Signal_1000_800_"+period]          + [year+'0119'+dti, path+"Signal_1000_800.txt"],
        # New signal points
        ##["Signal_200_100_"+period]           + [year+'9920'+dti, path+"Signal_200_100.txt"],
        ##["Signal_400_300_"+period]           + [year+'9921'+dti, path+"Signal_400_300.txt"],
        ##["Signal_500_400_"+period]           + [year+'9922'+dti, path+"Signal_500_400.txt"],
        ##["Signal_600_500_"+period]           + [year+'9923'+dti, path+"Signal_600_500.txt"],
        ##["Signal_800_700_"+period]           + [year+'9924'+dti, path+"Signal_800_700.txt"],
        ##["Signal_1000_900_"+period]          + [year+'9925'+dti, path+"Signal_1000_900.txt"],
        ["Signal_1400_100_"+period]          + [year+'9926'+dti, path+"Signal_1400_100.txt"],
        ["Signal_1400_400_"+period]          + [year+'9927'+dti, path+"Signal_1400_400.txt"],
        ["Signal_1400_600_"+period]          + [year+'9928'+dti, path+"Signal_1400_600.txt"],
        ["Signal_1400_1000_"+period]         + [year+'9929'+dti, path+"Signal_1400_1000.txt"],
        ["Signal_1400_1200_"+period]         + [year+'9930'+dti, path+"Signal_1400_1200.txt"],
        ##["Signal_1400_1300_"+period]         + [year+'9931'+dti, path+"Signal_1400_1300.txt"],
        ["Signal_2000_100_"+period]          + [year+'9932'+dti, path+"Signal_2000_100.txt"],
        ["Signal_2000_400_"+period]          + [year+'9933'+dti, path+"Signal_2000_400.txt"],
        ["Signal_2000_600_"+period]          + [year+'9934'+dti, path+"Signal_2000_600.txt"],
        ["Signal_2000_1000_"+period]         + [year+'9935'+dti, path+"Signal_2000_1000.txt"],
        ["Signal_2000_1200_"+period]         + [year+'9936'+dti, path+"Signal_2000_1200.txt"],
        ["Signal_2000_1800_"+period]         + [year+'9937'+dti, path+"Signal_2000_1800.txt"],
        ##["Signal_2000_1900_"+period]         + [year+'9938'+dti, path+"Signal_2000_1900.txt"],
    ]

    #if year == "18":
    #    Signal.append(["Signal_1000_600_"+period]          + [year+'9918'+dti, path+"Signal_1000_600.txt"])
    #else:
    #    Signal.append(["Signal_1000_600_"+period]          + [year+'0118'+dti, path+"Signal_1000_600.txt"])
        

    if period == "0_16":
        Signal_0_16 = Signal
    elif period == "1_16":
        Signal_1_16 = Signal
    elif period == "0_17":
        Signal_0_17 = Signal
    elif period == "0_18":
        Signal_0_18 = Signal
