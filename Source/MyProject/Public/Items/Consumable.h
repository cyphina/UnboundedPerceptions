
#pragma once
#include "Consumable.generated.h"

class UMySpell;

/**
* Consumables are just a spell in disguise.
*/
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
