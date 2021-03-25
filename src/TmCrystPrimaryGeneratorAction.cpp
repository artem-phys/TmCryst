/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystPrimaryGeneratorAction.cpp
/// \brief Implementation of the TmCrystPrimaryGeneratorAction class

#include <random>
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4DataSet.hh"

#include "TmCrystPrimaryGeneratorAction.hh"



TmCrystPrimaryGeneratorAction::TmCrystPrimaryGeneratorAction():G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEnvelopeBox(0)
  {
  //Number of emitted particles
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  //Load gamma background
  const int nrolls = 10; // number of experiments
  const int nstars = 10;   // maximum number of stars to distribute
  std::default_random_engine generator;
  std::discrete_distribution<int> distribution {2,2,1,1,2,2,1,1,2,2};

  int p[nrolls]={};
  for (int i=0; i<nrolls; ++i) {
    int number = distribution(generator);
    ++p[number];
  }

  std::cout << "a discrete_distribution:" << std::endl;
  for (int i=0; i<10; ++i)
    std::cout << i << ": " << std::string(p[i]*nstars/nrolls,'*') << std::endl;

  } 

TmCrystPrimaryGeneratorAction::~TmCrystPrimaryGeneratorAction()
{
  delete fParticleGun;
}




void TmCrystPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //Background loading


  //Random-generated direction of isotropic radiation
  G4double theta = acos(2*G4UniformRand()-1);
  G4double phi = G4UniformRand()*2*3.141592653979;
  G4double x = sin(theta)*cos(phi);
  G4double y = sin(theta)*sin(phi);
  G4double z = cos(theta);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));

  //Random-generated particle birth position in the source volume
  G4double x_pos = 0*cm;
  G4double y_pos = 0*cm;
  G4double z_pos = 0*cm;

  G4double det_size = 10.8 * mm;

  G4double x0 = x_pos+(2*G4UniformRand()-1)*det_size/2;
  G4double y0 = y_pos+(2*G4UniformRand()-1)*det_size/2;
  G4double z0 = z_pos+(2*G4UniformRand()-1)*det_size/2;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  //Set Energy 
  G4double E = 0 * keV;
  G4double ionCharge = 0;
  fParticleGun->SetParticleEnergy(E);
  fParticleGun->SetParticleCharge(ionCharge);

  //Set ions as emitted particles
  G4ParticleDefinition* Th232 = G4IonTable::GetIonTable()->GetIon(90,232);
  G4ParticleDefinition* U = G4IonTable::GetIonTable()->GetIon(92,238);
  G4ParticleDefinition* Am241 = G4IonTable::GetIonTable()->GetIon(95,241);
  

  //Ion abundance - data from raw product estimation
  G4int NTh232 = 1000;
  G4int NU = 100;
  G4int NAm241 = 10000;
  G4int NAllRadioactive = NTh232+NU+NAm241;

  //Random ion definition with respect to ions' abundances
  G4int random_nucleus = int(NAllRadioactive*G4UniformRand());

  if (random_nucleus < NTh232)
  {
    fParticleGun->SetParticleDefinition(Th232);
  }
  else
  {
    if (random_nucleus < NU)
    {
    fParticleGun->SetParticleDefinition(U);
    }
    else
    {
      fParticleGun->SetParticleDefinition(Am241);
    }
  }
  

  //Primary event generation
  fParticleGun->GeneratePrimaryVertex(anEvent);
}