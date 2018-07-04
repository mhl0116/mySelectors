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
   if (entry > 1000) return kTRUE; 
 
   cout << "entry: " << entry << endl;

   vector<GenParticle> genPhotons, genLeptons, genHadrons;
   vector<GenParticle> genPhotons_s1, genLeptons_s1, genHadrons_s1;
   vector<GenParticle> genPhotons_prompt, genLeptons_prompt, genHadrons_prompt;
   vector<GenParticle> genPhotons_hard, genLeptons_hard, genHadrons_hard;

   // 1, status1; 2, prompt; 3, hardProcess
   LoadGenParticles(genPhotons_s1, genLeptons_s1, genHadrons_s1, 1);
   LoadGenParticles(genPhotons_prompt, genLeptons_prompt, genHadrons_prompt, 2);
   LoadGenParticles(genPhotons_hard, genLeptons_hard, genHadrons_hard, 3);
   //cout << "nGen: " << genPhotons.size() << ", " << genLeptons.size() << ", " << genHadrons.size() << endl;

   nPromptPhoton->Fill(nPromptPho);
   nPromptPhoton_iso->Fill(nPromptPhoIso);
   nHardProcessPhoton->Fill(nHardProcessPho);
   nPromptPhotonFromHardProcess->Fill(nPromptPhoFromHardPro);
   nPromptPhotonFromHardProcess_notFromPho->Fill(nPromptPhoFromHardPro_notFromPho);
   nPromptPhoton_notFromPho->Fill(nPromptPho_notFromPho);

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

   nPromptPhoton->Write();
   nPromptPhoton_iso->Write();
   nHardProcessPhoton->Write();
   nPromptPhotonFromHardProcess->Write();
   nPromptPhotonFromHardProcess_notFromPho->Write();
   nPromptPhoton_notFromPho->Write();

   nGenParticle->Write();
   nGenParticle_stat1->Write();
   nGenParticle_stat22_23->Write();
   nGenParticle_hardProcess->Write();


   outputRootFile->Close();

}

void ttGamma::LoadGenParticles(vector<GenParticle>& pho, vector<GenParticle>& lep, vector<GenParticle>& had, int genCat = 0)
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

       GenParticle tmpGenP = GenParticle(genP1,genP2,genP3,genP4,genID,simpleMomId,genStatus,isPrompt,isHardProcess);

       //genCat:
       //1, status1; 2, prompt; 3, hardProcess
   
       if (genCat == 1 && genStatus != 1) continue;
       if (genCat == 2 && !isPrompt) continue;
       if (genCat == 3 && !isHardProcess) continue;

       if (genID == 22) pho.push_back(tmpGenP);
       if (abs(genID) == 11 || abs(genID) == 13 || abs(genID) == 15) lep.push_back(tmpGenP);
       if (abs(genID) > 100) had.push_back(tmpGenP);

       }
}

// set iso,frxioneIso, nearestDr, only change for cat1 particle
void ttGamma::SetVals(vector<GenParticle>& gp_cat1, vector<GenParticle> gp_cat2, vector<GenParticle> gp_cat3, double cone_iso, double cone_frix)
{

   const int nBinsForFrix = 10;
   const double initConeFrix = 0.0001;
   vector<GenParticle> tmpGP; tmpGP.clear();
   tmpGP.insert(tmpGP.end(), gp_cat1.begin(), gp_cat1.end());
   tmpGP.insert(tmpGP.end(), gp_cat2.begin(), gp_cat2.end());
   tmpGP.insert(tmpGP.end(), gp_cat3.begin(), gp_cat3.end());

   for (int igp = 0; igp < int(gp_cat1.size); igp++) {
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
           if (i == igp) continue;

           GenParticle gp2 = tmpGP[i];
           double gp2_et = gp2.P4().Et();
           double gp2_pt = gp2.P4().Pt();
           double gp2_eta = gp2.P4().Eta();
           double gp2_phi = gp2.P4().Phi();
           double dR = DeltaR(gp_eta,gp_phi,gp2_eta,gp2_phi);

           if (dR < smallestDr) smallestDr = dR; // smallest distance w.r.s interesting gen particle
           if (dR < cone_iso) pTSum += gp_pt2; // used for isolation cut

           for (int j = 0; j < nBinsForFrix; j++) { // save relavant info for frixione isolation
               double cone_var = (initConeFrix + j*cone_frix/nBinsForFrixione);
               if (dR < cone_var && cone_var < cone_frix) ets[j] += gp2_et/(1-cos(cone_var) )*(1-cos(cone_frix) );

               }
           }// loop all other particles

       gp_cat1[igp].SetIso(pTSum/gp_pt);
       gp_cat1[igp].SetSmallestDr(smallestDr);

       for (int k = 0; k < nBinsForFrix; k++) {
           if (ets[k] > gp_et) passFrix = false; break;
           }

       gp_cat1[igp].SetFrixioneIso(passFrix);

       }// loop particle type which is interesting for study
}
