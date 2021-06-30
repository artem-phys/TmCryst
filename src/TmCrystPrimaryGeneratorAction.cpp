/* This file is a part of a GEANT4 model of Tm2Al5O12 crystal for axion search experiment */
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */

/// \file TmCrystPrimaryGeneratorAction.cpp
/// \brief Implementation of the TmCrystPrimaryGeneratorAction class

#include <random> 
#include <vector>
#include <algorithm>

#include <fstream>
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4DataSet.hh"

#include "TmCrystPrimaryGeneratorAction.hh"

//global variable
G4int g_source_number;

TmCrystPrimaryGeneratorAction::TmCrystPrimaryGeneratorAction():G4VUserPrimaryGeneratorAction(),
  source(::g_source_number),
  fParticleGun(0),
  fEnvelopeBox(0)
  {
    std::cout << "Simulating source: "<< source << "... \n";

    //Number of emitted particles
    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);

    //Tries to load file from two sources, in order to be easily performed 
    //both at Threadripper remote machine and local machine
    LoadBackgroundGammasEnergyCDF("/home/artem/Desktop/TmCryst/fon.txt");
    LoadBackgroundGammasEnergyCDF("/home/kuzmichev/TmCryst/fon.txt");
  } 

TmCrystPrimaryGeneratorAction::~TmCrystPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//File fon.txt contains cummulative density function of the distribution of background gammas by energy . 
void TmCrystPrimaryGeneratorAction::LoadBackgroundGammasEnergyCDF(std::string file)
{
  std::ifstream fin;
  fin.open(file);
  //Check if opened
  if (!fin.is_open())
  {
    std::cout << "File " << file << "does not exist"  << "\n";
  }
  else
  {
    std::cout << "Reading background CDF file " << file << "..." << "\n";
    G4int i = 0;
    while(fin.eof()==0)
    {
      fin >> Egamma[i] >> CDF[i];
      ++i;
    }
  }
}

//Generate random gamma energy using loaded distribution
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

      if ((F >=lb)&&(F <= rb))
      {
        double x = G4UniformRand();
        E = Egamma[i] + x * (Egamma[i+1] - Egamma[i] );
        return E;
      }
    }
    return 0.0;
}



void TmCrystPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //Random-generated direction of isotropic radiation
  G4double theta = acos(2*G4UniformRand()-1);
  G4double phi = G4UniformRand()*CLHEP::twopi;
  G4double x = sin(theta)*cos(phi);
  G4double y = sin(theta)*sin(phi);
  G4double z = cos(theta);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(x,y,z));

  //Random-generated particle birth position in the source volume
  G4double x_pos = 0*cm;
  G4double y_pos = 0*cm;
  G4double z_pos = 0*cm;

  //code from Detector construction

  G4double det_mass = 1 kg; // CRYSTAL 1 KG (small 8.18g before)

  G4double det_volume = det_mass / density;

  G4double det_size = pow(det_volume, 1.0 / 3);

  G4double x0 = x_pos+(2*G4UniformRand()-1)*det_size/2;
  G4double y0 = y_pos+(2*G4UniformRand()-1)*det_size/2;
  G4double z0 = z_pos+(2*G4UniformRand()-1)*det_size/2;
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  
  if (source !=9) //source 0..8 - ions
    {

  //Set Energy 
  G4double E = 0 * keV;
  G4double ionCharge = 0;
  fParticleGun->SetParticleEnergy(E);
  fParticleGun->SetParticleCharge(ionCharge);

  //Set ions as emitted particles
  G4ParticleDefinition* Ra228 = G4IonTable::GetIonTable()->GetIon(86,228);
  G4ParticleDefinition* Th228 = G4IonTable::GetIonTable()->GetIon(90,228);
  G4ParticleDefinition* Ra226 = G4IonTable::GetIonTable()->GetIon(86,226);
  G4ParticleDefinition* Pb210 = G4IonTable::GetIonTable()->GetIon(82,210);
  G4ParticleDefinition* U235 = G4IonTable::GetIonTable()->GetIon(92,235);
  G4ParticleDefinition* K40 = G4IonTable::GetIonTable()->GetIon(19,40);
  G4ParticleDefinition* Co60= G4IonTable::GetIonTable()->GetIon(27,60);
  G4ParticleDefinition* Am241 = G4IonTable::GetIonTable()->GetIon(95,241);
  G4ParticleDefinition* Cs137 = G4IonTable::GetIonTable()->GetIon(55,137);
  

  //Random ion definition with respect to ions' abundances

  switch (source)
	{
	case 0:
		fParticleGun->SetParticleDefinition(Ra228);
		break;
	case 1:
		fParticleGun->SetParticleDefinition(Th228);
		break;
	case 2:
		fParticleGun->SetParticleDefinition(Ra226);
		break;
	case 3:
		fParticleGun->SetParticleDefinition(Pb210);
		break;
	case 4:
		fParticleGun->SetParticleDefinition(U235);
		break;
  case 5:
		fParticleGun->SetParticleDefinition(K40);
		break;
	case 6:
		fParticleGun->SetParticleDefinition(Co60);
		break;
	case 7:
		fParticleGun->SetParticleDefinition(Am241);
		break;
	case 8:
		fParticleGun->SetParticleDefinition(Cs137);
		break;
	default:
		std::cout << "Unknown";
		break;
	}

    }


    else 
    {
      //source 9: gamma
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
    }

  //Primary event generation
  fParticleGun->GeneratePrimaryVertex(anEvent);
}