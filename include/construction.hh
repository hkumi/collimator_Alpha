#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4Element.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    //G4LogicalVolume* GetScoringVolume1() const { return fScoringVolume_1; }

private:
    void DefineMaterials();
    
    G4Material* worldMat;
    G4Material* collimatorMat;
    G4Material* detectorMat;
    G4Material* vacuum;
    
    G4LogicalVolume* fScoringVolume;
    //G4LogicalVolume* fScoringVolume_1;
    
    G4double fBoxSize;
};

#endif
