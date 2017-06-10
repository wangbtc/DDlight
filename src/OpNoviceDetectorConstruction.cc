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

#include "OpNoviceDetectorConstruction.hh"

#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::OpNoviceDetectorConstruction()
  : G4VUserDetectorConstruction(), fLXe_mt(NULL)
{
  fLXe = NULL;
  fExpHall_x = fExpHall_y = fExpHall_z = 10.0*m;
  fTank_x    = fTank_y    = fTank_z    =  5.0*m;
  fLXeVol_x    = fLXeVol_y    = fLXeVol_z    =  1.0*m;
  fBubble_x  = fBubble_y  = fBubble_z  =  0.5*m;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceDetectorConstruction::~OpNoviceDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* OpNoviceDetectorConstruction::Construct()
{

// ------------- Materials -------------

  G4double a, z, density;
  G4int nelements;

// Air
//
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);

  G4Material* air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  air->AddElement(N, 70.*perCent);
  air->AddElement(O, 30.*perCent);

// Water
//
  G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);

  G4Material* water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

  //define LXe
  fLXe = new G4Material("LXe",z=54.,a=131.29*g/mole,density=3.020*g/cm3);
  
//
// ------------ Generate & Add Material Properties Table ------------
//
  G4double lxe_Energy[]    = { 7.0*eV , 7.07*eV, 7.14*eV };
  const G4int lxenum = sizeof(lxe_Energy)/sizeof(G4double);

  G4double lxe_SCINT[] = { 0.1, 1.0, 0.1 };
  assert(sizeof(lxe_SCINT) == sizeof(lxe_Energy));
  G4double lxe_RIND[]  = { 1.59 , 1.57, 1.54 };
  assert(sizeof(lxe_RIND) == sizeof(lxe_Energy));
  G4double lxe_ABSL[]  = { 35.*cm, 35.*cm, 35.*cm};
  assert(sizeof(lxe_ABSL) == sizeof(lxe_Energy));
  fLXe_mt = new G4MaterialPropertiesTable();
  fLXe_mt->AddProperty("FASTCOMPONENT", lxe_Energy, lxe_SCINT, lxenum);
  fLXe_mt->AddProperty("SLOWCOMPONENT", lxe_Energy, lxe_SCINT, lxenum);
  fLXe_mt->AddProperty("RINDEX",        lxe_Energy, lxe_RIND,  lxenum);
  fLXe_mt->AddProperty("ABSLENGTH",     lxe_Energy, lxe_ABSL,  lxenum);
  fLXe_mt->AddConstProperty("SCINTILLATIONYIELD",12000./MeV);
  fLXe_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  fLXe_mt->AddConstProperty("FASTTIMECONSTANT",20.*ns);
  fLXe_mt->AddConstProperty("SLOWTIMECONSTANT",45.*ns);
  fLXe_mt->AddConstProperty("YIELDRATIO",1.0);
  fLXe->SetMaterialPropertiesTable(fLXe_mt);


  // making quart
  G4Element* Si = new G4Element("Silicon","Si" , z=14., a=28.09*g/mole);
  G4Material* quartz = new G4Material("quartz", density=2.200*g/cm3, nelements=2);
  quartz->AddElement(Si, 1);
  quartz->AddElement(O , 2);
  G4double quartz_PP[3]   = { 5.0*eV, 6.69*eV, 7.50*eV }; // lambda range 4 ri
  G4double quartz_RIND[3] = { 1.51, 1.57, 1.61 };     // ref index
  G4double quartz_ABSL[3] = { 3.0*cm, 3.0*cm, 3.0*cm };// atten length
  G4MaterialPropertiesTable *quartz_mt = new G4MaterialPropertiesTable();
  quartz_mt->AddProperty("RINDEX", quartz_PP, quartz_RIND, 3);
  quartz_mt->AddProperty("ABSLENGTH", quartz_PP, quartz_ABSL, 3);
  quartz->SetMaterialPropertiesTable(quartz_mt);
  
  // Set the Birks Constant for the LXe scintillator

  fLXe->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
  if(fLXe_mt)fLXe_mt->AddConstProperty("SCINTILLATIONYIELD",12000./MeV);
  
  G4double photonEnergy[] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

