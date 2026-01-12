

# Geant4 Alpha Collimator Simulation

A Geant4 simulation of alpha particles passing through a collimator system for beam profile analysis.

# 📋 Project Overview

This simulation models:
- Source: 5 MeV alpha particles from a 5 mm diameter circular source
- Collimator: 5 mm thick copper with a 1 mm diameter hole
- Detector: Silicon detector positioned immediately after collimator (zero distance)
- Goal: Analyze the beam profile after collimation

# 🏗️ Geometry Setup

```
┌─────────────────────────────────────────────────┐
│                    WORLD (500mm)                 │
│                                                 │
│  SOURCE (5mm)        400mm          COLLIMATOR  │
│    ○ ────────────────▶●─────────────▶           │
│  z=-400mm           z=0mm          z=2.5-3.5mm  │
│                      (1mm hole)     (Detector)  │
└─────────────────────────────────────────────────┘
```

**Dimensions:**
- Source: 5 mm diameter circular source at z = -400 mm
- Collimator: 5 mm thick copper, 50×50 mm cross-section at z = 0 mm
- Hole: 1 mm diameter through collimator
- Detector: 1 mm thick silicon, 100×100 mm at z = 2.5-3.5 mm


## 🚀 Installation & Setup

### Prerequisites
- Geant4 (version 10.7 or later recommended)
- CMake (version 3.16 or later)
- ROOT (for data analysis)
- C++ compiler with C++17 support

### Building the Simulation

```bash
# Clone the repository
git clone https://github.com/hkumi/collimator_Alpha.git
cd collimator_Alpha

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DGeant4_DIR=/path/to/geant4/lib/Geant4-xx.yy.z

# Build
make -j4
```

## 🎯 Running the Simulation

# Basic Run (10,000 events)
```bash
cd build
./sim  run.mac
```



## 📊 Output Analysis

The simulation generates a ROOT file containing:

# Histograms:
- position - 2D histogram of hit positions on detector (x vs y)
- angle_energy - Angle vs energy correlation
- energy - Energy spectrum of alpha particles
- angle - Angular distribution


# Analyzing with ROOT:
```bash
# Open ROOT with output file
root output.root

# In ROOT session:
TH2F* h2 = (TH2F*)gDirectory->Get("position");
h2->Draw("colz");  # 2D position plot

# Create 1D projections
TH1D* hx = h2->ProjectionX("hx");
TH1D* hy = h2->ProjectionY("hy");
hx->Draw();  # X projection
hy->Draw();  # Y projection
```



# 📈 Expected Results

With zero distance between collimator and detector:
- **Beam Profile**: Sharp 1 mm diameter circle
- **Energy**: ~5 MeV (minimal losses)
- **Angular Spread**: < 0.1° (very collimated)


    




# 🙏 Acknowledgments

- Geant4 Collaboration for the simulation toolkit
- ROOT Team for analysis framework


