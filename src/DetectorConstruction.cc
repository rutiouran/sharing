#include "DetectorConstruction.hh"
#include "G4Material.hh"

#include "G4SDManager.hh"
#include "HeavyWaterSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4GlobalMagFieldMessenger.hh"

//G4ThreadLocal
//G4GlobalMagFieldMessenger* DetectorConstrucion::fMagFieldMessenger = 0;

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fScoringVolume(0)
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  G4bool checkOverlaps = true;		//option to switch on/off checking of volumes overlaps

  //
  //World
  //
  G4NistManager* nist = G4NistManager::Instance();
  
  G4double world_sizeXY = 3.*m;
  G4double world_sizeZ  = 3.*m;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld =
        new G4Box("World",	//its name
        0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);		//its size 

  G4LogicalVolume* logicWorld = 
        new G4LogicalVolume(solidWorld,		//its solid
			    world_mat, 		//its material
			    "World");		//its name
  G4VPhysicalVolume* PhysWorld =
	new G4PVPlacement(0,			//no rotation
		          G4ThreeVector(),	//at (0,0,0)
		          logicWorld,		//its logical volume
		          "world",		//its name
		          0,			//its mather volume
		          false,		//no boolean operation
		          0,			//copy number
		          checkOverlaps);	//overlaps checking

 //
 //vacuum chanber
 //
 G4NistManager* nist1 = G4NistManager::Instance();
 G4Material* vach_mat = nist1->FindOrBuildMaterial("G4_Galactic");
 
 G4ThreeVector pos1 = G4ThreeVector(0, 0, 0);

 G4double vach_pRMin = 0.*mm;		//its size 
 G4double vach_pRMax = 250.*mm;
 G4double vach_pDZ = 200.*mm;
 G4double vach_pSshi = 0.*deg;
 G4double vach_pDshi = 360.*deg; 
 G4Tubs* solidvach =
      new G4Tubs("vach",
        vach_pRMin, vach_pRMax, vach_pDZ/2, vach_pSshi, vach_pDshi);
 
 G4LogicalVolume* logicalvach =
      new G4LogicalVolume(solidvach,		//its solid
	       	          vach_mat,		//its material
                          "vach");		//its name
  
  new G4PVPlacement(0,				//no rotation
		    G4ThreeVector(),		//at(0,0,0)
                    logicalvach,			//its logical volume
		    "vach",			//its name
		    logicWorld,			//its mather volume
		    false, 			//no boolean operation
		    0, 				//copy number
		    checkOverlaps);		//overlaps checking

 //
 //hermetic window
 //
 G4NistManager* nist2 = G4NistManager::Instance();
 G4Material* hewi_mat = nist2->FindOrBuildMaterial("G4_Be");
 
 G4ThreeVector pos2 = G4ThreeVector(0, 150*mm, -5.05*mm);

 G4double hewi_DX = 3.*mm;			//its size
 G4double hewi_DY = 6.*mm;
 G4double hewi_DZ = 0.1*mm; 
 
 G4EllipticalTube* solidhewi =
   new G4EllipticalTube("solidhewi",
       hewi_DX/2, hewi_DY/2, hewi_DZ/2);

 G4LogicalVolume* logicalhewi =
   new G4LogicalVolume(solidhewi,		//its solid
		       hewi_mat,		//its material
		       "hewi");			//its name

   new G4PVPlacement(0,				//no rotation
		     pos2,			//at(0, 150.*mm, -5.05*mm)
		     logicalhewi,		//its logical volume
                     "hewi",			//its name
		     logicalvach,		//its mather volume
		     false,			//no boolean operation
		     0,				//copy number
                     checkOverlaps);		//overlaps checking
 //
 //Al chanber
 //
 G4NistManager* nist3 = G4NistManager::Instance();
 G4Material* Alch_mat = nist3->FindOrBuildMaterial("G4_Al");
 
 G4ThreeVector pos3 = G4ThreeVector(0, 0, 0);

 G4double Alch_pRMin = 0.*mm;
 G4double Alch_pRMax = 175.*mm;
 G4double Alch_DZ = 10.*mm;
 G4double Alch_pSshi = 0.*deg;
 G4double Alch_pDshi = 360.*deg;
 G4Tubs* solidAlch =
   new G4Tubs("Alch",
         Alch_pRMin, Alch_pRMax, Alch_DZ/2, Alch_pSshi, Alch_pDshi);

 G4LogicalVolume* logicalAlch = 
   new G4LogicalVolume(solidAlch,		//its solid
		       Alch_mat, 		//its material
		       "Alch");			//its name

     new G4PVPlacement(0,			//no rotation
		       pos3,			//at(0, 0, 0)
		       logicalAlch, 		//its logical volume
		       "Alch",			//its name
		       logicalvach, 		//its mather volume
		       false, 			//no boolean operation
		       0,			//copy number
		       checkOverlaps);		//overlaps checking
 //
 //Heavy Water
 //
 G4double z, a, density;	//Heavy Water material
 G4String name, symble;
 G4int ncomponents, natoms;
 a = 2.00*g/mole;		//specify the molar mass of deuterium
 G4Element* elD = new G4Element(name="deuterium", symble="D", z=1., a);		//creat deuterium

 a = 16.00*g/mole;
 G4Element* elO = new G4Element(name="oxygen", symble="O", z=8., a);

 density = 1.1079*g/cm3;

 G4Material* HeavyWater_mat = new G4Material(name="HeavyWater_mat", density, ncomponents=2);
 HeavyWater_mat->AddElement(elD, natoms=2);
 HeavyWater_mat->AddElement(elO, natoms=1);

 G4ThreeVector pos4 = G4ThreeVector(0.*mm, 150.*mm, -4.6*mm);

 G4double HeavyWater_DX = 3.*mm;
 G4double HeavyWater_DY = 6.*mm;
 G4double HeavyWater_DZ = 0.8*mm;
 G4EllipticalTube* solidHeavyWater =
    new G4EllipticalTube("HeavyWater",
        HeavyWater_DX/2, HeavyWater_DY/2, HeavyWater_DZ/2);

 G4LogicalVolume* logicHeavyWater =
    new G4LogicalVolume(solidHeavyWater,	//its solid
		        HeavyWater_mat,		//its material
		        "HeavyWater");		//its name

  new G4PVPlacement(0,				//no rotation
		    pos4,			//at (0, 150.*mm, -4.6*mm)
		    logicHeavyWater,		//its logical
		    "HeavyWater",		//its name
		    logicalAlch, 		//its mother volume
		    false, 			//no boolean operation
		    0,				//copy numble
		    checkOverlaps);		//overlaps checking

  fScoringVolume = logicHeavyWater;

  return PhysWorld;
}

void DetectorConstruction::ConstructSDandField()
{
 //G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
 
 //
 //Sensitive detectors
 //
 
 auto heavyWaterSD
   = new HeavyWaterSD("HeavyWaterSD", "HeavyWaterHitsCollection");
 G4SDManager::GetSDMpointer()->AddNewDetector(heavyWaterSD);
 SetSensitiveDetector("HeavyWater", heavyWaterSD);
}
