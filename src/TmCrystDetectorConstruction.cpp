/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystDetectorConstruction.cpp
/// \brief Implementation of the TmCrystDetectorConstruction class

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4VisAttributes.hh"

#include "TmCrystDetectorConstruction.hh"
#include "TmCrystEnergyDeposit.hh"



TmCrystDetectorConstruction::TmCrystDetectorConstruction()
: G4VUserDetectorConstruction()
{ }

TmCrystDetectorConstruction::~TmCrystDetectorConstruction()
{ }

G4VPhysicalVolume* TmCrystDetectorConstruction::Construct()
{  
  G4NistManager* nist = G4NistManager::Instance();

  G4bool checkOverlaps = true;

  // WORLD
  G4double world_size = 100*mm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidWorld = new G4Box("World", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");                      
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);


  //Tm2Al5O12 material

  
  // DETECTOR
  G4double det_size = 34*mm;
  G4Material* det_mat = nist->FindOrBuildMaterial("G4_Tm");                    
  G4Box* solidDet = new G4Box("Detector", 0.5*det_size, 0.5*det_size, 0.5*det_size);
  G4LogicalVolume* logicDet = new G4LogicalVolume(solidDet, det_mat, "Detector");
  new G4PVPlacement(0, G4ThreeVector(), logicDet, "Detector", logicWorld, false, 0, checkOverlaps);

  return physWorld;
}

void TmCrystDetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(0);

  //Initialisisation of Detector volume as a MultiFunctionalDetector
  G4MultiFunctionalDetector* Det = new G4MultiFunctionalDetector("Detector");
  G4SDManager::GetSDMpointer()->AddNewDetector(Det);

  G4VPrimitiveScorer* primitiv = new G4PSEnergyDeposit("edep");
  Det->RegisterPrimitive(primitiv);
  SetSensitiveDetector("Detector", Det);
}