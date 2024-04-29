#include "construction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include <algorithm>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{

    DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::DefineMaterials()
{

  G4NistManager *nist = G4NistManager::Instance();
  G4int ncomponents, natoms;
  G4double massfraction;

  G4double Vdens = 1.e-25*g/cm3;
  G4double Vpres = 1.e-19*pascal;
  G4double Vtemp = 0.1*kelvin;

  G4double a, z;
  C = nist->FindOrBuildElement("C");
  N  = new G4Element("Nitrogen","N",7.,14.007*g/mole);
  O  = new G4Element("Oxygen","O",8.,15.999*g/mole);
  F  = new G4Element("Fluorine","F",9.,18.998*g/mole);

  
  // vacuum
  Vacc = new G4Material("Galactic", z=1, a=1.01*g/mole, Vdens, kStateGas, Vtemp, Vpres);

  //Copper
  mat_copper = nist->FindOrBuildMaterial("G4_Cu");
  
  //silicon
  elSi = nist->FindOrBuildMaterial("G4_Si");
;
  
//....................End of scintillator material........................................




}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  // The world
  fBoxSize = 150*cm;


  sBox = new G4Box("world",                             //its name
                   fBoxSize/2,fBoxSize/2,fBoxSize/2);   //its dimensions

  fLBox = new G4LogicalVolume(sBox,                     //its shape
                             Vacc,                      //its material
                             "World");                  //its name

  fPBox = new G4PVPlacement(0,                          //no rotation
                            G4ThreeVector(),            //at (0,0,0)
                            fLBox,                      //its logical volume
                            "World",                    //its name
                            0,                          //its mother  volume
                            false,                      //no boolean operation
                            0);                         //copy number


  //The Borated polythylene_block1 with pinhole

  BoratedSize = 50*mm;
  Borated_thickness = 5*mm;
  Borated_Box1 = new G4Box("Borated1",                             //its name
                   BoratedSize/2,  BoratedSize/2,Borated_thickness/2);   //its dimensions



  Hole = new G4Tubs("BoxHole", 0.0*mm, 1*mm, 2.5*mm, 0*deg, 360*deg);

  Hole_LV = new G4LogicalVolume(Hole,                     //its shape
                              Vacc,                      //its material
                             "H1");                  //its name

   Borated_LV1 = new G4LogicalVolume(Borated_Box1,                     //its shape
                              mat_copper,                      //its material
                             "Borated1", 0,0,0);                  //its name



  Borated_PV1 = new G4PVPlacement(0,                          //no rotation
                            G4ThreeVector(0*mm,0*mm,400*mm),            //at (0,0,0)
                             Borated_LV1,                      //its logical volume
                            "Borated1",                    //its name
                            fLBox,                          //its mother  volume
                            false,                      //no boolean operation
                            0,true);                         //copy number

  Hole_PV = new G4PVPlacement(0,                          //no rotation
                            G4ThreeVector(0*mm,0*mm,0*mm),            //at (0,0,0)
                             Hole_LV,                      //its logical volume
                            "H1",                    //its name
                            Borated_LV1,                          //its mother  volume
                            false,                      //no boolean operation
                            0,true);                         //copy number


  G4double ScThick_1 =  3.0*mm;

  auto sScore_1 = new G4Box("sScore_1",
                            70/2*mm,70/2*mm,ScThick_1/2);

  auto fLScore_1 = new G4LogicalVolume(sScore_1,
                                        elSi,
                                      "fLScore_1");

  auto fPScore_r_1 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,500*mm),
                                    fLScore_1,
                                    "fPScore_r_1",
                                    fLBox,
                                    false,
                                    0,true);
  

  fScoringVolume_1 = fLScore_1;




  return fPBox;
}

//...ooooooooooooooooo..................................oooooooooooooooooooo......



void DetectorConstruction::ConstructSDandField()
{
 // sensitive detectors -----------------------------------------------------
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->SetVerboseLevel(0);

  //Define Multi-Detector and Register
  //--------------------------------------------------------------------------------------------
  G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector("IonPro");
  SDman->AddNewDetector(det);
//  fLScore->SetSensitiveDetector(det);
}
