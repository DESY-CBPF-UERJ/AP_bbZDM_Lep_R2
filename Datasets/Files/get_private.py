import os
import sys
import argparse

#find . -type d -print

#======SETUP=======================================================================================
parser = argparse.ArgumentParser()
parser.add_argument("-v", "--version")
parser.add_argument("-p", "--period")
args = parser.parse_args()


basedir = "signal_"+args.period[-2:]+"/dti_"+args.period[0]+"/v"+args.version+"/"
if os.path.isdir(basedir) is False:
    os.makedirs(basedir)


datasets = [
["Signal_1400_1000",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-1400_Ma-1000_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_1400_100",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-1400_Ma-100_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_1400_1200",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-1400_Ma-1200_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_1400_400",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-1400_Ma-400_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_1400_600",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-1400_Ma-600_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_2000_1000",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-2000_Ma-1000_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_2000_100",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-2000_Ma-100_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_2000_1200",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-2000_Ma-1200_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_2000_1800",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-2000_Ma-1800_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_2000_400",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-2000_Ma-400_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
["Signal_2000_600",    "/eos/user/d/dperezad/Store/Samples/NANOAODSIM/2017/bbHToZaToLLChiChi_2HDMa_MH-2000_Ma-600_MChi-45_TuneCP5_13TeV_madgraph-pythia8"],
]


for i in range(len(datasets)):
    file_out = basedir + datasets[i][0] + ".txt"

    files_list = os.listdir(datasets[i][1])

    with open(file_out, "w") as file:
        for input_file in files_list:
            file.write(os.path.join(datasets[i][1],input_file+"\n"))

