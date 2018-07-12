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
   if (entry >= 100000) return kTRUE;
   if (entry%1000 == 0) cout << "entry: " << entry << endl;

   vector<GenParticle> genPhotons_prompt, genLeptons_prompt, genHadrons_prompt;
   vector<GenParticle> genPhotons_hard, genLeptons_hard, genHadrons_hard;
   vector<GenParticle> genPhotons_prompt_notHard;

   // 1, status1;                                                                                                                                                                                           
   // 2, prompt;                                                                                                                                                                                            
   // 3, hardProcess                                                                                                                                                                                        
   // 4, status 22 or 23                                                                                                                                                                                    

   LoadGenParticles(genPhotons_prompt, genLeptons_prompt, genHadrons_prompt, 2);
   LoadGenParticles(genPhotons_hard, genLeptons_hard, genHadrons_hard, 3);
   //LoadGenParticles(genPhotons_prompt_notHard, genLeptons_prompt, genHadrons_prompt, 5);                                                                                                                  

   genPhotons_prompt_notHard.clear();
   GetPromptNotHardPhoton(genPhotons_hard,genPhotons_prompt,genPhotons_prompt_notHard);

   //cout << genPhotons_hard.size() << ", " << genPhotons_prompt.size() << ", " << genPhotons_prompt_notHard.size() << endl;                                                                                

   //SetVals(genPhotons_prompt_notHard, genLeptons_hard, 0.3, 0.05);
   //SetVals(genPhotons_hard, genLeptons_hard, 0.3, 0.05);
   
   SetVals(genPhotons_prompt_notHard, genHadrons_hard, 0.3, 0.3);
   SetVals(genPhotons_hard, genHadrons_hard, 0.3, 0.3);                                                                                                                                                  

   //SetVals(genPhotons_hard, genLeptons_hard, genHadrons_hard, 0.3, 0.05);                                                                                                                                 
   //SetVals(genPhotons_prompt_notHard, genLeptons_hard, genHadrons_hard, 0.3, 0.05);                                                                                                                       
   //SetVals(genPhotons_hard, genPhotons_prompt, 0.3, 0.05);                                                                                                                                                
   //return kTRUE;                                                                         

   nPhoton_hard->Fill(genPhotons_hard.size());
   nLepton_hard->Fill(genLeptons_hard.size());
   nHadron_hard->Fill(genHadrons_hard.size());

   nPhoton_prompt->Fill(genPhotons_prompt_notHard.size());
   nLepton_prompt->Fill(genLeptons_prompt.size());
   nHadron_prompt->Fill(genHadrons_prompt.size());

   for (int iLep_h=0; iLep_h < int(genLeptons_hard.size()); iLep_h++) {
       int tmpMomId = genLeptons_hard[iLep_h].ID_mom();
       hardId_lep->Fill(genLeptons_hard[iLep_h].ID());
       lep_simpleGrandMa->Fill(abs(tmpMomId) > 30 ? 29 : abs(tmpMomId));
       }
   for (int iHad_h=0; iHad_h < int(genHadrons_hard.size()); iHad_h++) {
       int tmpId = genHadrons_hard[iHad_h].ID();
       int tmpMomId = genHadrons_hard[iHad_h].ID_mom();
       hardId_had->Fill(abs(tmpId));
       hadId_hadMomId_hard->Fill(abs(tmpId), abs(tmpMomId) > 30 ? 29 : abs(tmpMomId));       
       had_simpleGrandMa->Fill(abs(tmpMomId) > 30 ? 29 : abs(tmpMomId));
       //if (abs(tmpMomId) > 30) cout << "bigID: " << tmpMomId << endl;
   }
   nHardLep_nHardHad->Fill(genLeptons_hard.size(), genHadrons_hard.size());

   //FillSimplePhotonVar(genPhotons_prompt, pTPhoton_prompt, etaPhoton_prompt, phiPhoton_prompt, isoPhoton_prompt, minDrPhoton_prompt, minDrPhoton_passFrix_prompt, isoPhoton_passFrix_prompt);            
   FillSimplePhotonVar(genPhotons_hard, pTPhoton_hard, etaPhoton_hard, phiPhoton_hard, isoPhoton_hard, minDrPhoton_hard, minDrPhoton_passFrix_hard, isoPhoton_passFrix_hard);
   FillSimplePhotonVar(genPhotons_prompt_notHard, pTPhoton_prompt, etaPhoton_prompt, phiPhoton_prompt, isoPhoton_prompt, minDrPhoton_prompt, minDrPhoton_passFrix_prompt, isoPhoton_passFrix_prompt);

   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_u, 2);   
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_d, 1);     
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_c, 4);     
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_s, 3);     
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_t, 6);     
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_b, 5);     
   FillMinDrByGenID(genPhotons_hard, minDrPhoton_hard_g, 21);     

   nPrompt_nPromptNotHard->Fill(genPhotons_prompt.size(),genPhotons_prompt_notHard.size());

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

   outputRootFile = new TFile("tmp.root","RECREATE");
   outputRootFile->cd();

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

   minDrPhoton_hard_u->Write();
   minDrPhoton_hard_d->Write();
   minDrPhoton_hard_c->Write();
   minDrPhoton_hard_s->Write();
   minDrPhoton_hard_t->Write();
   minDrPhoton_hard_b->Write();
   minDrPhoton_hard_g->Write();
   
   lep_simpleGrandMa->Write();
   had_simpleGrandMa->Write();

   outputRootFile->Close();

}

