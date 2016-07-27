#ifdef G4MULTITHREADED
	#include "G4MTRunManager.hh"
#else
	#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include <string>

#include "Randomize.hh"

int main(int argc, char** argv){

	G4UIExecutive *ui = 0;
	if(argc == 1){
		ui = new G4UIExecutive(argc, argv);
	}
	DetectorConstruction* detectorConstruction = new DetectorConstruction;

	if(argc == 3){
		if(strcmp(argv[1],"-cad")==0){
			detectorConstruction->SetCADFileName(argv[2]);
		}
		ui = new G4UIExecutive(argc, argv);
	}

	if(argc == 4){
		if(strcmp(argv[2],"-cad")==0){
			detectorConstruction->SetCADFileName(argv[3]);
		}
	}

	G4Random::setTheEngine(new CLHEP::RanecuEngine);


	#ifdef G4MULTITHREADED
		G4MTRunManager* runManager = new G4MTRunManager;
	#else
		G4RunManager* runManager = new G4RunManager;
	#endif

	runManager->SetUserInitialization(detectorConstruction);
	runManager->SetUserInitialization(new PhysicsList);
	runManager->SetUserInitialization(new ActionInitialization);

	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	if ( ! ui ) { 
	    // batch mode
	    G4String command = "/control/execute ";
	    G4String fileName = argv[1];
	    UImanager->ApplyCommand(command+fileName);
  	}
  	else { 
	    // interactive mode
	    UImanager->ApplyCommand("/control/execute init_vis.mac");
	    ui->SessionStart();
	    delete ui;
	}

	delete visManager;
	delete runManager;

}