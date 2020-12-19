#ifndef DeuteronNuclearPhysics_h
#define DeuteronNuclearPhysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class DeuteronNuclearPhysics : public G4VPhysicsConstructor
{
  public:
    DeuteronNuclearPhysics(const G4String& name = "deuteron");
    ~DeuteronNuclearPhysics();
  public:
    virtual void ConstructParticle() {};
    virtual void ConstructProcess();
};

#endif
