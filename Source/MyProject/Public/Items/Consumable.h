// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Consumable.generated.h"

/**
 * Consumables are just a spell in disguise.
 */

class UMySpell;

USTRUCT(Blueprintable)
struct FConsumableLookupRow : public FTableRowBase {
   GENERATED_USTRUCT_BODY()

   /**Multiplier for the effect*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int potency = 1;

   /**How many times can we use this item?  -1 = infinity times*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int doses = 1;

   /**Class of ability that is activated when this consumable is used*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TSubclassOf<UMySpell> abilityClass;
};
