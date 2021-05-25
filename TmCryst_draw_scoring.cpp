/* This root macro was created to draw scoring results for some particular gamma energy values*/
/* Creator: Artem, lab assistant of Low Backgound Measurement Laboatory, PNPI */


/*-------------------------------------------PREAMBLE-------------------------------------------*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1I.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TMath.h"
#include "TRandom.h"
#include "TVirtualFitter.h"
#include "TSpectrum.h"

Double_t AxionFitFunction(Double_t *x, Double_t *par) 
  {
  //Linear_background
  Double_t linear_background = par[0] + par[1]*x[0];


  Double_t sigma = 0.38;

  //Axion
  Double_t EA = 8.41;
  Double_t SA = par[2];
  Double_t Axion_peak = SA * TMath::Gaus(x[0],EA,sigma) /(sigma * TMath::Sqrt(TMath::TwoPi()));

  //Cuprum
  Double_t E1 = 8.028;
  Double_t S1 = par[3];
  Double_t Cuprum_peak = S1 * TMath::Gaus(x[0],E1,sigma) /(sigma * TMath::Sqrt(TMath::TwoPi()));

  //peak2
  Double_t E2 = 8.6;
  Double_t S2 = par[4];
  Double_t peak_2 = S1 * TMath::Gaus(x[0],E2,sigma) /(sigma * TMath::Sqrt(TMath::TwoPi()));

  //peak3
  Double_t E3 = 10.1;
  Double_t S3 = par[5];
  Double_t peak_3 = S1 * TMath::Gaus(x[0],E3,sigma) /(sigma * TMath::Sqrt(TMath::TwoPi()));
  
  //Fit function
  Double_t result = linear_background + Axion_peak  + Cuprum_peak; 

  return result;
  }


//Function to read scoring

TH1I * read_TmCryst_scroing(std::string simulation_mode, int source_number)   
{

  // Multipliers for hists depending on the number of simulated events and the abundance of nuclei in sample
  //double mult[9] = {10,10,1e4,1e3,1e4,1e9, 1, 1e3, 1e3};

  double mult[10] = {10,10,1e2,1e3,1e3,1e3, 1, 1e3, 1e3,1};

  std::string source_names[10] = {"Ra228","Th228","Ra226","Pb210","U235","K40","Co60","Am241","Cs137","Gamma background"};

  std::string source_name = source_names[source_number];

  // Setting up the file path
  std::string directory = "/home/artem/Desktop/Diplom ROOT analysis/TmCrystScoring";
  std::ostringstream filepath;
  if (simulation_mode == "run")
  {
    filepath << directory << "/TmCrystScoring_run_source_" << source_number << ".root";
  }
  else
  {
    filepath << directory << "/TmCrystScoring_specsource_" << source_number << ".root";
  }
  
  std::string f1 = filepath.str();
  const char* file = f1.c_str(); 
  
  // Setting up histogram name and title
  const char *hname = source_name.c_str();
  char *title = new char[70];
  std::sprintf(title,"Events count by bolometer. Source number %d ", source_number);
  

  // Creating pointer to TH1I
  int Emin = 0;
  int Emax= 1200;
  double bins_amount = 1000;
	TH1I *hist = new TH1I(hname, title, bins_amount, Emin , Emax); 

  // Creating input stream and check if opened
	TFile f(file);
  if (!f.IsOpen())
  {
    std::cout << "Error opening file " << file  << "\n";
  }
  else
  {
    std::cout << "Reading " << file << "..." << "\n";
  }

  //Get information from Tree inside scoring file
  TTree * EdepTree;
  f.GetObject("Detector_edep", EdepTree);
  Double_t edep;
  EdepTree->SetBranchAddress("Detector_edep_score", &edep);

  //Filling up the histogram with Gaussian convolution
  double resolution = 0.38;
  double rasm;
  double mult_factor = mult[source_number];

  Long64_t n_entries = (Long64_t)EdepTree->GetEntries();
  for(Long64_t i = 0; i < n_entries; i++) 
  {
    EdepTree->GetEntry(i);
    for (Long64_t j = 0; j < mult_factor; j++)
    {
      rasm = gRandom->Gaus(1000*edep, resolution);
      hist->Fill(rasm);
    }
  }
  return hist;
}


/*-------------------------------------------START--------------------------------------------*/

void TmCryst_draw_scoring(void)
{ 
  const Int_t nhist= 10; 

  // Creating ROOT file
  TFile* file = new TFile("TmCryst_scoring_results.root","recreate");

  //Creating hists
  TH1I * h_run[nhist];
  TH1I * h_spec[nhist]; // Spectra with 10 times greater

  //Cummulative spectrum
  const char *hname = "Simulation spectrum";
  const  char *title = "Events count by bolometer";
  int Emin = 0;
  int Emax= 1200;
  double bins_amount = 1000;

  TH1I * hist_all = new TH1I(hname, title, bins_amount, Emin , Emax); 

  //Filling hists from scoring files
  for(int s = 0 ; s < nhist; ++s)
  {
    h_run[s] = read_TmCryst_scroing("run", s);
    h_spec[s] = read_TmCryst_scroing("spec", s);

    hist_all->Add(h_run[s]);
  }
  
  //Draw histograms in different canvas
  std::string hnames[nhist] = {"Ra228","Th228","Ra226","Pb210","U235","K40","Co60","Am241","Cs137","Gamma background"};
  TCanvas *c[nhist];
  for (Int_t i=9;i<nhist;i++) 
  { 
    c[i] = new TCanvas(Form("c%d",i)); 
    h_run[i]->Draw(); 
 
    h_run[i]->SetDirectory(file);
    h_run[i]->GetYaxis()->SetTitle("Events number");
    h_run[i]->GetXaxis()->SetTitle("E, keV");

    //Style modification
    double fontsize = 0.06;
    h_run[i]->GetYaxis()->SetLabelSize(fontsize);
    h_run[i]->GetYaxis()->SetTitleSize(fontsize);
    h_run[i]->GetXaxis()->SetLabelSize(fontsize);
    h_run[i]->GetXaxis()->SetTitleSize(fontsize);
    gStyle->SetTitleFontSize(fontsize);
    gStyle->SetOptFit();
    gPad->Update();
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetLogy();
    gPad->SetGridy();
    gPad->SetGridx();

    //Stats modification
    TPaveStats *st = (TPaveStats*)h_run[i]->FindObject("stats");
    st->SetX1NDC(0.75);
    st->SetX2NDC(0.9);
    st->SetY1NDC(0.75);
    st->SetY2NDC(0.9);
  }

/*-------------------------------------------FIT--------------------------------------------*/
  /*
  const int npars = 6;
  Double_t par[npars] = {10,1,10,10,10,10};
  Double_t E_fit_min = 7;
  Double_t E_fit_max = 14.5;

  printf("Now fitting: Be patient\n");
  TF1 *fit = new TF1("fit",AxionFitFunction,E_fit_min,E_fit_max,npars);
  fit->SetParNames("a","b","S_{A}","S_{1}","S_{2}","S_{3}");

  fit->SetParLimits(0,0,1000);
  fit->SetParLimits(1,0,1000);
  fit->SetParLimits(2,0,10000);
  fit->SetParLimits(3,0,10000);
  fit->SetParLimits(4,0,10000);
  fit->SetParLimits(5,0,10000);

  TVirtualFitter::Fitter(h,6);
  fit->SetParameters(par);
  fit->SetNpx(1000);
  h->Fit("fit", "R");

  h->GetXaxis()->SetRangeUser(E_fit_min, E_fit_max);
  gPad->Update();
  */
}
