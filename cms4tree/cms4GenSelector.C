#define cms4GenSelector_cxx
// The class definition in cms4GenSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("cms4GenSelector.C")
// root> T->Process("cms4GenSelector.C","some options")
// root> T->Process("cms4GenSelector.C+")
//


#include "cms4GenSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

void cms4GenSelector::SetTag(TString tag, TString inTag)
{
   tag_ = tag;
   inTag_ = inTag;
}

void cms4GenSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void cms4GenSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t cms4GenSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);
   int nTotal = 320000;
   long event = *ull_eventMaker_evtevent_CMS3_obj;
//   if (event != 13141146) return kTRUE;
   if (entry > nTotal) return kTRUE;
   if (entry%1000 == 0) cout << "entry: " << entry << "/" << nTotal << endl;

   genPhotons_prompt.clear();
   genLeptons_prompt.clear();
   genHadrons_prompt.clear();
   genPhotons_hard.clear();
   genLeptons_hard.clear();
   genHadrons_hard.clear();
   genParticles_s1.clear();

   vector<GenParticle> genPhotons_prompt_notHard;
   // 1, status1;                                                                                                                                                                                           
   // 2, prompt;                                                                                                                                                                                            
   // 3, hardProcess                                                                                                                                                                                        
   // 4, status 22 or 23                                                                                                                                                                                    

   //LoadGenParticles(genPhotons_prompt, genLeptons_prompt, genHadrons_prompt, 2);
   //LoadGenParticles(genPhotons_hard, genLeptons_hard, genHadrons_hard, 3);
   //LoadGenParticles(genPhotons_prompt_notHard, genLeptons_prompt, genHadrons_prompt, 5);                                                                                                                  
   //LoadGenParticles(genParticles_s1);
   LoadGenParticles();
   
   genPhotons_prompt_notHard.clear();
   GetPromptNotHardPhoton(genPhotons_hard,genPhotons_prompt,genPhotons_prompt_notHard);

   //cout << "nHard: " << genPhotons_hard.size() << ", nPrompt: " << genPhotons_prompt.size() << ", nPrompt_notHard: " << genPhotons_prompt_notHard.size() << endl;
   //cout << genPhotons_hard.size() << ", " << genPhotons_prompt.size() << ", " << genPhotons_prompt_notHard.size() << endl;                                                                                

   //SetVals(genPhotons_prompt_notHard, genLeptons_hard, 0.3, 0.05);
   //SetVals(genPhotons_hard, genLeptons_hard, 0.3, 0.05);
   
   //SetVals(genPhotons_prompt_notHard, genHadrons_hard, 0.3, 0.05);
   //SetVals(genPhotons_hard, genHadrons_hard, 0.3, 0.05);                                                                                                                                                  

   SetVals(genPhotons_hard, genLeptons_hard, genHadrons_hard, 0.3, 0.05);
   SetVals(genPhotons_prompt, genLeptons_hard, genHadrons_hard, 0.3, 0.05);                                                                                                                       
   //SetVals(genPhotons_prompt_notHard, genLeptons_hard, genHadrons_hard, 0.3, 0.05);

   // overwrite isolation w.r.s to status 1 non-photon
   // flag = 1: doIso
   // flag = 2: doNearestDr
   // flag = 3: doFrix

   SetVals(genPhotons_hard, genParticles_s1, 0.3, 0.05, 1);  
   //SetVals(genPhotons_prompt_notHard, genParticles_s1, 0.3, 0.05, 1);
   SetVals(genPhotons_prompt, genParticles_s1, 0.3, 0.05, 1);

   vector<int> promptPhoIndex;
   promptPhoIndex = SelectDiPhoton(genPhotons_prompt, pT1,pT2,mGG,dPhiGG);
  // if (genPhotons_prompt.size() != 2) return kTRUE;
   if (promptPhoIndex.size() != 2) return kTRUE;
 
   vector<GenParticle> phos_h_sel, phos_p_sel;
 
   int nMatched = 0;
   GenParticle pho1_p = genPhotons_prompt[promptPhoIndex[0]];
   GenParticle pho2_p = genPhotons_prompt[promptPhoIndex[1]];


   if (pho1_p.ID_simpleMom() == 22 && ints_genMaker_genpsstatus_CMS3_obj[pho1_p.IDX_simpleMom()] == 23) {
      nMatched++;
      phos_h_sel.push_back(pho1_p);
      }
      else{
          phos_p_sel.push_back(pho1_p);
          }
   if (pho2_p.ID_simpleMom() == 22 && ints_genMaker_genpsstatus_CMS3_obj[pho2_p.IDX_simpleMom()] == 23) {
      nMatched++;
      phos_h_sel.push_back(pho2_p);
      }
      else{
          phos_p_sel.push_back(pho2_p);
          }
      
   nPromptPhoton_matchedToHard->Fill(nMatched);

   if (tag_ == "ttgg" && nMatched != 2) return kTRUE;
   if (tag_ == "ttg" && nMatched  != 1) return kTRUE;


   if (tag_ == "ttg") {
      int tmpMomMom_p = ints_genMaker_genpsidmother_CMS3_obj[phos_p_sel[0].IDX_mom()];

//      if (phos_p_sel[0].ID_mom() == 2212 && ints_genMaker_genpsstatus_CMS3_obj[phos_p_sel[0].IDX_mom()] != 4) cout << "status: " << ints_genMaker_genpsstatus_CMS3_obj[phos_p_sel[0].IDX_mom()] << ", id : " << phos_p_sel[0].ID_mom() << endl;
      tmpMomMom_p = abs(tmpMomMom_p) > 100 ? 0 : abs(tmpMomMom_p);
      momID_momMomID_p_ttg->Fill(abs(phos_p_sel[0].ID_mom()), tmpMomMom_p);

      int tmpMomMom_h = ints_genMaker_genpsidmother_CMS3_obj[phos_h_sel[0].IDX_mom()];
      tmpMomMom_h = abs(tmpMomMom_h) > 100 ? 0 : abs(tmpMomMom_h);
      momID_momMomID_h_ttg->Fill(abs(phos_h_sel[0].ID_mom()), tmpMomMom_h);

      int tmpNearMom = phos_p_sel[0].GetSmallestDrMomId();
      tmpNearMom = abs(tmpNearMom) > 100 ? 0 : abs(tmpNearMom);
      nearID_nearMomID->Fill(phos_p_sel[0].GetSmallestDrId(), tmpNearMom);
//cout << phos_p_sel[0].ID_mom() << ", " << ints_genMaker_genpsidmother_CMS3_obj[phos_p_sel[0].IDX_mom()] << endl;

      GenParticle pho = phos_p_sel[0];
      if ((abs(pho.ID_mom()) > 0 && abs(pho.ID_mom()) < 6 && abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) != 24 && abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) != 6)
          || abs(pho.ID_mom()) == 6 || pho.ID_mom() == 21 ){
   
         mGG_s->Fill((pho1_p.P4() + pho2_p.P4()).M());
         pT1_s->Fill(pho1_p.P4().Pt()/((pho1_p.P4() + pho2_p.P4()).M()) );
         pT2_s->Fill(pho2_p.P4().Pt()/((pho1_p.P4() + pho2_p.P4()).M()) );
         dPhiGG_s->Fill(abs(deltaPhi(pho1_p.P4().Phi(), pho2_p.P4().Phi() ) ));
         minDrPhoton_prompt_sel->Fill(pho.GetSmallestDr());
         }

      }


   if (tag_ == "ttgg") {

      int tmpMomMom1 = ints_genMaker_genpsidmother_CMS3_obj[phos_h_sel[0].IDX_mom()];
      tmpMomMom1 = abs(tmpMomMom1) > 100 ? 0 : abs(tmpMomMom1);
      int tmpMomMom2 = ints_genMaker_genpsidmother_CMS3_obj[phos_h_sel[1].IDX_mom()];
      tmpMomMom2 = abs(tmpMomMom2) > 100 ? 0 : abs(tmpMomMom2);
      momID_momMomID_h_ttgg->Fill(abs(phos_h_sel[0].ID_mom()), tmpMomMom1);
      momID_momMomID_h_ttgg->Fill(abs(phos_h_sel[1].ID_mom()), tmpMomMom2);

      int tmpNearMom1 = phos_h_sel[0].GetSmallestDrMomId();
      tmpNearMom1 = abs(tmpNearMom1) > 100 ? 0 : abs(tmpNearMom1);
      int tmpNearMom2 = phos_h_sel[1].GetSmallestDrMomId();
      tmpNearMom2 = abs(tmpNearMom2) > 100 ? 0 : abs(tmpNearMom2);
      nearID_nearMomID->Fill(phos_h_sel[0].GetSmallestDrId(), tmpNearMom1);
      nearID_nearMomID->Fill(phos_h_sel[1].GetSmallestDrId(), tmpNearMom2);
      
//cout << event << endl;
      }


