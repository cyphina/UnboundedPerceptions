#pragma once
#include "CoreMinimal.h"

DECLARE_DELEGATE(FTurnAction);

USTRUCT(BlueprintType, NoExport)
struct FUnitProperties {
   /**Name that we can refer to this unit by whenever trying to perform global operations in resourcemanager*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification")
   FText name;
   /**Portrait of the unit when selected.  Seen on actionbar*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification")
   UTexture2D* image = nullptr;
   /**Allows us to target this unit with single target spells.  Even with this false, the unit can be targetted by AOE spells*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification", meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   bool canTarget = true;
   /**Radius of the vision sphere we use to filter what enemies we can see and what corners are included into the vision polygon*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere)
   int visionRadius = 1000;
   /**If this unit has been selected, then information on it will be displayed*/
   bool isSelected = false;
   /**Capsule height of unit*/
   float height;
   /**Action to be done after finished turning*/
   FTurnAction turnAction;
};