void cms4GenSelector::LoadGenParticles(vector<GenParticle>& pho, vector<GenParticle>& lep, vector<GenParticle>& had, int genCat = 0, bool photonOnly = false)
{

  int nGenParticles = ints_genMaker_genpsid_CMS3_obj.GetSize();
  for (int i = 0; i < nGenParticles; i++) {

    float genP1 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fX[i];
    float genP2 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fY[i];
    float genP3 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fZ[i];
    float genP4 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fT[i];
    int genID = ints_genMaker_genpsid_CMS3_obj[i];
    int simpleMomId = ints_genMaker_genpsidsimplemother_CMS3_obj[i];
//    int simpleMomId = ints_genMaker_genpsidmother_CMS3_obj[i];
//    int simpleMomId =  ints_genMaker_genpsidsimplegrandma_CMS3_obj[i];
    int genStatus = ints_genMaker_genpsstatus_CMS3_obj[i];
    bool isPrompt = (*bools_genMaker_genpsIsPromptFinalState_CMS3_obj)[i];
    bool isHardProcess = (*bools_genMaker_genpsIsHardProcess_CMS3_obj)[i];


    //genCat:                                                                                                                                                                                               
                                                                                               
    //1, status1; 2, prompt; 3, hardProcess, 4, status22or23                                                                                                                                                
    if (genCat == 1 && genStatus != 1) continue;
    if (genCat == 2 && !(isPrompt) ) continue;
    if (genCat == 3 && !(/*isHardProcess &&*/ ( (genStatus == 21 && genID!=21) || genStatus == 22 || genStatus == 23 || genStatus == 24)) ) continue;
    if (genCat == 4 && !(genStatus == 22 || genStatus == 23)) continue;
    if (genCat == 5 && !(isPrompt && !isHardProcess) ) continue;
    //cout << genP1 << "," << genP2 << "," << genP3 << "," << genP4 << "," << genID << "," << simpleMomId << "," << genStatus << endl;                                                                      
    GenParticle tmpGenP = GenParticle(genP1,genP2,genP3,genP4,genID,simpleMomId,genStatus,isPrompt,isHardProcess);

    if (genID == 22) pho.push_back(tmpGenP);
    if (photonOnly) continue;
    if (abs(genID) == 11 || abs(genID) == 13 || abs(genID) == 15) lep.push_back(tmpGenP);
    if ((abs(genID) > 100 /*&& (genP1*genP1+genP2*genP2) > 100*/) || (abs(genID) > 0 && abs(genID) < 10) || genID == 21) had.push_back(tmpGenP);

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
    gp_cat1[igp].SetSmallestDrId(smallestDr_id);

    for (int k = 0; k < nBinsForFrix; k++) {
      if (ets[k] > gp_et) {
	passFrix = false; break;
      }
    }
    // cout << "pass/fail: " << (passFrix?1:0) << endl; cout << endl;                                                                                                                                       
                                                                                                    
    gp_cat1[igp].SetFrixioneIso(passFrix);

    }// loop particle type which is interesting for study                                      
}
                                                                                                                                                                                                            

// set iso,frxioneIso, nearestDr, only change for cat1 particle
void cms4GenSelector::SetVals(vector<GenParticle>& gp_cat1, vector<GenParticle> gp_cat2, double cone_iso, double cone_frix)
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
       int smallestDr_id = 0;
       int smallestDr_momId = 0;
       double smallestDr_pT = -1;
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
              smallestDr_status = gp2.Status();
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
       gp_cat1[igp].SetSmallestDrId(smallestDr_id);
       gp_cat1[igp].SetSmallestDrMomId(smallestDr_momId);

       for (int k = 0; k < nBinsForFrix; k++) {
           if (ets[k] > gp_et) {
              passFrix = false; break;
              }
           }
       // cout << "pass/fail: " << (passFrix?1:0) << endl; cout << endl;
       gp_cat1[igp].SetFrixioneIso(passFrix);

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



void cms4GenSelector::FillSimplePhotonVar(vector<GenParticle> phos, TH1F* pT, TH1F* eta, TH1F* phi, TH1F* iso, TH1F* minDr, TH1F* minDrPassFrix, TH1F* isoPassFrix)
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

      }

} 

void cms4GenSelector::GetPromptNotHardPhoton(vector<GenParticle> hard, vector<GenParticle> prompt, vector<GenParticle>& prompt_notHard)
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

void cms4GenSelector::FillMinDrByGenID(vector<GenParticle> phos, TH1F* minDr, int id){

  for (int i = 0; i < int(phos.size()); i++){

      GenParticle pho = phos[i];
      TLorentzVector phop4 = pho.P4();
      double phopt = phop4.Pt();
      double phoeta = phop4.Eta();
      double phophi = phop4.Phi();

      if (abs(phoeta) > 2.6 || phopt < 10) continue;
      double mindr = pho.GetSmallestDr();
      int minDrId = pho.GetSmallestDrId();
      int minDrMomId = pho.GetSmallestDrMomId();
//cout << "minDrMomId: " << minDrMomId << endl;
      if (abs(minDrId) == id && abs(minDrMomId) == 24) minDr->Fill(mindr);
//      if (abs(mindr) < 0.1 && id == 2 ) cout << "id: " << minDrId << ", momId: " << minDrMomId << endl;

  }


}
