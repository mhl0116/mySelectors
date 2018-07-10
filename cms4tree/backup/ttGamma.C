#define ttGamma_cxx
// The class definition in ttGamma.h has been generated automatically
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
// root> T->Process("ttGamma.C")
// root> T->Process("ttGamma.C","some options")
// root> T->Process("ttGamma.C+")
//


#include "ttGamma.h"
#include <TH2.h>
#include <TStyle.h>
#include "DataFormats/Math/interface/deltaR.h"

void ttGamma::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void ttGamma::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t ttGamma::Process(Long64_t entry)
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
   if (entry > 10000) return kTRUE; 
 
   if (entry%1000 == 0) cout << "entry: " << entry << endl;

   vector<GenParticle> genPhotons, genLeptons, genHadrons;
   vector<GenParticle> genPhotons_s1, genLeptons_s1, genHadrons_s1;
   vector<GenParticle> genPhotons_prompt, genLeptons_prompt, genHadrons_prompt;
   vector<GenParticle> genPhotons_hard, genLeptons_hard, genHadrons_hard;
   vector<GenParticle> genPhotons_s22_23, genLeptons_s22_23, genHadrons_s22_23;

   vector<GenParticle> genPhotons_prompt_notHard;
   // 1, status1;
   // 2, prompt;
   // 3, hardProcess
   // 4, status 22 or 23
   //LoadGenParticles(genPhotons_s1, genLeptons_s1, genHadrons_s1, 1);
   LoadGenParticles(genPhotons_prompt, genLeptons_prompt, genHadrons_prompt, 2);
   LoadGenParticles(genPhotons_hard, genLeptons_hard, genHadrons_hard, 3);
   //LoadGenParticles(genPhotons_prompt_notHard, genLeptons_prompt, genHadrons_prompt, 5);
   //LoadGenParticles(genPhotons_s22_23, genLeptons_s22_23, genHadrons_s22_23, 4);

   genPhotons_prompt_notHard.clear();
   GetPromptNotHardPhoton(genPhotons_hard,genPhotons_prompt,genPhotons_prompt_notHard);

   //cout << genPhotons_hard.size() << ", " << genPhotons_prompt.size() << ", " << genPhotons_prompt_notHard.size() << endl;

   //SetVals(genPhotons_s1, genLeptons_s1, genHadrons_s1, 0.3, 0.05);
   //SetVals(genPhotons_prompt_notHard, genLeptons_hard, genHadrons_hard, 0.3, 0.05);

   SetVals(genPhotons_prompt_notHard, genLeptons_hard, 0.3, 0.05);
   SetVals(genPhotons_hard, genLeptons_hard, 0.3, 0.05);
   //SetVals(genPhotons_prompt_notHard, genHadrons_hard, 0.3, 0.05);                                                                                                                       
   //SetVals(genPhotons_hard, genHadrons_hard, 0.3, 0.05);

   //SetVals(genPhotons_hard, genLeptons_hard, genHadrons_hard, 0.3, 0.05);
   //SetVals(genPhotons_s22_23, genLeptons_s22_23, genHadrons_s22_23, 0.3, 0.05);
   //SetVals(genPhotons_hard, genPhotons_prompt, 0.3, 0.05);
   //return kTRUE;

   nPhoton_s1->Fill(genPhotons_s1.size()); nPhoton_prompt->Fill(genPhotons_prompt_notHard.size()); nPhoton_hard->Fill(genPhotons_hard.size());
   nLepton_s1->Fill(genLeptons_s1.size()); nLepton_prompt->Fill(genLeptons_prompt.size()); nLepton_hard->Fill(genLeptons_hard.size());
   nHadron_s1->Fill(genHadrons_s1.size()); nHadron_prompt->Fill(genHadrons_prompt.size()); nHadron_hard->Fill(genHadrons_hard.size());
   nPhoton_s22_23->Fill(genPhotons_s22_23.size());
   nLepton_s22_23->Fill(genLeptons_s22_23.size());
   nHadron_s22_23->Fill(genHadrons_s22_23.size());

   //FillSimplePhotonVar(genPhotons_s1, pTPhoton_s1, etaPhoton_s1, phiPhoton_s1, isoPhoton_s1, minDrPhoton_s1, minDrPhoton_passFrix_s1, isoPhoton_passFrix_s1);
   //FillSimplePhotonVar(genPhotons_prompt, pTPhoton_prompt, etaPhoton_prompt, phiPhoton_prompt, isoPhoton_prompt, minDrPhoton_prompt, minDrPhoton_passFrix_prompt, isoPhoton_passFrix_prompt);
   
   FillSimplePhotonVar(genPhotons_hard, pTPhoton_hard, etaPhoton_hard, phiPhoton_hard, isoPhoton_hard, minDrPhoton_hard, minDrPhoton_passFrix_hard, isoPhoton_passFrix_hard);
   //FillSimplePhotonVar(genPhotons_s22_23, pTPhoton_s22_23, etaPhoton_s22_23, phiPhoton_s22_23, isoPhoton_s22_23, minDrPhoton_s22_23, minDrPhoton_passFrix_s22_23, isoPhoton_passFrix_s22_23);
   
   FillSimplePhotonVar(genPhotons_prompt_notHard, pTPhoton_prompt, etaPhoton_prompt, phiPhoton_prompt, isoPhoton_prompt, minDrPhoton_prompt, minDrPhoton_passFrix_prompt, isoPhoton_passFrix_prompt);     

   hardLep_id->Fill(genLeptons_hard[0].ID());
   hardLep_momId->Fill(genLeptons_hard[0].ID_mom());

   nPrompt_nPromptNotHard->Fill(genPhotons_prompt.size(),genPhotons_prompt_notHard.size());
   /*
   if (genPhotons_prompt.size() - genPhotons_prompt_notHard.size() == 1) {
      FillSimplePhotonVar(genPhotons_prompt_notHard, pTPhoton_prompt, etaPhoton_prompt, phiPhoton_prompt, isoPhoton_prompt, minDrPhoton_prompt, minDrPhoton_passFrix_prompt, isoPhoton_passFrix_prompt);
      FillSimplePhotonVar(genPhotons_hard, pTPhoton_hard, etaPhoton_hard, phiPhoton_hard, isoPhoton_hard, minDrPhoton_hard, minDrPhoton_passFrix_hard, isoPhoton_passFrix_hard);

      }
   */
   return kTRUE;
}

