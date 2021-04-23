
#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "EffectIconMap.generated.h"

USTRUCT(BlueprintType)
struct FGameplayEffectSlotData
{
   GENERATED_BODY()

   UPROPERTY(EditAnywhere)
   FText effectName;

   UPROPERTY(EditAnywhere, meta = (Categories = "Combat.EffectName"))
   FGameplayTag effectNameTag;

   UPROPERTY(EditAnywhere)
   TSoftObjectPtr<UTexture2D> effectIcon;

   /**
    * Use {key} to add formatting that replaces the named argument with the value of the spell with the correct level applied.
    * str - Strength
    * int - Intelligence
    * agi - Agility
    * und - Understanding
    * hit - Hitpoints
    * aoe - Area of Effect
    * dur - Duration
    * per - Period
    * Use [row_name] to add formatting that reads from the Composite Curve Table holding effect power values (e.g. [Reflective_Armor_DamageReduction]).
    */
   UPROPERTY(EditAnywhere)
   FText description;

   bool operator==(const FGameplayEffectSlotData& otherSlotData) const { return effectNameTag == otherSlotData.effectNameTag; }
};

FORCEINLINE uint8 GetTypeHash(const FGameplayEffectSlotData& effectData)
{
   return GetTypeHash(effectData.effectNameTag);
}

/*template <typename ElementType>
struct GameplayEffectUIDataKeyFuncs : DefaultKeyFuncs<ElementType, true>
{
   using KeyInitType     = typename TCallTraits<ElementType>::ParamType;
   using ElementInitType = typename TCallTraits<ElementType>::ParamType;

   // Calculates a hash index for a key.
   static FORCEINLINE uint32 GetKeyHash(KeyInitType& Key) { return GetTypeHash(Key.effectName); }

   // Used when trying to ensure bucket has correct value in hashtable. Matches the interactable actor with the struct holding its data.
   template <typename ComparableKey>
   static FORCEINLINE bool Matches(KeyInitType& A, ComparableKey& B)
   {
      return GetTypeHash(A.effectName) == GetTypeHash(B);
   }
};*/

UCLASS()
class MYPROJECT_API UEffectIcons : public UDataAsset
{
   GENERATED_BODY()

 public:
   UPROPERTY(EditAnywhere)
   TSet<FGameplayEffectSlotData> effectUIData;


   const FGameplayEffectSlotData* GetEffectIcon(const FGameplayTag& effectName)
   {
      FGameplayEffectSlotData effectDataKey;
      effectDataKey.effectNameTag = effectName;
      return effectUIData.Find(effectDataKey);
   }
};
