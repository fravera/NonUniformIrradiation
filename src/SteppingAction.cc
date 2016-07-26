#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "HistoManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{
  fDetectorHitSet = false;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // G4cout<<"UserSteppingAction_begin"<<G4endl;
  if (!fScoringVolume) { 
    // G4cout<<"Not fScoringVolume"<<G4endl;
    const DetectorConstruction* detectorConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  // G4cout<<"looking for volume..."<<G4endl;
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  G4double scoringVolumeDensity(0.);
  G4double scoringVolumeThickness(0.);
  // check if we are in scoring volume
  if (volume != fScoringVolume) return;
  else if(step->IsFirstStepInVolume()){
    fDetectorHitSet=true;
    G4ThreeVector HitPosition = step->GetPreStepPoint()->GetPosition();
    fEventAction->SetDetectorHit(HitPosition);
    // G4cout<<volume->GetName()<<" = "<<fScoringVolume->GetName()<<G4endl;
  }
  
  scoringVolumeDensity = volume->GetMaterial()->GetDensity();
  scoringVolumeThickness = ((G4Box*)volume->GetSolid())->GetZHalfLength()*2.;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);
  // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // G4double binArea = analysisManager->GetH2(2)->axis_x().bin_width(0) * analysisManager->GetH2(2)->axis_y().bin_width(0); 
  // G4cout<<edepStep/joule<<"  "<<scoringVolumeThickness/m<<"  "<<scoringVolumeDensity/(kg/m3)<<"  "<<scaleFactor/m2<<G4endl;
  // G4cout<<edepStep/joule*scoringVolumeThickness/m/scoringVolumeDensity/(kg/m3)<<G4endl;
  // G4cout<<edepStep/joule / (scoringVolumeThickness/m * binArea/m2 * scoringVolumeDensity/(kg/m3))<<"  "<<
  // edepStep/joule <<"  "<< scoringVolumeThickness/m <<"  "<< binArea/m2 <<"  "<< scoringVolumeDensity/(kg/m3)
  // <<"  "<<analysisManager->GetH2(2)->axis_x().bin_width(0)/m<<"  "<<analysisManager->GetH2(2)->axis_y().bin_width(0)/m<<G4endl;
  // fEventAction->AddDose(edepStep/joule );
  // fEventAction->AddDose(edepStep);
  // fEventAction->AddDose(edepStep/joule / (scoringVolumeThickness/m * binArea/m2 * scoringVolumeDensity/(kg/m3)));
  // fEventAction->AddDose(edepStep/joule / (scoringVolumeThickness/m * scoringVolumeDensity/(kg/m3)));

  // G4cout<<"UserSteppingAction_end"<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

