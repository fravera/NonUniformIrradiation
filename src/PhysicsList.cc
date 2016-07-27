#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "PhysicsList.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4RayleighScattering.hh"
#include "G4PenelopeRayleighModel.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4PenelopePhotoElectricModel.hh"
#include "G4ComptonScattering.hh"
#include "G4PenelopeComptonModel.hh"
#include "G4GammaConversion.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4eIonisation.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4eIonisation.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4eBremsstrahlung.hh"
#include "G4PenelopeBremsstrahlungModel.hh"
#include "G4eplusAnnihilation.hh"
#include "G4PenelopeAnnihilationModel.hh"
#include "G4eMultipleScattering.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"

//------------------------------------------------------------------------------------------------

PhysicsList::PhysicsList() : G4VModularPhysicsList(){

	G4LossTableManager::Instance();
	SetVerboseLevel(1);
	fEmPhysicsList = new G4EmStandardPhysics_option4();
	// SetPhysicsType(bElectromagnetic);

}

//------------------------------------------------------------------------------------------------

PhysicsList::~PhysicsList(){

}

//------------------------------------------------------------------------------------------------

void PhysicsList::ConstructParticle(){

	// G4Gamma::GammaDefinition();
	// G4Electron::ElectronDefinition();
	// G4Positron::PositronDefinition();
	// G4Proton::ProtonDefinition();
	fEmPhysicsList->ConstructParticle();

}

//------------------------------------------------------------------------------------------------

void PhysicsList::ConstructProcess(){

	// RegisterPhysics( new G4EmPenelopePhysics() );
	// G4VPhysicsConstructor* emPhysicsList = new G4EmPenelopePhysics();
	// emPhysicsList->ConstructProcess(); 

	AddTransportation();
	fEmPhysicsList->ConstructProcess();

	G4RayleighScattering* theRayleighScattering = new G4RayleighScattering();
	theRayleighScattering->SetEmModel(new G4PenelopeRayleighModel());
	// theRayleighScattering->SetHighEnergyLimit(1.*GeV);

	G4PhotoElectricEffect* thePhotoElectric = new G4PhotoElectricEffect();
	thePhotoElectric->SetEmModel(new G4PenelopePhotoElectricModel());
	// thePhotoElectric->SetHighEnergyLimit(1.*GeV);

	G4ComptonScattering* theComptonScattering = new G4ComptonScattering(); 
	theComptonScattering->SetEmModel(new G4PenelopeComptonModel()); 
	// theComptonScattering->SetHighEnergyLimit(1.*GeV);

	G4GammaConversion* theGammaConversion = new G4GammaConversion();
	theGammaConversion->SetEmModel(new G4PenelopeGammaConversionModel());
	// theGammaConversion->SetHighEnergyLimit(1.*GeV);

	G4eIonisation* theIonisation = new G4eIonisation(); 
	// theIonisation->SetEmModel(new G4PenelopeIonisationModel()); 
	// theIonisation->SetHighEnergyLimit(1.*GeV);

	G4eBremsstrahlung* theBremsstralung = new G4eBremsstrahlung();
	theBremsstralung->SetEmModel(new G4PenelopeBremsstrahlungModel());
	// theBremsstralung->SetHighEnergyLimit(1.*GeV);
	
	G4eplusAnnihilation* theAnnihilation = new G4eplusAnnihilation();
	theAnnihilation->SetEmModel(new G4PenelopeAnnihilationModel());
	// theIonisation->SetHighEnergyLimit(1.*GeV);

	theParticleIterator->reset();
	while( (*theParticleIterator)() ){
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4String particleName = particle->GetParticleName();
	 
	if (particleName == "gamma") {
	  // gamma         
		pmanager->AddDiscreteProcess(theRayleighScattering);
	 	pmanager->AddDiscreteProcess(thePhotoElectric);
	  	pmanager->AddDiscreteProcess(theComptonScattering);
	  	pmanager->AddDiscreteProcess(theGammaConversion);
	  
	} else if (particleName == "e-") {
	  //electron
	  pmanager->AddProcess(new G4eMultipleScattering, -1, 1,1);
	  pmanager->AddProcess(theIonisation,         -1, 2,2);
	  pmanager->AddProcess(theBremsstralung,     -1, 3,3);
	    
	} else if (particleName == "e+") {
	  //positron
	  pmanager->AddProcess(new G4eMultipleScattering, -1, 1,1);
	  pmanager->AddProcess(theIonisation,         -1, 2,2);
	  pmanager->AddProcess(theBremsstralung,     -1, 3,3);
	  pmanager->AddProcess(theAnnihilation,    0,-1,4);
	}


  }
	// // emPhysicsList->ConstructProcess(); 
	// G4ComptonScattering* theComptonScattering = new G4ComptonScattering(); 
	// theComptonScattering->SetEmModel(new G4PenelopeComptonModel()); 
	// theComptonScattering->SetHighEnergy(1.*GeV);
	// pmanager->AddDiscreteProcess(theComptonScattering);

	// G4eIonisation* theIonisation = new G4eIonisation(); 
	// theIonisation->SetEmModel(new G4PenelopeIonisationModel()); 
	// heIonisation->SetHighEnergy(1.*GeV);
	// pmanager->AddProcess(theIonisation,-1,1,1);

	// pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
	// pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
	// pmanager->AddProcess(new G4eBremsstrahlung(), -1,-3, 3);
}

//------------------------------------------------------------------------------------------------

void PhysicsList::SetCuts(){

	G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV,100*keV);

	SetCutValue(1*um, "gamma");
	SetCutValue(1*um, "e-");
	// SetCutValue(1*um, "e-");
	SetCutValue(1*um, "e+");
	// SetCutValue(1*um, "e+");
	SetCutValue(1*um,"proton");
	DumpCutValuesTable();

}