//if (phos_p_sel[0].ID_mom() == 6) cout << event << ", nearID: " << phos_p_sel[0].GetSmallestDrId() << ", " << phos_p_sel[0].P4().Pt() << endl;
   /*
   if (tag_=="ttgg" && nMatched == 1) {
      cout << event << endl;
      cout << pho1_p.ID_simpleMom() << ", " << ints_genMaker_genpsstatus_CMS3_obj[pho1_p.IDX_simpleMom()] << endl;
      cout << pho2_p.ID_simpleMom() << ", " << ints_genMaker_genpsstatus_CMS3_obj[pho2_p.IDX_simpleMom()] << endl;
      }
   */
   /*
   if (tag_ == "ttgg") {
      int nMatched = DiffPt_DiffDr_hard_prompt_ttGG(genPhotons_hard, genPhotons_prompt[promptPhoIndex[0]], genPhotons_prompt[promptPhoIndex[1]], diffPt_diffDr,
                                                    promptPhoton_motherID_0, promptPhoton_motherID_1);
      nPromptPhoton_matchedToHard->Fill(nMatched);
      }
   if (tag_ == "ttg") {
      int nMatched = DiffPt_DiffDr_hard_prompt_ttG(genPhotons_hard, genPhotons_prompt[promptPhoIndex[0]], genPhotons_prompt[promptPhoIndex[1]], diffPt_diffDr,
                                                   promptPhoton_motherID_0, promptPhoton_motherID_1);
      nPromptPhoton_matchedToHard->Fill(nMatched);
      }

   */

   nPhoton_hard->Fill(genPhotons_hard.size());
   nLepton_hard->Fill(genLeptons_hard.size());
   nHadron_hard->Fill(genHadrons_hard.size());

   nHard->Fill(genPhotons_hard.size()+genLeptons_hard.size()+genHadrons_hard.size()+2);

   nPhoton_prompt->Fill(genPhotons_prompt_notHard.size());
   nLepton_prompt->Fill(genLeptons_prompt.size());
   nHadron_prompt->Fill(genHadrons_prompt.size());
   nHardLep_nHardHad->Fill(genLeptons_hard.size(), genHadrons_hard.size());

   //FillSimplePhotonVar(genPhotons_prompt, pTPhoton_prompt, etaPhoton_prompt, phiPhoton_prompt, isoPhoton_prompt, minDrPhoton_prompt, minDrPhoton_passFrix_prompt, isoPhoton_passFrix_prompt);            
   FillSimplePhotonVar(phos_h_sel, pTPhoton_hard, etaPhoton_hard, phiPhoton_hard, isoPhoton_hard, minDrPhoton_hard, minDrPhoton_passFrix_hard, minDrPhoton_hard_sel, isoPhoton_passFrix_hard);
   FillSimplePhotonVar(phos_p_sel, pTPhoton_prompt, etaPhoton_prompt, phiPhoton_prompt, isoPhoton_prompt, minDrPhoton_prompt, minDrPhoton_passFrix_prompt, minDrPhoton_prompt_sel, isoPhoton_passFrix_prompt);

