#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonQMDPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "GammaNuclearPhysics.hh"
#include "ElectromagneticPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

//#include "G4DecayPhysics.hh"
//#include "G4RadioactiveDecayPhysics.hh"
#include "DeuteronNuclearPhysics.hh"
//#include "G4ionIonisation.hh"

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{ 
  G4int verb = 1;
  SetVerboseLevel(verb);

  AddTransportation();

  //Hadron Elastic scattering
  //
  RegisterPhysics(new G4HadronElasticPhysicsHP(verb));

  //Hadron Inelastic Physics
  //
  RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(verb));

  // Ion Elastic scattering
  //
  //RegisterPhysics(new G4IonElasticPhysics(verb));

  // Ion Inelastic physics
  //
  //RegisterPhysics(new G4IonPhysics(verb));
  //RegisterPhysics(new G4IonQMDPhysics(verb));
  RegisterPhysics(new G4IonPhysics(verb));

  // stopping Particles
  //
  RegisterPhysics(new G4StoppingPhysics(verb));

  // Gamma-Nuclear Physics
  //
  RegisterPhysics( new GammaNuclearPhysics("gamma"));
  
  // EM physics
  //
  RegisterPhysics(new ElectromagneticPhysics(verb));
  //RegisterPhysics(new G4EmStandardPhysics(verb));

  // Decay
  //
  RegisterPhysics(new G4DecayPhysics(verb));

  // Radioactive decay
  //
  RegisterPhysics(new G4RadioactiveDecayPhysics(verb));


  //G4ionIonisation
  //
  RegisterPhysics(new DeuteronNuclearPhysics(verb));



//  //Specify the complexity of the output information
//  SetVerboseLevel(1);
//  //Decay
//  RegisterPhysics(new G4DecayPhysics());
//
//  //Radioactive Decay
//  RegisterPhysics(new G4RadioactiveDecayPhysics());
//
//
//
//  RegisterPhysics(new ElectromagneticPhysics());
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::SetCuts()
{
  SetCutValue(0*mm, "proton");
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");
}
