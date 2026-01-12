#include "construction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include <algorithm>

DetectorConstruction::DetectorConstruction()
{
    DefineMaterials();
}

DetectorConstruction::~DetectorConstruction()
{ }

void DetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();
    
    // Vacuum for world and holes
    worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    
    // Collimator material (Brass or Copper)
    collimatorMat = nist->FindOrBuildMaterial("G4_Cu");  // Copper
    
    // Detector material (Silicon)
    detectorMat = nist->FindOrBuildMaterial("G4_Si");
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    // World volume
    fBoxSize = 500*mm;
    
    auto solidWorld = new G4Box("World", 
                                fBoxSize/2, 
                                fBoxSize/2, 
                                fBoxSize/2);
    
    auto logicWorld = new G4LogicalVolume(solidWorld, 
                                          worldMat, 
                                          "World");
    
    auto physWorld = new G4PVPlacement(0, 
                                       G4ThreeVector(), 
                                       logicWorld, 
                                       "World", 
                                       0, 
                                       false, 
                                       0, 
                                       true);
    
    // === COLLIMATOR ===
    // Main collimator block (5 mm thick brass/copper)
    G4double collimatorThickness = 5*mm;
    G4double collimatorSizeXY = 50*mm;
    
    auto solidCollimator = new G4Box("Collimator",
                                     collimatorSizeXY/2,
                                     collimatorSizeXY/2,
                                     collimatorThickness/2);
    
    auto logicCollimator = new G4LogicalVolume(solidCollimator,
                                               collimatorMat,
                                               "Collimator");
    
    // Position collimator at z = 0
    auto physCollimator = new G4PVPlacement(0,
                                            G4ThreeVector(0, 0, 0),
                                            logicCollimator,
                                            "Collimator",
                                            logicWorld,
                                            false,
                                            0,
                                            true);
    
    // === 1mm HOLE IN COLLIMATOR ===
    G4double holeRadius = 0.5*mm;  // 1mm diameter
    G4double holeLength = collimatorThickness;  // EXACTLY the collimator thickness
    
    auto solidHole = new G4Tubs("Hole",
                                0,
                                holeRadius,
                                holeLength/2,  // Half-length
                                0,
                                360*deg);
    
    auto logicHole = new G4LogicalVolume(solidHole,
                                         worldMat,
                                         "Hole");
    
    // Place hole at center of collimator
    auto physHole = new G4PVPlacement(0,
                                      G4ThreeVector(0, 0, 0),
                                      logicHole,
                                      "Hole",
                                      logicCollimator,
                                      false,
                                      0,
                                      true);
    
    // === DETECTOR (Silicon) ===
    G4double detectorThickness = 1*mm;
    G4double detectorSizeXY = 100*mm;
    
    auto solidDetector = new G4Box("Detector",
                                   detectorSizeXY/2,
                                   detectorSizeXY/2,
                                   detectorThickness/2);
    
    auto logicDetector = new G4LogicalVolume(solidDetector,
                                             detectorMat,
                                             "Detector");
    
    // Position detector RIGHT AFTER collimator (zero distance)
    // Collimator front is at z = collimatorThickness/2 = 2.5mm
    // Detector front should be at z = collimatorThickness/2 = 2.5mm
    // Detector center should be at z = collimatorThickness/2 + detectorThickness/2
    G4double detectorPosZ = collimatorThickness/2 + detectorThickness/2;
    
    auto physDetector = new G4PVPlacement(0,
                                          G4ThreeVector(0, 0, detectorPosZ),
                                          logicDetector,
                                          "Detector",
                                          logicWorld,
                                          false,
                                          0,
                                          true);
    
    fScoringVolume = logicDetector;
    
    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    
    // MultiFunctionalDetector for scoring
    G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector("DetectorMFD");
    SDman->AddNewDetector(det);
    
    // Set detector as sensitive
    fScoringVolume->SetSensitiveDetector(det);
}
