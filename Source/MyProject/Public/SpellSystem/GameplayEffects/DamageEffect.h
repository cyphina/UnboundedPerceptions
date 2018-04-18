// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "DamageStructs.h"
#include "DamageEffect.generated.h"

/**
 * Base class for all damage effects.  Using this we can make creating damage effect type gameplay effects easier 
 * via blueprints.  Does not have to be instant, but make sure to set that in child class.  
 */
UCLASS()
class MYPROJECT_API UDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()

	UDamageEffect();
	
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true", ExposeOnSpawn="true"))
	FDamageScalarStruct			damageVals;

	FSetByCallerFloat			strength;
	FSetByCallerFloat			intelligence;
	FSetByCallerFloat			agility;
	FSetByCallerFloat			understanding;
};

