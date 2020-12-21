#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "DeuteronNuclearPhysics.hh"

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{ 
  //Specify the complexity of the output information
  SetVerboseLevel(1);
  //Decay
  RegisterPhysics(new G4DecayPhysics());
  //Dadioactive Decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();

}