//
// Water
//
  G4double refractiveIndex1[] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  assert(sizeof(refractiveIndex1) == sizeof(photonEnergy));

  G4double absorption[] =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };

  assert(sizeof(absorption) == sizeof(photonEnergy));

  G4double scintilFast[] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };

  assert(sizeof(scintilFast) == sizeof(photonEnergy));

  G4double scintilSlow[] =
            { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
              7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
              3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
              4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
              7.00, 6.00, 5.00, 4.00 };

  assert(sizeof(scintilSlow) == sizeof(photonEnergy));

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       photonEnergy, refractiveIndex1,nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("ABSLENGTH",    photonEnergy, absorption,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("SLOWCOMPONENT",photonEnergy, scintilSlow,     nEntries)
        ->SetSpline(true);

  myMPT1->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.8);

  G4double energy_water[] = {
     1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
     1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  const G4int numentries_water = sizeof(energy_water)/sizeof(G4double);

  //assume 100 times larger than the rayleigh scattering for now.
  G4double mie_water[] = {
     167024.4*m, 158726.7*m, 150742  *m,
     143062.5*m, 135680.2*m, 128587.4*m,
     121776.3*m, 115239.5*m, 108969.5*m,
     102958.8*m, 97200.35*m, 91686.86*m,
     86411.33*m, 81366.79*m, 76546.42*m,
     71943.46*m, 67551.29*m, 63363.36*m,
     59373.25*m, 55574.61*m, 51961.24*m,
     48527.00*m, 45265.87*m, 42171.94*m,
     39239.39*m, 36462.50*m, 33835.68*m,
     31353.41*m, 29010.30*m, 26801.03*m,
     24720.42*m, 22763.36*m, 20924.88*m,
     19200.07*m, 17584.16*m, 16072.45*m,
     14660.38*m, 13343.46*m, 12117.33*m,
     10977.70*m, 9920.416*m, 8941.407*m,
     8036.711*m, 7202.470*m, 6434.927*m,
     5730.429*m, 5085.425*m, 4496.467*m,
     3960.210*m, 3473.413*m, 3032.937*m,
     2635.746*m, 2278.907*m, 1959.588*m,
     1675.064*m, 1422.710*m, 1200.004*m,
     1004.528*m, 833.9666*m, 686.1063*m
  };

  assert(sizeof(mie_water) == sizeof(energy_water));

  // gforward, gbackward, forward backward ratio
  G4double mie_water_const[3]={0.99,0.99,0.8};

  myMPT1->AddProperty("MIEHG",energy_water,mie_water,numentries_water)
        ->SetSpline(true);
  myMPT1->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
  myMPT1->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
  myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

  G4cout << "Water G4MaterialPropertiesTable" << G4endl;
  myMPT1->DumpTable();

  water->SetMaterialPropertiesTable(myMPT1);

  // Set the Birks Constant for the Water scintillator

  water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

//
// Air
//
  G4double refractiveIndex2[] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndex2, nEntries);

  G4cout << "Air G4MaterialPropertiesTable" << G4endl;
  myMPT2->DumpTable();

  air->SetMaterialPropertiesTable(myMPT2);

//
// LXe
//
  G4double refractiveIndex3[] =  //arXiv: 0307.044
            { 1.69, 1.69, 1.69, 1.69, 1.69, 1.69, 1.69,
              1.69, 1.69, 1.69, 1.69, 1.69, 1.69, 1.69,
              1.69, 1.69, 1.69, 1.69, 1.69, 1.69, 1.69,
              1.69, 1.69, 1.69, 1.69, 1.69, 1.69, 1.69,
              1.69, 1.69, 1.69, 1.69 };

  G4MaterialPropertiesTable* myMPT3 = new G4MaterialPropertiesTable();
  myMPT3->AddProperty("RINDEX", photonEnergy, refractiveIndex3, nEntries);

  G4cout << "LXe G4MaterialPropertiesTable" << G4endl;
  myMPT3->DumpTable();

  fLXe->SetMaterialPropertiesTable(myMPT3);


  
//
// ------------- Volumes --------------

// The experimental Hall
//
  G4Box* expHall_box = new G4Box("World",fExpHall_x,fExpHall_y,fExpHall_z);

  G4LogicalVolume* expHall_log = new G4LogicalVolume(expHall_box,air,"World",0,0,0);
  G4VPhysicalVolume* expHall_phys = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);

// The Water Tank
//
  G4Box* waterTank_box = new G4Box("Tank",fTank_x,fTank_y,fTank_z);
  G4LogicalVolume* waterTank_log = new G4LogicalVolume(waterTank_box,water,"Tank",0,0,0);
  G4VPhysicalVolume* waterTank_phys = new G4PVPlacement(0,G4ThreeVector(),waterTank_log,"Tank", expHall_log,false,0);
  G4VisAttributes vis_attr_wat;  vis_attr_wat.SetColour(0,0,1);   waterTank_log->SetVisAttributes(vis_attr_wat);
