// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "AttackState.h"

#include "RTSGameState.h"

#include "Unit.h"
#include "AIStuff/AIControllers/UnitController.h"

AttackState::AttackState()
{
}

AttackState::~AttackState()
{
}

void AttackState::Enter(AUnit& unit)
{

}

void AttackState::Exit(AUnit& unit)
{
   // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("WEE"));
}

void AttackState::Update(AUnit& unit, float deltaSeconds)
{
   //start counting when we're in range, since this is the time for the "attack animation"
   if (unit.combatParams.readyToAttack) {
      //if we're playing the attack animation, but our unit goes out of range, then cancel the attack and start moving closer
      if (unit.targetData.targetUnit->GetCanTarget()) {
         if (!unit.unitController->IsTargetInRange(unit.GetMechanicAdjValue(static_cast<int>(Mechanics::AttackRange)) + unit.combatParams.attackRangeCancel, unit.targetData.targetUnit->GetActorLocation())) {
            unit.combatParams.currentAttTime = 0;
            unit.combatParams.readyToAttack  = false;
            unit.unitController->AdjustPosition(unit.GetMechanicAdjValue(static_cast<int>(Mechanics::AttackRange)), unit.targetData.targetUnit);
         } else {
            //once we start the attack animation, just keep facing our target without any turning
            if (unit.unitController->IsFacingTarget(unit.targetData.targetUnit->GetActorLocation())) {
               if (unit.combatParams.currentAttTime < 2 / ((unit.GetSkillAdjValue(static_cast<int>(UnitStats::Attack_Speed)) + 100) * 0.01)) {
                  unit.combatParams.currentAttTime += deltaSeconds * unit.gameState->speedModifier;
               } else {
                  unit.combatParams.currentAttTime = 0;           
                  unit.Attack();
                  FAIMessage msg(AUnit::AIMessage_AttackReady, &unit);
                  FAIMessage::Send(unit.unitController, msg);
               }
            }
            else
            {
               unit.SetActorRotation(unit.unitController->FindLookRotation(unit.targetData.targetUnit->GetActorLocation()));
            }
         }
      }
   }
   else
   {
      //If the unit goes out of vision while we're attempting to chase it during our attack positional adjustment
      if(!unit.targetData.targetUnit->IsVisible())
      {
         unit.state->ChangeState(EUnitState::STATE_CHASING);
      }
   }
}
