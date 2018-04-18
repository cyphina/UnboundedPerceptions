// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	UMyAbilitySystemComponent();

public:
	FActiveGameplayEffectHandle			ApplyGameplayEffectSpecToSelf(OUT FGameplayEffectSpec &Spec, FPredictionKey PredictionKey);

	/** Get an outgoing GameplayEffectSpec that is ready to be applied to other things. */
	UFUNCTION(BlueprintCallable, Category = GameplayEffects)
	FGameplayEffectSpecHandle MakeOutgoingSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, FGameplayEffectContextHandle Context) const;

private:

	TMap<FGameplayTag, int>				purgeTagMap;

	/**Gets how many effects will be purged by this purge effect*/
	int									GetPurgeAmount(FGameplayTag purgeDescription);
};
