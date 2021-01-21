#include "EventAction.hh"
#include "RunAction.hh"
#include "HeavyWaterHit.hh"
#include "HeavyWaterSD.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include "g4root.hh"
#include <iomanip>

EventAction::EventAction()
: G4UserEventAction(),
  fHwHCID(-1)
{}

EventAction::~EventAction()
{}

HeavyWaterHitsCollection*
EventAction::GetHitsCollection(G4int hcID,
		               const G4Event* event) const
{
  auto hitsCollection
    = static_cast<HeavyWaterHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection )
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID" << hcID;
    G4Exception("EventAction::GetHitsCollection()",
        "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

void EventAction::PrintEventStatistics
     (G4double hwEdep, G4double hwTrackLength) const
{
  //Print event statistics
  G4cout
     << "Heavywater: total energy:"
     << std::setw(7) << G4BestUnit(hwEdep, "Energy")
     << "total track length:"
     << std::setw(7) << G4BestUnit(hwTrackLength, "Length")
     << G4endl;
}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event* event)
{
  //Get hits collection IDs (only once)
  if(fHwHCID == -1)
  {
    fHwHCID
       = G4SDManager::GetSDMpointer()->GetCollectionID("HeavyWaterHitsCollection");
  }

  //Get hits collection
  auto hwHC = GetHitsCollection(fHwHCID, event);

  //Get hit with total values
  auto hwHit = (*hwHC)[hwHC->entries()-1];

  //Print per event (modulo n)
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if((printModulo>0)&&(eventID%printModulo == 0))
  {
    G4cout << "---> End of event:" << eventID << G4endl;

    PrintEventStatistics(hwHit->GetEdep(), hwHit->GetTrackLength()); 
  }

  //Fill histograms, ntuple
  //

  //Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  
  //Fill histograms
  analysisManager->FillH1(0, hwHit->GetEdep());
  analysisManager->FillH1(1, hwHit->GetTrackLength()); 
  analysisManager->FillH1(2, hwHit->GetPid());
  //Fill ntuple
  analysisManager->FillNtupleDColumn(0, hwHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, hwHit->GetTrackLength());
  analysisManager->FillNtupleDColumn(2, hwHit->GetPid());
  analysisManager->AddNtupleRow();
}
