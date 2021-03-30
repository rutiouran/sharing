#ifndef HeavyWaterHit_h
#define HeavyWaterHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"
/// Heavy Water Target hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class HeavyWaterHit : public G4VHit
{
  public:
    HeavyWaterHit();
    HeavyWaterHit(const HeavyWaterHit&);
    virtual ~HeavyWaterHit();

    // operators
    const HeavyWaterHit& operator=(const HeavyWaterHit&);
    G4bool operator==(const HeavyWaterHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double dl);

//    //Set methods
//    void SetTrackID    (G4int track)	  {fTrackID = track;};
//    void SetEdep       (G4double de)      {fEdep = de;};
//    void SetTrackLength(G4double dl)      {fTrackLength = dl;};

    // get methods
    G4int    GetTrackID() const 	{return fTrackID;};
    G4double GetEdep() const		{return fEdep;};
    G4double GetTrackLength() const	{return fTrackLength;};

  private:
    G4int    fTrackID;
    G4double fEdep;
    G4double fTrackLength;
};

using HeavyWaterHitsCollection = G4THitsCollection<HeavyWaterHit>;

extern G4ThreadLocal G4Allocator<HeavyWaterHit>* HeavyWaterHitAllocator;

inline void* HeavyWaterHit::operator new(size_t)
{
  if (!HeavyWaterHitAllocator) {
    HeavyWaterHitAllocator = new G4Allocator<HeavyWaterHit>;
  }
  void *hit;
  hit = (void *) HeavyWaterHitAllocator->MallocSingle();
  return hit;
}

inline void HeavyWaterHit::operator delete(void *hit)
{
  if (!HeavyWaterHitAllocator) {
    HeavyWaterHitAllocator = new G4Allocator<HeavyWaterHit>;
  }
  HeavyWaterHitAllocator->FreeSingle((HeavyWaterHit*) hit);
}

inline void HeavyWaterHit::Add(G4double de, G4double dl)
{
  fEdep += de;
  G4cout << "求和步骤: fEdep = " << fEdep << G4endl; 
  fTrackLength += dl;
  G4cout << "求和步骤: fTrackLength = " << fTrackLength << G4endl;
}

#endif
