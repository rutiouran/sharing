#include "DeuteronNuclearPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Processes

#include "G4PhotoNuclearProcess.hh"
#include "G4CascadeInterface.hh"
#include "G4ionIonisation.hh"
#include "G4BetheBlochModel.hh"
#include "G4BraggIonModel.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4SystemOfUnits.hh"

#include "G4BuilderType.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"

#include "G4SystemOfUnits.hh"

DeuteronNuclearPhysics::DeuteronNuclearPhysics(const G4String& name)
: G4VPhysicsConstructor(name)
{ }

DeuteronNuclearPhysics::~DeuteronNuclearPhysics()
{ }

void DeuteronNuclearPhysics::ConstructProcess()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();

#if 0
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4String particleName = particle->GetParticleName();

    if( particleName == "deuteron" ) {
	  std::cout << "******** add d" << std::endl;
      //ph->RegisterProcess(new G4hMultipleScattering(), particle);
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
	  //ionIoni->SetEmModel(new G4BraggIonModel(), 1);
	  //ionIoni->SetEmModel(new G4BetheBlochModel(), 2);
      ph->RegisterProcess(ionIoni,                    particle);
      ph->RegisterProcess(new G4NuclearStopping(),    particle);
      
    }
#endif

  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {

      ph->RegisterProcess(new G4PhotoElectricEffect, particle);
      ph->RegisterProcess(new G4ComptonScattering,   particle);
      ph->RegisterProcess(new G4GammaConversion,     particle);
       
    } else if (particleName == "e-") {
    
      ph->RegisterProcess(new G4eMultipleScattering(), particle);
      ph->RegisterProcess(new G4eIonisation,           particle);
      ph->RegisterProcess(new G4eBremsstrahlung(),     particle);
            
    } else if (particleName == "e+") {
    
      ph->RegisterProcess(new G4eMultipleScattering(), particle);
      ph->RegisterProcess(new G4eIonisation,           particle);
      ph->RegisterProcess(new G4eBremsstrahlung(),     particle);
      ph->RegisterProcess(new G4eplusAnnihilation(),   particle);
                  
    } else if (particleName == "mu+" || 
               particleName == "mu-"    ) {

      ph->RegisterProcess(new G4MuMultipleScattering(), particle);
      ph->RegisterProcess(new G4MuIonisation,           particle);
      ph->RegisterProcess(new G4MuBremsstrahlung(),     particle);
      ph->RegisterProcess(new G4MuPairProduction(),     particle);
                   
    } else if( particleName == "proton" ||
               particleName == "pi-" ||
               particleName == "pi+"    ) {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);      
      ph->RegisterProcess(new G4hIonisation,           particle);
     
    } else if( particleName == "alpha" || 
               particleName == "He3"    ) {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4ionIonisation,         particle);
      ph->RegisterProcess(new G4NuclearStopping(),     particle);
            
    } else if( particleName == "GenericIon" ) {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      G4ionIonisation* ionIoni = new G4ionIonisation();
      ionIoni->SetEmModel(new G4IonParametrisedLossModel());
      ph->RegisterProcess(ionIoni,                    particle);
      ph->RegisterProcess(new G4NuclearStopping(),    particle);
      
    } else if ((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) && 
               (particle->GetParticleName() != "chargedgeantino")) {
	
	  //std::cout << particle->GetParticleName() << std::endl;
               
      //all others charged particles except geantino
      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4hIonisation(),         particle);
    }
  }
}
