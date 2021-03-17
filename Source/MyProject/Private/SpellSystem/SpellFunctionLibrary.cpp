#include "MyProject.h"
#include "SpellFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"

#include "RTSProjectile.h"

#include "MySpell.h"

#include "WorldObjects/Unit.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "RTSProjectileStrategy.h"
#include "TextFormatter.h"
#include "Engine/CompositeCurveTable.h"

const FGameplayTag USpellFunctionLibrary::CONFIRM_SPELL_TAG        = FGameplayTag::RequestGameplayTag("Skill.Name.Confirm Spell");
const FGameplayTag USpellFunctionLibrary::CONFIRM_SPELL_TARGET_TAG = FGameplayTag::RequestGameplayTag("Skill.Name.Confirm Target");

USpellFunctionLibrary::USpellFunctionLibrary(const FObjectInitializer& o) : Super(o)
{
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeGameplayEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, int Level, float Duration,
                                                                    float Period, FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags)
{
   FGameplayEffectSpecHandle effect = AbilityRef->MakeOutgoingGameplayEffectSpec(EffectClass, Level);
   effect.Data->DynamicAssetTags.AddTag(Elem); // Use add tag to check if tag is valid and prevents duplicate tags.
   effect.Data->DynamicAssetTags.AddTag(Name);
   effect.Data->DynamicAssetTags.AppendTags(assetTags);
   if(effect.Data->Def->DurationPolicy != EGameplayEffectDurationType::Instant) // Do this check since some instant effects rely on this procedure too
   {
      if(EffectClass.GetDefaultObject()->Executions.Num())
      {
         // Spells that have executions and durations need to be periodic but we don't want the period to do anything so set it to something really large.
         if(Period == 0)
         {
            Period = 999;
         }
      }
      effect.Data->Period = Period;
      effect.Data->SetDuration(Duration, true); // if we don't lock the duration, the duration will be recalcuated somewhere in active effect creation ...
   }
   return effect;
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeDamageEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, int Level, float Duration,
                                                                  float Period, FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags,
                                                                  FDamageScalarStruct damageVals)
{
   FGameplayEffectSpecHandle effect = MakeGameplayEffect(AbilityRef, EffectClass, Level, Duration, Period, Elem, Name, assetTags);
   effect.Data->DynamicAssetTags.AddTag(Elem); // Use add tag to check if tag is valid and prevents duplicate tags.
   effect.Data->DynamicAssetTags.AddTag(Name);

   // no period since damage is instant application
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Health"), damageVals.hitpoints);
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Strength"), damageVals.strength);
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Intelligence"), damageVals.intelligence);
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Agility"), damageVals.agility);
   effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Combat.Stats.Understanding"), damageVals.understanding);
   return effect;
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeStatChangeEffect(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, int Level, float Duration,
                                                                      float Period, FGameplayTag Elem, FGameplayTag Name, FGameplayTagContainer assetTags,
                                                                      TArray<FStatChange> StatChanges = TArray<FStatChange>())
{
   FGameplayEffectSpecHandle effect = MakeGameplayEffect(AbilityRef, EffectClass, Level, Duration, Period, Elem, Name, assetTags);
   for(FStatChange statChange : StatChanges)
   {
      effect.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(*(FString("Combat.Stats.") + statChange.changedAtt.GetName())),
                                           statChange.changeStatMagnitude);
      // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, *(FString("Combat.Stats.") + statChange.changedAtt.GetName()));
   }
   // statChanges = MoveTemp(StatChanges);
   return effect;
}

ARTSProjectile* USpellFunctionLibrary::SetupBulletTargetting(AUnit* casterRef, TSubclassOf<ARTSProjectile> bulletClass,
                                                             TSubclassOf<URTSProjectileStrategy>           projectileStrategyClass,
                                                             UPARAM(ref) TArray<FGameplayEffectSpecHandle> specHandles, bool canGoThroughWalls)
{
   FTransform spawnTransform = casterRef->GetActorTransform();
   spawnTransform.SetLocation(spawnTransform.GetLocation() + casterRef->GetActorForwardVector() * 10.f);

   URTSProjectileStrategy* projectileStrategy = nullptr;
   if(projectileStrategyClass)
   {
      projectileStrategy                    = NewObject<URTSProjectileStrategy>(casterRef, projectileStrategyClass);
      projectileStrategy->canGoThroughWalls = canGoThroughWalls;
   }
   else
   {
      projectileStrategy = URTSProjectileStrategy::StaticClass()->GetDefaultObject<URTSProjectileStrategy>();
   }
   projectileStrategy->defaultHitEffects = specHandles;

   ARTSProjectile* projectile =
       ARTSProjectile::MakeRTSProjectile(casterRef->GetWorld(), casterRef->GetTargetComponent(), casterRef->GetActorTransform(), bulletClass, projectileStrategy);

   return projectile;
}

