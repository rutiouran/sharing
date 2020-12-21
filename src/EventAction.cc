#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "g4root.hh"

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{} 

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}

void EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
#if 0
  std::cout << "edep: " << fEdep << std::endl;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->FillNtupleDColumn(4, fEdep);
  analysisManager->AddNtupleRow();


  fRunAction->AddEdep(fEdep);
#endif
}
