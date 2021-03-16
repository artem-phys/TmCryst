/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystDetectorConstruction.hh
/// \brief Definition of the TmCrystDetectorConstruction class

#ifndef TmCrystDetectorConstruction_h
#define TmCrystDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class TmCrystDetectorConstruction : public G4VUserDetectorConstruction 
{
  public:
    TmCrystDetectorConstruction();
    virtual ~TmCrystDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  protected:
};
#endif