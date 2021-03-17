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
#include "UserInput.h"
#include "SpellSystem/Calcs/DamageDelegateStore.h"

DECLARE_LOG_CATEGORY_CLASS(UpDamageSystem, Verbose, All);

static TAutoConsoleVariable<bool> CVARDamageDebugging(TEXT("RTSDebug.DamageDebugging"), false, TEXT("Used to toggle on or off logging for damage system."), ECVF_Cheat);

URTSDamageCalculation::URTSDamageCalculation(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
   AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef); 
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
   {
      d.damage = d.damage * (100 + d.targetUnit->GetStatComponent()->GetMechanicAdjValue(EMechanics::GlobalDamageModifier)) / 100;
   }

   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.ElementalNuke")))
   {
      if(d.targetUnit->GetStatComponent()->GetElementalStatValueFromElemTag(d.element, false) -
             d.sourceUnit->GetStatComponent()->GetElementalStatValueFromElemTag(d.element, true) <
         0)
      {
         d.damage *= 10;
      }
   }

   // This tag means that the target unit is a ward and can only take 1 damage maximum
   if(USpellDataLibrary::IsWard(d.targetUnit->GetAbilitySystemComponent()))
   {
      d.damage = -1;
   }

   // This tag means that
   if(USpellDataLibrary::IsGodMode(d.targetUnit->GetAbilitySystemComponent()))
   {
      d.damage = 0;
   }
}

void URTSDamageCalculation::PrintDamageCalcsBeforeProcessing(const FUpDamage& d, const int damageRange)
{
   if(CVARDamageDebugging.GetValueOnGameThread())
   {
      const auto& debugFormat = TEXT(R"-(-- %s Damage Values b4 Calculations ---
      %t Damage Roll %d
      %t Damage before Calculations: %d)-");

      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(debugFormat, *d.sourceUnit->GetGameName().ToString(), damageRange, d.damage));
   }
}

void URTSDamageCalculation::PrintPreDamageReductionValues(const FUpDamage& d)
{
   if(CVARDamageDebugging.GetValueOnGameThread())
   {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Turquoise, FString::Printf(TEXT("Piercing value before enemy defense reduction: %d \n"), d.piercing));
   }
}

void URTSDamageCalculation::PrintFinalCalculatedDamageValues(const FUpDamage& d)
{
   if(CVARDamageDebugging.GetValueOnGameThread())
   {
      const FString elementString = d.element.GetTagName().ToString();

      FString effectsString = "";
      for(const FGameplayTag& tag : d.effects)
      {
         effectsString += tag.GetTagName().ToString() + " ";
      }

      const auto& debugFormat = TEXT(
          R"-(-- %s FinalDamage Information ---
      %t Damage: %d
      %t Piercing: %d
      %t Accuracy: %d
      %t Crit: %d
      %t Element: %s
      %t Effects: %s)-");

      const FString debugString =
          FString::Printf(debugFormat, *d.sourceUnit->GetGameName().ToString(), d.damage, d.piercing, d.accuracy, d.crit, *elementString, *effectsString);
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, debugString);
   }
}

void URTSDamageCalculation::PrintCritRollInfo(const FUpDamage& d, const float percentageConversion, const float critRoll)
{
   if(CVARDamageDebugging.GetValueOnGameThread())
   {
      const float criticalChance = d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Chance);
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange,
                                       FString("Piercing Bonus: ") + FString::SanitizeFloat(percentageConversion) + FString(" Critical Bonus: ") +
                                           FString::SanitizeFloat(criticalChance));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString("Critical Roll: ") + FString::SanitizeFloat(critRoll));
   }
}

void URTSDamageCalculation::CalculateDamageReduction(FUpDamage& d, FGameplayTagContainer& effects) const
{
   CalculatePiercing(d.targetUnit, d, false); // Calculate defensive piercing

   PrintPreDamageReductionValues(d);

   ///--After we successfully calculate piercing, apply piercing effects
   /// 1 - Increase base damage by 1% per 35 points of affinity
   const float pierceToPercentBonus = d.piercing / 35.f;
   d.damage                         = d.damage * (100 + pierceToPercentBonus) / 100;
   const float critRoll             = FMath::FRandRange(1, 100);

   /// 2 - Piercing also increase critical chance (up to 30%)
   const float adjustedCritChance = 100.f - d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Chance);
   if(critRoll + FMath::Clamp(pierceToPercentBonus, 0.f, 30.f) > adjustedCritChance)
   {
      d.crit   = true;
      d.damage = d.damage * d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Damage);
   }
   else
   {
      d.crit = false;
   }

   PrintCritRollInfo(d, pierceToPercentBonus, critRoll);

   ///--End of piercing application

   CalculateAccuracy(d, effects);

   /// 4 - Apply any effects that need everything properly calculated
   ReceiveEffects(d, effects);

   PrintFinalCalculatedDamageValues(d);
}

void URTSDamageCalculation::CalculateAccuracy(FUpDamage& d, FGameplayTagContainer& effects) const
{
   // This tag that means the ability triggering this damage event can never miss
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.NeverMiss")))
   {
      d.accuracy = 0;
      return;
   }

   /// 3 - Calculate accuracy based on our accuracy and the opponent's dodge --.
   /// (Allies won't dodge our spells casted on them :D)
   if(d.sourceUnit->GetIsEnemy() != d.targetUnit->GetIsEnemy())
   {
      d.accuracy = d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Accuracy) / 35.f;
      d.accuracy = FMath::RandRange(0, 100) + FMath::Clamp(d.targetUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Dodge) - d.accuracy, 0.f, 95.f);
   }
}

void URTSDamageCalculation::CalculatePiercing(AUnit* unit, FUpDamage& d, bool isAtt)
{
   // TODO: Piecewise soulslike function calculation
   d.piercing += (-1 * static_cast<int>(isAtt)) * unit->GetStatComponent()->GetElementalStatValueFromElemTag(d.element, isAtt);
}

void URTSDamageCalculation::ShowDamageDealt(const FUpDamage& damageInfo)
{
   if(!damageInfo.targetUnit)
   {
      UE_LOG(LogTemp, Error, TEXT("Attempting to deal damage to no target..."));
      return;
   }

   UWorld*    worldRef = damageInfo.targetUnit->GetWorld();
   UDIRender* tRC      = NewObject<UDIRender>(damageInfo.targetUnit,
                                         Cast<AUserInput>(worldRef->GetGameInstance()->GetFirstLocalPlayerController(worldRef))->GetHUDManager()->damageIndicatorClass);
   if(tRC)
   {
      tRC->AttachToComponent(damageInfo.targetUnit->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
      tRC->RegisterComponent();

      if(damageInfo.accuracy > 100)
      {
         tRC->Text = (NSLOCTEXT("Combat", "Dodge", "Dodged!"));
      }
      else
      {
         tRC->Text = FText::AsNumber(damageInfo.damage);
      }

      if(damageInfo.crit)
      {
         tRC->SetRelativeScale3D(FVector(2.f));
      }

      tRC->SetTextRenderColor(USpellDataLibrary::elementalMap[damageInfo.element]);
      tRC->SetHorizontalAlignment(EHTA_Center);
      tRC->SetVerticalAlignment(EVRTA_TextBottom);
   }
}