//cout << phos_h_sel.size() << ", " << phos_p_sel.size() << endl;

   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_u_fromW, minDrPhoton_hard_u_notFromW, 2);   
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_d_fromW, minDrPhoton_hard_d_notFromW, 1);     
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_c_fromW, minDrPhoton_hard_c_notFromW, 4);     
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_s_fromW, minDrPhoton_hard_s_notFromW, 3);     
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_t_fromW, minDrPhoton_hard_t_notFromW, 6);     
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_b_fromW, minDrPhoton_hard_b_notFromW, 5);     
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_g_fromW, minDrPhoton_hard_g_notFromW, 21);     
/*
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_u_fromW, minDrPhoton_hard_u_notFromW, 2);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_d_fromW, minDrPhoton_hard_d_notFromW, 1);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_c_fromW, minDrPhoton_hard_c_notFromW, 4);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_s_fromW, minDrPhoton_hard_s_notFromW, 3);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_t_fromW, minDrPhoton_hard_t_notFromW, 6);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_b_fromW, minDrPhoton_hard_b_notFromW, 5);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_g_fromW, minDrPhoton_hard_g_notFromW, 21);
*/
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_u_fromW, minDrPhoton_prompt_u_notFromW, 2);   
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_d_fromW, minDrPhoton_prompt_d_notFromW, 1);   
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_c_fromW, minDrPhoton_prompt_c_notFromW, 4);   
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_s_fromW, minDrPhoton_prompt_s_notFromW, 3);   
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_t_fromW, minDrPhoton_prompt_t_notFromW, 6);   
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_b_fromW, minDrPhoton_prompt_b_notFromW, 5);   
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_g_fromW, minDrPhoton_prompt_g_notFromW, 21); 

   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_e_fromW, minDrPhoton_hard_e_notFromW, 11);
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_mu_fromW, minDrPhoton_hard_mu_notFromW, 13);
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_tau_fromW, minDrPhoton_hard_tau_notFromW, 15);
/*
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_e_fromW, minDrPhoton_hard_e_notFromW, 11);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_mu_fromW, minDrPhoton_hard_mu_notFromW, 13);
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_tau_fromW, minDrPhoton_hard_tau_notFromW, 15);
*/
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_e_fromW, minDrPhoton_prompt_e_notFromW, 11);
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_mu_fromW, minDrPhoton_prompt_mu_notFromW, 13);
   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_tau_fromW, minDrPhoton_prompt_tau_notFromW, 15);

   FillMinDrByGenID(phos_p_sel, minDrPhoton_prompt_p, minDrPhoton_prompt_p, 2212);
   FillMinDrByGenID(phos_h_sel, minDrPhoton_hard_p, minDrPhoton_hard_p, 2212);
//   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_p, minDrPhoton_hard_p, 2212);

//   nPrompt_nPromptNotHard->Fill(genPhotons_prompt.size(),genPhotons_prompt_notHard.size());

   return kTRUE;

}

void cms4GenSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void cms4GenSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   outputRootFile = new TFile("tmpRootFiles/tmp_" + tag_ + "_" + inTag_ + ".root","RECREATE");
   outputRootFile->cd();
   momID_momMomID_h_ttgg->Write();
   momID_momMomID_h_ttg->Write();
   momID_momMomID_p_ttg->Write();
   nHard->Write();

   pT1->Write();
   pT2->Write();
   mGG->Write();
   dPhiGG->Write();

   pT1_s->Write();
   pT2_s->Write();
   mGG_s->Write();
   dPhiGG_s->Write();

   nPhoton_prompt->Write();
   nPhoton_hard->Write();
   nLepton_prompt->Write();
   nLepton_hard->Write();
   nHadron_prompt->Write();
   nHadron_hard->Write();

   pTPhoton_prompt->Write();
   etaPhoton_prompt->Write();
   phiPhoton_prompt->Write();
   isoPhoton_prompt->Write();
   minDrPhoton_prompt->Write();
   minDrPhoton_passFrix_prompt->Write();
   isoPhoton_passFrix_prompt->Write();

   pTPhoton_hard->Write();
   etaPhoton_hard->Write();
   phiPhoton_hard->Write();
   isoPhoton_hard->Write();
   minDrPhoton_hard->Write();
   minDrPhoton_passFrix_hard->Write();
   isoPhoton_passFrix_hard->Write();

   nPrompt_nPromptNotHard->Write();
   hardId_lep->Write();
   hardId_had->Write();
   nHardLep_nHardHad->Write();
   hadId_hadMomId_hard->Write();   
nearID_nearMomID->Write();
 minDrPhoton_hard_u_fromW->Write();
 minDrPhoton_hard_d_fromW->Write();
 minDrPhoton_hard_c_fromW->Write();
 minDrPhoton_hard_s_fromW->Write();
 minDrPhoton_hard_t_fromW->Write();
 minDrPhoton_hard_b_fromW->Write();
 minDrPhoton_hard_g_fromW->Write();
 minDrPhoton_prompt_u_fromW->Write();
 minDrPhoton_prompt_d_fromW->Write();
 minDrPhoton_prompt_c_fromW->Write();
 minDrPhoton_prompt_s_fromW->Write();
 minDrPhoton_prompt_t_fromW->Write();
 minDrPhoton_prompt_b_fromW->Write();
 minDrPhoton_prompt_g_fromW->Write();
 minDrPhoton_hard_u_notFromW->Write();
 minDrPhoton_hard_d_notFromW->Write();
 minDrPhoton_hard_c_notFromW->Write();
 minDrPhoton_hard_s_notFromW->Write();
 minDrPhoton_hard_t_notFromW->Write();
 minDrPhoton_hard_b_notFromW->Write();
 minDrPhoton_hard_g_notFromW->Write();
 minDrPhoton_prompt_u_notFromW->Write();
 minDrPhoton_prompt_d_notFromW->Write();
 minDrPhoton_prompt_c_notFromW->Write();
 minDrPhoton_prompt_s_notFromW->Write();
 minDrPhoton_prompt_t_notFromW->Write();
 minDrPhoton_prompt_b_notFromW->Write();
 minDrPhoton_prompt_g_notFromW->Write();