void ttGamma::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ttGamma::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   outputRootFile = new TFile("tmp.root","RECREATE");
   outputRootFile->cd();

   nPhoton_s1->Write();
   nPhoton_prompt->Write();
   nPhoton_hard->Write();
   nLepton_s1->Write();
   nLepton_prompt->Write();
   nLepton_hard->Write();
   nHadron_s1->Write();
   nHadron_prompt->Write();
   nHadron_hard->Write();

   nPhoton_s22_23->Write();
   nLepton_s22_23->Write(); 
   nHadron_s22_23->Write(); 

   pTPhoton_s1->Write();
   etaPhoton_s1->Write();
   phiPhoton_s1->Write();
   isoPhoton_s1->Write();
   minDrPhoton_s1->Write();
   minDrPhoton_passFrix_s1->Write();
   isoPhoton_passFrix_s1->Write();

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

   pTPhoton_s22_23->Write();
   etaPhoton_s22_23->Write();
   phiPhoton_s22_23->Write();
   isoPhoton_s22_23->Write();
   minDrPhoton_s22_23->Write();
   minDrPhoton_passFrix_s22_23->Write();
   isoPhoton_passFrix_s22_23->Write();

   nPrompt_nPromptNotHard->Write();
   hardLep_id->Write();
   hardLep_momId->Write();

   outputRootFile->Close();

// }

void ttGamma::LoadGenParticles(vector<GenParticle>& pho, vector<GenParticle>& lep, vector<GenParticle>& had, int genCat = 0, bool photonOnly = false)
{

   int nGenParticles = ints_genMaker_genpsid_CMS3_obj.GetSize();
   for (int i = 0; i < nGenParticles; i++) {

       float genP1 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fX[i];
       float genP2 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fY[i];
       float genP3 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fZ[i];
       float genP4 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fT[i];
       int genID = ints_genMaker_genpsid_CMS3_obj[i];
       int simpleMomId = ints_genMaker_genpsidsimplemother_CMS3_obj[i];
       int genStatus = ints_genMaker_genpsstatus_CMS3_obj[i];
       bool isPrompt = (*bools_genMaker_genpsIsPromptFinalState_CMS3_obj)[i];
       bool isHardProcess = (*bools_genMaker_genpsIsHardProcess_CMS3_obj)[i];

       //genCat:
       //1, status1; 2, prompt; 3, hardProcess, 4, status22or23
   
       if (genCat == 1 && genStatus != 1) continue;
       //if (genP1 == 0 && genP2 == 0) cout << "checkStatus: " << genStatus << ", genCat: " << genCat << endl;
       if (genCat == 2 && !(isPrompt) ) continue;
       if (genCat == 3 && !(isHardProcess/* && genStatus != 21 && abs(genID) != 6*/) ) continue;
       if (genCat == 4 && !(genStatus == 22 || genStatus == 23)) continue;
       if (genCat == 5 && !(isPrompt && !isHardProcess) ) continue;

       //cout << genP1 << "," << genP2 << "," << genP3 << "," << genP4 << "," << genID << "," << simpleMomId << "," << genStatus << endl;
       GenParticle tmpGenP = GenParticle(genP1,genP2,genP3,genP4,genID,simpleMomId,genStatus,isPrompt,isHardProcess);

       if (genID == 22) pho.push_back(tmpGenP);
       if (photonOnly) continue;
       if (abs(genID) == 11 || abs(genID) == 13 || abs(genID) == 15) lep.push_back(tmpGenP);
       if ((abs(genID) > 100 /*&& (genP1*genP1+genP2*genP2) > 100*/) || abs(genID) < 11 || genID == 21) had.push_back(tmpGenP);

       }
}

