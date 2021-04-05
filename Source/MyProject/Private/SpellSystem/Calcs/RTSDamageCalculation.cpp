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

UCurveFloat*                      URTSDamageCalculation::piercingBonusCurve = nullptr;
static TAutoConsoleVariable<bool> CVARDamageDebugging(TEXT("Up.Debug.DamageValues"), false, TEXT("Used to toggle on or off logging for damage system."), ECVF_Cheat);

URTSDamageCalculation::URTSDamageCalculation(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
   AttStruct attributes;
   RelevantAttributesToCapture.Add(attributes.HealthDef);
   RelevantAttributesToCapture.Add(attributes.StrengthDef);
   RelevantAttributesToCapture.Add(attributes.UnderstandingDef);
   RelevantAttributesToCapture.Add(attributes.IntelligenceDef);
   RelevantAttributesToCapture.Add(attributes.AgilityDef);

   static ConstructorHelpers::FObjectFinder<UCurveFloat> PierceCurveRef(TEXT("/Game/RTS_Tutorial/Blueprints/Curves/Up_Curve_PiercingDamageMultiplier"));
   piercingBonusCurve = PierceCurveRef.Object;
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
   ApplyEffects(d, effects);
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
      d.damage = d.damage * (100 - d.targetUnit->GetStatComponent()->GetMechanicAdjValue(EMechanics::GlobalDamageModifier)) / 100;
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

   // This tag means that we'll dodge everything
   if(USpellDataLibrary::IsGodMode(d.targetUnit->GetAbilitySystemComponent()))
   {
      d.damage   = 0;
      d.accuracy = 101;
   }
}

void URTSDamageCalculation::PrintDamageCalcsBeforeProcessing(const FUpDamage& d, const int damageRange)
{
   if(CVARDamageDebugging.GetValueOnGameThread())
   {
      const auto& debugFormat = TEXT(R"-(-- %s Damage Values b4 Calculations ---
      %t Damage Roll: %d
      %t Damage before effects and amp: %d)-");

      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(debugFormat, *d.sourceUnit->GetGameName().ToString(), damageRange, d.damage));
   }
}

void URTSDamageCalculation::PrintAffinityAndDefense(const float affinity, const float defense)
{
   if(CVARDamageDebugging.GetValueOnGameThread())
   {
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Turquoise, FString::Printf(TEXT("Attacker Affinity: %f - Defender Defense: %f\n"), affinity, defense));
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
      %t Piercing Multiplier: %f
      %t Accuracy Roll: %f
      %t Crit: %d
      %t Element: %s
      %t Effects: %s)-");

      const FString debugString =
          FString::Printf(debugFormat, *d.sourceUnit->GetGameName().ToString(), d.damage, d.piercing, d.accuracy, d.crit, *elementString, *effectsString);
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, debugString);
   }
}

void URTSDamageCalculation::PrintCritRollInfo(const FUpDamage& d, const float critRoll)
{
   if(CVARDamageDebugging.GetValueOnGameThread())
   {
      const float criticalChance = d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Chance);
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange,
                                       FString("Piercing Multiplier: ") + FString::SanitizeFloat(d.piercing) + FString(" Critical Bonus: ") +
                                           FString::SanitizeFloat(criticalChance));
      GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString("Critical Roll: ") + FString::SanitizeFloat(critRoll));
   }
}

void URTSDamageCalculation::CalculateCritical(FUpDamage& d)
{
   const float critRoll           = FMath::FRandRange(1, 100);
   const float adjustedCritCutoff = 100.f - d.piercing * d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Chance);

   // Max 50% crit ratio
   if(critRoll > FMath::Clamp(adjustedCritCutoff, 50.f, 100.f))
   {
      d.crit   = true;
      d.damage = d.damage * (150 + d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Critical_Damage)) / 100;
   }
   else
   {
      d.crit = false;
   }

   PrintCritRollInfo(d, critRoll);
}

void URTSDamageCalculation::CalculateDamageReduction(FUpDamage& d, FGameplayTagContainer& effects) const
{
   CalculatePiercing(d);

   CalculateCritical(d);

   CalculateAccuracy(d, effects);

   ReceiveEffects(d, effects);

   PrintFinalCalculatedDamageValues(d);
}

void URTSDamageCalculation::CalculateAccuracy(FUpDamage& d, FGameplayTagContainer& effects) const
{
   // This tag that means the ability triggering this damage event can never miss
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.NeverMiss")))
   {
      return;
   }

   // (Allies won't dodge our spells casted on them :D)
   if(d.sourceUnit->GetIsEnemy() != d.targetUnit->GetIsEnemy())
   {
      // If our accuracy and their dodge is the same, we have a 95% chance of hitting.
      const float accuracyRoll = FMath::RandRange(5, 105);
      const float accuracyDodgeDifferential =
          d.targetUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Dodge) - d.sourceUnit->GetStatComponent()->GetSkillAdjValue(EUnitScalingStats::Accuracy);
      d.accuracy = accuracyDodgeDifferential / 35.f;
      d.accuracy = accuracyRoll + FMath::RoundToFloat(accuracyDodgeDifferential / 35.f);
   }
}

void URTSDamageCalculation::CalculatePiercing(FUpDamage& d)
{
   float elementalAffinity = d.sourceUnit->GetStatComponent()->GetElementalStatValueFromElemTag(d.element);
   float elementalDefense  = d.targetUnit->GetStatComponent()->GetElementalStatValueFromElemTag(d.element, false);

   // Healing does not get reduced if the unit has high defense. It only gets reduced by level.
   if(d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing")))
   {
      elementalDefense = d.targetUnit->GetStatComponent()->GetUnitLevel();
   }

   if(elementalDefense < 1)
   {
      elementalDefense = 1;
   }

   if(elementalAffinity < 1)
   {
      elementalAffinity = 1;
   }

   d.piercing = piercingBonusCurve->GetFloatValue(elementalAffinity / elementalDefense);
   d.damage   = d.damage * d.piercing;

   PrintAffinityAndDefense(elementalAffinity, elementalDefense);
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
