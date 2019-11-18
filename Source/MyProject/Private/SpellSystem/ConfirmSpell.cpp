// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ConfirmSpell.h"
#include "Unit.h"
#include "AbilitySystemComponent.h"
#include "SpellFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"

void UConfirmSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
   Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

   EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
