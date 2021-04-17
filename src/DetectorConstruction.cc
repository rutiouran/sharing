#include "DetectorConstruction.hh"
#include "G4Material.hh"

#include "G4SDManager.hh"
#include "HeavyWaterSD.hh"
#include "TrackerSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4EllipticalTube.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4double z, a, density, fractionmass;
  G4String name, symbol;
  G4int ncomponents, natoms;
	
  G4bool checkOverlaps = true;    //option to switch on/off checking of volumes overlaps

  //The parameters of the shielding external layer: Pb
  G4double R_Pb = 10.*mm;
  G4double DZ_Pb_front = 20.*mm;
  G4double DZ_Pb_behind = 20.*mm;

  //The parameters of the shielding internal layer: HDPE
  G4double R_Po = 10.*mm;
  G4double DZ_Po_front = 20.*mm;
  G4double DZ_Po_behind = 20.*mm;
 
  //The parameter of the vacuum chamber 
  G4double vach_pRMin = 0.*mm; 
  G4double vach_pRMax = 250.*mm;
  G4double vach_pDZ = 200.*mm;
  G4double vach_pSshi = 0.*deg;
  G4double vach_pDshi = 360.*deg;

  G4Tubs* solidvach =		//Soild vacuum chamber
      new G4Tubs("vach",
        vach_pRMin, vach_pRMax, vach_pDZ/2, vach_pSshi, vach_pDshi);

  //
  //World
  //
  G4NistManager* nist = G4NistManager::Instance();
  
