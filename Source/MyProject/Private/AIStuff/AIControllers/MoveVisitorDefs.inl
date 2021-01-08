#pragma once
#include "Unit.h"
#include "UnitController.h"
#include "UpAIHelperLibrary.h"

// Holds definitions for nested class within AUnitController. Not part of any public API so do not include anywhere besides UnitController's compilation unit

inline AUnit& AUnitController::MoveCompletedVisitor::FinishedMovingUnit() const
{
   return *finishedMovingUnitController->GetUnitOwner();
}

void AUnitController::MoveCompletedVisitor::operator()(FEmptyVariantState)
{
   finishedMovingUnitController->Stop();
}

void AUnitController::MoveCompletedVisitor::operator()(FVector targetVector)
{
   !UUpAIHelperLibrary::IsFacingTarget(&FinishedMovingUnit(), targetVector) ? finishedMovingUnitController->TurnTowardsPoint(targetVector)
                                                                                 : finishedMovingUnitController->onPosAdjDoneAct();
}

void AUnitController::MoveCompletedVisitor::operator()(AActor* targetActor)
{
   if(IsValid(targetActor)) // Ensure the actor wasn't destroyed after we finished our movement
      !UUpAIHelperLibrary::IsFacingTarget(&FinishedMovingUnit(), targetActor->GetActorLocation())
          ? finishedMovingUnitController->TurnTowardsActor(targetActor)
          : finishedMovingUnitController->onPosAdjDoneAct();
}

void AUnitController::MoveCompletedVisitor::operator()(AUnit* targetUnit)
{
   if(IsValid(targetUnit)) // Ensure the actor wasn't destroyed after we finished our movement
      !UUpAIHelperLibrary::IsFacingTarget(&FinishedMovingUnit(), targetUnit->GetActorLocation())
          ? finishedMovingUnitController->TurnTowardsActor(targetUnit)
          : finishedMovingUnitController->onPosAdjDoneAct();
}
