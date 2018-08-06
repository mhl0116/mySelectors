from ROOT import *
from tdrStyle import *
setTDRStyle()
from subprocess import call
import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-t', dest='tag', type=str, help='for each plot')
    parser.add_argument('--doME11', dest='doME11', action='store_true', default=False)
    parser.add_argument('--inputfile', dest='inputfile', type=str, help='inputfile')

    args = parser.parse_args()
    return args

def GetAndPlotOverlay(filename1,filename2,leg1,leg2,histname1,histname2,savepath,savename,xTitle,doLog=False):
    c = ROOT.TCanvas("c","",800,800)
    c.SetLeftMargin(0.2)
    if doLog: c.SetLogy()
    f1 = ROOT.TFile.Open(filename1)
    h1 = f1.Get(histname1)
    f2 = ROOT.TFile.Open(filename2)
    h2 = f2.Get(histname2)

    nEntry1 = h1.Integral() - h1.GetBinContent(h1.GetNbinsX()+1)  
    nEntry2 = h2.Integral() - h2.GetBinContent(h2.GetNbinsX()+1)

    print nEntry1, nEntry2

    if h1.Integral() > 0: h1.Scale(3.819/5)
    if h2.Integral() > 0: h2.Scale(0.01731)
#    if h1.Integral() > 0: h1.Scale(1/nEntry1)#3.819/nEntry1*36.9*1000)
#    if h2.Integral() > 0: h2.Scale(1/nEntry2)#*36.9*1000)
    h1.Rebin(2)
    h2.Rebin(2)
    dummy = ROOT.TH1D("dummy","dummy",1,h1.GetBinLowEdge(1),h1.GetBinLowEdge(h1.GetNbinsX())+h1.GetBinWidth(1))
    if doLog: c.SetLogy()
    dummy.SetMinimum(0)
    if doLog: dummy.SetMinimum(1)
    dummy.SetMaximum(1.5*max(h1.GetMaximum(),h2.GetMaximum()))
    dummy.SetLineColor(0)
    dummy.SetMarkerColor(0)
    dummy.SetLineWidth(0)
    dummy.SetMarkerSize(0)
    dummy.GetYaxis().SetTitle("Events/"+str(h1.GetBinWidth(1)))
    dummy.GetYaxis().SetTitleOffset(1.4)
    dummy.GetXaxis().SetTitle(xTitle)
    dummy.GetXaxis().SetTitleSize(0.05)
    dummy.Draw()

    h1.Draw("hist same")
    h1.SetLineWidth(3)
    h1.SetLineStyle(2)
    h1.SetLineColor(ROOT.TColor.GetColor("#CA4300"))
    h2.Draw("hist same")
    h2.SetLineWidth(3)
    h2.SetLineStyle(1)
    h2.SetLineColor(ROOT.TColor.GetColor("#0641B7"))

    #h.SetFillColor(ROOT.TColor.GetColor("#FFDF0A"))
    #ROOT.gStyle.SetOptStat(100000)
    #h.Draw("HIST")
    #print h.Integral()

    legend = ROOT.TLegend(0.4,0.75,0.93,0.9)
    legend.AddEntry(h1,leg1+" (nEntries: " + str(nEntry1) + ")","l")
    legend.AddEntry(h2,leg2+" (nEntries: " + str(nEntry2) + ")","l")
    legend.SetTextSize(0.04)
    legend.SetLineWidth(1)
    legend.SetFillColor(0)
    legend.SetBorderSize(1)
    legend.Draw("same")

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
#    latex.DrawLatex(0.50, 0.75, "nEntries: " + str(round(h.GetEntries(),2) ) )
#    latex.DrawLatex(0.50, 0.7, "Mean: " + str(round(h.GetMean(),2) ) )
#    latex.DrawLatex(0.50, 0.65, "StdDev: " + str(round(h.GetStdDev(),2) ) )

    call("chmod -R 755 " + savepath, shell=True)
    call("cp /home/users/hmei/public_html/backup/index.php " + savepath, shell=True)
    call("chmod -R 755 " + savepath, shell=True)

    c.SaveAs(savepath+savename+".png")
    c.SaveAs(savepath+savename+".pdf")

