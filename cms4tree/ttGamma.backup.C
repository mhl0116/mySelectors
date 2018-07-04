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
   //tree_.Scan("evt_event:genps_id:genps_p4.pt():genps_status:genps_isPromptFinalState:genps_isHardProcess:genps_id_simplemother", "genps_id==22 && genps_isPromptFinalState && genps_p4.pt()>10")
   //cout << "nGENParticle: " << ints_genMaker_genpsid_CMS3_obj.GetSize() << endl;

   int nPromptPho = 0;
   int nHardProcessPho = 0;
   int nPromptPhoFromHardPro = 0;
   int nPromptPhoFromHardPro_notFromPho = 0;
   int nPromptPho_notFromPho = 0;
   int nPromptPhoIso = 0;

   int nGenStat1 = 0;
   int nGenStat22_23 = 0;
   int nGenHard = 0;

   nGenParticle->Fill(int(ints_genMaker_genpsid_CMS3_obj.GetSize()));

   for (int i = 0; i < int(ints_genMaker_genpsid_CMS3_obj.GetSize()); i++) {

       int genID = ints_genMaker_genpsid_CMS3_obj[i];
       float genP1 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fX[i];
       float genP2 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fY[i];
       float genP3 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fZ[i];
       float genP4 = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fT[i];
       TLorentzVector p4(0,0,0,0); p4.SetPxPyPzE(genP1,genP2,genP3,genP4);
       float genPt = p4.Pt();
       float genEta = p4.Eta(); 
       float genPhi = p4.Phi();
       int simpleMomId = ints_genMaker_genpsidsimplemother_CMS3_obj[i];
 
       bool isPrompt = (*bools_genMaker_genpsIsPromptFinalState_CMS3_obj)[i];
       bool isHardProcess = (*bools_genMaker_genpsIsHardProcess_CMS3_obj)[i];
       int genState = ints_genMaker_genpsstatus_CMS3_obj[i];

       if (genState == 1) nGenStat1++;
       if (genState == 22  || genState == 23) nGenStat22_23++;
       if (isHardProcess) nGenHard++;

       bool phoIso = true;
       float dR_ = 0.00001;
       /*
       do {

	 float eSum = 0;

       for (int j = 0; j < int(ints_genMaker_genpsid_CMS3_obj.GetSize()); j++) {

	   if (j == i) continue;
           int isFinalState1_ = ints_genMaker_genpsstatus_CMS3_obj[j];
           if (isFinalState1_ != 1) continue;
	   int genID_ = ints_genMaker_genpsid_CMS3_obj[j];
           if (abs(genID_) == 12 || abs(genID_) == 14 || abs(genID_) == 16) continue;
//           int isFinalState1_ = ints_genMaker_genpsstatus_CMS3_obj[j];
//           if (isFinalState1_ != 1) continue;
	   float genP1_ = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fX[j];
	   float genP2_ = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fY[j];
	   float genP3_ = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fZ[j];
	   float genP4_ = floatROOTMathPxPyPzE4DROOTMathLorentzVectors_genMaker_genpsp4_CMS3_obj_fCoordinates_fT[j];
	   TLorentzVector p4_(0,0,0,0); p4_.SetPxPyPzE(genP1_,genP2_,genP3_,genP4_);
	   float genPt_ = p4_.Pt();
	   float genEta_ = p4_.Eta();
	   float genPhi_ = p4.Phi();

           float dR = deltaR(genEta,genPhi,genEta_,genPhi_);
 
           if (dR<=dR_*(1-cos(dR_))/(1-cos(0.05))) eSum+=genPt_;

           }

       if (eSum > genPt) {phoIso = false; break;}

       dR_ += 0.005;

//cout << "dR_: " << dR_ << endl;
       } while (dR_ <= 0.05);
       */
       if (genID == 22 && genPt > 10 && abs(genEta) < 2.5) {

         if (genState != 1) continue;
	 if (isPrompt) nPromptPho++;
         if (isPrompt && phoIso) nPromptPhoIso++;
	 if (isHardProcess) nHardProcessPho++;
	 if (isPrompt && isHardProcess) nPromptPhoFromHardPro++;
	 if (isPrompt && isHardProcess && simpleMomId != 22) nPromptPhoFromHardPro_notFromPho++;
	 if (isPrompt && simpleMomId != 22) nPromptPho_notFromPho++;

       }

       nGenParticle_stat1->Fill(nGenStat1);
       nGenParticle_stat22_23->Fill(nGenStat22_23);
       nGenParticle_hardProcess->Fill(nGenHard);
    

       }

   

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
