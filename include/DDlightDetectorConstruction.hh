//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DDlightDetectorConstruction_h
#define DDlightDetectorConstruction_h 1

#include "G4Material.hh"
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Cache.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DMXScintSD;
class DMXPmtSD;
class DMXPmtSD;

class DDlightDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DDlightDetectorConstruction();
    virtual ~DDlightDetectorConstruction();
    void ConstructSDandField();

  public:
    virtual G4VPhysicalVolume* Construct();

  private:
    G4double fExpHall_x;
    G4double fExpHall_y;
    G4double fExpHall_z;

    G4double fTank_x;
    G4double fTank_y;
    G4double fTank_z;

    G4double fTank_teflon_x;
    G4double fTank_teflon_y;
    G4double fTank_teflon_z;

    G4double fBox_x;
    G4double fBox_y;
    G4double fBox_z;

    G4double fLXeVol_x;
    G4double fLXeVol_y;
    G4double fLXeVol_z;

    G4double fBubble_x;
    G4double fBubble_y;
    G4double fBubble_z;

    G4double fBottle_Rmin;
    G4double fBottle_Rmax;
    G4double fBottle_Z;

  //Materials & Elements
    G4Material* fLXe;

  //Geometry
  G4MaterialPropertiesTable* fLXe_mt;

  //Volumes
  G4LogicalVolume*   pmt_log;
  G4VPhysicalVolume* pmt_phys;
  G4VPhysicalVolume* pmt_phys_reflect;

  G4LogicalVolume*   phcath_log;
  G4VPhysicalVolume* phcath_phys;
  G4VPhysicalVolume* phcath_phys_reflect;


  //  pointer to sensitive detectors
  G4Cache<DMXPmtSD*> pmtSD;
  G4Cache<DMXScintSD*> LXeSD;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DDlightDetectorConstruction_h*/
