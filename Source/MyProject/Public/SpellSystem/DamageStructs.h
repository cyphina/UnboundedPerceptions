#pragma once
#include "GameplayTags.h"
#include "DamageStructs.generated.h"

class AUnit;

/** Damage struct contains relevant information when calculating the amount of damage dealt*/
USTRUCT(BlueprintType, NoExport)
struct FUpDamage
{
   FUpDamage() {}
   FUpDamage(AUnit* sourceUnit, AUnit* targetUnit, int p, int d, int a, FGameplayTag e, FGameplayTag t, bool c) :
       sourceUnit{sourceUnit}, targetUnit{targetUnit}, piercing(p), damage(d), accuracy(a), element{e}, type{t}, crit(c)
   {
   }

   UPROPERTY(BlueprintReadWrite)
   AUnit* sourceUnit = nullptr;
   UPROPERTY(BlueprintReadWrite)
   AUnit* targetUnit = nullptr;
   UPROPERTY(BlueprintReadWrite)
   float piercing = 0;
   UPROPERTY(BlueprintReadWrite)
   int damage = 0;
   UPROPERTY(BlueprintReadWrite)
   float accuracy = 0;
   UPROPERTY(BlueprintReadWrite)
   FGameplayTag element = FGameplayTag(); // element of damage
   UPROPERTY(BlueprintReadWrite)
   FGameplayTag type = FGameplayTag(); // type - physical, magical
   UPROPERTY(BlueprintReadOnly)
   bool crit = false;
   UPROPERTY(BlueprintReadOnly)
   FGameplayTagContainer effects;

   bool DidMiss() const { return accuracy > 100; }
};

/** Most skills derive damage from a combination of these stats. */
USTRUCT(BlueprintType)
struct FDamageScalarStruct
{
   GENERATED_BODY()

   FDamageScalarStruct(int hits = 0, int str = 0, int intel = 0, int agi = 0, int und = 0) :
       hitpoints(hits), strength(str), intelligence(intel), agility(agi), understanding(und)
   {
   }

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int hitpoints = 0;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int strength = 0;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int intelligence = 0;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int agility = 0;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int understanding = 0;
};
