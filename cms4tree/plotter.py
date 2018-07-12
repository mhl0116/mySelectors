from ROOT import *
from tdrStyle import *
setTDRStyle()
from subprocess import call

def GetAndPlot(filename,histname,savepath,xTitle,doLog=False):
    c = ROOT.TCanvas("c","",800,800)
    if doLog: c.SetLogy()   
    f = ROOT.TFile.Open(filename)
    h = f.Get(histname)
    if doLog: h.SetMinimum(1)
    h.GetXaxis().SetTitle(xTitle)
    h.GetXaxis().SetTitleSize(0.05)
    h.SetFillColor(ROOT.TColor.GetColor("#FFDF0A"))
    ROOT.gStyle.SetOptStat(100000)
    h.Draw("HIST")
    #print h.Integral()
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
    latex.DrawLatex(0.50, 0.75, "nEntries: " + str(round(h.GetEntries(),2) ) )
    latex.DrawLatex(0.50, 0.7, "Mean: " + str(round(h.GetMean(),2) ) )
    latex.DrawLatex(0.50, 0.65, "StdDev: " + str(round(h.GetStdDev(),2) ) )

    call("chmod -R 755 " + savepath, shell=True)
    call("cp /home/users/hmei/public_html/backup/index.php " + savepath, shell=True)
    call("chmod -R 755 " + savepath, shell=True)

    c.SaveAs(savepath+histname+".png")
    c.SaveAs(savepath+histname+".pdf")

def GetAndPlot2D(filename,histname,savepath,xTitle,yTitle):
    c = ROOT.TCanvas("c","",800,800)
    c.SetRightMargin(0.15)
    f = ROOT.TFile.Open(filename)
    h = f.Get(histname)
    h.GetXaxis().SetTitle(xTitle)
    h.GetYaxis().SetTitle(yTitle)
    h.Draw("COLZ")
    #print h.Integral()
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
    latex.DrawLatex(0.50, 0.75, "nEntries: " + str(round(h.GetEntries(),2) ) )
    latex.DrawLatex(0.50, 0.7, "Mean: " + str(round(h.GetMean(),2) ) )
    latex.DrawLatex(0.50, 0.65, "StdDev: " + str(round(h.GetStdDev(),2) ) )

    call("cp /home/users/hmei/public_html/backup/index.php " + savepath, shell=True)
    call("chmod -R 755 " + savepath, shell=True)
    c.SaveAs(savepath+histname+".png")
    c.SaveAs(savepath+histname+".pdf")

filename = "tmp.root"
savepath = "/home/users/hmei/public_html/2018/20180711_genPhotonCheck/"
histnames = [\
"nPhoton_prompt","nPhoton_hard",
"nLepton_prompt","nLepton_hard",
"nHadron_prompt","nHadron_hard",
"pTPhoton_prompt","pTPhoton_hard",
"etaPhoton_prompt","etaPhoton_hard",
#"isoPhoton_prompt","isoPhoton_hard",
"phiPhoton_prompt","phiPhoton_hard",
"minDrPhoton_prompt","minDrPhoton_hard",
"minDrPhoton_passFrix_prompt","minDrPhoton_passFrix_hard",
"hardId_lep","hardId_had",
"minDrPhoton_hard_u","minDrPhoton_hard_d","minDrPhoton_hard_c",
"minDrPhoton_hard_s","minDrPhoton_hard_t","minDrPhoton_hard_b",
"minDrPhoton_hard_g",
"lep_simpleGrandMa","had_simpleGrandMa"
]

for histname in histnames:
    if "iso" in histname:
       GetAndPlot(filename, histname, savepath, histname, True)
    else:
       GetAndPlot(filename, histname, savepath, histname)

histnames = ['nPrompt_nPromptNotHard','nHardLep_nHardHad','hadId_hadMomId_hard']
xTitles = ['nPromptPhoton','nHardLep','id_had_hard']
yTitles = ['nPromptPhoton_notHard','nHardHad','id_hadMom_hard']

for i in range(len(histnames)):
    histname = histnames[i]
    xTitle = xTitles[i]
    yTitle = yTitles[i]
    GetAndPlot2D(filename, histname, savepath, xTitle, yTitle)
