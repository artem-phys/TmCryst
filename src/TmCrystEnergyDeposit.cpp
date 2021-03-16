/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystEnergyDeposit.cpp
/// \brief Implementation of the TmCrystEnergyDeposit class

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "TmCrystEnergyDeposit.hh"

TmCrystEnergyDeposit::TmCrystEnergyDeposit(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),EvtMap(0)
{
  SetUnit("keV");
}

TmCrystEnergyDeposit::TmCrystEnergyDeposit(G4String name, const G4String& unit, 
				     G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),EvtMap(0)
{
  SetUnit(unit);
}

TmCrystEnergyDeposit::~TmCrystEnergyDeposit()
{;}

G4bool TmCrystEnergyDeposit::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4Track * track = aStep->GetTrack();
  G4double time = track->GetGlobalTime();

  
  if (time > 2629743.83*s) 
  {
    track->SetTrackStatus(fStopAndKill);
  }
  return TRUE;
}

void TmCrystEnergyDeposit::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
				    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void TmCrystEnergyDeposit::EndOfEvent(G4HCofThisEvent*)
{;}

void TmCrystEnergyDeposit::clear()
{
  EvtMap->clear();
}

void TmCrystEnergyDeposit::DrawAll()
{;}

void TmCrystEnergyDeposit::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  energy deposit: " 
	   << *(itr->second)/GetUnitValue()
	   << " [" << GetUnit()<<"]"
	   << G4endl;
  }
}

void TmCrystEnergyDeposit::SetUnit(const G4String& unit)
{
	CheckAndSetUnit(unit,"Energy");
}