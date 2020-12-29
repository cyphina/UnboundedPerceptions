// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility.h"
#include "GameplayTagsManager.h"
#include "SpellDataManager.h"
#include "GameplayEffect.h"
#include "SpellSystem/DamageStructs.h"
#include "MySpell.generated.h"

class AUnit;

USTRUCT(BlueprintType)
struct FSpellDefaults {
   GENERATED_USTRUCT_BODY()

 public:
   FSpellDefaults() = default;
   FSpellDefaults(int id, UTexture2D* image) : id(id), image(image) {}
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spell")
   int id; // set id to same as in table

   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spell")
   UTexture2D* image;
};

/* FIRST ABILITY TAG SHOULD BE CD TAG
 * Abilities should start at level 1 not 0
 * Remember when creating spells in blueprints we must do a few things
 * 1. Ensure the ID matches the entry in the table (and create an entry in the table if there is not any)
 * 2. Call parent ActivateAbility
 * 3. Create matching gameplay tag and set it in the spell default properties
 * 4. Call Commit ability and EndAbility
 */
UCLASS()
class MYPROJECT_API UMySpell : public UGameplayAbility
{
   GENERATED_BODY()

   AUnit* unitRef;

 public:
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spell")
   FSpellDefaults spellDefaults;

   FGameplayTagContainer cooldownTags;
   static const int      NUM_SCALING_DAMAGE_ATT = 4; // Number of attributes damage effects can scale on

   UMySpell();
   void PostInitProperties() override;
   void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                        const FGameplayEventData* TriggerEventData) override;
   bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
   ///---!!!---we want to make the avatar actor the source of the spell since owner is our playercontroller---!!!---
   FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
   void                         CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
   void                         OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

   float GetCooldownTimeRemaining(const FGameplayAbilityActorInfo* ActorInfo) const override;

   UFUNCTION(BlueprintCallable, Category = "SpellChecks")
   bool IsOnCD(UAbilitySystemComponent* abilityComponent) const;

#pragma region accessors
   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetMaxLevel() const { return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->maxLevel; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetCDDuration(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetRange(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetReqLevel(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetCost(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   FUpSpellTargeting* GetTargeting() const { return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->targeting; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   FText GetDescription() const { return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->desc; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   FText GetName() const { return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->name; }

   /**Get the name of the primary element this spell is associated with*/
   UFUNCTION(BlueprintCallable, Category = "Spell")
   FText GetElem() const { return FText::FromString(USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->elem.ToString().RightChop(15)); }

   /**Get the name of the primary element this spell is associated with*/
   UFUNCTION(BlueprintCallable, Category = "Spell")
   FGameplayTag GetElemTag() const { return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->elem; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   TArray<int> GetPreReqs() const { return USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->preReqs; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   TArray<FText> GetPreReqNames() const
   {
      TArray<FText> preReqNames;
      for (int i : USpellDataManager::GetData().GetSpellInfo(spellDefaults.id)->preReqs) {
         preReqNames.Add(USpellDataManager::GetData().GetSpellInfo(i)->name);
      }
      return preReqNames;
   }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetSpellDuration(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   FDamageScalarStruct GetDamage(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetPeriod(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetCastTime(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetSecondaryTime(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetAOE(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetLevel(UAbilitySystemComponent* abilityComponent) const;

   /**Helper function to set the stat modifiers of a gameplay effect*/
   UFUNCTION(BlueprintCallable, Category = "Helper")
   FGameplayEffectSpecHandle SetScaling(FGameplayEffectSpecHandle specHandle);

   UFUNCTION(BlueprintCallable, Category = "Helper")
   FGameplayEffectSpecHandle SetPeriod(FGameplayEffectSpecHandle specHandle, float period);

   /**Creates a gameplay effect setting the duration and period accordint to the table values*/
   UFUNCTION(BlueprintCallable, Category = "Spell Creation Helper")
   struct FGameplayEffectSpecHandle CreateGameplayEffectFromTableValues(TSubclassOf<UGameplayEffect> effectClass, FGameplayTag name, FGameplayTagContainer assetTags);

#pragma endregion

 private:
   UFUNCTION(BlueprintCallable, Category = "Helper")
   UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

   /**Used to get the index from some spell information array based on current spell level
    * Invariant, AbilityLevels start at 1
    * @param currentLevel : Current level of spell property (ex. Fireball has had 2 points put into it so it's level 2)
    * @param numCategories : Number of levels in spell property (ex. there may be 3 range upgrades for a spell)
    * @param maxLevel : Number of times this spell can be leveld up (ex. there may 5 total level upgrades for a spell, only 3 upgrades increase range)
    */
   inline int GetIndex(int currentLevel, int numCategories, int maxLevel) const
   {
      int denom = maxLevel * currentLevel;
      if (denom != 0) return FMath::CeilToInt((float)numCategories / denom) - 1;
      return 0;
   }
};
