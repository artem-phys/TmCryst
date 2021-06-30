/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystDetectorConstruction.cpp
/// \brief Implementation of the TmCrystDetectorConstruction class

#include<cmath>

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
  G4double world_size = 100*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box* solidWorld = new G4Box("World", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");                      
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);

  // POLY COVER
  G4double poly_sizeXY = 92*cm;
  G4double poly_sizeZ = 96*cm;
  G4Material* poly_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Box* solidPoly = new G4Box("Poly", 0.5*poly_sizeXY, 0.5*poly_sizeXY, 0.5*poly_sizeZ);
  G4LogicalVolume* logicPoly  = new G4LogicalVolume(solidPoly , poly_mat, "Poly");
  new G4PVPlacement(0, G4ThreeVector(), logicPoly, "Poly", logicWorld, false, 0, checkOverlaps);

  // CADMIUM LAYER
  G4double cadmium_sizeXY = 82.15*cm;
  G4double cadmium_sizeZ = 86.15*cm;
  G4Material* cadmium_mat = nist->FindOrBuildMaterial("G4_Cd");
  G4Box* solidCadmium = new G4Box("Cadmium", 0.5*cadmium_sizeXY, 0.5*cadmium_sizeXY, 0.5*cadmium_sizeZ);
  G4LogicalVolume* logicCadmium  = new G4LogicalVolume(solidCadmium , cadmium_mat, "Cadmium");
  new G4PVPlacement(0, G4ThreeVector(), logicCadmium, "Cadmium", logicPoly, false, 0, checkOverlaps);

  // PLUMBUM COVER
  G4double plumbum_sizeXY = 82*cm;
  G4double plumbum_sizeZ = 86*cm;
  G4Material* plumbum_mat = nist->FindOrBuildMaterial("G4_Pb");
  G4Box* solidPlumbum = new G4Box("Plumbum", 0.5*plumbum_sizeXY, 0.5*plumbum_sizeXY, 0.5*plumbum_sizeZ);
  G4LogicalVolume* logicPlumbum = new G4LogicalVolume(solidPlumbum, plumbum_mat, "Plumbum");
  new G4PVPlacement(0, G4ThreeVector(), logicPlumbum, "Plumbum", logicCadmium, false, 0, checkOverlaps);                

  // COPPER INSIDE
  G4double copper_sizeXY = 50*cm;
  G4double copper_sizeZ = 54*cm;
  G4Material* copper_mat = nist->FindOrBuildMaterial("G4_Cu");
  G4Box* solidCopper = new G4Box("Copper", 0.5*copper_sizeXY, 0.5*copper_sizeXY, 0.5*copper_sizeZ);
  G4LogicalVolume* logicCopper = new G4LogicalVolume(solidCopper , copper_mat, "Copper");
  new G4PVPlacement(0, G4ThreeVector(), logicCopper, "Copper", logicPlumbum, false, 0, checkOverlaps);  

  // AIR CHAMBER
  G4double chamber_sizeXY = 10*cm;
  G4double chamber_sizeZ = 20*cm;
  G4Material* chamber_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Box* solidChamber = new G4Box("Chamber", 0.5*chamber_sizeXY, 0.5*chamber_sizeXY, 0.5*chamber_sizeZ);
  G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber , chamber_mat, "Chamber");
  new G4PVPlacement(0, G4ThreeVector(), logicChamber, "Chamber", logicCopper, false, 0, checkOverlaps); 


  //Tm2Al5O12 material
  G4double aTm = 168.934*g/mole;
  G4Element* elTm = new G4Element("Thulium","Tm",69.,aTm);
  G4double aAl = 26.982*g/mole;
  G4Element* elAl = new G4Element("Aluminium","Al",13.,aAl);
  G4double aO = 16.000*g/mole;
  G4Element* elO = new G4Element("Oxygen","O",8.,aO);

  G4double density = 6.49*g/cm3;
  G4Material* Tm3Al5O12 = new G4Material("Tm2Al5O12",density,3);
  Tm3Al5O12->AddElement(elTm,3);
  Tm3Al5O12->AddElement(elAl,5);
  Tm3Al5O12->AddElement(elO,12);

  // TM CRYSTAL

  G4double det_mass = 1 kg; // CRYSTAL 1 KG (small 8.18g before)

  G4double det_volume = det_mass / density;

  G4double det_size = pow(det_volume, 1.0 / 3);

  
  G4Material* det_mat = Tm3Al5O12;
  G4Box* solidTm = new G4Box("TmCrystal", 0.5*det_size, 0.5*det_size, 0.5*det_size);
  G4LogicalVolume* logicTm = new G4LogicalVolume(solidTm, det_mat, "TmCrystal");
  new G4PVPlacement(0, G4ThreeVector(), logicTm, "TmCrystal", logicChamber, false, 0, checkOverlaps);


  // TM CRYSTAL - DETECTOR ACTIVE VOLUME
  // It has a tiny dead layer
  G4double dead_thickness = 0.01 * mm;
  G4double active_size = det_size - dead_thickness;

  G4Box* solidDet = new G4Box("Detector", 0.5*active_size, 0.5*active_size, 0.5*active_size);
  G4LogicalVolume* logicDet = new G4LogicalVolume(solidDet, det_mat, "Detector");               
  
  new G4PVPlacement(0, G4ThreeVector(), logicDet, "Detector", logicTm, false, 0, checkOverlaps);

  return physWorld;
}

void TmCrystDetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(0);

  //Initialisisation of Detector volume as a MultiFunctionalDetector
  G4MultiFunctionalDetector* Det = new G4MultiFunctionalDetector("Detector");
  G4SDManager::GetSDMpointer()->AddNewDetector(Det);

  //G4VPrimitiveScorer* primitiv = new G4PSEnergyDeposit("edep");
  G4VPrimitiveScorer* primitiv = new TmCrystEnergyDeposit("edep");
  //Det->RegisterPrimitive(primitiv);
  Det->RegisterPrimitive(primitiv);
  SetSensitiveDetector("Detector", Det);
}