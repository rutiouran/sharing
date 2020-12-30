#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "DeuteronNuclearPhysics.hh"
#include "G4ionIonisation.hh"

#include "ElectromagneticPhysics.hh"

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{ 
  //Specify the complexity of the output information
  SetVerboseLevel(1);
  //Decay
  RegisterPhysics(new G4DecayPhysics());

  //Radioactive Decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  //G4ionIonisation
  RegisterPhysics(new DeuteronNuclearPhysics());

  RegisterPhysics(new ElectromagneticPhysics());
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();

}
