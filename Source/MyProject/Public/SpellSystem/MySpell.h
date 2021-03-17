#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility.h"
#include "GameplayTagsManager.h"
#include "SpellDataManager.h"
#include "GameplayEffect.h"
#include "SpellSystem/DamageStructs.h"
#include "SpellDefaults.h"
#include "MySpell.generated.h"

class URTSAbilitySystemComponent;
class AUnit;

/* 
 * Abilities should start at level 1 not 0
 * Remember when creating spells in blueprints we must do a few things
 * 1. Setup the default properties
 *    - Ensure the ID matches the entry in the table (and create an entry in the table if there is not any)
 *    - Give it an image too
 * 2. Call parent ActivateAbility
 * 4. Call Commit ability and EndAbility
 */
UCLASS()
class MYPROJECT_API UMySpell : public UGameplayAbility
{
   GENERATED_BODY()

 public:
   UMySpell();

   UFUNCTION(BlueprintCallable, Category = "Spell")
   const FSpellDefaults& GetSpellDefaults() const { return spellDefaults; }

   UFUNCTION(BlueprintCallable, Category = "SpellChecks")
   bool IsOnCD(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetMaxLevel() const { return spellDefaults.MaxLevel; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetCDDuration(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetRange(UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetReqLevel(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetCost(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   FText GetDescription() const { return spellDefaults.Desc; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   FText GetSpellName() const { return spellDefaults.Name; }

   /**Get the name of the primary element this spell is associated with*/
   UFUNCTION(BlueprintCallable, Category = "Spell")
   FText GetElem() const { return FText::FromString(spellDefaults.Elem.ToString().RightChop(15)); }

   /**Get the name of the primary element this spell is associated with*/
   UFUNCTION(BlueprintCallable, Category = "Spell")
   FGameplayTag GetElemTag() const { return spellDefaults.Elem; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   TArray<TSubclassOf<UMySpell>> GetPreReqs() const { return GetSpellDefaults().PreReqs; }

   UFUNCTION(BlueprintCallable, Category = "Spell")
   TArray<FText> GetPreReqNames() const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetSpellDuration(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   FDamageScalarStruct GetDamage(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetPeriod(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetCastTime(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetSecondaryTime(const UAbilitySystemComponent* abilityComponent) const;

   UFUNCTION(BlueprintCallable, Category = "Spell")
   float GetAOE(const UAbilitySystemComponent* abilityComponent) const;

   /** You can call GetAbilityLevel() on an active spell (aka in spell blueprints) which doesn't require us to pass in an abilityComponent */
   UFUNCTION(BlueprintCallable, Category = "Spell")
   int GetLevel(const UAbilitySystemComponent* abilityComponent) const;

   /**
    * Creates a gameplay effect setting the duration and period according to the table values
    * @param effectClass - Type of effect we want to create. This effect will have its values like duration and period set from the table values.
    * @param name - Name of the effect. The effect doesn't need to have a name if it's instantaneous.
    * @param assetTags - Any extra tags we want this effect to have (like secondary elements ,etc.)
    */
   UFUNCTION(BlueprintCallable, Category = "Spell Creation Helper")
   struct FGameplayEffectSpecHandle CreateGameplayEffectFromTableValues(TSubclassOf<UGameplayEffect> effectClass, FGameplayTag name, FGameplayTagContainer assetTags);

   UFUNCTION(BlueprintCallable)
   UEnvQuery* GetDefaultQueryForSpell(UWorld* worldRef);

   /**
    * Some abilities have logic that blocks the caster till the spell is active. This means the AI shouldn't do anything till EndAbility is called.
    * Other abilities do something but don't require the spell caster to stay blocked, so we need to tell the AI they are free to move onto their next action (see ability called Up_Ability_Grenade)
    */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   void FinishBlockingCaster();

   TSubclassOf<UUpSpellTargeting> GetTargeting() const;

   /** Our version of cooldown just checks for the name tag of this spell */
   bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                      OUT FGameplayTagContainer* OptionalRelevantTags) const override;

   /** We want to make the avatar actor the source of the spell since owner is our player controller */
   FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;

   /** Overrides functionality to apply cooldown and cost */
   void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

   float GetCooldownTimeRemaining(const FGameplayAbilityActorInfo* ActorInfo) const override;

   /**
    * This function is used to tell us when the caster doesn't have to worry about the spell anymore (some spells may keep doing things but the caster doesn't affect the spell
    * anymore like dropping a totem). Sometimes we have spells that take a while to perform so this may be called a few seconds after we activate the spell.
    * If you forget to call EndAbility, you risk possibly screwing up the AI system (cast spell tasks won't finish).
    */
   void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                   bool bReplicateEndAbility, bool bWasCancelled) override;

   /**
    * Used to get the 0'based index from some spell information array based on current spell level
    * ! Invariant - AbilityLevels start at 1
    * @param currentLevel : Current level of spell property (ex. Fireball has had 2 points put into it so it's level 2)
    * @param numCategories : Number of levels in spell property (ex. there may be 3 range upgrades for a spell)
    * @param maxLevel : Number of times this spell can be leveled up (ex. there may 5 total level upgrades for a spell, only 3 upgrades increase range)
    */
   static int GetIndex(int currentLevel, int numCategories, int maxLevel);

 protected:
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spell")
   FSpellDefaults spellDefaults;

 private:
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   FGameplayEffectSpecHandle SetScaling(FGameplayEffectSpecHandle specHandle);

   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   FGameplayEffectSpecHandle SetPeriod(FGameplayEffectSpecHandle specHandle, float period);

   ///
   /// Active accessors allow us to get data for a spell that is being activated since the actor info is already set and we can find the ability system component from that.
   /// Only use them within a BP derived from UMySpell.
   ///

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   UAbilitySystemComponent* GetAvatarAbilitySystemComponent() const;

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   AUnit* GetAvatarUnit() const;

   /** Whenever we need to target something in a "Wait Target Data" node we should use this or the node that makes target location info from the avatar actor */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spell Helper")
   FGameplayAbilityTargetingLocationInfo MakeTargetLocationInfoFromTargetComponent();

   /** Whenever we need to target something in a "Wait Target Data" node we should use this or the node that makes target location info a location we can pass in */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spell Helper")
   FGameplayAbilityTargetingLocationInfo MakeTargetLocationInfoFromLocation(FVector targetLocation);

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   FGameplayAbilityTargetDataHandle GetAvatarTargetData() const;

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell.
    * (Will crash if the current target is not a unit i.e. you cast an AOE spell) */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   AUnit* GetAvatarTargetUnit() const;

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   FVector GetAvatarActorLocation() const;

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   FVector GetAvatarTargetLocation() const;

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   int GetActiveSpellRange() const { return GetRange(GetAvatarAbilitySystemComponent()); }

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   float GetActiveSpellAOE() const { return GetAOE(GetAvatarAbilitySystemComponent()); }

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   float GetActiveSpellDuration() const { return GetSpellDuration(GetAvatarAbilitySystemComponent()); }

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   float GetActiveSpellCDDuration() const { return GetCDDuration(GetAvatarAbilitySystemComponent()); }

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   float GetActivePeriod() const { return GetPeriod(GetAvatarAbilitySystemComponent()); }

   /** Only use this inside ActivateAbility in a blueprint derived from UMySpell */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   FDamageScalarStruct GetActiveDamage() const { return GetDamage(GetAvatarAbilitySystemComponent()); }

   /**
    * Only use this inside ActivateAbility in a blueprint derived from UMySpell.
    * Gives us the collision trace channel for units that would be considered an enemy to the caster.
    */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   TArray<TEnumAsByte<ECollisionChannel>> GetTraceChannelForEnemy() const;

   /**
   * Only use this inside ActivateAbility in a blueprint derived from UMySpell.
   * Gives us the collision trace channel for units that would be considered a friendly to the caster.
   */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   TArray<TEnumAsByte<ECollisionChannel>> GetTraceChannelForFriendly() const;

   /**
   * Only use this inside ActivateAbility in a blueprint derived from UMySpell.
   * Gives us the collision object channel for units that would be considered a friendly to the caster.
   */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   TArray<TEnumAsByte<ECollisionChannel>> GetObjectChannelForEnemy() const;

   /** Applies several other effects if the damage effect hits. The Damage effect values is read off the table. */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   void ApplyEffectsTargetIfNoMiss(const FGameplayAbilityTargetDataHandle& targetData, const TArray<FGameplayEffectSpecHandle>& effects);

   /**
    * Call this to check if the last hit was a miss or not.
    * Call this after enacting a damage effect to quickly determine if other effects should apply.
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spell Helper")
   bool IsLastHitAMiss();

   /**
   * Only use this inside ActivateAbility in a blueprint derived from UMySpell.
   * Gives us the collision object channel for units that would be considered a friendly to the caster.
   */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   TArray<TEnumAsByte<ECollisionChannel>> GetObjectChannelForFriendly() const;

   FGameplayAbilitySpec* GetAbilitySpec(const UAbilitySystemComponent* abilityComponent) const;

   template <typename T>
   T GetSpellDefaultValueChecked(const UAbilitySystemComponent* abilityComponent, T spellValue) const
   {
      FGameplayAbilitySpec* abilitySpec = GetAbilitySpec(abilityComponent);
      if(abilitySpec)
      {
         return spellValue;
      }
      return T();
   }

   template <typename T>
   T GetSpellDefaultValueChecked(const UAbilitySystemComponent* abilityComponent, const TArray<T>& spellValues) const
   {
      FGameplayAbilitySpec* abilitySpec = GetAbilitySpec(abilityComponent);
      if(abilitySpec)
      {
         const int numUpgrades = spellValues.Num();
         const int levelIndex  = GetIndex(abilitySpec->Level, numUpgrades, GetMaxLevel());
         if(levelIndex >= 0 && numUpgrades > 0)
         {
            return spellValues[levelIndex];
         }
      }
      return T();
   }

   /**
    * @brief Checks to see if we already sent the AI a message about finishing the whole spell (the spell may have some logic left but the caster is free to do other actions)
    */
   bool bCasterDone = false;
};