minDrPhoton_hard_e_notFromW->Write();
minDrPhoton_hard_mu_notFromW->Write();
minDrPhoton_hard_tau_notFromW->Write();
minDrPhoton_hard_e_fromW->Write();
minDrPhoton_hard_mu_fromW->Write();
minDrPhoton_hard_tau_fromW->Write();
   
minDrPhoton_prompt_e_notFromW->Write();
minDrPhoton_prompt_mu_notFromW->Write();
minDrPhoton_prompt_tau_notFromW->Write();
minDrPhoton_prompt_e_fromW->Write();
minDrPhoton_prompt_mu_fromW->Write();
minDrPhoton_prompt_tau_fromW->Write();

   lep_simpleGrandMa->Write();
   had_simpleGrandMa->Write();
dR_twoHardPhoton->Write();
dR_hard_prompt_photon->Write();

diffPt_diffDr->Write();
nPromptPhoton_matchedToHard->Write();
promptPhoton_motherID_0->Write();
promptPhoton_motherID_1->Write();

 minDrPhoton_prompt_sel->Write();
 minDrPhoton_hard_sel->Write();
minDrPhoton_hard_p->Write();
minDrPhoton_prompt_p->Write();
   outputRootFile->Close();

}


void cms4GenSelector::LoadGenParticles(vector<GenParticle>& genP)
{

  int nGenParticles = ints_genMaker_genpsid_CMS3_obj.GetSize();
  for (int i = 0; i < nGenParticles; i++) {

    float genP1 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fX[i];
    float genP2 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fY[i];
    float genP3 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fZ[i];
    float genP4 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fT[i];
    int genID = ints_genMaker_genpsid_CMS3_obj[i];
    int simpleMomId = ints_genMaker_genpsidsimplemother_CMS3_obj[i];
    int momId = ints_genMaker_genpsidmother_CMS3_obj[i];
    int simpleMomIdx = ints_genMaker_genpsidxsimplemother_CMS3_obj[i];
    int momIdx = ints_genMaker_genpsidxmother_CMS3_obj[i];
    int genStatus = ints_genMaker_genpsstatus_CMS3_obj[i];
    bool isPrompt = (*bools_genMaker_genpsIsPromptFinalState_CMS3_obj)[i];
    bool isHardProcess = (*bools_genMaker_genpsIsHardProcess_CMS3_obj)[i];

    GenParticle tmpGenP = GenParticle(genP1,genP2,genP3,genP4,genID,momId,simpleMomId,momIdx,simpleMomIdx,genStatus,isPrompt,isHardProcess);
    if (genStatus == 1 && genID != 22) genP.push_back(tmpGenP);

  }
}

void cms4GenSelector::LoadGenParticles(/*vector<GenParticle>& pho, vector<GenParticle>& lep, vector<GenParticle>& had, int genCat = 0, bool photonOnly = false*/)
{

  int nGenParticles = ints_genMaker_genpsid_CMS3_obj.GetSize();
  for (int i = 0; i < nGenParticles; i++) {

    float genP1 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fX[i];
    float genP2 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fY[i];
    float genP3 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fZ[i];
    float genP4 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fT[i];
    int genID = ints_genMaker_genpsid_CMS3_obj[i];
    int simpleMomId = ints_genMaker_genpsidsimplemother_CMS3_obj[i];
    int momId = ints_genMaker_genpsidmother_CMS3_obj[i];
    int simpleMomIdx = ints_genMaker_genpsidxsimplemother_CMS3_obj[i];
    int momIdx = ints_genMaker_genpsidxmother_CMS3_obj[i];

    if (simpleMomId == 2212) simpleMomId = 0;
    if (momId == 2212) momId = 0;
    if (abs(simpleMomId) > 100) simpleMomId = 0;
    if (abs(momId) > 100) momId = 0;

    int genStatus = ints_genMaker_genpsstatus_CMS3_obj[i];
    bool isPrompt = (*bools_genMaker_genpsIsPromptFinalState_CMS3_obj)[i];
    bool isHardProcess = (*bools_genMaker_genpsIsHardProcess_CMS3_obj)[i];

    //cout << "simpleid: " << simpleMomId << ", simpleIdx: " << simpleMomIdx << ", momid: " << momId << ", momIdx: " << momIdx << endl; 
/*
    //genCat:                                                                                                                                                                                               
    //1, status1; 2, prompt; 3, hardProcess, 4, status22or23                                                                                                                                                
    if (genCat == 1 && genStatus != 1) continue;
    if (genCat == 2 && !(isPrompt) ) continue;
    if (genCat == 3 && !(genStatus == 21 || genStatus == 22 || genStatus == 23 || genStatus == 24) ) continue;
    //if (genCat == 4 && !(genStatus == 22 || genStatus == 23)) continue;
    //if (genCat == 5 && !(isPrompt && !isHardProcess) ) continue;
    //cout << genP1 << "," << genP2 << "," << genP3 << "," << genP4 << "," << genID << "," << simpleMomId << "," << genStatus << endl;                                                                      
    GenParticle tmpGenP = GenParticle(genP1,genP2,genP3,genP4,genID,simpleMomId,genStatus,isPrompt,isHardProcess);

    if (genID == 22) pho.push_back(tmpGenP);
    if (photonOnly) continue;
    if (abs(genID) == 11 || abs(genID) == 13 || abs(genID) == 15) lep.push_back(tmpGenP);
    if (genCat == 3 && ( (abs(genID) > 0 && abs(genID) < 10) || genID == 21) ) had.push_back(tmpGenP);
    if (genCat == 2 && (abs(genID) < 10 || genID == 21 || abs(genID) > 100) ) had.push_back(tmpGenP);
*/
//============
    GenParticle tmpGenP = GenParticle(genP1,genP2,genP3,genP4,genID,momId,simpleMomId,momIdx,simpleMomIdx,genStatus,isPrompt,isHardProcess);

    if (genStatus == 1 && genID != 22 && abs(genID) != 12 && abs(genID) != 14 && abs(genID) != 16) genParticles_s1.push_back(tmpGenP);

    if (isPrompt) {
       if (genID == 22) genPhotons_prompt.push_back(tmpGenP);
       if (abs(genID) == 11 || abs(genID) == 13) genLeptons_prompt.push_back(tmpGenP);
       if (abs(genID) > 100) genHadrons_prompt.push_back(tmpGenP);
       }

    if (genStatus == 21 || genStatus == 22 || genStatus == 23 || genStatus == 24) { 
       if (genID == 22) genPhotons_hard.push_back(tmpGenP); 
       if (abs(genID) == 11 || abs(genID) == 13 || abs(genID) == 15) genLeptons_hard.push_back(tmpGenP);
       if (abs(genID) < 10 || abs(genID) == 21) genHadrons_hard.push_back(tmpGenP);
       }

  }
}