//  G4double world_sizeXY = 1.2*2*(vach_pRMax + R_Pb + R_Po);
//  G4double world_sizeZ  = 1.2*(vach_pDZ + DZ_Pb_front + DZ_Pb_behind + DZ_Po_front + DZ_Po_behind);
//  G4double worldsize = world_sizeZ>world_sizeXY?world_sizeZ:world_sizeXY;
  
  G4double worldsize = 3.*m;

  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld =
        new G4Box("World",	//its name
        0.5*worldsize, 0.5*worldsize, 0.5*worldsize);	//its size 

  G4LogicalVolume* logicWorld = 
        new G4LogicalVolume(solidWorld,		//its solid
			    world_mat, 		//its material
			    "World");		//its name
  G4VPhysicalVolume* PhysWorld =
	new G4PVPlacement(0,			//no rotation
		          G4ThreeVector(),	//at (0,0,0)
		          logicWorld,		//its logical volume
		          "World",		//its name
		          0,			//its mather volume
		          false,		//no boolean operation
		          0,			//copy number
		          checkOverlaps);	//overlaps checking

  //
  //Tracker
  //
  G4double Trac_RMax = 1.00001*m;
  G4double Trac_RMin = 1.00000*m;
  G4double Trac_SPhi = 0.*deg;
  G4double Trac_STheta = 0.*deg;
  G4double Trac_DPhi = 360.*deg;
  G4double Trac_DTheta = 360.*deg;

  G4Material* tracker_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4ThreeVector tracker_pos = G4ThreeVector(0.*mm, 150.*mm, -4.6*mm);

  G4Sphere* solidTracker =
      new G4Sphere("Tracker",
      Trac_RMin, Trac_RMax, Trac_SPhi, Trac_DPhi, Trac_STheta, Trac_DTheta);

  G4LogicalVolume* logicTracker =
      new G4LogicalVolume(solidTracker,         //its solid
                          tracker_mat,          //its material
                          "Tracker");           //its name
  
        new G4PVPlacement(0,                    //no rotation
			  tracker_pos,
                          logicTracker,         //its logical volume
                          "Tracker",            //its name
                          logicWorld,           //its mather volume
                          false,                //no boolean operation
                          0,                    //copy number
                          checkOverlaps);       //overlaps checking
 
 //
 //Shield(material:Boron containing polyethylene + Pb)
 //
 G4Material* shie_mat1_poly = nist->FindOrBuildMaterial("G4_POLYETHYLENE");		//material: polyethylene
 
 a = 10.811*g/mole;									//material:B4C
 G4Element* elB = new G4Element(name="Boron", symbol="B", z=5., a);

 a = 12.01*g/mole;
 G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);

 density = 2.52*g/cm3;
 G4Material* shie_mat1_B4C = new G4Material(name="B4C", density, ncomponents=2);
 shie_mat1_B4C->AddElement(elB, natoms=4);
 shie_mat1_B4C->AddElement(elC, natoms=1); 
 
 density = 0.09983*g/cm3;
 G4Material* shie_mat = new G4Material("Boron cantaining polythelene", density, ncomponents=2);	//material:Boron cantaining polythelene
 shie_mat->AddMaterial(shie_mat1_poly, fractionmass=92.0*perCent);
 shie_mat->AddMaterial(shie_mat1_B4C,  fractionmass= 8.0*perCent);

 G4Material* shie_mat2_Pb = nist->FindOrBuildMaterial("G4_Pb");                         //material:Pb

 //shie_Po_out
 G4double shie_Po_out_pRMin = 0.00*mm;
 G4double shie_Po_out_pRMax = 250.0*mm+R_Po;
 G4double shie_Po_out_pDZ   = 200.0*mm+DZ_Po_front+DZ_Po_behind;
 G4double shie_Po_out_PSshi = 0.*deg;
 G4double shie_Po_out_PDshi = 360.*deg;

 G4Tubs* solidShie_Po_out = 
	new G4Tubs("Shie_Po_out",	//its name
	shie_Po_out_pRMin, shie_Po_out_pRMax, 0.5*shie_Po_out_pDZ, shie_Po_out_PSshi, shie_Po_out_PDshi);		//its size

 //shie_Po_windows
 G4double shie_Po_win_DX = 1.5*mm;
 G4double shie_Po_win_DY = 3.0*mm;
 G4double shie_Po_win_DZ = DZ_Po_front/2;

 G4EllipticalTube* solidShie_Po_Wind =
        new G4EllipticalTube("Shie_Po_Wind",       //its name
        shie_Po_win_DX, shie_Po_win_DY, shie_Po_win_DZ);               //its size
 
 G4ThreeVector shie_Po_wind_pos = G4ThreeVector(0., 150.*mm, -100.*mm-DZ_Po_behind/2);

 //Shie_Po
 G4ThreeVector shie_Po_in_pos = G4ThreeVector(0., 0., (DZ_Po_front-DZ_Po_behind)/2.);
 G4ThreeVector shie_Po_pos = G4ThreeVector(0., 0., (DZ_Po_behind-DZ_Po_front)/2.);

 G4SubtractionSolid* solidShie_Po1 =
	new G4SubtractionSolid("Shie_Po1", 		//its solid name
			       solidShie_Po_out,	//solid1
			       solidvach,		//solid2
			       0,			//solid2_rotation
			       shie_Po_in_pos);		//solid2_tansition

 G4SubtractionSolid* solidShie_Po =
        new G4SubtractionSolid("Shie_Po",               //its solid name
                               solidShie_Po1,           //solid1
                               solidShie_Po_Wind,       //solid2
			       0,			//solid2_rotation
			       shie_Po_wind_pos);	//solid2_tansition

 G4LogicalVolume* logicShie_Po =
	new G4LogicalVolume(solidShie_Po,	//its solid
			    shie_mat,		//its material
			    "Shie_Po");		//its name

  new G4PVPlacement(0,                          //no rotation
                    shie_Po_pos,                //at (0., 0., (DZ_Po_behind-DZ_Po_front)/2.)
                    logicShie_Po, 	        //its logical volume
                    "Shie_Po",                  //its name
                    logicWorld,                 //its mather volume
                    true,                       //boolean operation:subtraction
                    0,                          //copy number
                    checkOverlaps);             //overlaps checking

 //Target
 //
 G4double Zposition = 0.;
 for(G4int copyNb = 0; copyNb<3; copyNb++)
 {
   if(copyNb != 2)
   Zposition = 100.*mm+0.005*mm+copyNb*DZ_Po_behind/2.;
   else
   Zposition = 100.*mm-0.005*mm+copyNb*DZ_Po_behind/2.;
 
   G4Tubs* targetS
         = new G4Tubs("Target",
                      0, 250.0*mm+R_Po, 0.005*mm, 0.*deg, 360.*deg);

   G4LogicalVolume* targetLV =
   new G4LogicalVolume(targetS,                 //its solid
                       shie_mat,                //its material
                       "Target");               //its name

     new G4PVPlacement(0,			       //no rotation
                       G4ThreeVector(0, 0, Zposition), //at(0, 0, 0)
                       targetLV,                       //its logical volume
                       "Target",	               //its name
                       logicShie_Po,      	       //its mather volume
                       false,			       //no boolean operation
                       copyNb,                         //copy number
                       checkOverlaps);                 //overlaps checking
 }
 
 //shie_Pb_out
 G4double shie_Pb_out_pRMin = 0.00*mm;
 G4double shie_Pb_out_pRMax = 250.0*mm+R_Pb+R_Po;
 G4double shie_Pb_out_pDZ   = 200.0*mm+DZ_Po_front+DZ_Po_behind+DZ_Pb_front+DZ_Pb_behind;
 G4double shie_Pb_out_PSshi = 0.*deg;
 G4double shie_Pb_out_PDshi = 360.*deg;

 G4Tubs* solidShie_Pb_out =
        new G4Tubs("Shie_Pb_out",         //it name
        shie_Pb_out_pRMin, shie_Pb_out_pRMax, 0.5*shie_Pb_out_pDZ, shie_Pb_out_PSshi, shie_Pb_out_PDshi);           //its size

 //shie_Pb_windows
 G4double shie_Pb_win_DX = 1.5*mm;
 G4double shie_Pb_win_DY = 3.0*mm;
 G4double shie_Pb_win_DZ = DZ_Pb_front/2;

 G4EllipticalTube* solidShie_Pb_Wind =
        new G4EllipticalTube("Shie_Pb_Wind",       //its name
        shie_Pb_win_DX, shie_Pb_win_DY, shie_Pb_win_DZ);               //its size

 G4ThreeVector shie_Pb_wind_pos = G4ThreeVector(0., 150.*mm, -100.*mm-DZ_Po_front/2-DZ_Po_behind/2-DZ_Pb_behind/2); 

 //Shie_Pb
 G4ThreeVector shie_Pb_in_pos = G4ThreeVector(0., 0., (DZ_Pb_front - DZ_Pb_behind)/2.);
 G4ThreeVector shie_Pb_pos = G4ThreeVector(0., 0., (DZ_Po_behind + DZ_Pb_behind - DZ_Po_front - DZ_Pb_front)/2.);

 G4SubtractionSolid* solidShie_Pb1 =
        new G4SubtractionSolid("Shie_Pb1",              //its solid name
                               solidShie_Pb_out,        //solid1
                               solidShie_Po_out,        //solid2
			       0,			//solid2_rotation
			       shie_Pb_in_pos);		//solid2_transition
 
 G4SubtractionSolid* solidShie_Pb =
        new G4SubtractionSolid("Shie_Pb",               //its solid name
                               solidShie_Pb1,           //solid1
                               solidShie_Pb_Wind,       //solid2
                               0,			//solid2_rotation
                               shie_Pb_wind_pos);	//solid2_transition

 G4LogicalVolume* logicShie_Pb =
        new G4LogicalVolume(solidShie_Pb,       //its solid
                            shie_mat2_Pb,       //its material
                            "Shie_Pb");         //its name

  new G4PVPlacement(0,                          //no rotation
                    shie_Pb_pos,                //at (0., 0., (DZ_Po_front + DZ_Pb_front - DZ_Po_behind - DZ_Pb_behind)/2.)
                    logicShie_Pb,               //its logical volume
                    "Shie_Pb",  	        //its name
                    logicWorld,                 //its mather volume
                    true,                       //boolean operation:subtraction
                    0,                          //copy number
                    checkOverlaps);             //overlaps checking


 //
 //vacuum chanber
 //
 G4Material* vach_mat = nist->FindOrBuildMaterial("G4_Galactic");
 
 G4LogicalVolume* logicalvach =
      new G4LogicalVolume(solidvach,		//its solid
	       	          vach_mat,		//its material
                          "vach");		//its name
  
  new G4PVPlacement(0,				//no rotation
		    G4ThreeVector(),		//at(0,0,0)
                    logicalvach,		//its logical volume
		    "vach",			//its name
		    logicWorld,			//its mather volume
		    false, 			//no boolean operation
		    0, 				//copy number
		    checkOverlaps);		//overlaps checking

 //
 //hermetic window
 //
 G4Material* hewi_mat = nist->FindOrBuildMaterial("G4_Be");
 
 G4ThreeVector hewi_pos = G4ThreeVector(0, 150*mm, -5.05*mm);

 G4double hewi_DX = 3.*mm;			//its size
 G4double hewi_DY = 6.*mm;
 G4double hewi_DZ = 0.1*mm; 
 
 G4EllipticalTube* solidhewi =
   new G4EllipticalTube("hewi",
       hewi_DX/2, hewi_DY/2, hewi_DZ/2);

 G4LogicalVolume* logicalhewi =
   new G4LogicalVolume(solidhewi,		//its solid
		       hewi_mat,		//its material
		       "hewi");			//its name

   new G4PVPlacement(0,				//no rotation
		     hewi_pos,			//at(0, 150.*mm, -5.05*mm)
		     logicalhewi,		//its logical volume
                     "hewi",			//its name
		     logicalvach,		//its mather volume
		     false,			//no boolean operation
		     0,				//copy number
                     checkOverlaps);		//overlaps checking
 //
 //Al chanber
 //
 G4Material* Alch_mat = nist->FindOrBuildMaterial("G4_Al");
 
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
		       G4ThreeVector(),		//at(0, 0, 0)
		       logicalAlch, 		//its logical volume
		       "Alch",			//its name
		       logicalvach, 		//its mather volume
		       false, 			//no boolean operation
		       0,			//copy number
		       checkOverlaps);		//overlaps checking

 //
 //Cooling Water
 //
 G4Material* CoWa_mat = nist->FindOrBuildMaterial("G4_WATER");

 G4ThreeVector CoWa_pos = G4ThreeVector(0.*mm, 150.*mm, 2.5*mm);
 
 G4double CoWa_DX = 6.*mm;
 G4double CoWa_DY = 12.*mm;
 G4double CoWa_DZ = 1.5*mm;
 G4EllipticalTube* solidCoWa =
   new G4EllipticalTube("CoWa",
       CoWa_DX, CoWa_DY, CoWa_DZ);

 G4LogicalVolume* logicalCoWa =
   new G4LogicalVolume(solidCoWa,		//its solid
		       CoWa_mat,		//its material
                       "CoWa");			//its name

   new G4PVPlacement(0,				//no rotation
		     CoWa_pos,			//at(0.*mm, 150.*mm, 2.5*mm)
		     logicalCoWa,		//its logical volume
		     "CoWa",			//its name
		     logicalAlch,		//its mather volume
		     false,			//no boolean operation
		     0,				//copy number
		     checkOverlaps);		//overlaps checking
  
 //
 //Heavy Water
 //
 G4Isotope* O16= new G4Isotope("O16", 8, 16); 
 G4Element* O  = new G4Element("O_of_Heavy_Water", "O" , 1);
 O->AddIsotope(O16, 100*perCent);

 G4Isotope* H2 = new G4Isotope("H2",1,2);
 G4Element* D  = new G4Element("D_of_Heavy_Water", "D", 1);
 D->AddIsotope(H2, 100*perCent);

 G4Material* D2O = new G4Material("D2O", 1.11*g/cm3, ncomponents=2,
                        kStateLiquid, 293.15*kelvin, 1*atmosphere);
 D2O->AddElement(D, natoms=2);
 D2O->AddElement(O, natoms=1);

 G4ThreeVector hewa_pos = G4ThreeVector(0.*mm, 150.*mm, -4.6*mm);

 G4double HeavyWater_DX = 1.5*mm;
 G4double HeavyWater_DY = 3.*mm;
 G4double HeavyWater_DZ = 0.4*mm;
 G4EllipticalTube* solidHeavyWater =
    new G4EllipticalTube("HeavyWater_SO",
        HeavyWater_DX, HeavyWater_DY, HeavyWater_DZ);

 G4LogicalVolume* logicHeavyWater =
    new G4LogicalVolume(solidHeavyWater,	//its solid
		        D2O,			//its material
		        "HeavyWater_LV");	//its name

  new G4PVPlacement(0,				//no rotation
		    hewa_pos,			//at (0, 150.*mm, -4.6*mm)
		    logicHeavyWater,		//its logical
		    "HeavyWater_PV",		//its name
		    logicalAlch, 		//its mother volume
		    false, 			//no boolean operation
		    0,				//copy numble
		    checkOverlaps);		//overlaps checking