// The LXe
//
  G4Box* LXeVol_box = new G4Box("LXeVol",fLXeVol_x,fLXeVol_y,fLXeVol_z);
  G4LogicalVolume* LXeVol_log = new G4LogicalVolume(LXeVol_box,fLXe,"LXeVol",0,0,0);
  G4VPhysicalVolume* LXeVol_phys = new G4PVPlacement(0,G4ThreeVector(),LXeVol_log,"LXeVol", expHall_log,false,0);
  G4VisAttributes vis_attr_lxe;  vis_attr_lxe.SetColour(1,0,0);   LXeVol_log->SetVisAttributes(vis_attr_lxe);

  // The Air Bubble
//
//   G4Box* bubbleAir_box = new G4Box("Bubble",fBubble_x,fBubble_y,fBubble_z);

//   G4LogicalVolume* bubbleAir_log
//     = new G4LogicalVolume(bubbleAir_box,air,"Bubble",0,0,0);

// //G4VPhysicalVolume* bubbleAir_phys =
//       new G4PVPlacement(0,G4ThreeVector(0,2.5*m,0),bubbleAir_log,"Bubble",
//                         waterTank_log,false,0);

// ------------- Surfaces --------------
//
// Water Tank
//
  G4OpticalSurface* opWaterSurface = new G4OpticalSurface("WaterSurface");
  opWaterSurface->SetType(dielectric_dielectric);
  opWaterSurface->SetFinish(ground);
  opWaterSurface->SetModel(unified);

  new G4LogicalBorderSurface("WaterSurface",
                                 waterTank_phys,expHall_phys,opWaterSurface);


// Water Tank
//
  G4OpticalSurface* opLXeSurface = new G4OpticalSurface("LXeSurface");
  opLXeSurface->SetType(dielectric_dielectric);
  opLXeSurface->SetFinish(ground);
  opLXeSurface->SetModel(unified);

  new G4LogicalBorderSurface("LXeSurface", LXeVol_phys,expHall_phys,opLXeSurface);


  // Air Bubble
//
  // G4OpticalSurface* opAirSurface = new G4OpticalSurface("AirSurface");
  // opAirSurface->SetType(dielectric_dielectric);
  // opAirSurface->SetFinish(polished);
  // opAirSurface->SetModel(glisur);

  // G4LogicalSkinSurface* airSurface =
  //         new G4LogicalSkinSurface("AirSurface", bubbleAir_log, opAirSurface);

  // G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
  //       (airSurface->GetSurface(bubbleAir_log)->GetSurfaceProperty());

  // if (opticalSurface) opticalSurface->DumpInfo();


  //The PMTs
  // Photomultiplier: ETL 9829 QA ****************************************

  //  G4double pmtHeight    = 12.0*cm;
  //  G4double pmtRadius    = 2.6*cm;
  G4double pmtHeight    = 20.0*cm;
  G4double pmtRadius    = 50*cm;
  G4double pmtVOffset   = 1.0*cm;
  G4double pmtVPosition = 5*m;//-0.5*(LXeTubeHeight+pmtHeight)+pmtVOffset;

  G4Sphere* pmt_window = new G4Sphere("pmt_sphere", 0.*cm, 2.*pmtRadius, 0.*deg, 360.*deg, 0.*deg, 30.0*deg);
  G4Tubs* pmt_tube = new G4Tubs("pmt_tube", 0.*cm,  pmtRadius, 0.5*pmtHeight,0.*deg, 360.*deg);
  
  G4UnionSolid* pmt_sol = new G4UnionSolid("pmt_sol", pmt_tube, pmt_window, G4Transform3D(G4RotationMatrix(), G4ThreeVector(0,0,0.5*pmtHeight -2.*pmtRadius*std::cos(30.0*deg))));
  
  pmt_log  = new G4LogicalVolume(pmt_sol, quartz, "pmt_log", 0,0,0);
  pmt_phys = new G4PVPlacement(0,G4ThreeVector(0.*cm, 0.*cm, pmtVPosition), "pmt_phys", pmt_log, LXeVol_phys, false, 0);

  G4OpticalSurface* pmt_opsurf = new G4OpticalSurface("pmt_opsurf",unified, polished, dielectric_dielectric);
  G4LogicalBorderSurface* pmt_surf =  new G4LogicalBorderSurface("pmt_surf", LXeVol_phys, pmt_phys, pmt_opsurf);

  G4VisAttributes vis_attr;   vis_attr.SetColour(1.0, 0.0, 1.0);  pmt_log->SetVisAttributes(vis_attr);

  //  pmt_log->GetVisAttributes()->SetColour(0.0, 0.0, 1.0);
  //  pmt_log->SetVisAttributes(G4VisAttribute(0,1,0));

  //  pmt_log->SetColor(blue)
  //  G4VisAttributes* pmt_surf= new G4VisAttributes(blue);
