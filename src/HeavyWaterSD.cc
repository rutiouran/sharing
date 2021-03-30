#include "HeavyWaterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "g4root.hh"

HeavyWaterSD::HeavyWaterSD( const G4String& name, 
                            const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr)
{
  collectionName.insert(hitsCollectionName);
}

HeavyWaterSD::~HeavyWaterSD() 
{}

void HeavyWaterSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new HeavyWaterHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  fHitsCollection->insert(new HeavyWaterHit());
}

G4bool HeavyWaterSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
  //Energy deposit
  auto edep = step->GetTotalEnergyDeposit();
 
  //Step length
  G4double stepLength = 0.;
  if( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0)
  {
    stepLength = step->GetStepLength();
    G4cout << "记录一次步长" << G4endl;
  }

  if(edep == 0. && stepLength == 0.) return false;

//  HeavyWaterHit* newHit = new HeavyWaterHit();	//无效方法
//
//  newHit->SetTrackID(step->GetTrack()->GetTrackID());
//  newHit->SetEdep(edep);
//  newHit->SetTrackLength(stepLength);
//  newHit->Add(edep, stepLength);
//  fHitsCollection->insert(newHit);			//无效方法

  //Fill histograms and ntuple
  auto analysisManager = G4AnalysisManager::Instance();

  G4int pid = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  
  G4int ParentID = step->GetTrack()->GetParentID();
  if(pid == 2112)
  G4cout << "pid: " << pid << ", edep: " << edep << ", step: " << step << G4endl; 
  
  if(pid<1e9)
  {
     analysisManager->FillH1(2, pid);
  }
  else
  {
     analysisManager->FillH1(3, pid);
  }

  analysisManager->FillNtupleDColumn(2, 0, pid);
  analysisManager->AddNtupleRow(2);

  //Avoid double counting
  //if(pid == 2112) step->GetTrack()->SetTrackStatus(fStopAndKill);

  auto touchable = (step->GetPreStepPoint()->GetTouchable());
    
  // Get calorimeter cell id 
  auto layerNumber = touchable->GetReplicaNumber(1);
  
  // Get hit accounting data for this cell
  auto hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("HeavyWaterSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
  }         

  // Add values
  hit->Add(edep, stepLength);
  return true;
}

void HeavyWaterSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     auto nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits in the tracker chambers: " << G4endl;
     //for ( std::size_t i=0; i<nofHits; ++i ) 
     //(*fHitsCollection)[0]->Print();
  }
}
