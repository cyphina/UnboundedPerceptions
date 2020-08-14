#pragma once
#include "CoreMinimal.h"

USTRUCT(BlueprintType, NoExport)
struct FUnitProperties {
   /**Name that we can refer to this unit by whenever trying to perform global operations in UpResourceManager*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification")
   FText name;

   /**Portrait of the unit when selected.  Seen on actionbar*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WorldObject Classification")
   UTexture2D* image = nullptr;

   /**If this unit has been selected, then information on it will be displayed*/
   bool isSelected = false;

   /**Capsule height of unit*/
   float height;
};
