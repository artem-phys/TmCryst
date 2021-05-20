﻿/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

#include "G4TScoreNtupleWriter.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ScoringManager.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4SystemOfUnits.hh"
#include "G4HadronicProcessStore.hh"
#include "Shielding.hh"
#include "g4root.hh"

#include "TmCrystDetectorConstruction.hh"
#include "TmCrystActionInitialization.hh"
#include "TmCrystPrimaryGeneratorAction.hh"


int main(int argc,char** argv)
{
//Run manager
G4RunManager* runManager = new G4RunManager;
runManager->SetUserInitialization(new TmCrystDetectorConstruction);
runManager->SetUserInitialization(new Shielding);
runManager->SetUserInitialization(new TmCrystActionInitialization);

//Reduce Physics list verbosity
G4HadronicProcessStore * hps = G4HadronicProcessStore::Instance();
hps->SetVerbose(0);

//Initialization
runManager->Initialize();

//Visualisation and UI
G4VisManager* visManager = new G4VisExecutive;
visManager->SetVerboseLevel(1);
visManager->Initialize();
G4UImanager* UImanager = G4UImanager::GetUIpointer();

//Write score 
G4TScoreNtupleWriter<G4AnalysisManager> scoreNtupleWriter;
scoreNtupleWriter.SetVerboseLevel(0);
const G4String scoring_filename = "TmCrystScoring.root";
scoreNtupleWriter.SetFileName(scoring_filename);

if ( argc == 1 ) 
{
    //Start visualisation
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);//UI creation
    UImanager->ApplyCommand("/control/execute vis.mac");//Run macros vis.mac 
    ui->SessionStart(); //UI start
    delete ui;
}

else 
{
    //Name of required macros to execute was given through command line
    G4String command = "/control/execute ";
    G4String fileName = argv[1];

    //Get required source from the name of a macro file
    std::string source = fileName;
    source.erase(source.end()-4, source.end());
    source.erase(source.begin(), source.begin()+11);
    std::cout << source << '\n';
    int S = std::stoi(source);

    //Write score 

    scoreNtupleWriter.SetVerboseLevel(0);
    const G4String scoring_filename = "TmCrystScoring_source_" + std::to_string(S) + ".root";
    scoreNtupleWriter.SetFileName(scoring_filename);

    UImanager->ApplyCommand(command+fileName);
}

delete runManager;
return 0;
}