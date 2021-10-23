// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyProject.h"
#include "Item.h"
#include "Stats/BaseCharacter.h"
#include "UnitStatStruct.h"
#include "Equip.generated.h"

/**
 * Equippable objects are/will be stored in an equipment class.  They give us stat bonuses, and eventually may impact our auto attack or something.
 */

USTRUCT(Blueprintable)
struct FEquipLookupRow : public FTableRowBase {
   GENERATED_USTRUCT_BODY()

   /**What bonuses are increased: 0-6 attributes, 7-38 stats, 39-43 vitals, and rest are mechanics*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FDefaultStats stats;

   /**Special effects of the weapon to be considered*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TArray<FName> effects;

   /**Level of equipment*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int level = 1;
};