// set iso,frxioneIso, nearestDr, only change for cat1 particle       

void cms4GenSelector::SetVals(vector<GenParticle>& gp_cat1, vector<GenParticle> gp_cat2, vector<GenParticle> gp_cat3, double cone_iso, double cone_frix)
{

  const int nBinsForFrix = 100;
  const double initConeFrix = 0.000000000001;
  vector<GenParticle> tmpGP; tmpGP.clear();
  //   tmpGP.insert(tmpGP.end(), gp_cat1.begin(), gp_cat1.end());                                                                                                                                           
                                                                                                      
  tmpGP.insert(tmpGP.end(), gp_cat2.begin(), gp_cat2.end());
  tmpGP.insert(tmpGP.end(), gp_cat3.begin(), gp_cat3.end());
  //cout << "tmpGP size: " << tmpGP.size() << endl;                                                                                                                                                         
  //cout << "gp_cat1.size: " << gp_cat1.size() << endl;                                                                                                                                                    

  for (int igp = 0; igp < int(gp_cat1.size()); igp++) {
    GenParticle gp = gp_cat1[igp];
    double gp_et = gp.P4().Et();
    double gp_pt = gp.P4().Pt();
    double gp_eta = gp.P4().Eta();
    double gp_phi = gp.P4().Phi();
    double pTSum = 0;
    double smallestDr = 9999;
    int smallestDr_id = 0;
    int smallestDr_momId = 0;
    double smallestDr_pT = -1;
    double smallestDr_eta = 999;
    int smallestDr_status = 0;
    double ets[nBinsForFrix] = {};
    bool passFrix = true;

    for (int i = 0; i < int(tmpGP.size()); i++) {
      if (i == igp) continue;

      GenParticle gp2 = tmpGP[i];
      double gp2_et = gp2.P4().Et();
      double gp2_pt = gp2.P4().Pt();
      double gp2_eta = gp2.P4().Eta();
      double gp2_phi = gp2.P4().Phi();
      double dR = deltaR(gp_eta,gp_phi,gp2_eta,gp2_phi);

      if (dR < smallestDr) {
	smallestDr = dR; // smallest distance w.r.s interesting gen particle                                                                                                                                
                                                                                                 
	smallestDr_id = gp2.ID();
	smallestDr_momId = gp2.ID_mom();
	smallestDr_pT = gp2_pt;
	smallestDr_eta = gp2_eta;
	smallestDr_status = gp2.Status();
//cout << "pt: " << gp_pt << ", " << smallestDr << endl;
      }
      if (dR < cone_iso) {pTSum += gp2_pt; /*cout << "pTSum: " << pTSum << endl;*/} // used for isolation cut                                                                                               

      for (int j = 0; j < nBinsForFrix; j++) { // save relavant info for frixione isolation                                                                                                                 
                                                                                               
	double cone_var = (initConeFrix + j*cone_frix/nBinsForFrix);
	if (dR < cone_var && cone_var < cone_frix) ets[j] += gp2_et/(1-cos(cone_var) )*(1-cos(cone_frix) );

      }
    }// loop all other particles                                                                                                                                                                            

    //cout << "pTSum: " << pTSum << ", gp_pt: " << gp_pt << endl;                                                                                                                                          
                                                                                                    
    gp_cat1[igp].SetIso(pTSum);
    gp_cat1[igp].SetSmallestDr(smallestDr);
    gp_cat1[igp].SetSmallestDrId(smallestDr_id);
    gp_cat1[igp].SetSmallestDrMomId(smallestDr_momId);
    gp_cat1[igp].SetSmallestDrStatus(smallestDr_status);

       //if (abs(smallestDr_id) >= 11 && abs(smallestDr_id) <= 15) cout << "pTSum: " << pTSum << ", gp_pt: " << gp_pt << ", smallestDr: " << smallestDr << ", smallestDr_id: " << smallestDr_id << ", smallestDr_momId: " << smallestDr_momId << ", smallestDr_status: " << smallestDr_status << endl;

    for (int k = 0; k < nBinsForFrix; k++) {
      if (ets[k] > gp_et) {
	passFrix = false; break;
         }
        }
    // cout << "pass/fail: " << (passFrix?1:0) << endl; cout << endl;                                                                                                                                       
                                                                                                    
    gp_cat1[igp].SetFrixioneIso(passFrix);

    //cout << "phoIso: " << gp_cat1[igp].GetIso() << endl;

    }// loop particle type which is interesting for study                                      
}
                                                                                                                                                                                                            

