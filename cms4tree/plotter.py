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
    h.Draw()
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
savepath = "/home/users/hmei/public_html/2018/20180709_genPhotonCheck/"
histnames = [\
"nPhoton_s1","nPhoton_prompt","nPhoton_hard",
"nLepton_s1","nLepton_prompt","nLepton_hard",
"nHadron_s1","nHadron_prompt","nHadron_hard",
"pTPhoton_s1","pTPhoton_prompt","pTPhoton_hard","pTPhoton_s22_23",
"etaPhoton_s1","etaPhoton_prompt","etaPhoton_hard","etaPhoton_s22_23",
"isoPhoton_s1","isoPhoton_prompt","isoPhoton_hard","isoPhoton_s22_23",
"phiPhoton_s1","phiPhoton_prompt","phiPhoton_hard","phiPhoton_s22_23",
"minDrPhoton_s1","minDrPhoton_prompt","minDrPhoton_hard","minDrPhoton_s22_23",
"minDrPhoton_passFrix_s1","minDrPhoton_passFrix_prompt","minDrPhoton_passFrix_hard","minDrPhoton_passFrix_s22_23",
"nPhoton_s22_23","nLepton_s22_23","nHadron_s22_23",
"isoPhoton_passFrix_s1","isoPhoton_passFrix_prompt","isoPhoton_passFrix_hard","isoPhoton_passFrix_s22_23",
"hardId_lep","hardId_had"
]

for histname in histnames:
    if "iso" in histname:
       GetAndPlot(filename, histname, savepath, histname, True)
    else:
       GetAndPlot(filename, histname, savepath, histname)

histnames = ['nPrompt_nPromptNotHard','nHardLep_nHardHad']
xTitles = ['nPromptPhoton','nHardLep']
yTitles = ['nPromptPhoton_notHard','nHardHad']

for i in range(len(histnames)):
    histname = histnames[i]
    xTitle = xTitles[i]
    yTitle = yTitles[i]
    GetAndPlot2D(filename, histname, savepath, xTitle, yTitle)
