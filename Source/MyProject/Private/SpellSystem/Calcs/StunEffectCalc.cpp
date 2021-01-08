// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "StunEffectCalc.h"
#include "AbilitySystemComponent.h"
#include "../MySpell.h"

#include "WorldObjects/Unit.h"
#include "AIStuff/AIControllers/UnitController.h"
#include "BrainComponent.h"
#include "UnitMessages.h"

void UStunEffectCalc::Execute_Implementation
(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const
{
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
   AUnit*                   sourceUnit      = nullptr,* targetUnit = nullptr;
   // if our components exist
   if(ownerComponent)
   {
      sourceUnit = Cast<AUnit>(ownerComponent->AvatarActor);
   }
   if(targetComponent)
   {
      targetUnit = Cast<AUnit>(targetComponent->AvatarActor);
   }

   if(targetUnit)
   {
      const FAIMessage msg(UnitMessages::AIMessage_Stunned, targetUnit);
      FAIMessage::Send(targetUnit->GetUnitController(), msg);
   }
}