// set iso,frxioneIso, nearestDr, only change for cat1 particle
void cms4GenSelector::SetVals(vector<GenParticle>& gp_cat1, vector<GenParticle> gp_cat2, double cone_iso, double cone_frix, int flag=0)
{
// flag = 1: doIso
// flag = 2: doNearestDr
// flag = 3: doFrix
//
   const int nBinsForFrix = 10;
   const double initConeFrix = 0.000000000001;
   vector<GenParticle> tmpGP; tmpGP.clear();
   //tmpGP.insert(tmpGP.end(), gp_cat1.begin(), gp_cat1.end());
   tmpGP.insert(tmpGP.end(), gp_cat2.begin(), gp_cat2.end());

   //cout  << "tmpGP size: " << tmpGP.size() << endl;
   //cout << "gp_cat1.size: " << gp_cat1.size() << endl;

   for (int igp = 0; igp < int(gp_cat1.size()); igp++) {
       GenParticle gp = gp_cat1[igp];
       double gp_et = gp.P4().Et();
       double gp_pt = gp.P4().Pt();
       double gp_eta = gp.P4().Eta();
       double gp_phi = gp.P4().Phi();
       double pTSum = 0;
       double smallestDr = 9999;
       int smallestDr_id = 0;
       int smallestDr_momId = 0;
       double smallestDr_pT = -1;
       double smallestDr_eta = 999;
       int smallestDr_status = 0;
       double ets[nBinsForFrix] = {};
       bool passFrix = true;

       for (int i = 0; i < int(tmpGP.size()); i++) {
	 //         if (i == igp) continue;

           GenParticle gp2 = tmpGP[i];
           double gp2_et = gp2.P4().Et();
           double gp2_pt = gp2.P4().Pt();
           double gp2_eta = gp2.P4().Eta();
           double gp2_phi = gp2.P4().Phi();
           double dR = deltaR(gp_eta,gp_phi,gp2_eta,gp2_phi);

           if (dR < smallestDr) {
              smallestDr = dR; // smallest distance w.r.s interesting gen particle
              smallestDr_id = gp2.ID();
              smallestDr_momId = gp2.ID_mom();
              smallestDr_pT = gp2_pt;
              smallestDr_eta = gp2_eta;
              smallestDr_status = gp2.Status();
           }

           if (dR < cone_iso) {pTSum += gp2_pt; /*cout << "pTSum: " << pTSum << endl;*/} // used for isolation cut

           for (int j = 0; j < nBinsForFrix; j++) { // save relavant info for frixione isolation
               double cone_var = (initConeFrix + j*cone_frix/nBinsForFrix);
               if (dR < cone_var && cone_var < cone_frix) ets[j] += gp2_et/(1-cos(cone_var) )*(1-cos(cone_frix) );

               }
           }// loop all other particles

       //if (smallestDr<3) cout << "pTSum: " << pTSum << ", gp_pt: " << gp_pt << ", smallestDr: " << smallestDr << ", smallestDr_id: " << smallestDr_id << ", smallestDr_momId: " << smallestDr_momId << ", smallestDr_status: " << smallestDr_status << endl;
       if (flag == 1) gp_cat1[igp].SetIso(pTSum);
       if (flag == 2 || flag == 1) {
          gp_cat1[igp].SetSmallestDr(smallestDr);
          gp_cat1[igp].SetSmallestDrId(smallestDr_id);
          gp_cat1[igp].SetSmallestDrMomId(smallestDr_momId);
          gp_cat1[igp].SetSmallestDrStatus(smallestDr_status);
          }

       for (int k = 0; k < nBinsForFrix; k++) {
           if (ets[k] > gp_et) {
              passFrix = false; break;
              }
           }
       // cout << "pass/fail: " << (passFrix?1:0) << endl; cout << endl;
       if (flag == 3) gp_cat1[igp].SetFrixioneIso(passFrix);
/*
       if (smallestDr > 0.3) {
          cout << "pt_hard: " << gp_pt << ", eta_hard: " << gp_eta << endl;
          cout << "pt_prompt: " << smallestDr_pT << ", eta_prompt: " << smallestDr_eta << endl;
          for (int i = 0; i < int(tmpGP.size()); i++) cout << tmpGP[i].P4().Pt() << " ";
          cout << endl;
          }
*/
       //cout << "cloestStatus: " << smallestDr_status << ", cloestID: " << smallestDr_id << ", cloestMomID: " << smallestDr_momId << ", cloestPt: " << smallestDr_pT << endl;
       
       /*
       if (!passFrix ) {
          cout << "Et: " << gp_et << ", smallestDr: " << smallestDr << endl;
          for (int k = 0; k < nBinsForFrix; k++) {
              cout << "ets[" << k << "]: " << ets[k] << endl;
              }
       }
       */
       //     cout << "pTSum: " << pTSum << ", smallestDr: " << smallestDr << endl;
       
       }// loop particle type which is interesting for study
}



