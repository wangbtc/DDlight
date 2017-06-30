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
// $Id: DDlightActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file DDlightActionInitialization.cc
/// \brief Implementation of the DDlightActionInitialization class


#include "DDlightEventAction.hh"
#include "DDlightEventActionMessenger.hh"

#include "DDlightActionInitialization.hh"
#include "DDlightPrimaryGeneratorAction.hh"
#include "DDlightRunAction.hh"
#include "DDlightSteppingAction.hh"
#include "DDlightStackingAction.hh"
#include "DDlightSteppingVerbose.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DDlightActionInitialization::DDlightActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DDlightActionInitialization::~DDlightActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DDlightActionInitialization::BuildForMaster() const
{
  SetUserAction(new DDlightRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DDlightActionInitialization::Build() const
{
  SetUserAction(new DDlightPrimaryGeneratorAction());
  SetUserAction(new DDlightRunAction());
  //  SetUserAction(new DMXEventAction()); //BP, breaks the code
  SetUserAction(new DDlightEventAction()); //BP, breaks the code
  SetUserAction(new DDlightSteppingAction());
  SetUserAction(new DDlightStackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose*
               DDlightActionInitialization::InitializeSteppingVerbose() const
{
  return new DDlightSteppingVerbose();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
