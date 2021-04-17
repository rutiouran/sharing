#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4IonQMDPhysics.hh"
#include "GammaNuclearPhysics.hh"

#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"

#include "ElectromagneticPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonPhysicsPHP.hh"

PhysicsList::PhysicsList()
:G4VModularPhysicsList()
{
  G4int verb = 1;
  SetVerboseLevel(verb);

  //RegisterPhysics( new HadronElasticPhysicsHP(verb) );
  //RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));
  //RegisterPhysics( new G4HadronPhysicsQGSP_BERT_HP(verb));
  //RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
  //RegisterPhysics( new G4HadronInelasticQBBC(verb));
  //RegisterPhysics( new G4HadronPhysicsINCLXX(verb));
  //RegisterPhysics( new G4IonQMDPhysics(verb));
  //RegisterPhysics( new G4IonPhysicsXS(verb));
  //RegisterPhysics(new G4EmStandardPhysics(verb));
  //RegisterPhysics( new G4IonINCLXXPhysics(verb));
  // Hadron Inelastic Physics
  //RegisterPhysics( new G4HadronPhysicsQGSP_BIC_AllHP(verb));
  // Ion Elastic scattering
  //RegisterPhysics( new G4IonElasticPhysics(verb));

  ////////////////////////////////////////////////////////////////////////////////////
  // Hadron Elastic scattering
  RegisterPhysics( new G4HadronHElasticPhysics(verb));

  // Ion Inelastic Physics
  
RegisterPhysics( new G4IonPhysicsPHP(verb));

  // stopping Particles
  //RegisterPhysics( new G4StoppingPhysics(verb));
      
  // Gamma-Nuclear Physics
  //RegisterPhysics( new GammaNuclearPhysics("gamma"));

  // EM physics
  RegisterPhysics(new ElectromagneticPhysics());

  // seems mandatory ...
  // Decay
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::SetCuts()
{
  //SetCutValue(0*mm, "proton");
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");      
}
