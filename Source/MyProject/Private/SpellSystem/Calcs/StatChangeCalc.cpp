// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StatChangeCalc.h"
#include "AbilitySystemComponent.h"
#include "Stats/RTSAttributeSet.h"
#include "SpellDataManager.h"
#include "WorldObjects/Unit.h"

UStatChangeCalc::UStatChangeCalc(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
   AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef);
   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.ExplosivenessDef);
   RelevantAttributesToCapture.Add(attributes.EnduranceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);
   RelevantAttributesToCapture.Add(attributes.LuckDef);
   RelevantAttributesToCapture.Add(attributes.Critical_ChanceDef);
   RelevantAttributesToCapture.Add(attributes.Critical_DamageDef);
   RelevantAttributesToCapture.Add(attributes.AccuracyDef);
   RelevantAttributesToCapture.Add(attributes.DodgeDef);
   RelevantAttributesToCapture.Add(attributes.Attack_SpeedDef);
   RelevantAttributesToCapture.Add(attributes.Cast_SpeedDef);
   RelevantAttributesToCapture.Add(attributes.MovementSpeedDef);
   RelevantAttributesToCapture.Add(attributes.AttackRangeDef);
   RelevantAttributesToCapture.Add(attributes.WeaponPowerDef);
   RelevantAttributesToCapture.Add(attributes.GlobalDamageModifierDef);

   // InvalidScopedModifierAttributes.Add(attributes.HealthDef);
}

void UStatChangeCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const
{
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
   AUnit *                  sourceUnit = nullptr, *targetUnit = nullptr;
   // if our components exist
   if (ownerComponent)
      sourceUnit = Cast<AUnit>(ownerComponent->GetAvatarActor()); // If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing
                                                             // by trying to get the AvatarActor variable from
   if (targetComponent) targetUnit = Cast<AUnit>(targetComponent->GetAvatarActor()); // a null pointer.

   GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Stat Buff Activated!"));

   // outExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::StrengthAttribute(), EGameplayModOp::Additive, strBonus));
   /*if (sourceUnit || targetUnit)
   {
           FGameplayEffectModifiedAttribute modifiedAtt;
           for(FStatChange statChange : USpellFunctionLibrary::GetStatChanges())
           {
                   modifiedAtt.Attribute = statChange.changedAtt;
                   modifiedAtt.TotalMagnitude = statChange.changeStatMagnitude;
                   executionParams.GetOwningSpecForPreExecuteMod()->ModifiedAttributes.Add(modifiedAtt);
                   


           }
           targetUnit->ApplyGameplayEffects(executionParams.GetOwningSpec());
   }*/
}
