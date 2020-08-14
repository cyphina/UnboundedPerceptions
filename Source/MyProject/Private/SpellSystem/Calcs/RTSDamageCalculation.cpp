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
#include "SpellFunctionLibrary.h"
#include "UpStatComponent.h"
#include "UserInput.h"
#include "SpellSystem/Calcs/RTSDamageCallbacks.h"

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

void URTSDamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                                                   FGameplayEffectCustomExecutionOutput&           outExecutionOutput) const
{
   FUpDamage damage;

   // Owner component gotten from instigator ability component which gotten from owner we set as our player controller but that isn't an ability system holder
   UAbilitySystemComponent* ownerComponent  = executionParams.GetSourceAbilitySystemComponent();
   UAbilitySystemComponent* targetComponent = executionParams.GetTargetAbilitySystemComponent();
   AUnit *                  sourceUnit = nullptr, *targetUnit = nullptr;

   // TODO: Handle non unit damage (from traps or something)
   // If our components exist
   if(ownerComponent)
      sourceUnit = Cast<AUnit>(
          ownerComponent
              ->AvatarActor); // If our AbilitySystemComponents are valid, we get each their owning actors and put them in variables. This is mostly to prevent crashing
                              // by trying to get the AvatarActor variable from
   if(targetComponent)
      targetUnit = Cast<AUnit>(targetComponent->AvatarActor); // a null pointer.

   if(sourceUnit && targetUnit) {
      // Setup targteting
      damage.sourceUnit = sourceUnit;
      damage.targetUnit = targetUnit;

      // Get Element of this Damage
      FGameplayTagContainer tags = executionParams.GetOwningSpec().DynamicAssetTags;
      damage.element             = tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"))).GetByIndex(0);

      // Start calculating damage based off our attributes and the attributes the ability scales off of
      FAggregatorEvaluateParameters evalParams;
      evalParams.SourceTags = executionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
      evalParams.TargetTags = executionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

      AttStruct attributes;
      float     captureVal = 0;
      for(FGameplayEffectAttributeCaptureDefinition captureDef : RelevantAttributesToCapture) {
         if(executionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evalParams, captureVal))
            // Multiplies the actual value of our stat by the overriden capture value (allowing us to customize the value in the editor)
            damage.damage += ownerComponent->GetNumericAttribute(captureDef.AttributeToCapture) * captureVal / 100;
      }
      UnitDamageTarget(damage, tags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.DamageEffects"))));
   }
}

#pragma region Calculations

void URTSDamageCalculation::BroadcastDamageEvents(FUpDamage& d) const
{
   // Used for skills like soul assumption (dota)
   DamageEvents::OnDamageEvent.Broadcast(d);
   // Used to do unit specific damage tracking (including death for received)
   d.sourceUnit->OnUnitDamageDealtEvent.Execute(d);
   d.sourceUnit->OnUnitDamageReceivedEvent.Execute(d);
}

// See comments for UnitDamageTarget below
void URTSDamageCalculation::DamageTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   // No need to calculate piercing from the source since it is not a unit
   ApplyEffects(d, effects);
   CalculateDamageReduction(d, effects);

   // Accuracy check
   if(d.accuracy > 100) {
      BroadcastDamageEvents(d);
      return;
   }

   // Clamp damage
   if(d.damage <= 0)
      d.damage = 1;

   // Kill the unit if its hp falls below 0, else update information
   if(d.targetUnit->statComponent->GetVitalCurValue(EVitals::Health) <= 0) {
      if(!d.targetUnit->GetAbilitySystemComponent()->IsImmortal()) {
         d.targetUnit->Die();
      }
   }

   BroadcastDamageEvents(d);
}

