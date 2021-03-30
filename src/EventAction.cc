#include "EventAction.hh"
#include "RunAction.hh"

#include "HeavyWaterHit.hh"
#include "HeavyWaterSD.hh"

#include "TrackerHit.hh"
#include "TrackerSD.hh"

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
  fHeavyHCID(-1),
  fTrackHCID(-1) 
{}

EventAction::~EventAction()
{}

HeavyWaterHitsCollection*
EventAction::GetHeavyHitsCollection(G4int hcID,
		               const G4Event* event) const
{
  auto HeavyhitsCollection
    = static_cast<HeavyWaterHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! HeavyhitsCollection )
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID" << hcID;
    G4Exception("EventAction::GetHitsCollection()",
        "MyCode0003", FatalException, msg);

  }

  return HeavyhitsCollection;
}

TrackerHitsCollection*
EventAction::GetTrackHitsCollection(G4int hcID,
                               const G4Event* event) const
{
  auto TrackhitsCollection
    = static_cast<TrackerHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! TrackhitsCollection )
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID" << hcID;
    G4Exception("EventAction::GetHitsCollection()",
        "MyCode0003", FatalException, msg);

  }

  return TrackhitsCollection;
}

//void EventAction::PrintHeavyEventStatistics
//     (G4double heavyEdep, G4double heavyTrackLength) const
//{
//  //Print event statistics
//  G4cout
//     << "Heavywater: total energy:"
//     << std::setw(7) << G4BestUnit(heavyEdep, "Energy")
//     << "total track length:"
//     << std::setw(7) << G4BestUnit(heavyTrackLength, "Length")
//     << G4endl;
//}

//void EventAction::PrintTrackEventStatistics
//     (G4int trackID, G4ThreeVector trackPos) const
//{
//  G4cout
//     << "Tracker:" << G4endl
//     << "trackID:"
//     //<< std::setw(7) << trackID
//     << "position:"
//     //<< std::setw(7) << G4BestUnit(trackPos, "Length")
//     << G4endl;
//}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event* event)
{
  //Get hits collection IDs (only once)
  if(fHeavyHCID == -1)
  {
    fHeavyHCID
       = G4SDManager::GetSDMpointer()->GetCollectionID("HeavyWaterHitsCollection");
  }

  if(fTrackHCID == -1)
  {
    fTrackHCID
       = G4SDManager::GetSDMpointer()->GetCollectionID("TrackerHitsCollection");
  }

  //Get hits collection
  auto heavyHC = GetHeavyHitsCollection(fHeavyHCID, event);
  auto trackHC = GetTrackHitsCollection(fTrackHCID, event);

  //Get hit with total values
  auto heavyHit = (*heavyHC)[heavyHC->entries()-1];
  auto trackHit = (*trackHC)[trackHC->entries()-1];

//  //Print per event (modulo n)
//  auto eventID = event->GetEventID();
//  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
//  
//  //if((printModulo>0)&&(eventID%printModulo == 0))
//  if((printModulo>0)&&(eventID%10000 == 0))
//  {
//    G4cout << "---> End of event:" << eventID << G4endl;
//
//    //PrintHeavyEventStatistics(heavyHit->GetEdep(), heavyHit->GetTrackLength());
//    //PrintTrackEventStatistics(trackHit->GetTrackID(), trackHit->GetPos());
//  }


  //Fill histograms, ntuple
  //

  //Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  //Fill histograms
  analysisManager->FillH1(0, heavyHit->GetEdep());
  G4cout << "Fill : heavywater edep: " << heavyHit->GetEdep() << G4endl;

  analysisManager->FillH1(1, heavyHit->GetTrackLength());
  G4cout << "Fill : heavywater TrackLength: " << heavyHit->GetTrackLength() << G4endl;

  //Fill ntuple
  analysisManager->FillNtupleDColumn(1, 0, heavyHit->GetEdep());
  analysisManager->FillNtupleDColumn(1, 1, heavyHit->GetTrackLength());
  analysisManager->AddNtupleRow();
}
