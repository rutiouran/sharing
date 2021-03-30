#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "TrackerHit.hh"
#include "HeavyWaterHit.hh"

#include "G4ThreeVector.hh"

//class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

private:
  HeavyWaterHitsCollection* GetHeavyHitsCollection(G4int hcID, const G4Event* event) const;
  
  TrackerHitsCollection*    GetTrackHitsCollection(G4int hcID, const G4Event* event) const;

//  void PrintHeavyEventStatistics(G4double heavyEdep, G4double heavyTrackLength) const;
//  void PrintTrackEventStatistics(G4int trackID, G4ThreeVector trackPos) const;


  G4int fHeavyHCID;
  G4int fTrackHCID;
};
#endif