def GetAndPlot(filename,histname,savepath,xTitle,doLog=False):
    c = ROOT.TCanvas("c","",800,800)
    if doLog: c.SetLogy()   
    f = ROOT.TFile.Open(filename)
    h = f.Get(histname)

    dummy = ROOT.TH1D("dummy","dummy",1,h.GetBinLowEdge(1),h.GetBinLowEdge(h.GetNbinsX())+h.GetBinWidth(1))
    if doLog: c.SetLogy()
    dummy.SetMinimum(0)
    if doLog: dummy.SetMinimum(1)
    dummy.SetMaximum(1.5*h.GetMaximum())
    dummy.SetLineColor(0)
    dummy.SetMarkerColor(0)
    dummy.SetLineWidth(0)
    dummy.SetMarkerSize(0)
    dummy.GetYaxis().SetTitle("Events/"+str(h.GetBinWidth(1)))
    dummy.GetYaxis().SetTitleOffset(1.4)
    dummy.GetXaxis().SetTitle(xTitle)
    dummy.GetXaxis().SetTitleSize(0.05)
    dummy.Draw()

    h.GetXaxis().SetTitle(xTitle)
    h.GetXaxis().SetTitleSize(0.05)
    h.SetFillColor(ROOT.TColor.GetColor("#FFDF0A"))
    ROOT.gStyle.SetOptStat(100000)
    h.Draw("HIST same")
    #h.Draw("text same")
    dummy.Draw("same")
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

def GetAndPlot2D(filename,histname,savepath,xTitle,yTitle,tag=""):
    c = ROOT.TCanvas("c","",800,800)
    c.SetRightMargin(0.15)
#    c.SetLogz()
    f = ROOT.TFile.Open(filename)
    h = f.Get(histname)
    h.Scale(1/h.Integral())
    h.GetXaxis().SetTitle(xTitle)
    h.GetYaxis().SetTitle(yTitle)
    h.Draw("COLZ")
    #h.Draw("text same")
    print h.Integral()
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
    latex.DrawLatex(0.50, 0.75, "nEntries: " + str(round(h.GetEntries(),2) ) )
    latex.DrawLatex(0.50, 0.7, "Mean: " + str(round(h.GetMean(),2) ) )
    latex.DrawLatex(0.50, 0.65, "StdDev: " + str(round(h.GetStdDev(),2) ) )

    call("cp /home/users/hmei/public_html/backup/index.php " + savepath, shell=True)
    call("chmod -R 755 " + savepath, shell=True)
    c.SaveAs(savepath+histname+"_"+tag+".png")
    c.SaveAs(savepath+histname+"_"+tag+".pdf")

args=ParseOption()
tag = args.tag

filename1 = "tmpRootFiles/tmp_ttg_1.root"
filename2 = "tmpRootFiles/tmp_ttgg_1.root"
#filename1 = "tmpRootFiles/tmp_ttg.root"
#filename2 = "tmpRootFiles/tmp_ttgg.root"

#savepath = "/home/users/hmei/public_html/2018/20180711_genPhotonCheck/"
#savepath = "/home/users/hmei/public_html/2018/20180713_genPhotonCheck_fromW/"
#savepath = "/home/users/hmei/public_html/2018/20180713_genPhotonCheck_notFromW/"
if tag == "ttg":
   savepath = "/home/users/hmei/public_html/2018/20180730_genPhotonCheck_ttG/"
elif tag == "ttgg":
   savepath = "/home/users/hmei/public_html/2018/20180730_genPhotonCheck_ttGG/"
#savepath = "/home/users/hmei/public_html/2018/20180717_genPhotonCheck_test/"

savepath1 = "/home/users/hmei/public_html/2018/20180730_genPhotonCheck_ttG/"
savepath2 = "/home/users/hmei/public_html/2018/20180730_genPhotonCheck_ttGG/"
savepath = "/home/users/hmei/public_html/2018/20180802_genPhotonCheck_2/"
#savepath = "/home/users/hmei/public_html/2018/20180717_genPhotonCheck_pt10_eta2p6/"


