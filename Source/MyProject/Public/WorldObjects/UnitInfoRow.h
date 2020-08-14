#pragma once
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

/** Used so we can store enemy unit data in a table*/
USTRUCT(BlueprintType)
struct FUnitInfoRow : public FTableRowBase {
   GENERATED_BODY()

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FText name;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   UTexture2D* image;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int strength;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int understanding;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int intelligence;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int explosiveness;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int endurance;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int agility;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int luck;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int baseResist;

   /**Ids 19-31*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TMap<int, int> additionalResist;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int baseAffinity;

   /**Ids 6-18*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TMap<int, int> additionalAffinities;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int baseHealth;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int mana;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int movespeed;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int attackrange;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int globalDamageModifier;
};
