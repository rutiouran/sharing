#include "HeavyWaterHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<HeavyWaterHit>* HeavyWaterHitAllocator = 0;

HeavyWaterHit::HeavyWaterHit()
 : G4VHit(),
   fEdep(0.),
   fTrackLength(0.)
{}

HeavyWaterHit::~HeavyWaterHit() 
{}

HeavyWaterHit::HeavyWaterHit(const HeavyWaterHit& right)
  : G4VHit()
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
}

const HeavyWaterHit& HeavyWaterHit::operator=(const HeavyWaterHit& right)
{
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;

  return *this;
}

G4bool HeavyWaterHit::operator==(const HeavyWaterHit& right) const
{
  return ( this == &right ) ? true : false;
}

void HeavyWaterHit::Print()
{
  G4cout
     << "Edep: " 
     << std::setw(7) << G4BestUnit( fEdep, "Energy")
     << "Track length: " 
     << std::setw(7) << G4BestUnit( fTrackLength, "Length")
     << G4endl;
}
