#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "PhysicsList.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "g4root.hh"

namespace {
  void PrintUsage(){
    G4cerr << "Usage:" << G4endl;
    G4cerr << "example [-m macro ] [-u UIsession ] [-t nThreads]" << G4endl;
    G4cerr << "  note -t option is available only for multi-threaded mode." << G4endl;
}
}
int main(int argc,char **argv)		//Detect interactive mode (if no arguments ) and define UI session
{
  G4UIExecutive *ui = 0;
  if (argc == 1)
  {
  ui = new G4UIExecutive(argc, argv);
  }
 


  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();	//modifing~~~~~~~~~~~~~~~~~~~
  std::string outputFileName("output.root");
  analysisManager->SetFileName(outputFileName);
  analysisManager->OpenFile();
  analysisManager->SetActivation(true);

  analysisManager->CreateNtuple("particle", "particle");
  analysisManager->CreateNtupleDColumn("pid");
#if 0		
  analysisManager->CreateNtupleDColumn("px");
  analysisManager->CreateNtupleDColumn("py");
  analysisManager->CreateNtupleDColumn("pz");
  analysisManager->CreateNtupleDColumn("energy");
  analysisManager->CreateNtupleDColumn("edep");
#endif
  analysisManager->FinishNtuple();



  //Potionally:choose a different Randow engine
  //
#ifdef G4MULTITHREADED
  G4MTRunManger* runManager = new G4MTRRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif
  
  //Set mandatory initialization classes
  //
  //Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  //Physics list
  //G4VModularPhysicsList* physicsList = new QBBC;
  G4VModularPhysicsList* physicsList = new PhysicsList();
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  //User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  //Initialization visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();		//Get the pointer to the User Interface manager

  //Process macro or start UI session
  //
  if(!ui)		//batch mode
  {
  G4String command = "/control/execute ";
  G4String fileName = argv[1];
  UImanager->ApplyCommand(command+fileName);
  }
  else			//interactive mode
  {
  UImanager->ApplyCommand("/control/execute init_vis.mac");
  ui->SessionStart();
  delete ui;
  }

  

  //if ( analysisManager->IsActive() )		//modifing~~~~~~~~~~~~~~~~~~~~~~
  {
  std::cout << "end of running" << std::endl;
  analysisManager->Write();
  analysisManager->CloseFile();
  }



delete visManager;
delete runManager;
}

