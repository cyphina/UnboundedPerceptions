// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "InvisibilityExec.h"
#include "WorldObjects/Unit.h"
#include "AbilitySystemComponent.h"

void UInvisibilityExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const
{
   UAbilitySystemComponent* ownerComponent = executionParams.GetSourceAbilitySystemComponent();
   AUnit *                  sourceUnit = nullptr;

   if (ownerComponent)
      sourceUnit = Cast<AUnit>(ownerComponent->AvatarActor);

   if(sourceUnit->GetIsEnemy())
   {
      //The check in UpdateVisibleEnemies() should turn this back to visibile if there is true sight
      //sourceUnit->GetDefaultAttachComponent()->SetVisibility(false);
   }
}
