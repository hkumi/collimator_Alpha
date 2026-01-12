#include "run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

MyRunAction::MyRunAction()
{
    // Create analysis manager
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->SetNtupleMerging(true);
    man->SetVerboseLevel(1);
    
    // Create ntuple
    man->CreateNtuple("AlphaData", "Alpha particle data at detector");
    man->CreateNtupleDColumn("x_mm");
    man->CreateNtupleDColumn("y_mm");
    man->CreateNtupleDColumn("energy_MeV");
    man->CreateNtupleDColumn("angle_deg");
    man->FinishNtuple();
    
    // Create histograms
    man->CreateH2("position", "Position distribution on detector",
                  200, -10, 10, 200, -10, 10);  // x,y in mm
    man->CreateH2("angle_energy", "Angle vs Energy",
                  100, 0, 5, 100, 0, 5.5);  // angle(deg) vs energy(MeV)
    man->CreateH1("energy", "Energy spectrum",
                  100, 0, 5.5);  // energy in MeV
    man->CreateH1("angle", "Angular distribution",
                  100, 0, 5);  // angle in degrees
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output"+strRunID.str()+".root");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    // Save and close output
    man->Write();
    man->CloseFile();
}
