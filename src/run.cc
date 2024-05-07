#include "run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
MyRunAction::MyRunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetNtupleMerging(true);
    man->SetVerboseLevel( 1 );
    

    
 
    man->CreateNtuple("thermal","thermal");
    man->CreateNtupleDColumn("Energy");
    man->FinishNtuple(0);


    man->CreateH2("xy1 ","xy1", 100, -0.1, 0.1, 100, -0.1, 0.1);
    

    man->CreateH2("a1 ","angle vs energy_1", 100, 0, 100, 100, 0, 2);
    
    man->CreateH1("Energy","Energy", 100,0 , 5);
       
    
    




}

MyRunAction::~MyRunAction()
{

}

void MyRunAction::BeginOfRunAction(const G4Run* run )
{

     
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID;
    strRunID << runID;

    man->OpenFile("output"+strRunID.str()+".root");
}
void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile();

}