void cms4GenSelector::FillSimplePhotonVar(vector<GenParticle> phos, TH1F* pT, TH1F* eta, TH1F* phi, TH1F* iso, TH1F* minDr, TH1F* minDrPassFrix, TH1F* minDr_sel, TH1F* isoPassFrix)
{

  for (int i = 0; i < int(phos.size()); i++) {

      GenParticle pho = phos[i];
      TLorentzVector phop4 = pho.P4();
      double phopt = phop4.Pt();
      double phoeta = phop4.Eta();
      double phophi = phop4.Phi();
      double isolation = pho.GetIso();
//cout << pho.GetIso() << endl;


//      if (abs(phoeta) > 2.6 || phopt < 25 /*|| isolation/phopt > 0.1) continue;
      pT->Fill(phopt);
      eta->Fill(phoeta);
      phi->Fill(phophi);
      iso->Fill(isolation/phopt);
      int minDrId = pho.GetSmallestDrId();
      int minDrMomId = pho.GetSmallestDrMomId();
      int minDrStatus = pho.GetSmallestDrStatus();

//      if (abs(pho.ID_mom()) == 21 || abs(pho.ID_mom()) == 6) minDr->Fill(pho.GetSmallestDr());
//      if (abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) == 24 || (abs(pho.ID_mom()) == 5 && abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) == 6) ) minDr->Fill(pho.GetSmallestDr()); 
/*      if ( (abs(pho.ID_mom()) > 0 && abs(pho.ID_mom()) < 6 && abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) != 24 && abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) != 6)
        || abs(pho.ID_mom()) == 6 || pho.ID_mom() == 21) */
/*          !(abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) <= 6 && abs(ints_genMaker_genpsidmother_CMS3_obj[pho.IDX_mom()]) > 0) )*/ minDr->Fill(pho.GetSmallestDr());

//      if ((/*abs(minDrId) <= 6 && minDrId != 0 &&*/abs(minDrId) >= 11 && abs(minDrId) <=15 && abs(minDrMomId) == 24) /*|| minDrId == 21*/) minDr_sel->Fill(pho.GetSmallestDr());

      //      cout << "minDr: " << pho.GetSmallestDr() << endl;
      if (pho.GetFrixioneIso() ) {
         minDrPassFrix->Fill(pho.GetSmallestDr());
         isoPassFrix->Fill(pho.GetIso());
         }

      }

} 

void cms4GenSelector::FillMinDrByGenID(vector<GenParticle> phos, TH1F* minDr_fromW, TH1F* minDr_notFromW, int id){

  for (int i = 0; i < int(phos.size()); i++){

      GenParticle pho = phos[i];
      TLorentzVector phop4 = pho.P4();
      double phopt = phop4.Pt();
      double phoeta = phop4.Eta();
      double phophi = phop4.Phi();
      int phoMomId = pho.ID_mom();
      double isolation = pho.GetIso();
      bool passFrix = pho.GetFrixioneIso();

//      if (abs(phoeta) > 2.6 || phopt < 25 || isolation/phopt > 0.1 || !passFrix) continue;
      double mindr = pho.GetSmallestDr();
      int minDrId = pho.GetSmallestDrId();
      int minDrMomId = pho.GetSmallestDrMomId();
      int minDrStatus = pho.GetSmallestDrStatus();
//cout << "minDrMomId: " << minDrMomId << endl;
//cout << "minDrId: " << minDrId << ", minDrStatus: " << minDrStatus << ", minDrMomId: " << minDrMomId << endl;
      //if (abs(minDrId) == id && id ==21) cout << "phoMother: " << phoMomId << endl; 
      //      if (abs(minDrId) == id && id == abs(phoMomId)) minDr->Fill(mindr);                                                                                                                                
      if (abs(minDrId) == id && abs(minDrMomId) == 24) minDr_fromW->Fill(mindr);
      if (abs(minDrId) == id && abs(minDrMomId) != 24) minDr_notFromW->Fill(mindr);

//if (abs(minDrId) > 10 && abs(minDrId) != 11 && abs(minDrId) != 13 && abs(minDrId) != 15 && abs(minDrId) != 21) cout << "id: " << minDrId << endl;
//      if (abs(mindr) < 0.1 && id == 2 ) cout << "id: " << minDrId << ", momId: " << minDrMomId << endl;

  }


}

void cms4GenSelector::GetPromptNotHardPhoton(vector<GenParticle> hard, vector<GenParticle> prompt, vector<GenParticle>& prompt_notHard)
{

  for (int i = 0; i < int(prompt.size()); i++) {
      double eta1 = prompt[i].P4().Eta();
      double phi1 = prompt[i].P4().Phi();
      double pT1 = prompt[i].P4().Pt();
      int id1 = prompt[i].P4().Pt();
      int idSimpleMom1 = prompt[i].ID_simpleMom();
      int idxSimpleMom1 = prompt[i].IDX_simpleMom();

/*
      bool closeToHard = false;
      for (int j = 0 ; j < int(hard.size()); j++) {
        double eta2 = hard[j].P4().Eta();
        double phi2 = hard[j].P4().Phi();
        double pT2 = hard[j].P4().Pt();

        double dR = deltaR(eta1,phi1,eta2,phi2);

        if (dR < 0.3 && abs(pT1-pT2) < 10) {
           closeToHard = true;
           break;
           }

        }
*/

      if (!(idSimpleMom1 == 22 && ints_genMaker_genpsstatus_CMS3_obj[idxSimpleMom1] == 23)) prompt_notHard.push_back(prompt[i]);

      }
}


vector<int> cms4GenSelector::SelectDiPhoton(vector<GenParticle> phos, TH1F* h_pT1, TH1F* h_pT2, TH1F* h_mgg, TH1F* h_dPhiGG) {

  vector<int> indexs; indexs.clear();

  if (phos.size() != 2) return indexs;

  GenParticle g1,g2;
  double pT1,pT2;
  pT1 = 0; pT2 = 0;
  int index1, index2;
  index1 = -1; index2 = -1;

  for (int i = 0; i < int(phos.size()); i++) {
      GenParticle pho = phos[i];
      double pT = pho.P4().Pt();
      if (pT > pT1) {pT1 = pT; g1 = pho; index1 = i;}
      }
  for (int i = 0; i < int(phos.size()); i++) {
      GenParticle pho = phos[i];
      double pT = pho.P4().Pt();
      if (pT > pT2 && pT < pT1) {pT2 = pT; g2 = pho; index2 = i;}
      }


  bool pass = true;

  TLorentzVector gg = g1.P4() + g2.P4();
  double mgg = gg.M();
  if (pT1 < 35 || pT2 < 25 || pT1/mgg < 1/3.0 || pT2/mgg < 1/4.0 || mgg < 80) pass = false;
  double iso1 = phos[index1].GetIso()/phos[index1].P4().Pt();
  double iso2 = phos[index2].GetIso()/phos[index2].P4().Pt();
  if (iso1 > 0.1 || iso2 > 0.1) pass = false;
  if (!g1.GetFrixioneIso() || !g2.GetFrixioneIso()) pass = false;

  if (pass) {

     h_pT1->Fill(pT1/mgg);
     h_pT2->Fill(pT2/mgg);
     h_mgg->Fill(mgg);
     indexs.push_back(index1);
     indexs.push_back(index2);
     h_dPhiGG->Fill( abs(deltaPhi(g1.P4().Phi(), g2.P4().Phi() ) ));

     }

  return indexs;
}