FText USpellFunctionLibrary::ParseDesc(const FText& inputText, const UAbilitySystemComponent* compRef, UMySpell* spell, UCompositeCurveTable* effectPowerTableRef)
{
   FTextFormatPatternDefinitionRef descriptionPatternDef = MakeShared<FTextFormatPatternDefinition, ESPMode::ThreadSafe>();
   descriptionPatternDef->SetArgStartChar(TEXT('['));
   descriptionPatternDef->SetArgEndChar(TEXT(']'));

   const FString descriptionString = inputText.ToString();

   FFormatNamedArguments effectArgs;
   const FTextFormat     effectFormat = FTextFormat::FromString(descriptionString, descriptionPatternDef);

   if(FCString::Strchr(*descriptionString, descriptionPatternDef->ArgStartChar) != nullptr)
   {
      if(effectFormat.IsValid())
      {
         TArray<FString> effectFormatTokens;

         effectFormat.GetFormatArgumentNames(effectFormatTokens);

         for(const FString& token : effectFormatTokens)
         {
            if(effectPowerTableRef->GetRowMap().Contains(*token))
            {
               FRealCurve* effectPowerCurve = effectPowerTableRef->GetRowMap()[*token];
               float       minTime, maxTime;
               effectPowerCurve->GetTimeRange(minTime, maxTime);
               const int   index               = UMySpell::GetIndex(spell->GetLevel(compRef), maxTime, spell->GetMaxLevel());
               const float effectTokenArgValue = effectPowerCurve->Eval(index + 1);
               effectArgs.Add(*token, effectTokenArgValue);
            }
         }
      }
   }

   FFormatNamedArguments args;

   static const TCHAR* strKey      = TEXT("str");
   static const TCHAR* intKey      = TEXT("int");
   static const TCHAR* agiKey      = TEXT("agi");
   static const TCHAR* undKey      = TEXT("und");
   static const TCHAR* hpKey       = TEXT("hit");
   static const TCHAR* aoeKey      = TEXT("aoe");
   static const TCHAR* durationKey = TEXT("dur");
   static const TCHAR* periodKey   = TEXT("per");

   args.Add(strKey, spell->GetDamage(compRef).strength);
   args.Add(intKey, spell->GetDamage(compRef).intelligence);
   args.Add(agiKey, spell->GetDamage(compRef).agility);
   args.Add(undKey, spell->GetDamage(compRef).understanding);
   args.Add(hpKey, spell->GetDamage(compRef).hitpoints);
   args.Add(aoeKey, spell->GetAOE(compRef));
   args.Add(durationKey, spell->GetSpellDuration(compRef));
   args.Add(periodKey, spell->GetPeriod(compRef));

   return FText::Format(FText::FromString(FTextFormatter::FormatStr(effectFormat, effectArgs, false, true)), args);
}

void USpellFunctionLibrary::SpellConfirmSwap(TSubclassOf<UMySpell> confirmSpell, TSubclassOf<UMySpell> originalSpell, AUnit* ownerRef, bool bSwapInConfirm)
{
   TSubclassOf<UMySpell> spellToReplace, replacementSpell;
   if(bSwapInConfirm)
   {
      spellToReplace   = originalSpell;
      replacementSpell = confirmSpell;
   }
   else
   {
      spellToReplace   = confirmSpell;
      replacementSpell = originalSpell;
   }

   SpellSwap(spellToReplace, replacementSpell, ownerRef);
}

void USpellFunctionLibrary::SpellSwap(TSubclassOf<UMySpell> originalSpell, TSubclassOf<UMySpell> newSpell, AUnit* ownerRef)
{
   const int slot = ownerRef->GetAbilitySystemComponent()->GetAbilities().Find(originalSpell);

   if(slot == INDEX_NONE)
   {
      UE_LOG(LogTemp, Error, TEXT("Cannot find original spell to swap with"));
      return;
   }

   ownerRef->GetAbilitySystemComponent()->SetSpellAtSlot(newSpell, slot);
}
