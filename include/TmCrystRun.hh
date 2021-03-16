/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystRun.hh
/// \brief Definition of the TmCrystRun class

#ifndef TmCrystRun_h
#define TmCrystRun_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "G4StatAnalysis.hh"

/// Run class
///
/// In RecordEvent() there is collected information event per event 
/// from Hits Collections, and accumulated statistic for the run 

class TmCrystRun : public G4Run
{
  public:
    TmCrystRun();
    virtual ~TmCrystRun();

    virtual void RecordEvent(const G4Event*);

  private:
    G4int DetID; 
    G4int AllEvents;
    G4int PrintModulo;
};

#endif

    