// set iso,frxioneIso, nearestDr, only change for cat1 particle
void ttGamma::SetVals(vector<GenParticle>& gp_cat1, vector<GenParticle> gp_cat2, vector<GenParticle> gp_cat3, double cone_iso, double cone_frix)
{

   const int nBinsForFrix = 10;
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
       double smallestDr_pT = -1;
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
              smallestDr_pT = gp2_pt;
	   }
           if (dR < cone_iso) {pTSum += gp2_pt; /*cout << "pTSum: " << pTSum << endl;*/} // used for isolation cut

           for (int j = 0; j < nBinsForFrix; j++) { // save relavant info for frixione isolation
               double cone_var = (initConeFrix + j*cone_frix/nBinsForFrix);
               if (dR < cone_var && cone_var < cone_frix) ets[j] += gp2_et/(1-cos(cone_var) )*(1-cos(cone_frix) );

               }
           }// loop all other particles

       // cout << "pTSum: " << pTSum << ", gp_pt: " << gp_pt << endl;
       gp_cat1[igp].SetIso(pTSum);
       gp_cat1[igp].SetSmallestDr(smallestDr);

       for (int k = 0; k < nBinsForFrix; k++) {
           if (ets[k] > gp_et) {
              passFrix = false; break;
              }
           }
       // cout << "pass/fail: " << (passFrix?1:0) << endl; cout << endl;
       gp_cat1[igp].SetFrixioneIso(passFrix);

       /*
       if (!passFrix ) {
          cout << "Et: " << gp_et << ", smallestDr: " << smallestDr << endl;
          for (int k = 0; k < nBinsForFrix; k++) {
              cout << "ets[" << k << "]: " << ets[k] << endl;
              }
       }

       if (smallestDr > 4) cout << "pTSum: " << pTSum << ", smallestDr: " << smallestDr << endl;
       */
       // if (smallestDr < 0.3) cout << "cloestID: " << smallestDr_id << ", cloestPt: " << smallestDr_pT << endl;
       }// loop particle type which is interesting for study
}


// set iso,frxioneIso, nearestDr, only change for cat1 particle
void ttGamma::SetVals(vector<GenParticle>& gp_cat1, vector<GenParticle> gp_cat2, double cone_iso, double cone_frix)
{

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

           if (dR < smallestDr) smallestDr = dR; // smallest distance w.r.s interesting gen particle
           if (dR < cone_iso) {pTSum += gp2_pt; /*cout << "pTSum: " << pTSum << endl;*/} // used for isolation cut

           for (int j = 0; j < nBinsForFrix; j++) { // save relavant info for frixione isolation
               double cone_var = (initConeFrix + j*cone_frix/nBinsForFrix);
               if (dR < cone_var && cone_var < cone_frix) ets[j] += gp2_et/(1-cos(cone_var) )*(1-cos(cone_frix) );

               }
           }// loop all other particles

       // cout << "pTSum: " << pTSum << ", gp_pt: " << gp_pt << endl;
       gp_cat1[igp].SetIso(pTSum);
       gp_cat1[igp].SetSmallestDr(smallestDr);

       for (int k = 0; k < nBinsForFrix; k++) {
           if (ets[k] > gp_et) {
              passFrix = false; break;
              }
           }
       // cout << "pass/fail: " << (passFrix?1:0) << endl; cout << endl;
       gp_cat1[igp].SetFrixioneIso(passFrix);

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



void ttGamma::FillSimplePhotonVar(vector<GenParticle> phos, TH1F* pT, TH1F* eta, TH1F* phi, TH1F* iso, TH1F* minDr, TH1F* minDrPassFrix, TH1F* isoPassFrix)
{

  for (int i = 0; i < int(phos.size()); i++) {

      GenParticle pho = phos[i];
      TLorentzVector phop4 = pho.P4();
      double phopt = phop4.Pt();
      double phoeta = phop4.Eta();
      double phophi = phop4.Phi();

      if (abs(phoeta) > 2.6 || phopt < 10) continue;
      pT->Fill(phopt);
      eta->Fill(phoeta);
      phi->Fill(phophi);
      iso->Fill(pho.GetIso());
      minDr->Fill(pho.GetSmallestDr());
      //      cout << "minDr: " << pho.GetSmallestDr() << endl;
      if (pho.GetFrixioneIso() ) {
         minDrPassFrix->Fill(pho.GetSmallestDr());
         isoPassFrix->Fill(pho.GetIso());
         }

      if (phopt < 1) {

       //  cout << "pt: " << phopt << ", eta: " << phoeta << ", phi: " << phophi << ", status: " << pho.Status() << endl;

         }

      }

} 

void ttGamma::GetPromptNotHardPhoton(vector<GenParticle> hard, vector<GenParticle> prompt, vector<GenParticle>& prompt_notHard)
{

  for (int i = 0; i < int(prompt.size()); i++) {
      double eta1 = prompt[i].P4().Eta();
      double phi1 = prompt[i].P4().Phi();

      for (int j = 0 ; j < int(hard.size()); j++) {
	double eta2 = hard[j].P4().Eta();
        double phi2 = hard[j].P4().Phi();
          
        double dR = deltaR(eta1,phi1,eta2,phi2);

        if (dR > 0.3) prompt_notHard.push_back(prompt[i]);

        }
      }
}
