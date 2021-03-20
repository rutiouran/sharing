#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fWorldBox(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="deuteron");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(13.*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event
  //
  //G4double worldSizeXY = 0;
//  G4double worldSizeZ  = 0;
//
//  if(!fWorldBox)
//  {
//    G4LogicalVolume* worLV
//      = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
//    if(worLV) 
//      fWorldBox = dynamic_cast<G4Box*>(worLV->GetSolid());
//  }
//
//  if ( fWorldBox )
//  {
//    //worldSizeXY = fWorldBox->GetXHalfLengt()*2.;
//    worldSizeZ = fWorldBox->GetZHalfLength()*2.;
//  }
//  else
//  {
//    G4ExceptionDescription msg;
//    msg << "Envelope volume of box shape not found.\n";
//    msg << "Perhaps you have changed geometry.\n";
//    msg << "The gun will be place at the center.";
//    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
//     "MyCode0002",JustWarning,msg);
//  }

  G4double x0 = 0.*mm;
  G4double y0 = 150.*mm;
  //G4double z0 = -1.1/1.2/2*worldSizeZ;
  G4double z0 = -300.*mm;

  //set gun position 
  fParticleGun->SetParticlePosition( G4ThreeVector(x0, y0, z0) );
  //fParticleGun->SetParticleCharge(1*eplus);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
