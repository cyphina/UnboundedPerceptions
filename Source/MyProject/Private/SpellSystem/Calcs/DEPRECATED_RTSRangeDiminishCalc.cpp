#include "MyProject.h"
#include "DEPRECATED_RTSRangeDiminishCalc.h"

#include "MySpell.h"
#include "StatChangeCalc.h"
#include "RTSAttributeSet.h"
#include "Unit.h"
#include "UpStatComponent.h"

UDEPRECATED_RTSRangeDiminishCalc::UDEPRECATED_RTSRangeDiminishCalc()
{
   const AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef);
   RelevantAttributesToCapture.Add(attributes.ManaDef);
   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);

   ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Data.Thresholds"));
   ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Data.PowerScaling"));
}

void UDEPRECATED_RTSRangeDiminishCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                   FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();

   AUnit* sourceUnit = nullptr;
   AUnit* targetUnit = nullptr;

   if(ownerComponent)
   {
      sourceUnit = Cast<AUnit>(ownerComponent->AvatarActor);
   }
   if(targetComponent)
   {
      targetUnit = Cast<AUnit>(targetComponent->AvatarActor);
   }

   if(sourceUnit && targetUnit)
   {
      const auto& calcDef = executionParams.GetOwningSpec().Def->Executions[0];

      // GetValidScopedModifierAttributeCaptureDefinitions(modifiedAtts); - Just lists all the captured attributes
      TArray<float>                                             currentMagnitudes;
      const TArray<FGameplayEffectExecutionScopedModifierInfo>& modifiedAtts = calcDef.CalculationModifiers;

      for(int i = 0; i < executionParams.GetOwningSpec().ModifiedAttributes.Num(); ++i)
      {
         currentMagnitudes.AddDefaulted(currentMagnitudes.Num() + 1);
         modifiedAtts[i].ModifierMagnitude.AttemptCalculateMagnitude(executionParams.GetOwningSpec(), currentMagnitudes[i]);
      }

      FAggregatorEvaluateParameters evalParams;
      evalParams.SourceTags = executionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
      evalParams.TargetTags = executionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

      float numCutoffValues = 0;
      executionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Combat.Effect.Data.Thresholds"), evalParams, numCutoffValues);

      float bonusPerCutoff;
      executionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Combat.Effect.Data.PowerScaling"), evalParams, bonusPerCutoff);

      float rangeBonusIndex = 0;

      const float maxRange = Cast<UMySpell>(executionParams.GetOwningSpec().GetEffectContext().GetAbility())->GetAOE(ownerComponent);

      for(int i = 1; i < numCutoffValues; ++i)
      {
         const float rangeEnd                 = (maxRange / numCutoffValues) * i;
         const float sourceAndTargDistSquared = FVector::DistSquared2D(sourceUnit->GetActorLocation(), targetUnit->GetActorLocation());
         if(sourceAndTargDistSquared < rangeEnd)
         {
            rangeBonusIndex = numCutoffValues - i;
            break;
         }
      }

      float baseAttributeBonus;

      FGameplayTagContainer effectTags;
      executionParams.GetOwningSpec().GetAllAssetTags(effectTags);

      const TArray<FActiveGameplayEffectHandle>& existingRangeEffect = targetComponent->GetActiveEffectsWithAllTags(effectTags);

      FActiveGameplayEffectHandle statChangeEffectHandle;
      for(const FActiveGameplayEffectHandle& handle : existingRangeEffect)
      {
         if(targetComponent->GetActiveGameplayEffect(handle)->Spec.Def->Period == 0)
         {
            statChangeEffectHandle = handle;
         }
      }

      if(statChangeEffectHandle.IsValid())
      {
         FActiveGameplayEffect* statChangeEffect = const_cast<FActiveGameplayEffect*>(targetComponent->GetActiveGameplayEffect(statChangeEffectHandle));
         targetComponent->SetActiveGameplayEffectLevel(statChangeEffectHandle, rangeBonusIndex);
      }

      if(existingRangeEffect.Num() > 0)
      {
         for(int i = 0; i < modifiedAtts.Num(); ++i)
         {
            if(modifiedAtts[i].CapturedAttribute != FGameplayEffectAttributeCaptureDefinition())
            {
               FGameplayAttribute     attToModify    = modifiedAtts[i].CapturedAttribute.AttributeToCapture;
               const int              attributeIndex = URTSAttributeSet::GetAtts().Find(attToModify);
               EAttributes            att            = static_cast<EAttributes>(attributeIndex);
               FActiveGameplayEffect* AGE            = const_cast<FActiveGameplayEffect*>(targetComponent->GetActiveGameplayEffect(existingRangeEffect[0]));
               FGameplayEffectSpec*   MutableSpec    = executionParams.GetOwningSpecForPreExecuteMod();

               if(executionParams.AttemptCalculateCapturedAttributeMagnitude(modifiedAtts[i].CapturedAttribute, evalParams, baseAttributeBonus))
               {
                  const float attToModifyBaseVal = ownerComponent->GetNumericAttributeBase(attToModify);
                  const float rangeBonus         = attToModifyBaseVal * rangeBonusIndex * bonusPerCutoff / 100;

                  if(FGameplayEffectModifiedAttribute* modifiedAtt = MutableSpec->GetModifiedAttribute(attToModify))
                  {
                     targetUnit->GetStatComponent()->ModifyStats(
                         targetUnit->GetStatComponent()->GetAttributeAdjValue(att) - modifiedAtt->TotalMagnitude + baseAttributeBonus + rangeBonus, att);

                     modifiedAtt->TotalMagnitude = baseAttributeBonus + rangeBonus;
                  }
                  else
                  {
                     MutableSpec->AddModifiedAttribute(attToModify)->TotalMagnitude = baseAttributeBonus + rangeBonus;
                  }
               }
            }
         }
      }
      else
      {
         for(int i = 0; i < modifiedAtts.Num(); ++i)
         {
            if(modifiedAtts[i].CapturedAttribute != FGameplayEffectAttributeCaptureDefinition())
            {
               FGameplayAttribute   attToModify           = modifiedAtts[i].CapturedAttribute.AttributeToCapture;
               const float          attToModifyBaseVal    = ownerComponent->GetNumericAttributeBase(attToModify);
               const float          attToModifyCurrentVal = ownerComponent->GetNumericAttribute(attToModify);
               const int            attributeIndex        = URTSAttributeSet::GetAtts().Find(attToModify);
               EAttributes          att                   = static_cast<EAttributes>(attributeIndex);
               FGameplayEffectSpec* effectSpec            = executionParams.GetOwningSpecForPreExecuteMod();

               if(executionParams.AttemptCalculateCapturedAttributeMagnitude(modifiedAtts[i].CapturedAttribute, evalParams, baseAttributeBonus))
               {
                  URTSAbilitySystemComponent* RASC       = Cast<URTSAbilitySystemComponent>(targetComponent);
                  const float                 rangeBonus = attToModifyBaseVal * rangeBonusIndex * bonusPerCutoff / 100;
                  const float                 totalBonus = baseAttributeBonus + bonusPerCutoff * rangeBonus;
                  /*if(GetLastBonus(RASC) != totalBonus)
                  {
                     float currentAdjVal = targetUnit->GetStatComponent()->GetAttributeAdjValue(att);
                     targetUnit->GetStatComponent()->ModifyStats(currentAdjVal - GetLastBonus(RASC) + totalBonus, att);
                     // TODO: GET a CUstom Context
                     //CacheLastBonus(RASC, totalBonus);
                  }*/
               }
            }
         }
      }
   }
}

/*void URTSRangeDiminishCalc::CacheLastBonus(URTSAbilitySystemComponent* ASC, float newRange) const
{
   ASC->lastRange = newRange;
}

float URTSRangeDiminishCalc::GetLastBonus(URTSAbilitySystemComponent* ASC) const
{
   return ASC->lastRange;
}*/
