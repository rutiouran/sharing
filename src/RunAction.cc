#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"

#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
: G4UserRunAction()
{
  //Set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  
  //Create analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using" << analysisManager->GetType() << G4endl;

  //Create directories
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  //Book histograms, ntuple
  //

  //Creating histograms
  analysisManager->CreateH1("Ehwt", "Edep in heavy water target", 100, 0., 20*MeV);
  analysisManager->CreateH1("Lhwt", "trackL in heavy water target", 100, 0., 2*cm);
  analysisManager->CreateH1("pid1", "Pid in heavy water target behind 1e9", 100, -3000, 3000);
  analysisManager->CreateH1("Pid2", "Pid in heavy water target after 1e9", 100, 1000000000, 1000100000);

  analysisManager->CreateH3("neutron flux", "neutron flux in the tracker",
		            100, -1.5, 1.5,
			    100, -1.5, 1.5,
			    100, -1.5, 1.5);
  analysisManager->CreateH1("nefl-x", "neutron flux in the tracker of the x axis", 100, -1, 1);
  analysisManager->CreateH1("nefl-y", "neutron flux in the tracker of the y axis", 100, -1, 1);
  analysisManager->CreateH1("nefl-z", "neutron flux in the tracker of the z axis", 100, -1, 1);

  analysisManager->SetFirstNtupleId(1);

  //Creating ntuple
  analysisManager->CreateNtuple("HeavyWater", "HeavyWater");
  analysisManager->CreateNtupleDColumn("Ehwt");
  analysisManager->CreateNtupleDColumn("Lhwt");
//  analysisManager->FinishNtuple();
//
//  analysisManager->CreateNtuple("pid", "pid");
  analysisManager->CreateNtupleDColumn("Pid1");
  analysisManager->CreateNtupleDColumn("Pid2");
  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("neutronEnergy", "neutronEnergy");
  analysisManager->CreateNtupleDColumn("neutronEnergy1");
  analysisManager->CreateNtupleDColumn("neutronfluxx");
  analysisManager->CreateNtupleDColumn("neutronfluxy");
  analysisManager->CreateNtupleDColumn("neutronfluxz");
  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("Target", "Target");
  for(G4int i=1; i<=3; i++)
  {
    analysisManager->CreateNtupleDColumn(title1+std::to_string(i));
    analysisManager->CreateNtupleDColumn(title2+std::to_string(i));
  }
  analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
  auto particleMap = StackingAction::Instance()->getNumberOfParticlesPerID();

  for(auto particle : particleMap)
  {
     std::cout << "pid: " << particle.first << ", #: " << particle.second << std::endl;
  }

  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  //Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  //Open an output file	
  G4String fileName = "output";
  analysisManager->OpenFile(fileName);  
}

void RunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  //save histograms and ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}
