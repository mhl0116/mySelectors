#ifndef myClasses_H
#define myClasses_H

#include <TLorentzVector.h>
class GenParticle
{

public:

  GenParticle();
  GenParticle(double px, double py, double pz, double e, int id, int id_mom, int status, bool isPrompt, bool isHard);

  TLorentzVector P4() {return p4_;}
  int ID() {return id_;}
  int ID_mom() {return id_mom_;}
  int Status() {return status_;}
  bool IsPrompt() {return isPrompt_;}
  bool IsHard() {return isHard_;}

  void SetIso(double iso) {iso_ = iso;} 
  void SetFrixioneIso(bool passFrixioneIso) {passFrixioneIso_ = passFrixioneIso;}
  void SetSmallestDr(double smallestDr) {smallestDr_ = smallestDr;}

  double GetIso() {return iso_;}
  bool GetFrixioneIso() {return passFrixioneIso_;}
  double GetSmallestDr() {return smallestDr_;}
 
  // two vectors saving (1-cosDelta)/(1-cosDelta_0) and sumEt/Photon eta for various Delta within Delta_0

  ~GenParticle();

private:

  TLorentzVector p4_;
  int id_;
  int id_mom_;
  int status_;
  bool isPrompt_;
  bool isHard_;

  double iso_;
  bool passFrixioneIso_;
  double smallestDr_; 

};


GenParticle::GenParticle() {}

GenParticle::GenParticle(double px, double py, double pz, double e, int id, int id_mom, int status, bool isPrompt, bool isHard)
{

 id_ = id; id_mom_ = id_mom; status_ = status;
 isPrompt_ = isPrompt; isHard_ = isHard;

 p4_.SetPxPyPzE(px,py,pz,e);

// if (p4_.Pt() == 0) cout << "px: " << px << ", py: " << py << ", pz: " << pz << ", e: " << e << ", status: " << status << endl;
}

GenParticle::~GenParticle() {}


#endif
