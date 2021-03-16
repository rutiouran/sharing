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
  analysisManager->CreateH1("Ehwt", "Edep in heavy water target", 100, 0., 13*MeV);
  analysisManager->CreateH1("Lhwt", "trackL in heavy water target", 100, 0., 1*cm);
  analysisManager->CreateH1("pid1", "Pid in heavy water target behind 1e9", 100, -3000, 3000);
  analysisManager->CreateH1("Pid2", "Pid in heavy water target after 1e9", 100, 1000000000, 1000100000);

  analysisManager->SetFirstNtupleId(1);

  //Creating ntuple
  analysisManager->CreateNtuple("EdepTrackLen", "EdepTrackLen");
  analysisManager->CreateNtupleDColumn("Ehwt");
  analysisManager->CreateNtupleDColumn("Lhwt");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("pid", "pid");
  analysisManager->CreateNtupleDColumn("Pid1");
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
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl;
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl;
    }

    G4cout << " Edeu : mean = "
       << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy")
       << " rms = "
       << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;

    G4cout << " Ldeu : mean = "
      << G4BestUnit(analysisManager->GetH1(1)->mean(), "Length")
      << " rms = "
      << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Length") << G4endl;

    //G4cout << " Pid : mean = "
      //<< G4BestUnit(analysisManager->GetH1(2)->mean(), "Pid")
      //<< " rms = "
      //<< G4BestUnit(analysisManager->GetH1(2)->rms(), "pid") << G4endl; 
  
  //save histograms and ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}
}
