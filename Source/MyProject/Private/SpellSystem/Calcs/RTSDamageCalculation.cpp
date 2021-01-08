// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSDamageCalculation.h"
#include "WorldObjects/Unit.h"
#include "AIStuff/AIControllers/UnitController.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags.h"
#include "StatChangeCalc.h"
#include "BaseCharacter.h"
#include "DIRender.h"
#include "HUDManager.h"
#include "SpellDataLibrary.h"
#include "SpellFunctionLibrary.h"
#include "WorldObjects/Components/UpStatComponent.h"
#include "UpStatComponent.h"
#include "UserInput.h"
#include "SpellSystem/Calcs/DamageDelegateStore.h"

DECLARE_LOG_CATEGORY_CLASS(UpDamageSystem, Verbose, All);

static TAutoConsoleVariable<bool> CVARDamageDebugging(TEXT("debug.DamageDebugging"), false, TEXT("Used to toggle on or off logging for damage system."),
                                                      ECVF_SetByConsole);

URTSDamageCalculation::URTSDamageCalculation(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
   // TODO: Fix this. This should be used to capture the units stats at a point of time (important for networked games). We need to stop overriding this and actually use it as intended
   AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef); // Some spells deal damage based on how much health we have
   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);
}

void URTSDamageCalculation::BroadcastDamageEvents(FUpDamage& d) const
{
   // Used for skills like soul assumption (dota)
   DamageEvents::OnDamageEvent.Broadcast(d);
   // Used to do unit specific damage tracking (including death for received)
   d.sourceUnit->OnUnitDamageDealt().Broadcast(d);
   d.targetUnit->OnUnitDamageReceived().Broadcast(d);
}

void URTSDamageCalculation::CalculateDamage(FUpDamage& d, FGameplayTagContainer& effects) const
{
   const int damageRange = FMath::RandRange(75, 105);
   d.damage              = d.damage * (damageRange / 100.f);

   PrintDamageCalcsBeforeProcessing(d, damageRange);

   d.piercing = 0;
   d.accuracy = d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Accuracy);

   ApplyEffects(d, effects);
   CalculatePiercing(d.sourceUnit, d, true); // Calculate offensive piercing
}

void URTSDamageCalculation::ApplyEffects(FUpDamage& d, FGameplayTagContainer& effects) const
{
}

void URTSDamageCalculation::ReceiveEffects(FUpDamage& d, FGameplayTagContainer& effects) const
{
   // Apply effects from buffs and armor
   // --This tag means this damage is pure, that is it doesn't get reduced by the global damage modifier--  Ex: Iron Maiden
   if(!effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Absolute")))
      d.damage = d.damage * (100 - d.targetUnit->GetStatComponent()->GetMechanicAdjValue(EMechanics::GlobalDamageModifier)) / 100;

   // This tag that means the ability triggering this damage event can never miss
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.NeverMiss"))) d.accuracy = 0;

   // This tag means that the target unit is a ward and can only take 1 damage maximum
   if(USpellDataLibrary::IsWard(d.targetUnit->GetAbilitySystemComponent())) d.damage = -1;

   // This tag means that
   if(USpellDataLibrary::IsGodMode(d.targetUnit->GetAbilitySystemComponent())) d.damage = 0;
}

void URTSDamageCalculation::PrintDamageCalcsBeforeProcessing(FUpDamage& d, const int damageRange)
{
   if(CVARDamageDebugging.GetValueOnGameThread()) {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald,
                                       FString::Printf(TEXT("Printing information for unit %s"), *d.sourceUnit->GetStatComponent()->GetName()));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Rolled %d in damage roll"), damageRange));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Unit %s did %d damage!"), *d.sourceUnit->GetGameName().ToString(), d.damage));
   }
}

void URTSDamageCalculation::PrintPreDamageReductionValues(FUpDamage& d)
{
   if(CVARDamageDebugging.GetValueOnGameThread()) {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Piercing value: ") + FString::FromInt(d.piercing));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Damage b4 piercing: ") + FString::FromInt(d.damage));
   }
}

