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
   fTrackID(-1),
   fEdep(0.),
   fTrackLength(0.),
   fPid(0),
   fStep(-1)
{}

HeavyWaterHit::~HeavyWaterHit() 
{}

HeavyWaterHit::HeavyWaterHit(const HeavyWaterHit& right)
  : G4VHit()
{
  fTrackID     = right.fTrackID;
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  fPid         = right.fPid;
  fStep        = right.fStep;
}

const HeavyWaterHit& HeavyWaterHit::operator=(const HeavyWaterHit& right)
{
  fTrackID     = right.fTrackID;
  fEdep        = right.fEdep;
  fTrackLength = right.fTrackLength;
  fPid         = right.fPid;
  fStep        = right.fStep;

  return *this;
}

G4bool HeavyWaterHit::operator==(const HeavyWaterHit& right) const
{
  return ( this == &right ) ? true : false;
}
