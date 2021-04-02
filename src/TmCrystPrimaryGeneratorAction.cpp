/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystPrimaryGeneratorAction.cpp
/// \brief Implementation of the TmCrystPrimaryGeneratorAction class

#include <random>
#include <fstream>
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4DataSet.hh"

#include "TmCrystPrimaryGeneratorAction.hh"



TmCrystPrimaryGeneratorAction::TmCrystPrimaryGeneratorAction():G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEnvelopeBox(0)
  {
  //Number of emitted particles
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  LoadBackgroundGammasEnergyCDF("/home/artem/Desktop/TmCryst/fon.txt");
  } 

TmCrystPrimaryGeneratorAction::~TmCrystPrimaryGeneratorAction()
{
  delete fParticleGun;
}


void TmCrystPrimaryGeneratorAction::LoadBackgroundGammasEnergyCDF(std::string file)
{
  std::ifstream fin;
  fin.open(file);
  //Check if opened
  if (!fin.is_open())
  {
    std::cout << "Error opening file " << file  << "\n";
  }
  else
  {
    std::cout << "Reading background CDF file " << file << "..." << "\n";
  }

  //Cycle reading file
  G4int i = 0;
  while(fin.eof()==0)
  {
    fin >> Egamma[i] >> CDF[i];
    //G4cout << "Egamma[i] =" << Egamma[i] << " - CDF[i] =" << CDF[i] << "\n";
    ++i;
  }
}

double TmCrystPrimaryGeneratorAction::RandomGammaEnergy()
{
  G4double E = 0;
  G4double F = G4UniformRand();
  double lb;
  double rb;
    for (int i = 0; i < 152; i++)
    {
      
      lb = CDF[i];
      rb = CDF[i+1];

      //G4cout << "F =" << F << "\n";
      //G4cout << "lb =" << lb << " rb =" << rb << "\n";
      //G4cout << "Egamma[i] =" << Egamma[i] << "Egamma[i + 1] =" << Egamma[i + 1] << "\n";
      if ((F >=lb)&&(F <= rb))
      {
        double x = G4UniformRand();
        E = Egamma[i] + x * (Egamma[i+1] - Egamma[i] );
        //if (lb == 0){G4cout << "E =" << E << "\n";}
        return E;
      }
    }
    return 0.0;
}



void TmCrystPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //Random-generated direction of isotropic radiation
  G4double theta = acos(2*G4UniformRand()-1);
  G4double phi = G4UniformRand()*2*3.141592653979;
  G4double x = sin(theta)*cos(phi);
  G4double y = sin(theta)*sin(phi);
  G4double z = cos(theta);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));

  //Random-generated particle birth position in the source volume
  G4double x_pos = 0*cm;
  G4double y_pos = 0*cm;
  G4double z_pos = 0*cm;

  G4double det_size = 10.8 * mm;

  G4double x0 = x_pos+(2*G4UniformRand()-1)*det_size/2;
  G4double y0 = y_pos+(2*G4UniformRand()-1)*det_size/2;
  G4double z0 = z_pos+(2*G4UniformRand()-1)*det_size/2;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  //Equally likely generate gamma or ion
  G4double GammaOrIon = G4UniformRand();
  if (GammaOrIon <= 0.5)
    {

  //Set Energy 
  G4double E = 0 * keV;
  G4double ionCharge = 0;
  fParticleGun->SetParticleEnergy(E);
  fParticleGun->SetParticleCharge(ionCharge);

  //Set ions as emitted particles
  G4ParticleDefinition* Th232 = G4IonTable::GetIonTable()->GetIon(90,232);
  G4ParticleDefinition* U = G4IonTable::GetIonTable()->GetIon(92,238);
  G4ParticleDefinition* Am241 = G4IonTable::GetIonTable()->GetIon(95,241);
  

  //Ion abundance - data from raw product estimation
  G4int NTh232 = 1000;
  G4int NU = 100;
  G4int NAm241 = 10000;
  G4int NAllRadioactive = NTh232+NU+NAm241;

  //Random ion definition with respect to ions' abundances
  G4int random_nucleus = int(NAllRadioactive*G4UniformRand());

  if (random_nucleus < NTh232)
  {
    fParticleGun->SetParticleDefinition(Th232);
  }
  else
  {
    if (random_nucleus < NU)
    {
    fParticleGun->SetParticleDefinition(U);
    }
    else
    {
      fParticleGun->SetParticleDefinition(Am241);
    }
  }

    }

    else 
    {
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* gammaParticle = particleTable->FindParticle("gamma");

  
  
      fParticleGun->SetParticleDefinition(gammaParticle);
      G4double gammaE = RandomGammaEnergy() * keV;
      //G4cout << "gammaE = " << gammaE / keV << "keV" << "\n";
      fParticleGun->SetParticleEnergy(gammaE);

      

      //Gamma random position outside bolometer
      G4double chamber_sizeXY = 10*cm;
      G4double chamber_sizeZ = 20*cm;
      while ((x0<det_size) || (y0<det_size) || (z0<det_size))
      {
        x0 = x_pos+(2*G4UniformRand()-1)*chamber_sizeXY/2;
        y0 = y_pos+(2*G4UniformRand()-1)*chamber_sizeXY/2;
        z0 = z_pos+(2*G4UniformRand()-1)*chamber_sizeZ/2;
      }

      
      fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

      //G4cout << "gamma E = " << gammaE << "\n";
      //G4cout << "gamma x0 = " << x0 << "y0 = " << y0 << "z0 = " << z0 << "\n";
      //G4cout << "gamma x = " << x << "y = " << y << "z = " << z << "\n";
      
    }

  //Primary event generation
  fParticleGun->GeneratePrimaryVertex(anEvent);
}