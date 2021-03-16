/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystRunAction.hh
/// \brief Definition of the TmCrystRunAction class

#ifndef TmCrystRunAction_h
#define TmCrystRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class TmCrystRunAction: public G4UserRunAction
{
  public:
    TmCrystRunAction();
    virtual ~TmCrystRunAction();
    
    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};
#endif