int cms4GenSelector::DiffPt_DiffDr_hard_prompt_ttGG(vector<GenParticle> phos, GenParticle pho1_p, GenParticle pho2_p, TH2F* diffPt_diffDr, TH1F* mom_0, TH1F* mom_1) {

  int counter = 0; // count number of hard photon matched to a prompt photon

  GenParticle pho1_h, pho2_h;
  if (phos[0].P4().Pt() > phos[1].P4().Pt()) {
     pho1_h = phos[0];
     pho2_h = phos[1];
     } else {
            pho1_h = phos[1];
            pho2_h = phos[0];
            }

  double pT1_h = pho1_h.P4().Pt();
  double pT2_h = pho2_h.P4().Pt();

  double diffPt1 = abs(pho1_h.P4().Pt() - pho1_p.P4().Pt());
  double diffPt2 = abs(pho2_h.P4().Pt() - pho2_p.P4().Pt());
  double diffDr1 = deltaR_p4(pho1_h.P4(), pho1_p.P4());
  double diffDr2 = deltaR_p4(pho2_h.P4(), pho2_p.P4());

  //if (diffPt2 > abs(pho2_h.P4().Pt() - pho1_p.P4().Pt() ) ) cout << pho1_h.P4().Pt() << ", " << pho2_h.P4().Pt() << ", " << pho1_p.P4().Pt() << ", "  << pho2_p.P4().Pt() << endl;

  diffPt_diffDr->Fill(diffPt1/pT1_h, diffDr1);
  diffPt_diffDr->Fill(diffPt2/pT2_h, diffDr2);

  int match[4] = {0,0,0,0};

  if (diffPt1/pT1_h < 0.2 && diffDr1 < 0.3) {counter++; match[0]=1;}
  if (diffPt2/pT2_h < 0.2 && diffDr2 < 0.3) {counter++; match[1]=1;}

  if (counter==0) {

     double diffPt1_sp = abs(pho1_h.P4().Pt() - pho2_p.P4().Pt());
     double diffPt2_sp = abs(pho2_h.P4().Pt() - pho1_p.P4().Pt());
     double diffDr1_sp = deltaR_p4(pho1_h.P4(), pho2_p.P4());
     double diffDr2_sp = deltaR_p4(pho2_h.P4(), pho1_p.P4());

     if (diffPt1_sp/pT2_h < 0.2 && diffDr1_sp < 0.3) {counter++; match[2] = 1;}
     if (diffPt2_sp/pT1_h < 0.2 && diffDr2_sp < 0.3) {counter++; match[3] = 1;}

  }

//  if (counter==0) {
     mom_0->Fill(abs(pho1_p.ID_mom()));
     mom_0->Fill(abs(pho2_p.ID_mom()));
//     }
  if (counter==1) {

     if (match[0]==1) mom_1->Fill(pho2_p.ID_mom());
     if (match[1]==1) mom_1->Fill(pho1_p.ID_mom());


     }

  return counter;
}


int cms4GenSelector::DiffPt_DiffDr_hard_prompt_ttG(vector<GenParticle> phos, GenParticle pho1_p, GenParticle pho2_p, TH2F* diffPt_diffDr, TH1F* mom_0, TH1F* mom_1) {

  int counter = 0; // count number of hard photon matched to a prompt photon

  GenParticle pho1_h = phos[0];
  double pT1_h = pho1_h.P4().Pt();

  double diffPt1 = abs(pho1_h.P4().Pt() - pho1_p.P4().Pt());
  double diffPt2 = abs(pho1_h.P4().Pt() - pho2_p.P4().Pt());
  double diffDr1 = deltaR_p4(pho1_h.P4(), pho1_p.P4());
  double diffDr2 = deltaR_p4(pho1_h.P4(), pho2_p.P4());

  if (diffPt1 < diffPt2) {

     diffPt_diffDr->Fill(diffPt1/pT1_h, diffDr1);
     if (diffPt1/pT1_h < 0.2 && diffDr1 < 0.3) counter++;

     } else {

            diffPt_diffDr->Fill(diffPt2/pho1_h.P4().Pt(), diffDr2);
            if (diffPt2/pT1_h < 0.2 && diffDr2 < 0.3) counter++;

            }

//  if (counter==0){
     mom_0->Fill(abs(pho1_p.ID_mom()));
     mom_0->Fill(abs(pho2_p.ID_mom()));
//     }
  if (counter==1){
     mom_1->Fill((diffPt1<diffPt2) ? pho2_p.ID_mom() : pho1_p.ID_mom());
     }

  return counter;
}



double cms4GenSelector::deltaR_p4(TLorentzVector p4_1, TLorentzVector p4_2) {

  double dR = -1;
  double eta1 = p4_1.Eta(); double phi1 = p4_1.Phi();
  double eta2 = p4_2.Eta(); double phi2 = p4_2.Phi();

  dR = deltaR(eta1,phi1,eta2,phi2);

  return dR;
}


