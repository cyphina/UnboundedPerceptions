// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffect.h"
#include "DamageStructs.h"
#include "RTSDamageEffect.generated.h"

/**
 * This class is how we create any type of damage effect in the game.
 * Simple damage effects are created by getting a ability component reference and then calling ApplyGameplayEffectToSelf.
 * For damage dealt from the environment (no ability component on a trap for obv reasons), get the targets affected by the trap, and call ApplyGameplayEffectToSelf.
 * A simpler interface will be exposed in the RTSGameplayAbilityComponent
 * Can set various parameters on this effect to deal DOT damage, and scale damage based on various stats or just sit the values to a fixed number.
 * The calculation for damage is handled in RTSDamageCalculation and the values can easily be overrided (see UMySpell::SetScaling).
 * Most spells have values based on a data table, but we can manually override this on traps and such.
 */
UCLASS()
class MYPROJECT_API URTSDamageEffect : public UGameplayEffect
{
   GENERATED_BODY()

 public:
   URTSDamageEffect();

 private:
   UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
   FDamageScalarStruct damageVals;

   FSetByCallerFloat strength;
   FSetByCallerFloat intelligence;
   FSetByCallerFloat agility;
   FSetByCallerFloat understanding;
   FSetByCallerFloat health;
};
