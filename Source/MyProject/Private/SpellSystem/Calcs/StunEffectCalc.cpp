// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StunEffectCalc.h"
#include "AbilitySystemComponent.h"
#include "WorldObjects/Unit.h"
#include "../MySpell.h"

void UStunEffectCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const
{
	UAbilitySystemComponent* ownerComponent = executionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
	AUnit* sourceUnit = nullptr, *targetUnit = nullptr;
	//if our components exist
	if (ownerComponent)
		sourceUnit = Cast<AUnit>(ownerComponent->AvatarActor); //If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing by trying to get the AvatarActor variable from
	if (targetComponent)
		targetUnit = Cast<AUnit>(targetComponent->AvatarActor); //a null pointer.

}