histnames = [\
"nHard",
"nPhoton_prompt","nPhoton_hard",
"nLepton_prompt","nLepton_hard",
"nHadron_prompt","nHadron_hard",
"pTPhoton_prompt","pTPhoton_hard",
"etaPhoton_prompt","etaPhoton_hard",
"isoPhoton_prompt","isoPhoton_hard",
"phiPhoton_prompt","phiPhoton_hard",
"minDrPhoton_prompt","minDrPhoton_hard",
"minDrPhoton_passFrix_prompt","minDrPhoton_passFrix_hard",
"pT1","pT2","mGG","nPromptPhoton_matchedToHard",
"promptPhoton_motherID_0","promptPhoton_motherID_1",
"minDrPhoton_prompt_sel","minDrPhoton_hard_sel",
#"hardId_lep","hardId_had",
"minDrPhoton_hard_u_fromW","minDrPhoton_hard_d_fromW","minDrPhoton_hard_c_fromW",
"minDrPhoton_hard_s_fromW","minDrPhoton_hard_t_fromW","minDrPhoton_hard_b_fromW",
"minDrPhoton_hard_g_fromW",
"minDrPhoton_hard_u_notFromW","minDrPhoton_hard_d_notFromW","minDrPhoton_hard_c_notFromW",
"minDrPhoton_hard_s_notFromW","minDrPhoton_hard_t_notFromW","minDrPhoton_hard_b_notFromW",
"minDrPhoton_hard_g_notFromW",
"minDrPhoton_prompt_u_fromW","minDrPhoton_prompt_d_fromW","minDrPhoton_prompt_c_fromW",
"minDrPhoton_prompt_s_fromW","minDrPhoton_prompt_t_fromW","minDrPhoton_prompt_b_fromW",
"minDrPhoton_prompt_g_fromW",
"minDrPhoton_prompt_u_notFromW","minDrPhoton_prompt_d_notFromW","minDrPhoton_prompt_c_notFromW",
"minDrPhoton_prompt_s_notFromW","minDrPhoton_prompt_t_notFromW","minDrPhoton_prompt_b_notFromW",
"minDrPhoton_prompt_g_notFromW",
"minDrPhoton_hard_e_fromW","minDrPhoton_hard_mu_fromW","minDrPhoton_hard_tau_fromW",
"minDrPhoton_prompt_e_fromW","minDrPhoton_prompt_mu_fromW","minDrPhoton_prompt_tau_fromW",
#"lep_simpleGrandMa","had_simpleGrandMa",
"minDrPhoton_prompt_p","minDrPhoton_hard_p",
"dR_twoHardPhoton","dR_hard_prompt_photon"
]

for histname in histnames:
#    GetAndPlot(filename1, histname, savepath1, histname)
#    GetAndPlot(filename2, histname, savepath2, histname)
    savename = histname
    xTitle = histname
#    GetAndPlotOverlay(filename1,filename2,"ttG","ttGG",histname,histname,savepath,savename,xTitle)


################################################################################################

tags1 = ["_u_fromW","_d_fromW","_c_fromW","_s_fromW","_t_fromW","_b_fromW","_g_fromW","_e_fromW","_mu_fromW","_tau_fromW","_p"]
tags2 = ["_u_notFromW","_d_notFromW","_c_notFromW","_s_notFromW","_t_notFromW","_b_notFromW","_g_notFromW"]

#GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","minDrPhoton_prompt","minDrPhoton_hard",savepath,"minDr_promptFromTTG_hardFromTTGG","minDr")
#GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","minDrPhoton_prompt_sel","minDrPhoton_hard",savepath,"minDr_promptFromTTG_hardFromTTGG_sel","minDr")


GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","pT1","pT1",savepath,"pT1","pT1")
GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","pT2","pT2",savepath,"pT2","pT2")
GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","pT1_s","pT1",savepath,"pT1_s","pT1")
GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","pT2_s","pT2",savepath,"pT2_s","pT2")
GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","mGG","mGG",savepath,"mGG","mGG")
GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","mGG_s","mGG",savepath,"mGG_s","mGG")
GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","dPhiGG","dPhiGG",savepath,"dPhiGG","dPhiGG")
GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","dPhiGG_s","dPhiGG",savepath,"dPhiGG_s","dPhiGG")



'''
for tag in tags1:
    GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","minDrPhoton_prompt"+tag,"minDrPhoton_hard"+tag,savepath,"minDr_promptFromTTG_hardFromTTGG"+tag,"minDr"+tag)
for tag in tags2:
    GetAndPlotOverlay(filename1,filename2,"ttG","ttGG","minDrPhoton_prompt"+tag,"minDrPhoton_hard"+tag,savepath,"minDr_promptFromTTG_hardFromTTGG"+tag,"minDr"+tag)
'''

################################################################################################


histnames = ['momID_momMomID_h_ttg','momID_momMomID_h_ttgg','momID_momMomID_p_ttg']#'nPrompt_nPromptNotHard','nHardLep_nHardHad','hadId_hadMomId_hard']
xTitles = ['momID','momID','momID']#'nPromptPhoton','nHardLep','id_had_hard']
yTitles = ['momMomID','momMomID','momMomID']#''nPromptPhoton_notHard','nHardHad','id_hadMom_hard']

for i in range(len(histnames)):
    histname = histnames[i]
    xTitle = xTitles[i]
    yTitle = yTitles[i]
#    GetAndPlot2D(filename1, histname, savepath, xTitle, yTitle,"ttg")
#    GetAndPlot2D(filename2, histname, savepath, xTitle, yTitle,"ttgg")
#    GetAndPlot2D(filename2, histname, savepath, xTitle, yTitle)

GetAndPlot2D(filename1, histnames[0], savepath, xTitle, yTitle)
GetAndPlot2D(filename1, histnames[2], savepath, xTitle, yTitle)
GetAndPlot2D(filename2, histnames[1], savepath, xTitle, yTitle)

