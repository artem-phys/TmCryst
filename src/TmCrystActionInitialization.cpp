/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystActionInitialization.cpp
/// \brief Implementation of the TmCrystActionInitialization class

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "TmCrystPrimaryGeneratorAction.hh"
#include "TmCrystActionInitialization.hh"
#include "TmCrystRunAction.hh"
#include "TmCrystRun.hh"


TmCrystActionInitialization::TmCrystActionInitialization()
 : G4VUserActionInitialization()
{
}

TmCrystActionInitialization::~TmCrystActionInitialization()
{}

void TmCrystActionInitialization::Build() const
{
  SetUserAction(new TmCrystRunAction);
  SetUserAction(new TmCrystPrimaryGeneratorAction);
}
