/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystActionInitialization.hh
/// \brief Definition of the TmCrystActionInitialization class

#ifndef TmCrystActionInitialization_h
#define TmCrystActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class TmCrystActionInitialization: public G4VUserActionInitialization
{
  public:
    TmCrystActionInitialization();
    virtual ~TmCrystActionInitialization();
    virtual void Build() const;
};

#endif