//   G4VisAttributes* pmt_vat= new G4VisAttributes(blue);
//   pmt_vat->SetForceSolid(true);
//   pmt_vat->SetVisibility(true);
//   pmt_log->SetVisAttributes(pmt_vat);
//				 "pmt_phys", pmt_log
//   pmt_log  = new G4LogicalVolume(pmt_sol, pmt_mat, "pmt_log");


   //works, toy
//   G4Box* pmt_box = new G4Box("pmtvol",fLXeVol_x,-fLXeVol_y/2,fLXeVol_z/2);
//   G4LogicalVolume* pmt_log = new G4LogicalVolume(pmt_box,fLXe,"pmtvol",0,0,0); 
//   G4VPhysicalVolume* pmt_phys = new G4PVPlacement(0,G4ThreeVector(),pmt_log,"pmtvol", expHall_log,false,0);

  // G4VPhysicalVolume pmt_phys = new G4PVPlacement(0,G4ThreeVector(0.*cm, 0.*cm, pmtVPosition),
//  						 "pmt_phys", pmt_log, LXeVol_box, false, 0);

//   G4VPhysicalVolume pmt_phys = new G4PVPlacement(0,G4ThreeVector(0.*cm, 0.*cm, pmtVPosition),
// 						 "pmt_phys", pmt_log, LXeVol_box, false, 0);

//   G4LogicalVolume* waterTank_log
//     = new G4LogicalVolume(waterTank_box,water,"Tank",0,0,0);

  
  // G4OpticalSurface* pmt_opsurf = new G4OpticalSurface
  //   ("pmt_opsurf",unified, polished, dielectric_dielectric);
  // //G4LogicalBorderSurface* pmt_surf =
  // new G4LogicalBorderSurface
  //   ("pmt_surf", LXe_phys, pmt_phys, pmt_opsurf);
  
  // G4VisAttributes* pmt_vat= new G4VisAttributes(blue);
  // pmt_vat->SetForceSolid(true);
  // pmt_vat->SetVisibility(true);
  // pmt_log->SetVisAttributes(pmt_vat);
  
  
  

  //
// Generate & Add Material Properties Table attached to the optical surfaces
//
  const G4int num = 2;
  G4double ephoton[num] = {2.034*eV, 4.136*eV};

 //OpticalWaterSurface
  G4double refractiveIndex[num] = {1.35, 1.40};
  G4double specularLobe[num]    = {0.3, 0.3};
  G4double specularSpike[num]   = {0.2, 0.2};
  G4double backScatter[num]     = {0.2, 0.2};

  G4MaterialPropertiesTable* myST1 = new G4MaterialPropertiesTable();

  myST1->AddProperty("RINDEX",                ephoton, refractiveIndex, num);
  myST1->AddProperty("SPECULARLOBECONSTANT",  ephoton, specularLobe,    num);
  myST1->AddProperty("SPECULARSPIKECONSTANT", ephoton, specularSpike,   num);
  myST1->AddProperty("BACKSCATTERCONSTANT",   ephoton, backScatter,     num);

  G4cout << "Water Surface G4MaterialPropertiesTable" << G4endl;
  myST1->DumpTable();

  opWaterSurface->SetMaterialPropertiesTable(myST1);
  
  //OpticalLXeSurface
  G4double refractiveIndexLXe[num] = {1.59, 1.62};

  G4MaterialPropertiesTable* myST3 = new G4MaterialPropertiesTable();

  myST3->AddProperty("RINDEX",                ephoton, refractiveIndexLXe, num);
  myST3->AddProperty("SPECULARLOBECONSTANT",  ephoton, specularLobe,    num);
  myST3->AddProperty("SPECULARSPIKECONSTANT", ephoton, specularSpike,   num);
  myST3->AddProperty("BACKSCATTERCONSTANT",   ephoton, backScatter,     num);

  G4cout << "Water Surface G4MaterialPropertiesTable" << G4endl;
  myST3->DumpTable();


  opLXeSurface->SetMaterialPropertiesTable(myST3);
  
  //OpticalAirSurface
  // G4double reflectivity[num] = {0.3, 0.5};
  // G4double efficiency[num]   = {0.8, 1.0};

  // G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();

  // myST2->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  // myST2->AddProperty("EFFICIENCY",   ephoton, efficiency,   num);

  // G4cout << "Air Surface G4MaterialPropertiesTable" << G4endl;
  // myST2->DumpTable();

  // opAirSurface->SetMaterialPropertiesTable(myST2);

//always return the physical World
  return expHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
