/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystRunAction.cpp
/// \brief Implementation of the TmCrystRunAction class

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "TmCrystRunAction.hh"
#include "TmCrystRun.hh"
#include "TmCrystPrimaryGeneratorAction.hh"

TmCrystRunAction::TmCrystRunAction(): 
G4UserRunAction()
{ }

TmCrystRunAction::~TmCrystRunAction()
{ }

G4Run* TmCrystRunAction::GenerateRun()
{ return new TmCrystRun; }

void TmCrystRunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void TmCrystRunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();

  const TmCrystPrimaryGeneratorAction* generatorAction = 
    static_cast<const TmCrystPrimaryGeneratorAction*>(
        G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String partName;
  if (generatorAction) 
  {
    G4ParticleDefinition* particle 
      = generatorAction->GetParticleGun()->GetParticleDefinition();
      partName = particle->GetParticleName();
  } 
        
  //print
  G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------"
     << G4endl
     << "  The run was " << nofEvents << " events ";
     
}