void URTSDamageCalculation::UnitDamageTarget(FUpDamage& d, FGameplayTagContainer effects) const
{
   CalculateDamage(d, effects);          // Calculate how much damage we do based on all of our stats
   CalculateDamageReduction(d, effects); // Calculate how much our damage is resisted based on target's stats

   // Accuracy check
   if(d.accuracy > 100) {
      BroadcastDamageEvents(d);
      return;
   }

   // Clamp damage to always deal 1 damage even on highly resisted hits
   if(d.damage <= 0)
      d.damage = 1;

   // Add lifesteal effects as healing here (since we have to calculate damage reduction first)
   // TODO: Maybe add a stat for lifesteal %
   if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Lifesteal")))
      d.sourceUnit->statComponent->ModifyStats<false>(d.targetUnit->statComponent->GetVitalCurValue(EVitals::Health) + d.damage, EVitals::Health);

   // Record some statistics about our units which AI can use
   float worldTime = d.sourceUnit->statComponent->GetWorld()->GetTimeSeconds();

   // Drain or add health depending on healing or damage
   if(UNLIKELY(!d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing")))) {
      d.damage *= -1;

      // Kill the unit if it's health drops below 0 and stop us from attacking anymore (if we were autoattacking)
      if(d.targetUnit->statComponent->GetVitalCurValue(EVitals::Health) <= 0) {
         if(!d.targetUnit->GetAbilitySystemComponent()->IsImmortal()) {
            d.targetUnit->Die();
         }
      }

      BroadcastDamageEvents(d);
   }

   void URTSDamageCalculation::CalculateDamage(FUpDamage & d, FGameplayTagContainer & effects) const
   {
      const int damageRange = FMath::RandRange(75, 105);
      d.damage              = d.damage * (damageRange / 100.f);

      PrintDamageCalcsBeforeProcessing(d, damageRange);

      d.piercing = 0;
      d.accuracy = d.sourceUnit->statComponent->GetSkillAdjValue(EUnitScalingStats::Accuracy);

      ApplyEffects(d, effects);
      CalculatePiercing(d.sourceUnit, d, true); // Calculate offensive piercing
   }

   void URTSDamageCalculation::ApplyEffects(FUpDamage & d, FGameplayTagContainer & effects) const {}

   void URTSDamageCalculation::ReceiveEffects(FUpDamage & d, FGameplayTagContainer & effects) const
   {
      // Apply effects from buffs and armor
      // --This tag means this damage is pure, that is it doesn't get reduced by the global damage modifier--  Ex: Iron Maiden
      if(!effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Absolute")))
         d.damage = d.damage * (100 - d.targetUnit->statComponent->GetMechanicAdjValue(EMechanics::GlobalDamageModifier)) / 100;

      // This tag that means the ability triggering this damage event can never miss
      if(effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.NeverMiss")))
         d.accuracy = 0;

      // This tag means that the target unit is a ward and can only take 1 damage maximum
      if(d.targetUnit->GetAbilitySystemComponent()->IsWard())
         d.damage = -1;

      // This tag means that
      if(d.targetUnit->GetAbilitySystemComponent()->IsGodMode())
         d.damage = 0;
   }

   void URTSDamageCalculation::PrintDamageCalcsBeforeProcessing(FUpDamage & d, const int damageRange)
   {
      if(CVARDamageDebugging.GetValueOnGameThread()) {
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Printing information for unit %s"), *d.sourceUnit->statComponent->GetName()));
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Rolled %d in damage roll"), damageRange));
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString::Printf(TEXT("Unit %s did %d damage!"), *d.sourceUnit->GetGameName().ToString(), d.damage));
      }
   }

   void URTSDamageCalculation::PrintPreDamageReductionValues(FUpDamage & d)
   {
      if(CVARDamageDebugging.GetValueOnGameThread()) {
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Piercing value: ") + FString::FromInt(d.piercing));
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, FString("Damage b4 piercing: ") + FString::FromInt(d.damage));
      }
   }

   void URTSDamageCalculation::PrintCritOccurence(FUpDamage & d)
   {
      if(CVARDamageDebugging.GetValueOnGameThread())
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Crit!") + FString::FromInt(d.damage));
   }

   void URTSDamageCalculation::PrintFinalCalculatedDamage(FUpDamage & d)
   {
      if(CVARDamageDebugging.GetValueOnGameThread())
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Final Damage: ") + FString::FromInt(d.damage));
   }

   void URTSDamageCalculation::CalculateDamageReduction(FUpDamage & d, FGameplayTagContainer & effects) const
   {
      CalculatePiercing(d.targetUnit, d, false); // Calculate defensive piercing

      PrintPreDamageReductionValues(d);

      ///--After we successfully calculate piercing, apply piercing effects
      /// 1 - Increase base damage by 1% per 35 points of piercing
      float percentageConversion = d.piercing / 35.f;
      d.damage                   = d.damage * (100 + percentageConversion) / 100;
      float critRoll             = FMath::FRandRange(1, 100);

      if(CVARDamageDebugging.GetValueOnGameThread()) {
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,
                                          FString("Calculated CritPerc! ") +
                                              FString::FromInt(d.sourceUnit->statComponent->GetSkillAdjValue(EUnitScalingStats::Critical_Chance) + percentageConversion));
         GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Crit roll perc = ") + FString::SanitizeFloat(critRoll + percentageConversion));
      }

      /// 2 - Increase critical chance
      if(critRoll + percentageConversion >
         100 - d.sourceUnit->statComponent->GetSkillAdjValue(EUnitScalingStats::Critical_Chance)) // piercing will increase crit chance as well
      {
         d.crit = true;

         PrintCritOccurence(d);

         d.damage = d.damage * d.sourceUnit->statComponent->GetSkillAdjValue(EUnitScalingStats::Critical_Damage);
      } else
         d.crit = false;
      ///--End of piercing application

      PrintFinalCalculatedDamage(d);

      /// 3 - Calculate accuracy based on our accuracy and the opponent's dodge --.
      d.accuracy = d.accuracy / 35.f;
      d.accuracy = FMath::RandRange(0, 100) + d.sourceUnit->statComponent->GetSkillAdjValue(EUnitScalingStats::Dodge) - d.accuracy; // accuracy now becomes dodge roll

      /// 4 - Apply any effects that need everything properly calculated
      ReceiveEffects(d, effects);
   }

   void URTSDamageCalculation::CalculatePiercing(AUnit * unit, FUpDamage & d, bool isAtt)
   {
      // Apply effects from buffs and weapons
      if(d.element.GetTagName() == "Combat.Element.Arcane") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Arcane_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Arcane_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Blood") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Blood_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Blood_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Chaos") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Chaos_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Chaos_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Dark") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Darkness_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Darkness_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Earth") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Earth_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Earth_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Electric") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Electric_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Electric_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Ethereal") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Ethereal_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Ethereal_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Fire") {
         d.piercing +=
             unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Fire_Aff) * isAtt - !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Fire_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Force") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Physical_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Physical_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Light") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Light_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Light_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Poison") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Poison_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Poison_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Water") {
         d.piercing += unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Water_Aff) * isAtt -
                       !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Water_Resist);
      } else if(d.element.GetTagName() == "Combat.Element.Wind") {
         d.piercing +=
             unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Wind_Aff) * isAtt - !isAtt * unit->statComponent->GetSkillAdjValue(EUnitScalingStats::Wind_Resist);
      }
   }

   void URTSDamageCalculation::ShowDamageDealt(const FUpDamage& d)
   {
      // Auto attach makes the new component the root for the sucessive components
      // TODO: Maybe create a singleton interface to split up this hudmanager functionality for this specific use
      UDIRender* tRC = NewObject<UDIRender>(
          d.targetUnit, Cast<AUserInput>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController(GetWorld()))->GetHUDManager()->damageIndicatorClass);
      if(tRC) {
         tRC->AttachToComponent(d.targetUnit->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
         tRC->RegisterComponent();

         if(d.accuracy > 100)
            tRC->Text = (NSLOCTEXT("Combat", "Dodge", "Dodged!"));
         else
            tRC->Text = FText::FromString(FString::FromInt(d.damage));

         if(d.crit)
            tRC->SetWorldSize(tRC->textSize * 2);
         else
            tRC->SetWorldSize(tRC->textSize);

         tRC->SetTextRenderColor(USpellFunctionLibrary::elementalMap[d.element]);
         tRC->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
         tRC->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextBottom);
      }
   }
#pragma endregion
