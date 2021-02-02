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


const FGameplayTag USpellFunctionLibrary::CONFIRM_SPELL_TAG        = FGameplayTag::RequestGameplayTag("Skill.Name.Confirm Spell");
const FGameplayTag USpellFunctionLibrary::CONFIRM_SPELL_TARGET_TAG = FGameplayTag::RequestGameplayTag("Skill.Name.Confirm Target");

USpellFunctionLibrary::USpellFunctionLibrary(const FObjectInitializer& o) :
   Super(o)
{
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeGameplayEffect
(UGameplayAbility* AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float                Duration,
 float             Period, FGameplayTag                     Elem, FGameplayTag Name, FGameplayTagContainer assetTags)
{
   FGameplayEffectSpecHandle effect = AbilityRef->MakeOutgoingGameplayEffectSpec(EffectClass, Level);
   effect.Data->DynamicAssetTags.AddTag(Elem); // Use add tag to check if tag is valid and prevents duplicate tags.
   effect.Data->DynamicAssetTags.AddTag(Name);
   effect.Data->DynamicAssetTags.AppendTags(assetTags);
   if(effect.Data->Def->DurationPolicy != EGameplayEffectDurationType::Instant) // Do this check since some instant effects rely on this procedure too
   {
      effect.Data->Period = Period;
      effect.Data->SetDuration(Duration, true); // if we don't lock the duration, the duration will be recalcuated somewhere in active effect creation ...
   }
   return effect;
}

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeDamageEffect
(UGameplayAbility*   AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float                Duration,
 float               Period, FGameplayTag                     Elem, FGameplayTag Name, FGameplayTagContainer assetTags,
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

FGameplayEffectSpecHandle USpellFunctionLibrary::MakeStatChangeEffect
(UGameplayAbility*   AbilityRef, TSubclassOf<UGameplayEffect> EffectClass, float Level, float                Duration,
 float               Period, FGameplayTag                     Elem, FGameplayTag Name, FGameplayTagContainer assetTags,
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

ARTSProjectile* USpellFunctionLibrary::SetupBulletTargetting
(AUnit*                                 casterRef, TSubclassOf<ARTSProjectile> bulletClass, URTSProjectileStrategy* projectileStrategy,
 UPARAM(ref) FGameplayEffectSpecHandle& specHandle, bool                       canGoThroughWalls)
{
   FTransform spawnTransform = casterRef->GetActorTransform();
   spawnTransform.SetLocation(spawnTransform.GetLocation() + casterRef->GetActorForwardVector() * 10.f);

   projectileStrategy->canGoThroughWalls = canGoThroughWalls;
   projectileStrategy->defaultHitEffects.Append(TArray<FGameplayEffectSpecHandle>{specHandle});
   ARTSProjectile* projectile =
   ARTSProjectile::MakeRTSProjectile(casterRef->GetWorld(), casterRef->GetTargetComponent(), casterRef->GetActorTransform(), bulletClass, projectileStrategy);

   return projectile;
}

FText USpellFunctionLibrary::ParseDesc(FText inputText, UAbilitySystemComponent* compRef, UMySpell* spell, TMap<FString, FString> args = TMap<FString, FString>())
{
   static const TCHAR* strKey      = TEXT("{str}");
   static const TCHAR* intKey      = TEXT("{int}");
   static const TCHAR* agiKey      = TEXT("{agi}");
   static const TCHAR* undKey      = TEXT("{und}");
   static const TCHAR* hpKey       = TEXT("{hp}");
   static const TCHAR* aoekey      = TEXT("{aoe}");
   static const TCHAR* durationkey = TEXT("{duration}");
   static const TCHAR* rangekey    = TEXT("{range}");

   FString parsedString = inputText.ToString();

   parsedString.Replace(strKey, *FString::FromInt(spell->GetDamage(compRef).strength));
   parsedString.Replace(intKey, *FString::FromInt(spell->GetDamage(compRef).intelligence));
   parsedString.Replace(agiKey, *FString::FromInt(spell->GetDamage(compRef).agility));
   parsedString.Replace(undKey, *FString::FromInt(spell->GetDamage(compRef).understanding));
   parsedString.Replace(hpKey, *FString::FromInt(spell->GetDamage(compRef).hitpoints));

   parsedString.Replace(aoekey, *FString::FromInt(spell->GetAOE(compRef)));
   parsedString.Replace(durationkey, *FString::SanitizeFloat(spell->GetSpellDuration(compRef)));
   parsedString.Replace(rangekey, *FString::FromInt(spell->GetRange(compRef)));

   return FText::FromString(parsedString);
}

void USpellFunctionLibrary::SpellConfirmSwap(TSubclassOf<UMySpell> confirmSpell, TSubclassOf<UMySpell> originalSpell, AUnit* ownerRef, bool bSwapInConfirm)
{
   TSubclassOf<UMySpell> spellToReplace, replacementSpell;
   if(bSwapInConfirm)
   {
      spellToReplace   = originalSpell;
      replacementSpell = confirmSpell;
   } else
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
