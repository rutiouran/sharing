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
  auto hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  //for (G4int i=0; i<1; i++ ) {
    fHitsCollection->insert(new HeavyWaterHit());
  //}
}

G4bool HeavyWaterSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
  // energy deposit
  auto edep = step->GetTotalEnergyDeposit();
  
  // energy loss
  //auto edep = step->GetDeltaEnergy();
  
  //Fill histograms and ntuple
  auto analysisManager = G4AnalysisManager::Instance();

  G4int pid = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  if(pid == 2112)
  std::cout << "pid: " << pid << ", edep: " << edep << ", step: " << step << std::endl; 
  
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
  
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  if ( edep==0. && stepLength == 0. ) return false;      

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
     (*fHitsCollection)[0]->Print();
  }
}
