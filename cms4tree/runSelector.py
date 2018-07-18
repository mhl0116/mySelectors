import ROOT
import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-t', dest='tag', type=str, help='for each plot')
    parser.add_argument('-n', dest='inTag', type=str, default='1')

    args = parser.parse_args()
    return args

args=ParseOption()

#filename = "DUMMYFILENAME_UF.root"
#filename = "DUMMYFILENAME_RU.root"
tag = args.tag
inTag = args.inTag

#tag = "RU"

#filename = "DUMMYFILENAME_"+tag+".root"
#filename = args.inputfile

#tag += chamberType

#treename = "cscRootMaker/Events"
treename = "Events"
selectorname = "cms4GenSelector.C"

savedir = "DUMMY" #"/home/mhl/public_html/2017/20171017_cscSeg/"

if tag == "ttg":
   f_ = ROOT.TFile.Open("/hadoop/cms/store/user/namin/run2_moriond17_cms4/ProjectMetis/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_CMS4_V00-00-02_2017Sep27/merged_ntuple_"+inTag+".root")
elif tag == "ttgg":
   f_ = ROOT.TFile.Open("/hadoop/cms/store/user/namin/run2_moriond17_cms4/ProjectMetis/TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_CMS4_V00-00-02_2017Sep27/merged_ntuple_"+inTag+".root")
t_ = f_.Get(treename)
n_ = t_.GetEntries()
s_ = ROOT.TSelector.GetSelector(selectorname)
s_.SetTag(tag,inTag)
t_.Process(s_)