void URTSDamageCalculation::PrintCritOccurrence(FUpDamage& d)
{
   if(CVARDamageDebugging.GetValueOnGameThread()) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Crit!") + FString::FromInt(d.damage));
}

void URTSDamageCalculation::PrintFinalCalculatedDamage(FUpDamage& d)
{
   if(CVARDamageDebugging.GetValueOnGameThread()) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Final Damage: ") + FString::FromInt(d.damage));
}

void URTSDamageCalculation::CalculateDamageReduction(FUpDamage& d, FGameplayTagContainer& effects) const
{
   CalculatePiercing(d.targetUnit, d, false); // Calculate defensive piercing

   PrintPreDamageReductionValues(d);

   ///--After we successfully calculate piercing, apply piercing effects
   /// 1 - Increase base damage by 1% per 35 points of piercing
   const float percentageConversion = d.piercing / 35.f;
   d.damage                         = d.damage * (100 + percentageConversion) / 100;
   const float critRoll             = FMath::FRandRange(1, 100);

   if(CVARDamageDebugging.GetValueOnGameThread()) {
      GEngine->AddOnScreenDebugMessage(
          -1, 5.0f, FColor::Red,
          FString("Calculated CritPerc! ") +
              FString::FromInt(d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Chance) + percentageConversion));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Crit roll perc = ") + FString::SanitizeFloat(critRoll + percentageConversion));
   }

   /// 2 - Piercing also increase critical chance
   if(critRoll + percentageConversion > 100 - d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Chance)) {
      d.crit = true;

      PrintCritOccurrence(d);

      d.damage = d.damage * d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Damage);
   } else
      d.crit = false;
   ///--End of piercing application

   PrintFinalCalculatedDamage(d);

   /// 3 - Calculate accuracy based on our accuracy and the opponent's dodge --.
   d.accuracy = d.accuracy / 35.f;
   d.accuracy = FMath::RandRange(0, 100) + d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Dodge) - d.accuracy; // accuracy now becomes dodge roll

   /// 4 - Apply any effects that need everything properly calculated
   ReceiveEffects(d, effects);
}

void URTSDamageCalculation::CalculatePiercing(AUnit* unit, FUpDamage& d, bool isAtt)
{
   // Apply effects from buffs and weapons
   if(d.element.GetTagName() == "Combat.Element.Arcane") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Arcane_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Arcane_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Blood") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Blood_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Blood_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Chaos") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Chaos_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Chaos_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Dark") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Darkness_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Darkness_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Earth") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Earth_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Earth_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Electric") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Electric_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Electric_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Ethereal") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Ethereal_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Ethereal_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Fire") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Fire_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Fire_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Force") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Physical_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Physical_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Light") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Light_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Light_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Poison") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Poison_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Poison_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Water") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Water_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Water_Resist);
   } else if(d.element.GetTagName() == "Combat.Element.Wind") {
      d.piercing += unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Wind_Aff) * isAtt -
                    !isAtt * unit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Wind_Resist);
   }
}

void URTSDamageCalculation::ShowDamageDealt(UWorld* worldRef, const FUpDamage& damageInfo)
{
   UDIRender* tRC = NewObject<UDIRender>(damageInfo.targetUnit,
                                         Cast<AUserInput>(worldRef->GetGameInstance()->GetFirstLocalPlayerController(worldRef))->GetHUDManager()->damageIndicatorClass);
   if(tRC) {
      tRC->AttachToComponent(damageInfo.targetUnit->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
      tRC->RegisterComponent();

      if(damageInfo.accuracy > 100)
         tRC->Text = (NSLOCTEXT("Combat", "Dodge", "Dodged!"));
      else
         tRC->Text = FText::AsNumber(damageInfo.damage);

      if(damageInfo.crit)
         tRC->SetWorldSize(tRC->textSize * 2);
      else
         tRC->SetWorldSize(tRC->textSize);

      tRC->SetTextRenderColor(USpellDataLibrary::elementalMap[damageInfo.element]);
      tRC->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
      tRC->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextBottom);
   }
}
