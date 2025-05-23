import os


def get_samples( basedir, period ):

    list_basedir = os.listdir(basedir)
    periodTag = period + "_files"

    samples = {
        'Signal_400_100':           [i for i in list_basedir if 'Signal_400_100' == i.split("_"+periodTag)[0]],
        'Signal_400_200':           [i for i in list_basedir if 'Signal_400_200' == i.split("_"+periodTag)[0]],
        'Signal_500_100':           [i for i in list_basedir if 'Signal_500_100' == i.split("_"+periodTag)[0]],
        'Signal_500_200':           [i for i in list_basedir if 'Signal_500_200' == i.split("_"+periodTag)[0]],
        'Signal_500_300':           [i for i in list_basedir if 'Signal_500_300' == i.split("_"+periodTag)[0]],
        'Signal_600_100':           [i for i in list_basedir if 'Signal_600_100' == i.split("_"+periodTag)[0]],
        'Signal_600_200':           [i for i in list_basedir if 'Signal_600_200' == i.split("_"+periodTag)[0]],
        'Signal_600_300':           [i for i in list_basedir if 'Signal_600_300' == i.split("_"+periodTag)[0]],
        'Signal_600_400':           [i for i in list_basedir if 'Signal_600_400' == i.split("_"+periodTag)[0]],
        'Signal_800_100':           [i for i in list_basedir if 'Signal_800_100' == i.split("_"+periodTag)[0]],
        'Signal_800_200':           [i for i in list_basedir if 'Signal_800_200' == i.split("_"+periodTag)[0]],
        'Signal_800_300':           [i for i in list_basedir if 'Signal_800_300' == i.split("_"+periodTag)[0]],
        'Signal_800_400':           [i for i in list_basedir if 'Signal_800_400' == i.split("_"+periodTag)[0]],
        'Signal_800_600':           [i for i in list_basedir if 'Signal_800_600' == i.split("_"+periodTag)[0]],
        'Signal_1000_100':          [i for i in list_basedir if 'Signal_1000_100' == i.split("_"+periodTag)[0]],
        'Signal_1000_200':          [i for i in list_basedir if 'Signal_1000_200' == i.split("_"+periodTag)[0]],
        'Signal_1000_300':          [i for i in list_basedir if 'Signal_1000_300' == i.split("_"+periodTag)[0]],
        'Signal_1000_400':          [i for i in list_basedir if 'Signal_1000_400' == i.split("_"+periodTag)[0]],
        'Signal_1000_600':          [i for i in list_basedir if 'Signal_1000_600' == i.split("_"+periodTag)[0]],
        'Signal_1000_800':          [i for i in list_basedir if 'Signal_1000_800' == i.split("_"+periodTag)[0]],
        'Signal_200_100':           [i for i in list_basedir if 'Signal_200_100' == i.split("_"+periodTag)[0]],
        'Signal_400_300':           [i for i in list_basedir if 'Signal_400_300' == i.split("_"+periodTag)[0]],
        'Signal_500_400':           [i for i in list_basedir if 'Signal_500_400' == i.split("_"+periodTag)[0]],
        'Signal_600_500':           [i for i in list_basedir if 'Signal_600_500' == i.split("_"+periodTag)[0]],
        'Signal_800_700':           [i for i in list_basedir if 'Signal_800_700' == i.split("_"+periodTag)[0]],
        'Signal_1000_900':          [i for i in list_basedir if 'Signal_1000_900' == i.split("_"+periodTag)[0]],
        'Signal_1400_100':          [i for i in list_basedir if 'Signal_1400_100' == i.split("_"+periodTag)[0]],
        'Signal_1400_400':          [i for i in list_basedir if 'Signal_1400_400' == i.split("_"+periodTag)[0]],
        'Signal_1400_600':          [i for i in list_basedir if 'Signal_1400_600' == i.split("_"+periodTag)[0]],
        'Signal_1400_1000':         [i for i in list_basedir if 'Signal_1400_1000' == i.split("_"+periodTag)[0]],
        'Signal_1400_1200':         [i for i in list_basedir if 'Signal_1400_1200' == i.split("_"+periodTag)[0]],
        'Signal_1400_1300':         [i for i in list_basedir if 'Signal_1400_1300' == i.split("_"+periodTag)[0]],
        'Signal_2000_100':          [i for i in list_basedir if 'Signal_2000_100' == i.split("_"+periodTag)[0]],
        'Signal_2000_400':          [i for i in list_basedir if 'Signal_2000_400' == i.split("_"+periodTag)[0]],
        'Signal_2000_600':          [i for i in list_basedir if 'Signal_2000_600' == i.split("_"+periodTag)[0]],
        'Signal_2000_1000':         [i for i in list_basedir if 'Signal_2000_1000' == i.split("_"+periodTag)[0]],
        'Signal_2000_1200':         [i for i in list_basedir if 'Signal_2000_1200' == i.split("_"+periodTag)[0]],
        'Signal_2000_1800':         [i for i in list_basedir if 'Signal_2000_1800' == i.split("_"+periodTag)[0]],
        'Signal_2000_1900':         [i for i in list_basedir if 'Signal_2000_1900' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_PtZ-0To50':     [i for i in list_basedir if 'DYJetsToLL_PtZ-0To50' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_PtZ-3To50':     [i for i in list_basedir if 'DYJetsToLL_PtZ-3To50' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_PtZ-50To100':   [i for i in list_basedir if 'DYJetsToLL_PtZ-50To100' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_PtZ-100To250':  [i for i in list_basedir if 'DYJetsToLL_PtZ-100To250' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_PtZ-250To400':  [i for i in list_basedir if 'DYJetsToLL_PtZ-250To400' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_PtZ-400To650':  [i for i in list_basedir if 'DYJetsToLL_PtZ-400To650' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_PtZ-650ToInf':  [i for i in list_basedir if 'DYJetsToLL_PtZ-650ToInf' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_Pt-0To3':       [i for i in list_basedir if 'DYJetsToLL_Pt-0To3' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_Pt-Inclusive':  [i for i in list_basedir if 'DYJetsToLL_Pt-Inclusive' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_Pt-50To100':    [i for i in list_basedir if 'DYJetsToLL_Pt-50To100' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_Pt-100To250':   [i for i in list_basedir if 'DYJetsToLL_Pt-100To250' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_Pt-250To400':   [i for i in list_basedir if 'DYJetsToLL_Pt-250To400' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_Pt-400To650':   [i for i in list_basedir if 'DYJetsToLL_Pt-400To650' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_Pt-650ToInf':   [i for i in list_basedir if 'DYJetsToLL_Pt-650ToInf' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_M-10to50':      [i for i in list_basedir if 'DYJetsToLL_M-10to50' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-Inclusive':  [i for i in list_basedir if 'DYJetsToLL_HT-Inclusive' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-70to100':    [i for i in list_basedir if 'DYJetsToLL_HT-70to100' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-100to200':   [i for i in list_basedir if 'DYJetsToLL_HT-100to200' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-200to400':   [i for i in list_basedir if 'DYJetsToLL_HT-200to400' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-400to600':   [i for i in list_basedir if 'DYJetsToLL_HT-400to600' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-600to800':   [i for i in list_basedir if 'DYJetsToLL_HT-600to800' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-800to1200':  [i for i in list_basedir if 'DYJetsToLL_HT-800to1200' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-1200to2500': [i for i in list_basedir if 'DYJetsToLL_HT-1200to2500' == i.split("_"+periodTag)[0]],
        'DYJetsToLL_HT-2500toInf':  [i for i in list_basedir if 'DYJetsToLL_HT-2500toInf' == i.split("_"+periodTag)[0]],
        'TTTo2L2Nu':                [i for i in list_basedir if 'TTTo2L2Nu' == i.split("_"+periodTag)[0]],
        'TTToSemiLeptonic':         [i for i in list_basedir if 'TTToSemiLeptonic' == i.split("_"+periodTag)[0]],
        'ST_tW_top':                [i for i in list_basedir if 'ST_tW_top' == i.split("_"+periodTag)[0]],
        'ST_tW_antitop':            [i for i in list_basedir if 'ST_tW_antitop' == i.split("_"+periodTag)[0]],
        'ST_t-channel_top':         [i for i in list_basedir if 'ST_t-channel_top' == i.split("_"+periodTag)[0]],
        'ST_t-channel_antitop':     [i for i in list_basedir if 'ST_t-channel_antitop' == i.split("_"+periodTag)[0]],
        'ST_s-channel':             [i for i in list_basedir if 'ST_s-channel' == i.split("_"+periodTag)[0]],
        'ZZ_Inclusive':             [i for i in list_basedir if 'ZZ_Inclusive' == i.split("_"+periodTag)[0]],
        'ZZ_Others':                [i for i in list_basedir if 'ZZ_Others' == i.split("_"+periodTag)[0]],
        'ZZTo2L2Nu':                [i for i in list_basedir if 'ZZTo2L2Nu' == i.split("_"+periodTag)[0]],
        'ZZTo4L':                   [i for i in list_basedir if 'ZZTo4L' == i.split("_"+periodTag)[0]],
        'ZZTo2Q2L':                 [i for i in list_basedir if 'ZZTo2Q2L' == i.split("_"+periodTag)[0]],
        'WZ_Inclusive':             [i for i in list_basedir if 'WZ_Inclusive' == i.split("_"+periodTag)[0]],
        'WZ_Others':                [i for i in list_basedir if 'WZ_Others' == i.split("_"+periodTag)[0]],
        'WZTo3LNu':                 [i for i in list_basedir if 'WZTo3LNu' == i.split("_"+periodTag)[0]],
        'WZTo2Q2L':                 [i for i in list_basedir if 'WZTo2Q2L' == i.split("_"+periodTag)[0]],
        'WW':                       [i for i in list_basedir if 'WW' == i.split("_"+periodTag)[0]],
        'WWTo2L2Nu':                [i for i in list_basedir if 'WWTo2L2Nu' == i.split("_"+periodTag)[0]],
        'WZZ':                      [i for i in list_basedir if 'WZZ' == i.split("_"+periodTag)[0]],
        'WWZ':                      [i for i in list_basedir if 'WWZ' == i.split("_"+periodTag)[0]],
        'ZZZ':                      [i for i in list_basedir if 'ZZZ' == i.split("_"+periodTag)[0]],
        'WWW':                      [i for i in list_basedir if 'WWW' == i.split("_"+periodTag)[0]],
        'TTWW':                     [i for i in list_basedir if 'TTWW' == i.split("_"+periodTag)[0]],
        'TTWZ':                     [i for i in list_basedir if 'TTWZ' == i.split("_"+periodTag)[0]],
        'TTZZ':                     [i for i in list_basedir if 'TTZZ' == i.split("_"+periodTag)[0]],
        'TWZToLL_thad_Wlept':       [i for i in list_basedir if 'TWZToLL_thad_Wlept' == i.split("_"+periodTag)[0]],
        'TWZToLL_tlept_Whad':       [i for i in list_basedir if 'TWZToLL_tlept_Whad' == i.split("_"+periodTag)[0]],
        'TWZToLL_tlept_Wlept':      [i for i in list_basedir if 'TWZToLL_tlept_Wlept' == i.split("_"+periodTag)[0]],
        'TTWJetsToLNu':             [i for i in list_basedir if 'TTWJetsToLNu' == i.split("_"+periodTag)[0]],
        'TTWJetsToQQ':              [i for i in list_basedir if 'TTWJetsToQQ' == i.split("_"+periodTag)[0]],
        'TTZToQQ':                  [i for i in list_basedir if 'TTZToQQ' == i.split("_"+periodTag)[0]],
        'TTZToLL':                  [i for i in list_basedir if 'TTZToLL' == i.split("_"+periodTag)[0]],
        'TTZToNuNu':                [i for i in list_basedir if 'TTZToNuNu' == i.split("_"+periodTag)[0]],
        'tZq_ll':                   [i for i in list_basedir if 'tZq_ll' == i.split("_"+periodTag)[0]],
        'ttH_HToZZ':                [i for i in list_basedir if 'ttH_HToZZ' == i.split("_"+periodTag)[0]],
        'ttHTobb':                  [i for i in list_basedir if 'ttHTobb' == i.split("_"+periodTag)[0]],
        'ttHToTauTau':              [i for i in list_basedir if 'ttHToTauTau' == i.split("_"+periodTag)[0]],
        'GluGluHToWWTo2L2Nu':       [i for i in list_basedir if 'GluGluHToWWTo2L2Nu' == i.split("_"+periodTag)[0]],
        'GluGluHToZZTo2L2Q':        [i for i in list_basedir if 'GluGluHToZZTo2L2Q' == i.split("_"+periodTag)[0]],
        'GluGluHToZZTo4L':          [i for i in list_basedir if 'GluGluHToZZTo4L' == i.split("_"+periodTag)[0]],
        'WplusH_HToZZTo2L2X':       [i for i in list_basedir if 'WplusH_HToZZTo2L2X' == i.split("_"+periodTag)[0]],
        'WplusH_HToZZTo4L':         [i for i in list_basedir if 'WplusH_HToZZTo4L' == i.split("_"+periodTag)[0]],
        'WminusH_HToZZTo2L2X':      [i for i in list_basedir if 'WminusH_HToZZTo2L2X' == i.split("_"+periodTag)[0]],
        'WminusH_HToZZTo4L':        [i for i in list_basedir if 'WminusH_HToZZTo4L' == i.split("_"+periodTag)[0]],
        'ZH_HToBB_ZToLL':           [i for i in list_basedir if 'ZH_HToBB_ZToLL' == i.split("_"+periodTag)[0]],
        'ZH_HToZZ':                 [i for i in list_basedir if 'ZH_HToZZ' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-15To20':         [i for i in list_basedir if 'QCD_Mu_Pt-15To20' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-20To30':         [i for i in list_basedir if 'QCD_Mu_Pt-20To30' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-30To50':         [i for i in list_basedir if 'QCD_Mu_Pt-30To50' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-50To80':         [i for i in list_basedir if 'QCD_Mu_Pt-50To80' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-80To120':        [i for i in list_basedir if 'QCD_Mu_Pt-80To120' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-120To170':       [i for i in list_basedir if 'QCD_Mu_Pt-120To170' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-170To300':       [i for i in list_basedir if 'QCD_Mu_Pt-170To300' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-300To470':       [i for i in list_basedir if 'QCD_Mu_Pt-300To470' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-470To600':       [i for i in list_basedir if 'QCD_Mu_Pt-470To600' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-600To800':       [i for i in list_basedir if 'QCD_Mu_Pt-600To800' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-800To1000':      [i for i in list_basedir if 'QCD_Mu_Pt-800To1000' == i.split("_"+periodTag)[0]],
        'QCD_Mu_Pt-1000ToInf':      [i for i in list_basedir if 'QCD_Mu_Pt-1000ToInf' == i.split("_"+periodTag)[0]],
        'WGToLNuG':                 [i for i in list_basedir if 'WGToLNuG' == i.split("_"+periodTag)[0]],
        'QCD_Pt_15to30':            [i for i in list_basedir if 'QCD_Pt_15to30' == i.split("_"+periodTag)[0]],
        'QCD_Pt_30to50':            [i for i in list_basedir if 'QCD_Pt_30to50' == i.split("_"+periodTag)[0]],
        'QCD_Pt_50to80':            [i for i in list_basedir if 'QCD_Pt_50to80' == i.split("_"+periodTag)[0]],
        'QCD_Pt_80to120':           [i for i in list_basedir if 'QCD_Pt_80to120' == i.split("_"+periodTag)[0]],
        'QCD_Pt_120to170':          [i for i in list_basedir if 'QCD_Pt_120to170' == i.split("_"+periodTag)[0]],
        'QCD_Pt_170to300':          [i for i in list_basedir if 'QCD_Pt_170to300' == i.split("_"+periodTag)[0]],
        'QCD_Pt_300to470':          [i for i in list_basedir if 'QCD_Pt_300to470' == i.split("_"+periodTag)[0]],
        'QCD_Pt_470to600':          [i for i in list_basedir if 'QCD_Pt_470to600' == i.split("_"+periodTag)[0]],
        'QCD_Pt_600to800':          [i for i in list_basedir if 'QCD_Pt_600to800' == i.split("_"+periodTag)[0]],
        'QCD_Pt_800to1000':         [i for i in list_basedir if 'QCD_Pt_800to1000' == i.split("_"+periodTag)[0]],
        'QCD_Pt_1000to1400':        [i for i in list_basedir if 'QCD_Pt_1000to1400' == i.split("_"+periodTag)[0]],
        'QCD_Pt_1400to1800':        [i for i in list_basedir if 'QCD_Pt_1400to1800' == i.split("_"+periodTag)[0]],
        'QCD_Pt_1800to2400':        [i for i in list_basedir if 'QCD_Pt_1800to2400' == i.split("_"+periodTag)[0]],
        'QCD_Pt_2400to3200':        [i for i in list_basedir if 'QCD_Pt_2400to3200' == i.split("_"+periodTag)[0]],
        'QCD_Pt_3200toInf':         [i for i in list_basedir if 'QCD_Pt_3200toInf' == i.split("_"+periodTag)[0]],
        'TTToHadronic':             [i for i in list_basedir if 'TTToHadronic' == i.split("_"+periodTag)[0]],
        'ZH_HToBB_ZToNuNu':         [i for i in list_basedir if 'ZH_HToBB_ZToNuNu' == i.split("_"+periodTag)[0]],
        'ZZTo2Q2Nu':                [i for i in list_basedir if 'ZZTo2Q2Nu' == i.split("_"+periodTag)[0]],
        'QCD_B_HT100to200':         [i for i in list_basedir if 'QCD_B_HT100to200' == i.split("_"+periodTag)[0]],
        'QCD_B_HT200to300':         [i for i in list_basedir if 'QCD_B_HT200to300' == i.split("_"+periodTag)[0]],
        'QCD_B_HT300to500':         [i for i in list_basedir if 'QCD_B_HT300to500' == i.split("_"+periodTag)[0]],
        'QCD_B_HT500to700':         [i for i in list_basedir if 'QCD_B_HT500to700' == i.split("_"+periodTag)[0]],
        'QCD_B_HT700to1000':        [i for i in list_basedir if 'QCD_B_HT700to1000' == i.split("_"+periodTag)[0]],
        'QCD_B_HT1000to1500':       [i for i in list_basedir if 'QCD_B_HT1000to1500' == i.split("_"+periodTag)[0]],
        'QCD_B_HT1500to2000':       [i for i in list_basedir if 'QCD_B_HT1500to2000' == i.split("_"+periodTag)[0]],
        'QCD_B_HT2000toInf':        [i for i in list_basedir if 'QCD_B_HT2000toInf' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_Pt-Inclusive':  [i for i in list_basedir if 'WJetsToLNu_Pt-Inclusive' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_Pt-100To250':   [i for i in list_basedir if 'WJetsToLNu_Pt-100To250' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_Pt-250To400':   [i for i in list_basedir if 'WJetsToLNu_Pt-250To400' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_Pt-400To600':   [i for i in list_basedir if 'WJetsToLNu_Pt-400To600' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_Pt-600ToInf':   [i for i in list_basedir if 'WJetsToLNu_Pt-600ToInf' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_0J':            [i for i in list_basedir if 'WJetsToLNu_0J' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_1J':            [i for i in list_basedir if 'WJetsToLNu_1J' == i.split("_"+periodTag)[0]],
        'WJetsToLNu_2J':            [i for i in list_basedir if 'WJetsToLNu_2J' == i.split("_"+periodTag)[0]],
        'WHJet_HToBB_WToLNu':       [i for i in list_basedir if 'WHJet_HToBB_WToLNu' == i.split("_"+periodTag)[0]],
        'QCD_HT50to100':            [i for i in list_basedir if 'QCD_HT50to100' == i.split("_"+periodTag)[0]],
        'QCD_HT100to200':           [i for i in list_basedir if 'QCD_HT100to200' == i.split("_"+periodTag)[0]],
        'QCD_HT200to300':           [i for i in list_basedir if 'QCD_HT200to300' == i.split("_"+periodTag)[0]],
        'QCD_HT300to500':           [i for i in list_basedir if 'QCD_HT300to500' == i.split("_"+periodTag)[0]],
        'QCD_HT500to700':           [i for i in list_basedir if 'QCD_HT500to700' == i.split("_"+periodTag)[0]],
        'QCD_HT700to1000':          [i for i in list_basedir if 'QCD_HT700to1000' == i.split("_"+periodTag)[0]],
        'QCD_HT1000to1500':         [i for i in list_basedir if 'QCD_HT1000to1500' == i.split("_"+periodTag)[0]],
        'QCD_HT1500to2000':         [i for i in list_basedir if 'QCD_HT1500to2000' == i.split("_"+periodTag)[0]],
        'QCD_HT2000toInf':          [i for i in list_basedir if 'QCD_HT2000toInf' == i.split("_"+periodTag)[0]],
        'Z1JetsToNuNu_PtZ-150To250':   [i for i in list_basedir if 'Z1JetsToNuNu_PtZ-150To250' == i.split("_"+periodTag)[0]],
        'Z1JetsToNuNu_PtZ-250To400':   [i for i in list_basedir if 'Z1JetsToNuNu_PtZ-250To400' == i.split("_"+periodTag)[0]],
        'Z1JetsToNuNu_PtZ-400ToInf':   [i for i in list_basedir if 'Z1JetsToNuNu_PtZ-400ToInf' == i.split("_"+periodTag)[0]],
        'Z1JetsToNuNu_PtZ-50To150':    [i for i in list_basedir if 'Z1JetsToNuNu_PtZ-50To150' == i.split("_"+periodTag)[0]],
        'Z2JetsToNuNu_PtZ-150To250':   [i for i in list_basedir if 'Z2JetsToNuNu_PtZ-150To250' == i.split("_"+periodTag)[0]],
        'Z2JetsToNuNu_PtZ-250To400':   [i for i in list_basedir if 'Z2JetsToNuNu_PtZ-250To400' == i.split("_"+periodTag)[0]],
        'Z2JetsToNuNu_PtZ-400ToInf':   [i for i in list_basedir if 'Z2JetsToNuNu_PtZ-400ToInf' == i.split("_"+periodTag)[0]],
        'Z2JetsToNuNu_PtZ-50To150':    [i for i in list_basedir if 'Z2JetsToNuNu_PtZ-50To150' == i.split("_"+periodTag)[0]],
        'ggToZZTo2e2mu':            [i for i in list_basedir if 'ggToZZTo2e2mu' == i.split("_"+periodTag)[0]],
        'ggToZZTo2e2nu':            [i for i in list_basedir if 'ggToZZTo2e2nu' == i.split("_"+periodTag)[0]],
        'ggToZZTo2e2tau':           [i for i in list_basedir if 'ggToZZTo2e2tau' == i.split("_"+periodTag)[0]],
        'ggToZZTo2mu2nu':           [i for i in list_basedir if 'ggToZZTo2mu2nu' == i.split("_"+periodTag)[0]],
        'ggToZZTo2mu2tau':          [i for i in list_basedir if 'ggToZZTo2mu2tau' == i.split("_"+periodTag)[0]],
        'ggToZZTo4e':               [i for i in list_basedir if 'ggToZZTo4e' == i.split("_"+periodTag)[0]],
        'ggToZZTo4mu':              [i for i in list_basedir if 'ggToZZTo4mu' == i.split("_"+periodTag)[0]],
        'ggToZZTo4tau':             [i for i in list_basedir if 'ggToZZTo4tau' == i.split("_"+periodTag)[0]],
        'Data_A':                   [i for i in list_basedir if 'Data' in i and '_A_' in i and periodTag in i],
        'Data_B':                   [i for i in list_basedir if 'Data' in i and '_B_' in i and periodTag in i],
        'Data_C':                   [i for i in list_basedir if 'Data' in i and '_C_' in i and periodTag in i],
        'Data_D':                   [i for i in list_basedir if 'Data' in i and '_D_' in i and periodTag in i],
        'Data_E':                   [i for i in list_basedir if 'Data' in i and '_E_' in i and periodTag in i],
        'Data_F':                   [i for i in list_basedir if 'Data' in i and '_F_' in i and periodTag in i],
        'Data_G':                   [i for i in list_basedir if 'Data' in i and '_G_' in i and periodTag in i],
        'Data_H':                   [i for i in list_basedir if 'Data' in i and '_H_' in i and periodTag in i],
    }
    
    empty_samples = []
    for sample in samples:
        if len(samples[sample]) == 0:
            empty_samples.append(sample)

    for sample in empty_samples:
        del samples[sample]

    return samples


