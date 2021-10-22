#pragma once

#include "BaseCharacter.h"
#include "GameplayTags.h"

// Having these seperate structs make these properties easier to set in the editor, but causes a bit more ocde duplication
// TODO: Use variant with these enums?

USTRUCT(BlueprintType, NoExport)
struct FUnitAttributeStruct
{
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   EAttributes att;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int defaultValue;
};

USTRUCT(BlueprintType, NoExport)
struct FUnitScalingStatsStruct
{
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   EUnitScalingStats stat;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int defaultValue;
};

USTRUCT(BlueprintType, NoExport)
struct FUnitVitalStruct
{
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   EVitals vit;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int defaultValue;
};

USTRUCT(BlueprintType, NoExport)
struct FUnitMechanicStruct
{
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   EMechanics mech;

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int defaultValue;
};

/**Used to setup initial stats for a unit or equipment*/
USTRUCT(BlueprintType, NoExport)
struct FDefaultStats
{
   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (TitleProperty = "att"))
   TArray<FUnitAttributeStruct> defaultAttributes;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (TitleProperty = "stat"))
   TArray<FUnitScalingStatsStruct> defaultUnitScalingStats;

   UPROPERTY(EditAnywhere, Category = "ShootingStats", meta = (TitleProperty = "vit"))
   TArray<FUnitVitalStruct> defaultVitals;

   UPROPERTY(EditAnywhere, Category = "ShootingStats", meta = (TitleProperty = "mech"))
   TArray<FUnitMechanicStruct> defaultMechanics;

   /**Element of standard attacks (Defaults to "None" aka nonelemental)*/
   UPROPERTY(EditAnywhere, Category = "ShootingStats", Meta = (Categories = "Combat.Element"))
   FGameplayTag element = FGameplayTag::RequestGameplayTag("Combat.Element.None");
};
