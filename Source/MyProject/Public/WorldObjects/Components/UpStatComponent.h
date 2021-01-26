// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Stats/BaseCharacter.h"
#include "UpStatComponent.generated.h"

struct FUpDamage;
class UMyAttributeSet;

/** A component used for anything that has stats. Can be used on units, or maybe towers.
 * Doesn't handle any logic with the stats, only provides means to access them and properly modify them.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpStatComponent : public UActorComponent
{
   GENERATED_BODY()

 public:
   UUpStatComponent();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetAttributeBaseValue(EAttributes att) const { return baseC->GetAttribute(static_cast<uint8>(att))->GetBaseValue(); };

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetSkillBaseValue(EUnitScalingStats skill) const { return baseC->GetSkill(static_cast<uint8>(skill))->GetBaseValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalBaseValue(EVitals vit) const { return baseC->GetVital(static_cast<uint8>(vit))->GetBaseValue(); };

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetMechanicBaseValue(EMechanics mec) const { return baseC->GetMechanic(static_cast<uint8>(mec))->GetBaseValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetAttributeAdjValue(EAttributes att) const { return baseC->GetAttribute(static_cast<uint8>(att))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetSkillAdjValue(EUnitScalingStats skill) const { return baseC->GetSkill(static_cast<uint8>(skill))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetMechanicAdjValue(EMechanics mec) const { return baseC->GetMechanic(static_cast<uint8>(mec))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalAdjValue(EVitals vit) const { return baseC->GetVital(static_cast<uint8>(vit))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE float GetVitalCurValue(EVitals vit) const { return baseC->GetVital(static_cast<uint8>(vit))->GetCurrentValue(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE int GetUnitLevel() const { return baseC->GetLevel(); }

   UFUNCTION(BlueprintCallable, Category = "StatAccessors")
   void SetUnitLevel(int newLevel) const { return baseC->SetLevel(newLevel); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatAccessors")
   FORCEINLINE UMyAttributeSet* GetAttSet() const { return baseC->GetAttSet(); }

   /**
   * Call whenever we want stats to get recalculated. Typically done after a stat change
   * @param updatedStat - Attribute that was changed
   */
   UFUNCTION(BlueprintCallable, Category = "Stats")
   void UpdateStats(const FGameplayAttribute& updatedStat) const;
   
   /**
    * @brief Allows us to apply some bonuses to this hero's stat
    * @tparam bModifyBase - Modify the base stat or the adjusted value
    * @tparam  StatType - Enum representing what stat to modify
    * @param value - New value for the stat
    * @param specificStatType - Actual enum value
    */
   template <bool bModifyBase = false, typename StatType>
   void ModifyStats(int value, StatType specificStatType)
   {
      static_assert(std::is_enum<StatType>::value, "Must pass one of the stat Enums!");
      const int specificStatTypeVal = static_cast<int>(specificStatType);

      if constexpr(std::is_same<StatType, EAttributes>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetAttributeAdj(specificStatTypeVal, value);
         } else {
            baseC->SetAttributeBase(specificStatTypeVal, value);
         }
         UpdateStats(UMyAttributeSet::IndexAtts(static_cast<int>(specificStatTypeVal)));
      } else if constexpr(std::is_same<StatType, EUnitScalingStats>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetSkillAdj(specificStatTypeVal, value);
         } else {
            baseC->SetSkillBase(specificStatTypeVal, value);
         }
      } else if constexpr(std::is_same<StatType, EVitals>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetVitalAdj(specificStatTypeVal, value);
         } else {
            baseC->SetVitalBase(specificStatTypeVal, value);
         }
      } else if constexpr(std::is_same<StatType, EMechanics>::value) {
         if constexpr(!bModifyBase) {
            baseC->SetMechanicAdj(specificStatTypeVal, value);
         } else {
            baseC->SetMechanicBase(specificStatTypeVal, value);
         }
      } else {
         static_assert(std::is_same_v<StatType*, void>, "Error, please ensure you're passing in one of the correct stat enums");
      }
   }

   FOnStatsUpdated& OnBaseStatsUpdated() const;
   FOnStatsUpdated& OnStatsUpdated() const;

 protected:
   void BeginPlay() override;
   void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

 private:
   /** Used to interface with the stat data (which is actually held in an attribute set) */
   TUniquePtr<FBaseCharacter> baseC = nullptr;
};
