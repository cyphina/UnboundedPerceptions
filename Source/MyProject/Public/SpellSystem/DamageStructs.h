#pragma once

#include "GameplayTags.h"

/**This header is so we can just include information about these structs without having to include other parts of a class had we put the structs in those headers*/

/**Damage struct contains relevant information when calculating the amount of damage dealt*/
struct Damage
{
	Damage() {}
	Damage(int p, int d, int a, FGameplayTag e, FGameplayTag t, bool c) : piercing(p), damage(d), accuracy(a), element(e), type(t), crit(c) {}
	int piercing = 0;
	int damage = 0; //how much damage dealt
	int accuracy = 0;
	FGameplayTag element = FGameplayTag(); //element of damage
	FGameplayTag type = FGameplayTag(); //type - physical, magical
	bool crit = false;
};

USTRUCT(BlueprintType, NoExport)
struct FDamageScalarStruct
{
	UPROPERTY(BlueprintReadWrite)
	int strength;
	UPROPERTY(BlueprintReadWrite)
	int intelligence;
	UPROPERTY(BlueprintReadWrite)
	int agility;
	UPROPERTY(BlueprintReadWrite)
	int understanding;
};