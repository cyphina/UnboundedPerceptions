#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "Stats/RTSAttributeSet.h"
#include "StatChangeCalc.generated.h"

/**
 * For instantaneous stat effects, use a period of 999 and set "Execute periodic effect on application"
 */
UCLASS()
class MYPROJECT_API UStatChangeCalc : public UGameplayEffectExecutionCalculation
{
   GENERATED_UCLASS_BODY()
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                               OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};

struct AttStruct
{
   /// Declare attributes to be captured
   DECLARE_ATTRIBUTE_CAPTUREDEF(Health); // declares two variables (pointer to UProperty that will be captured and GameplayEffectAttributeCapture)
   DECLARE_ATTRIBUTE_CAPTUREDEF(Mana);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Understanding);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Explosiveness);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Endurance);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Agility);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Luck);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Critical_Chance);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Critical_Damage);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Accuracy);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Dodge);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Attack_Speed);
   DECLARE_ATTRIBUTE_CAPTUREDEF(Cast_Speed);
   DECLARE_ATTRIBUTE_CAPTUREDEF(MovementSpeed);
   DECLARE_ATTRIBUTE_CAPTUREDEF(AttackRange);
   DECLARE_ATTRIBUTE_CAPTUREDEF(WeaponPower);
   DECLARE_ATTRIBUTE_CAPTUREDEF(GlobalDamageModifier);
   DECLARE_ATTRIBUTE_CAPTUREDEF(HealthRegenModifier);

   AttStruct()
   {
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Health, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Mana, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Strength, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Understanding, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Intelligence, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Explosiveness, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Endurance, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Agility, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Luck, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Critical_Chance, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Critical_Damage, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Accuracy, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Dodge, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Attack_Speed, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, Cast_Speed, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, MovementSpeed, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, AttackRange, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, WeaponPower, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, GlobalDamageModifier, Source, false);
      DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, HealthRegenModifier, Target, false);
   }
};
