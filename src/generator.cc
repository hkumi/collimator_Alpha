#include "generator.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <cmath>

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
    
    // Define alpha particle
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("alpha");
    fParticleGun->SetParticleDefinition(particle);
    
    // 5 MeV alpha particles
    fParticleGun->SetParticleEnergy(5*MeV);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    // Random source position within 5mm diameter (2.5mm radius)
    G4double sourceRadius = 2.5*mm;
    G4double r = G4UniformRand() * sourceRadius;
    G4double phi = G4UniformRand() * 2 * CLHEP::pi;
    
    G4double x = r * std::cos(phi);
    G4double y = r * std::sin(phi);
    
    // Source positioned 400mm before collimator
    G4double z = -400*mm;
    
    fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
    
    // Direction: aim toward the hole with some angular spread
    // Calculate maximum angle to just cover the 1mm hole at 400mm distance
    G4double maxAngle = std::atan(0.5*mm / 400*mm);  // ~0.07 degrees
    
    // Generate random direction within this cone
    G4double theta = G4UniformRand() * maxAngle;
    G4double dirPhi = G4UniformRand() * 2 * CLHEP::pi;
    
    G4double dirX = std::sin(theta) * std::cos(dirPhi);
    G4double dirY = std::sin(theta) * std::sin(dirPhi);
    G4double dirZ = std::cos(theta);
    
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dirX, dirY, dirZ));
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
