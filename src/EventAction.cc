#include "EventAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4GenericTrap.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fDetectorHitPosition(G4ThreeVector(-1000,-1000,-1000))
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{    
	// G4cout<<"BeginOfEventAction"<<G4endl;
	fEdep = 0.;
	fDetectorHitPosition = G4ThreeVector(-1000,-1000,-1000);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{   
	// accumulate statistics in run action
	// G4cout<<"EndOfEventAction"<<G4endl;
	fRunAction->AddEdep(fEdep);
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  	G4double binArea = analysisManager->GetH2(2)->axis_x().bin_width(0) * analysisManager->GetH2(2)->axis_y().bin_width(0); 

	const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  	G4double scoringVolumeDensity = detectorConstruction->GetScoringVolume()->GetMaterial()->GetDensity();
	G4double scoringVolumeThickness = ((G4Box*)detectorConstruction->GetScoringVolume()->GetSolid())->GetZHalfLength()*2.;
	G4double scoringVolumeLength = ((G4Box*)detectorConstruction->GetScoringVolume()->GetSolid())->GetXHalfLength()*2.;
	G4double filterVolumeThickness  = ((G4GenericTrap*)detectorConstruction->GetFilterVolume()->GetSolid())->GetZHalfLength()*2.;

	fDose = fEdep/ (scoringVolumeThickness * binArea * scoringVolumeDensity);
	// fDose = fEdep/ (scoringVolumeThickness * binArea * scoringVolumeDensity);

	// G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	if(fDetectorHitPosition.z()!=-1000){
		// G4cout<<fDetectorHitPosition.x()/cm<<"  "<<fDetectorHitPosition.y()/cm<<"  "<<fDetectorHitPosition.z()/cm<<"  "<<fEdep<<G4endl;
		// G4cout<< scoringVolumeLength*0.5 << "  " << (scoringVolumeLength*0.5 + fDetectorHitPosition.x())/scoringVolumeLength << "  " <<
		// filterVolumeThickness  << "  " << (scoringVolumeLength*0.5 + fDetectorHitPosition.x())/scoringVolumeLength*filterVolumeThickness <<G4endl;
		analysisManager->FillH1(2,((scoringVolumeLength*0.5 - fDetectorHitPosition.x())/scoringVolumeLength*filterVolumeThickness)/mm,fEdep);
		analysisManager->FillH1(3,((scoringVolumeLength*0.5 - fDetectorHitPosition.x())/scoringVolumeLength*filterVolumeThickness)/mm,fDose/gray);
		analysisManager->FillH2(1,fDetectorHitPosition.x()/mm,fDetectorHitPosition.y()/mm,fEdep);
		analysisManager->FillH2(2,fDetectorHitPosition.x()/mm,fDetectorHitPosition.y()/mm,fDose/gray);
		// G4cout<<fDetectorHitPosition.x()/cm<<"  "<<fDetectorHitPosition.y()/cm<<"  "<<fDetectorHitPosition.z()/cm<<"  "<<fEdep<<G4endl;
	}
	else if(fEdep>0){
		G4cout<<" !!! Error !!! "<<fDetectorHitPosition.x()/cm<<"  "<<fDetectorHitPosition.y()/cm<<"  "<<fDetectorHitPosition.z()/cm<<"  "<<fEdep<<G4endl;
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
