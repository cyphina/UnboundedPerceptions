#pragma once
#include "GameplayTags.h"

/** This header is so we can just include information about these structs without having to include other parts of a class had we put the structs in those headers*/

class AUnit;

/** Damage struct contains relevant information when calculating the amount of damage dealt*/
USTRUCT(BlueprintType, NoExport)
struct FUpDamage {
   FUpDamage() {}
   FUpDamage(AUnit* sourceUnit, AUnit* targetUnit, int p, int d, int a, FGameplayTag e, FGameplayTag t, bool c) :
       sourceUnit{sourceUnit}, targetUnit{targetUnit}, piercing(p), damage(d), accuracy(a), element{e}, type{t}, crit(c) {}

   UPROPERTY(BlueprintReadWrite)
   AUnit* sourceUnit = nullptr;
   UPROPERTY(BlueprintReadWrite)
   AUnit* targetUnit = nullptr;
   UPROPERTY(BlueprintReadWrite)
   int piercing = 0;
   UPROPERTY(BlueprintReadWrite)
   int damage = 0; // how much damage dealt
   UPROPERTY(BlueprintReadWrite)
   int accuracy = 0;
   UPROPERTY(BlueprintReadWrite)
   FGameplayTag element = FGameplayTag(); // element of damage
   UPROPERTY(BlueprintReadWrite)
   FGameplayTag type = FGameplayTag(); // type - physical, magical
   UPROPERTY(BlueprintReadOnly)
   bool crit = false;
   UPROPERTY(BlueprintReadOnly)
   FGameplayTagContainer effects;
};

/** Most skills derive damage from a combination of these stats. */
USTRUCT(BlueprintType, NoExport)
struct FDamageScalarStruct {
   UPROPERTY(BlueprintReadWrite)
   int hitpoints;
   UPROPERTY(BlueprintReadWrite)
   int strength;
   UPROPERTY(BlueprintReadWrite)
   int intelligence;
   UPROPERTY(BlueprintReadWrite)
   int agility;
   UPROPERTY(BlueprintReadWrite)
   int understanding;
};