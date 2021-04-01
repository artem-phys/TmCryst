/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystPrimaryGeneratorAction.hh
/// \brief Definition of the TmCrystPrimaryGeneratorAction class

#ifndef TmCrystPrimaryGeneratorAction_h
#define TmCrystPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "globals.hh"


class G4ParticleGun;
class G4Event;
class G4Box;

class TmCrystPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    TmCrystPrimaryGeneratorAction();
    virtual ~TmCrystPrimaryGeneratorAction();
    void LoadBackgroundGammasEnergyCDF(std::string file);
    double RandomGammaEnergy();

    // Sets the parameters of the source of the initial particles
    virtual void GeneratePrimaries(G4Event*);         
  
    // Access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    

    G4ParticleGun*  fParticleGun;
    G4Box* fEnvelopeBox;// temp volume variable
    G4IonTable* ionTable;
    G4ParticleDefinition* ion;
    G4int Egamma[160];
    G4double CDF[160];
};
#endif
