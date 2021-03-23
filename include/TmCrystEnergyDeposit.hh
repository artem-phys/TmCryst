/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystEnergyDeposit.hh
/// \brief Definition of the TmCrystEnergyDeposit class

#ifndef TmCrystEnergyDeposit_h
#define TmCrystEnergyDeposit_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4VPrimitivePlotter.hh"
#include "G4THitsMap.hh"

class TmCrystEnergyDeposit : public G4VPrimitivePlotter
{
 
 public: // with description
      TmCrystEnergyDeposit(G4String name, G4int depth=0); // default unit
      TmCrystEnergyDeposit(G4String name, const G4String& unit, G4int depth=0);
      virtual ~TmCrystEnergyDeposit();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

 public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

      virtual void SetUnit(const G4String& unit);

  private:
      G4int HCID;
      G4double global_time;
      G4THitsMap<G4double>* EvtMap;
};
#endif
