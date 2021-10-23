// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StatChangeEffect.h"
#include "GameplayTagsManager.h"
#include "SpellSystem/Calcs/StatChangeCalc.h"

UStatChangeEffect::UStatChangeEffect(const FObjectInitializer& ObjectInitializer) :
   Super(ObjectInitializer)
{
   // Set the duration using setduration
   DurationPolicy = EGameplayEffectDurationType::HasDuration;
   ChanceToApplyToTarget.SetValue(1.f);
   Period = 0.f; // If this effect has a period it will periodically modify our stats

   //x DEPRECIATED
   /* No longer used since we just create new classes for each stat change instead of using this one for all of them
    FGameplayTagNode* statTagNodes = UGameplayTagsManager::Get().FindTagNode("Combat.Stats").Get();
   for (TSharedPtr<FGameplayTagNode> it : statTagNodes->GetChildTagNodes()) {
      FSetByCallerFloat     newStatChangeEffect;
      FGameplayModifierInfo mInfo;
      newStatChangeEffect.DataTag = it->GetCompleteTag();

      mInfo.Attribute         = FindFieldChecked<UProperty>(UMyAttributeSet::StaticClass(), it->GetSimpleTagName());
      mInfo.ModifierOp        = EGameplayModOp::Additive;
      mInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(newStatChangeEffect);
      Modifiers.Add(mInfo);
   }*/
}
