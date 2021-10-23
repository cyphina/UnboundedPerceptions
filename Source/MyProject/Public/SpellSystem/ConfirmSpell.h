// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpellSystem/MySpell.h"
#include "ConfirmSpell.generated.h"

/**
 * A spell that replaces the spell that was casted and is used to confirm any actions needed after triggering the spell cast
 */
UCLASS()
class MYPROJECT_API UConfirmSpell : public UMySpell
{
   GENERATED_BODY()

public:

   void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
      const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
