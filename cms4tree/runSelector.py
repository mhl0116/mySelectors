import ROOT
import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-t', dest='tag', type=str, help='for each plot')
    parser.add_argument('--doME11', dest='doME11', action='store_true', default=False)
    parser.add_argument('--inputfile', dest='inputfile', type=str, help='inputfile')

    args = parser.parse_args()
    return args

args=ParseOption()

#filename = "DUMMYFILENAME_UF.root"
#filename = "DUMMYFILENAME_RU.root"
tag = args.tag
doME11 = args.doME11
#tag = "RU"
chamberType = ""

#filename = "DUMMYFILENAME_"+tag+".root"
filename = args.inputfile

#tag += chamberType

#treename = "cscRootMaker/Events"
treename = "Events"
selectorname = "cms4GenSelector.C"

savedir = "DUMMY" #"/home/mhl/public_html/2017/20171017_cscSeg/"

#f_ = ROOT.TFile.Open("/hadoop/cms/store/group/snt/run2_mc2017//TTGamma_SingleLeptFromT_TuneCP5_PSweights_13TeV_madgraph_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_CMS4_V09-04-13/merged_ntuple_1.root")
#f_ = ROOT.TFile.Open("/hadoop/cms/store/group/snt/run2_moriond17/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/V08-00-16/merged_ntuple_1.root")
f_ = ROOT.TFile.Open("/hadoop/cms/store/user/namin/run2_moriond17_cms4/ProjectMetis/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_2017Sep27/merged_ntuple_1.root")
t_ = f_.Get(treename)
n_ = t_.GetEntries()
s_ = ROOT.TSelector.GetSelector(selectorname)
#s_.SetInputs(n_,tag)
t_.Process(s_)
