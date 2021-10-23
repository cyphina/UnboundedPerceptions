// Created 3/20/21 4:43 AM

#pragma once
#include "GameplayEffect.h"
#include "DamageStructs.h"
#include "RTSHealingEffect.generated.h"

UCLASS()
class MYPROJECT_API URTSHealingEffect : public UGameplayEffect
{
   GENERATED_BODY()

 public:
   URTSHealingEffect();

 private:
   UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
   FDamageScalarStruct damageVals;

   FSetByCallerFloat strength;
   FSetByCallerFloat intelligence;
   FSetByCallerFloat agility;
   FSetByCallerFloat understanding;
   FSetByCallerFloat health;
   FSetByCallerFloat healthRegenModifier;
};
