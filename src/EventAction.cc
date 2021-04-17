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

  std::cout << "trackHC: " << trackHC->entries() << std::endl;

//  //Get hit with total values
//  auto heavyHit = (*heavyHC)[heavyHC->entries()-1];
//  auto trackHit = (*trackHC)[trackHC->entries()-1];

  //Fill histograms, ntuple
  //

  //Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  G4double heavyEdep = 0.; // The deposit energy in heavy water
  G4double heavyTral = 0.; // The track length in heavy water

  for(long unsigned int i=0; i<heavyHC->entries(); i++)
  {
    heavyEdep = (*heavyHC)[i]->GetEdep();
    heavyTral = (*heavyHC)[i]->GetTrackLength();
    if((*heavyHC)[i]->GetTrackID()!=1 && (*heavyHC)[i]->GetStep()==0 && (*heavyHC)[i]->GetPid()<1e9)
      analysisManager->FillNtupleDColumn(1, 2, (*heavyHC)[i]->GetPid());
    else
      analysisManager->FillNtupleDColumn(1, 3, (*heavyHC)[i]->GetPid());
  }
  analysisManager->FillNtupleDColumn(1, 0, heavyEdep);
  analysisManager->FillNtupleDColumn(1, 1, heavyTral);
  analysisManager->AddNtupleRow(1);

//  analysisManager->FillNtupleDColumn(2, 0, (*trackHC)[0]->GetKineticEnergy());
//  analysisManager->FillNtupleDColumn(2, 1, (*trackHC)[0]->GetPos().x()/CLHEP::m);
//  analysisManager->FillNtupleDColumn(2, 2, (*trackHC)[0]->GetPos().y()/CLHEP::m);
//  analysisManager->FillNtupleDColumn(2, 3, (*trackHC)[0]->GetPos().z()/CLHEP::m);
//  analysisManager->AddNtupleRow(2); 

//  //Fill histograms
//  analysisManager->FillH1(0, heavyHit->GetEdep());
//  analysisManager->FillH1(1, heavyHit->GetTrackLength());
//  if(heavyHit->GetTrackID()<1e9)
//  {
//     analysisManager->FillH1(2, heavyHit->GetTrackID());
//  }
//  else
//  {
//     analysisManager->FillH1(3, heavyHit->GetTrackID());
//  }
//
//  if(heavyHit->GetTrackID() != 1000010020) G4cout << "Fill pid" << G4endl;
//
//  //Fill ntuple
//  analysisManager->FillNtupleDColumn(1, 0, heavyHit->GetEdep());
//  analysisManager->FillNtupleDColumn(1, 1, heavyHit->GetTrackLength());
//  analysisManager->AddNtupleRow(1);
//
//  analysisManager->FillNtupleDColumn(2, 0, heavyHit->GetTrackID());
//  analysisManager->AddNtupleRow(2);
//
//  if(trackHit->GetTrackID() == 2112)
//  {
//  //std::cout << "neutern in tracker" << ": " << localPos.x() << ", " << localPos.y() << ", " << localPos.z() << std::endl;
//
//  analysisManager->FillH3(0, trackHit->GetPos().x()/CLHEP::m,
//                             trackHit->GetPos().y()/CLHEP::m,
//                             trackHit->GetPos().z()/CLHEP::m);
//
//  analysisManager->FillH1(4, trackHit->GetPos().x()/CLHEP::m);
//  analysisManager->FillH1(5, trackHit->GetPos().y()/CLHEP::m);
//  analysisManager->FillH1(6, trackHit->GetPos().z()/CLHEP::m);
//
//  analysisManager->FillNtupleDColumn(3, 0, trackHit->GetKineticEnergy());
//  analysisManager->FillNtupleDColumn(3, 1, trackHit->GetPos().x()/CLHEP::m);
//  analysisManager->FillNtupleDColumn(3, 2, trackHit->GetPos().y()/CLHEP::m);
//  analysisManager->FillNtupleDColumn(3, 3, trackHit->GetPos().z()/CLHEP::m);
//  analysisManager->AddNtupleRow(3);
//  }
}
