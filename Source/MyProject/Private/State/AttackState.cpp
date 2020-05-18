// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "AttackState.h"

#include "RTSGameState.h"

#include "Unit.h"
#include "AIStuff/AIControllers/UnitController.h"
#include "UserInput.h"
#include "BrainComponent.h"

AttackState::AttackState() {}

AttackState::~AttackState() {}

void AttackState::Enter(AUnit& unit) {}

void AttackState::Exit(AUnit& unit)
{
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("WEE"));
}

void AttackState::Update(AUnit& unit, float deltaSeconds)
{
   // Checks to see if we're positioned correctly
   if (unit.combatParams.readyToAttack) {
      if (LIKELY(unit.targetData.targetUnit->GetCanTarget())) {
         //if we're playing the attack animation, but our unit goes out of range, then cancel the attack and start moving closer
         if (!unit.unitController->IsTargetInRange(unit.GetMechanicAdjValue(EMechanics::AttackRange) + unit.combatParams.attackRangeCancel,
                                                   unit.targetData.targetUnit->GetActorLocation())) {
            unit.combatParams.currentAttTime = 0;
            unit.combatParams.readyToAttack  = false;
            unit.unitController->AdjustPosition(unit.GetMechanicAdjValue(EMechanics::AttackRange), unit.targetData.targetUnit);
         } else {
            // Once we start the attack animation, just keep facing our target without any turning
            if (unit.unitController->IsFacingTarget(unit.targetData.targetUnit->GetActorLocation())) {
               if (LIKELY(unit.combatParams.currentAttTime < 2 / ((unit.GetSkillAdjValue(EUnitScalingStats::Attack_Speed) + 100) * 0.01))) {
                  unit.combatParams.currentAttTime += deltaSeconds * unit.controllerRef->GetGameState()->speedModifier;
               } else {
                  unit.combatParams.currentAttTime = 0;
                  unit.PlayAttackEffects();

                  FAIMessage msg(AUnit::AIMessage_AttackReady, &unit);
                  FAIMessage::Send(unit.unitController, msg);
               }
            } else { unit.SetActorRotation(unit.unitController->FindLookRotation(unit.targetData.targetUnit->GetActorLocation())); }
         }
      }
   } else {
      //If the unit goes out of vision while we're attempting to chase it during our attack positional adjustment
      if (!unit.targetData.targetUnit->IsVisible()) { unit.state->ChangeState(EUnitState::STATE_CHASING); }
   }
}