// Visualization attributes

  G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* shieldVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  G4VisAttributes* heavywaterVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,0.0));

  logicWorld      ->SetVisAttributes(boxVisAtt);
  logicShie_Pb    ->SetVisAttributes(shieldVisAtt);
  logicShie_Po    ->SetVisAttributes(shieldVisAtt);
  logicHeavyWater ->SetVisAttributes(heavywaterVisAtt);

//  std::cout << *(G4Material::GetMaterialTable()) << std::endl;	//Get material table

  return PhysWorld;
}

void DetectorConstruction::ConstructSDandField()
{
 G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
 
 //
 //Sensitive detectors
 //
 auto heavyWaterSD
   = new HeavyWaterSD("sharing/HeavyWaterSD", "HeavyWaterHitsCollection");
 G4SDManager::GetSDMpointer()->AddNewDetector(heavyWaterSD);
 SetSensitiveDetector("HeavyWater_LV", heavyWaterSD);

 auto trackerSD
   = new TrackerSD("sharing/TrackerSD", "TrackerHitsCollection");
 G4SDManager::GetSDMpointer()->AddNewDetector(trackerSD);
 SetSensitiveDetector("Tracker", trackerSD);

// auto targetSD
//   = new TargetSD("sharing/TargetSD", "TargetHitsCollection");
// G4SDManager::GetSDMpointer()->AddNewDetector(targetSD);
// SetSensitiveDetector("Target", targetSD);
}
