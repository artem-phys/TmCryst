/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystPrimaryGeneratorAction.cpp
/// \brief Implementation of the TmCrystPrimaryGeneratorAction class

#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "TmCrystPrimaryGeneratorAction.hh"



TmCrystPrimaryGeneratorAction::TmCrystPrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEnvelopeBox(0)
  {
  //Number of emitted particles
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  //Set gamma as emitted particle
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);

  //Set gamma Energy 
  G4double E = G4UniformRand() * 100 * keV;
  fParticleGun->SetParticleEnergy(E);
  } 

TmCrystPrimaryGeneratorAction::~TmCrystPrimaryGeneratorAction()
{
  delete fParticleGun;
}



void TmCrystPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
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

  G4double x0 = x_pos+(2*G4UniformRand()-1)*3*cm;
  G4double y0 = y_pos+(2*G4UniformRand()-1)*3*cm;
  G4double z0 = z_pos+(2*G4UniformRand()-1)*3*cm;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  //Primary event generation
  fParticleGun->GeneratePrimaryVertex(anEvent);
}