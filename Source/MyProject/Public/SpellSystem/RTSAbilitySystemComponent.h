#pragma once

#include "AbilitySystemComponent.h"
#include "DEPRECATED_RTSRangeDiminishCalc.h"

#include "RTSAbilitySystemComponent.generated.h"

struct FDamageScalarStruct;
class UMySpell;

/**
 * @brief Spells that a unit starts out with.
 */
USTRUCT(BlueprintType)
struct FDefaultLearnedAbility
{
   GENERATED_BODY()

   UPROPERTY(EditAnywhere)
   TSoftClassPtr<UMySpell> spellClass = nullptr;

   UPROPERTY(EditAnywhere, Meta = (ClampMin = "1"))
   int initialLevel = 1;
};

/**
 * Custom ability component with extra functionality
 * All units need a copy of this if we want to allow them to get status effects. That means every unit has this.
 */
UCLASS()
class MYPROJECT_API URTSAbilitySystemComponent : public UAbilitySystemComponent
{
   GENERATED_BODY()

   URTSAbilitySystemComponent();

 public:
   void BeginPlay() override;

   /** Get the class of the spell at this slot (CHECKED INDEX ACCESS) */
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   TSubclassOf<UMySpell> GetSpellAtSlot(int index) const;

   /** We can use this to apply a damage effect if we care to know if the damage effect was blocked or missed */
   UFUNCTION(BlueprintCallable, Category = "Spell Helper")
   bool ApplyDamageEffectSpecToTarget(const FGameplayEffectSpecHandle& damageEffectSpecHandle, UAbilitySystemComponent* targetComp);

   UFUNCTION(BlueprintCallable)
   const TArray<TSubclassOf<UMySpell>>& GetAbilities() const { return abilities; }

   void SetSpellAtSlot(TSubclassOf<UMySpell> spellClassToSet, int slotIndex);

   int FindSlotIndexOfSpell(TSubclassOf<UMySpell> spellToLookFor) const;

   /** Attempts to removes invisibility effect. Usually called when a unit performs an action when they are invisible, thus breaking the invisibility*/
   void TryRemoveInvisibility();

   /** Overriden to allow us to add things like purging buffs*/
   FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& Spec, FPredictionKey PredictionKey) override;

   FGameplayEffectSpecHandle MakeDamageEffect(FDamageScalarStruct damageScalars, FGameplayTag attackElement);
   bool                      ApplyDamageToSelf(FDamageScalarStruct damageScalars, FGameplayTag attackElement);
   bool                      ApplyDamageToTarget(URTSAbilitySystemComponent* targetComponent, FDamageScalarStruct damageScalars, FGameplayTag attackElement);

   FGameplayAbilitySpec*       FindAbilitySpecFromClass(TSubclassOf<UGameplayAbility> InAbilityClass);
   const FGameplayAbilitySpec* FindAbilitySpecFromClass(TSubclassOf<UGameplayAbility> InAbilityClass) const;

   /** Dedicated attribute for passing in parameters to our effect exec calculations */
   UPROPERTY(meta = (SystemGameplayAttribute = "true"))
   float EffectParameter;

   static FProperty* GetEffectParameterProperty();

   static const FGameplayEffectAttributeCaptureDefinition& GetEffectParameterCapture();

 protected:
   /**
    * For allies, this is the list of abilities that are in their skill slot (max 6).
    * Heroes can potentially swap out skills from their spell book to a skill slot.
    * For enemies, this is all the skills they have.
    */
   UPROPERTY(Transient)
   TArray<TSubclassOf<class UMySpell>> abilities;

   /**
    * Abilities that this unit should start out with. Used to give enemies a set of abilities to work with,
    * although enemies could gain or lose abilities in certain scenarios in which that would refelct in the abilities property.
    */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Defaults")
   TArray<FDefaultLearnedAbility> defaultAbilities;

 private:
   class AUnit* unitOwnerRef;

   TUniquePtr<FThreadSafeCounter> abilitiesLoadedCounter = nullptr;
};
