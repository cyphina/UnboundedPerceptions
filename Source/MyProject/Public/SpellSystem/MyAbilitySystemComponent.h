// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

/**
 * Custom ability component with extra functionality
 */

UCLASS()
class MYPROJECT_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
   GENERATED_BODY()

   UMyAbilitySystemComponent();

 public:
   /**Overriden to allow us to add things like purging buffs*/
   FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& Spec, FPredictionKey PredictionKey) override;

   /** Get an outgoing GameplayEffectSpec that is ready to be applied to other things. */
   FGameplayEffectSpecHandle MakeOutgoingSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, FGameplayEffectContextHandle Context) const override;

 private:
   TMap<FGameplayTag, int> purgeTagMap;

   /**Gets how many effects will be purged by this purge effect*/
   int GetPurgeAmount(FGameplayTag purgeDescription);
};
