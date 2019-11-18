// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MultiPressSpell.h"

#include "Unit.h"
#include "UnitController.h"

#include "SpellFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "ConfirmSpell.h"

UMultiPressSpell::UMultiPressSpell()
{
   nextPressSpell = UConfirmSpell::StaticClass();
}

void UMultiPressSpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
   Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
   //Get old spell id from parameters saved 
   AUnit* ownerRef = Cast<AUnit>(GetAvatarActorFromActorInfo());
 
   USpellFunctionLibrary::SpellConfirmSwap(nextPressSpell, GetClass(), ownerRef, false);

   //After finished confirmation stop to reset all spell variables and let AI transition from idle state
   ownerRef->GetUnitController()->Stop();
}
