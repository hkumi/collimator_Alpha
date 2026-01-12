#include "stepping.hh"
#include "construction.hh"
#include "event.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
    fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    // Check if particle is alpha
    if (step->GetTrack()->GetDefinition()->GetParticleName() != "alpha") {
        return;
    }
    
    // Get detector construction
    const DetectorConstruction* detectorConstruction = 
        static_cast<const DetectorConstruction*>(
            G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    // Get current volume
    G4LogicalVolume* volume = step->GetPreStepPoint()
        ->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    // Check if we're in the detector
    if (volume != detectorConstruction->GetScoringVolume()) {
        return;
    }
    
    // Get analysis manager
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    // If this is the first step in the detector
    if (step->IsFirstStepInVolume()) {
        G4StepPoint* postStepPoint = step->GetPostStepPoint();
        
        // Get position on detector (in mm)
        G4ThreeVector position = postStepPoint->GetPosition();
        G4double x = position.x() / mm;
        G4double y = position.y() / mm;
        
        // Get energy at detector
        G4double energy = postStepPoint->GetKineticEnergy() / MeV;
        
        // Get angle (relative to z-axis)
        G4ThreeVector momentumDir = postStepPoint->GetMomentumDirection();
        G4double angle = std::acos(momentumDir.z()) / deg;
        
        // Fill histograms
        man->FillH2(0, x, y);           // Position distribution
        man->FillH2(1, angle, energy);  // Angle vs Energy
        man->FillH1(0, energy);         // Energy spectrum
        man->FillH1(1, angle);          // Angular distribution
        
        // Fill ntuple
        man->FillNtupleDColumn(0, 0, x);
        man->FillNtupleDColumn(0, 1, y);
        man->FillNtupleDColumn(0, 2, energy);
        man->FillNtupleDColumn(0, 3, angle);
        man->AddNtupleRow(0);
        
        // Optional: kill track after detection
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}
