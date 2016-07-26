#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "CADMesh.hh"
#include "G4GenericTrap.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "vector"

//------------------------------------------------------------------------------------------------

DetectorConstruction::DetectorConstruction():	G4VUserDetectorConstruction(),
											fScoringVolume(0),
											fFilterVolume(0){
	fCADFileName = "";

}

//------------------------------------------------------------------------------------------------

DetectorConstruction::~DetectorConstruction(){

}

//------------------------------------------------------------------------------------------------

G4VPhysicalVolume* DetectorConstruction::Construct(){

	G4NistManager *nist = G4NistManager::Instance();

	G4double atomicNumber = 1.;
	G4double massOfMole = 1.008*g/mole;
	G4double density = 1.e-25*g/cm3;
	G4double temperature = 2.73*kelvin;
	G4double pressure = 3.e-18*pascal;
	G4Material* Vacuum = new G4Material("interGalactic", atomicNumber, massOfMole, density, kStateGas, temperature, pressure);

	G4double worldX = 25*cm,  worldY = 25*cm, worldZ = 25*cm;
	G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");

	G4Box *worldSolid = new G4Box("World", worldX*0.5, worldY*0.5, worldZ*0.5);
	G4LogicalVolume *worldLogic = new G4LogicalVolume(worldSolid,/*worldMaterial*/Vacuum,"World");
	G4VPhysicalVolume* worldPhysic = new G4PVPlacement(0, G4ThreeVector(), worldLogic, "World", 0, false, 0, true);

	G4VSolid *filterSolid;
	G4Material* filterMaterial = nist->FindOrBuildMaterial("G4_Al");
	G4double filterX = 20*cm,  filterY = 20*cm, filterZ = 30*mm;
	G4double distanceFromDetector = 20*mm;
	G4ThreeVector filterPosition = G4ThreeVector(0, 0, -filterZ*0.5 - distanceFromDetector);

	G4RotationMatrix * rot = new G4RotationMatrix();
    rot->rotateY(180*deg);
    rot->rotateZ(180*deg);

	if(fCADFileName == ""){

		std::vector<G4TwoVector> filterVertices(8);
		filterVertices[0] = G4TwoVector( -0.5*filterX, -0.5*filterY);
		filterVertices[1] = G4TwoVector(  0.5*filterX, -0.5*filterY);
		filterVertices[2] = G4TwoVector(  0.5*filterX,  0.5*filterY);
		filterVertices[3] = G4TwoVector( -0.5*filterX,  0.5*filterY);

		filterVertices[4] = G4TwoVector( -0.5*filterX, -0.5*filterY);
		filterVertices[5] = G4TwoVector( -0.5*filterX, -0.5*filterY);
		filterVertices[6] = G4TwoVector( -0.5*filterX,  0.5*filterY);
		filterVertices[7] = G4TwoVector( -0.5*filterX,  0.5*filterY);

		filterSolid = new G4GenericTrap("Filter", filterZ*0.5, filterVertices);
	}
	// else if(true){
	// 	CADMesh * tet_mesh = new CADMesh((char*) fCADFileName.c_str(), "STL");
 //        tet_mesh->SetScale(1.5);
 //        tet_mesh->SetMaterial(filterMaterial); // We have to do this before making the G4AssemblyVolume.

 //        G4AssemblyVolume * cad_assembly = tet_mesh->TetrahedralMesh();

 //        G4Translate3D translation(0., 0., 0.);
 //        G4Transform3D rotation = G4Rotate3D();
 //        G4Transform3D transform = translation*rotation;

 //        cad_assembly->MakeImprint(worldLogic, transform, 0, 0);
	// }
	else{
		CADMesh * mesh = new CADMesh((char*) fCADFileName.c_str());
	    mesh->SetScale(mm);
	    mesh->SetOffset(G4ThreeVector());
	    mesh->SetReverse(false);
	    mesh->SetMaterial(filterMaterial);
	    // filterSolid = new G4Box("World", worldX*0.5, worldY*0.5, worldZ*0.5);
	    filterSolid = mesh->TessellatedMesh();
	}


	G4LogicalVolume *filterLogic = new G4LogicalVolume(filterSolid, filterMaterial, "Filter");
	new G4PVPlacement(rot, filterPosition, filterLogic, "Filter", worldLogic, false, 0, true);


	G4double detectorX = 20*cm,  detectorY = 20*cm, detectorZ = 1*nm;
	G4Material* detectorMaterial = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

	G4Box *detectorSolid = new G4Box("Detector", detectorX*0.5, detectorY*0.5, detectorZ*0.5);
	G4LogicalVolume *detectorLogic = new G4LogicalVolume(detectorSolid,detectorMaterial,"Detector");
	new G4PVPlacement(0, G4ThreeVector(), detectorLogic, "Detector", worldLogic, false, 0, true);

	fScoringVolume = detectorLogic;
	fFilterVolume  = filterLogic  ;

	return worldPhysic;

}