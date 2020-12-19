#include "DeuteronNuclearPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Processes

#include "G4PhotoNuclearProcess.hh"
#include "G4CascadeInterface.hh"

#include "G4SystemOfUnits.hh"

DeuteronNuclearPhysics::DeuteronNuclearPhysics(const G4String& name)
: G4VPhysicsConstructor(name)
{ }

DeuteronNuclearPhysics::~DeuteronNuclearPhysics()
{ }

void DeuteronNuclearPhysics::ConstructProcess()
{
  G4ProcessManager* pManager = G4Deuteron::Deuteron()->GetProcessManager();

  G4PhotoNuclearProcess* process = new G4PhotoNuclearProcess();

  G4CascadeInterface* bertini = new G4CascadeInterface();
  bertini->SetMaxEnergy(13*MeV);
  process->RegisterMe(bertini);

  pManager->AddDiscreteProcess(process);
}
