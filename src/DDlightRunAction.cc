//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Make this appear first!
#include "G4Timer.hh"

#include "DMXAnalysisManager.hh"

#include "DDlightRunAction.hh"

#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DDlightRunAction::DDlightRunAction()
    : G4UserRunAction(),
      fTimer(0),
      fOutputFile("test.root")
{
  fTimer = new G4Timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DDlightRunAction::~DDlightRunAction()
{
  delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DDlightRunAction::BeginOfRunAction(const G4Run *aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();

  Book();
}

void DDlightRunAction::Book()
{
  // Get/create analysis manager
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  // Open an output file
  man->OpenFile(fOutputFile);
  man->SetFirstHistoId(1);
  man->SetFirstNtupleId(1);

  // ---- primary ntuple ------
  // id==1
  man->CreateNtuple("tree1", "Particle Source Energy");
  man->CreateNtupleDColumn("energy");
  man->FinishNtuple();

  // ---- secondary ntuple ------
  //id==2
  man->CreateNtuple("tree2", "Scintillation Hits Info");
  man->CreateNtupleDColumn("Event");
  man->CreateNtupleDColumn("e_prim");
  man->CreateNtupleDColumn("tot_e");
  man->CreateNtupleDColumn("s_hits");
  man->CreateNtupleDColumn("xe_time");
  man->CreateNtupleDColumn("num_ph");
  man->CreateNtupleDColumn("avphtime");
  man->CreateNtupleDColumn("firstpart");
  man->CreateNtupleDColumn("firstparte");
  man->CreateNtupleDColumn("gamma");
  man->CreateNtupleDColumn("neutron");
  man->CreateNtupleDColumn("posi");
  man->CreateNtupleDColumn("elec");
  man->CreateNtupleDColumn("other");
  man->CreateNtupleDColumn("seed1");
  man->CreateNtupleDColumn("seed2");
  man->FinishNtuple();

  // ---- tertiary ntuple ------
  //id==3
  man->CreateNtuple("tree3", "PMT Hits Info");
  man->CreateNtupleDColumn("event");
  man->CreateNtupleDColumn("hits");
  man->CreateNtupleDColumn("xpos");
  man->CreateNtupleDColumn("ypos");
  man->CreateNtupleDColumn("zpos");
  man->FinishNtuple();

  // test ntuple
  //id=4
  man->CreateNtuple("testing", "Just for testing");
  man->CreateNtupleDColumn("test1");
  man->FinishNtuple();

  // Creating 1-dimensional histograms
  man->CreateH1("h1", "Source Energy /keV", 1000, 0., 10000.);
  man->CreateH1("h2", "Energy Deposit /keV", 1000, 0., 1000.);
  man->CreateH1("h3", "Nuclear Recoil Edep /keV", 100, 0., 100.);
  man->CreateH1("h4", "Number of Photons - LowE", 200, 0., 200.);
  man->CreateH1("h5", "Number of Photons - HighE", 100, 0., 10000.);
  man->CreateH1("h6", "Average Photon Arrival/ns", 200, 0., 200.);
  man->CreateH1("h7", "1st event Photon Arrival", 200, 0., 200.);
  man->CreateH1("h8", "Gamma Energy Deposit/keV", 1000, 0., 1000.);
  man->CreateH1("h9", "Neutron Ener Deposit/keV", 1000, 0., 1000.);
  man->CreateH1("h10", "Electron Ener Deposit/keV", 1000, 0., 1000.);
  man->CreateH1("h11", "Positron Ener Deposit/keV", 1000, 0., 1000.);
  man->CreateH1("h12", "Other Ener Deposit/keV", 1000, 0., 1000.);
  man->CreateH1("test", "Testing", 100, 0., 100.);

  //Creating 2-dimensional histograms
  man->CreateH2("hh1", "PMT Hit Pattern",
                300, -30., 30., 300, -30., 30.);
  man->CreateH2("hh2", "1st event PMT Hit Pattern",
                300, -30., 30., 300, -30., 30.);

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DDlightRunAction::EndOfRunAction(const G4Run *aRun)
{
  auto man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
