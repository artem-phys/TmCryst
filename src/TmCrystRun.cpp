/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystRun.cpp
/// \brief Implementation of the TmCrystRun class

#include "G4THitsMap.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4SDmessenger.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4VSensitiveDetector.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "TmCrystRun.hh"

TmCrystRun::TmCrystRun()
 : G4Run(), 
  DetID(-1),
  AllEvents(1000000),
  PrintModulo(10000)
{ }

TmCrystRun::~TmCrystRun()
{ }

void TmCrystRun::RecordEvent(const G4Event* event)
{
  if ( DetID < 0 ) 
  {
    DetID = G4SDManager::GetSDMpointer()->GetCollectionID("Detector/edep"); 
  }

  G4int evtNb = event->GetEventID();
  
  //Progress bar
  if (evtNb%PrintModulo == 0) 
  { 
    G4cout << G4endl << "Progress " << evtNb << "/" << AllEvents << G4endl;
  }      
  
  //Hits collections
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if(!HCE) return;
  
  G4Run::RecordEvent(event);  
}