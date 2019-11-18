// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellSystem/MySpell.h"
#include "MultiPressSpell.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UMultiPressSpell : public UMySpell
{
   GENERATED_BODY()

   UMultiPressSpell();

public:
   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
   TSubclassOf<UMySpell> nextPressSpell;

 protected:
   void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                   bool bWasCancelled) override;
};
