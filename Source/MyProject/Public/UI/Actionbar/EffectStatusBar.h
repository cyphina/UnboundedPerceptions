#pragma once

#include "SlotContainer.h"
#include "GameplayEffect.h"
#include "EffectStatusBar.generated.h"

class UEffectSlot;
class UEffectIcons;

/**
 * Part of the actionbar, shown in single unit views denoting the status effects they have
 */
UCLASS(meta = (DisableNativeTick))
class MYPROJECT_API UEffectStatusBar : public USlotContainer
{
   GENERATED_BODY()

 public:
   int GetNumValidItems() const override;

 protected:
   void NativeOnInitialized() override;
   void NativeConstruct() override;
   void NativeDestruct() override;
   bool CheckEffectStillExistsAndAdjustIndex();

   FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

   UPROPERTY(BlueprintReadWrite)
   TArray<UEffectSlot*> effectSlots;

 private:
   /**Updates the number of status effects displayed.  Called after status effect bar is displayed*/
   UFUNCTION(BlueprintCallable, Category = "Skills")
   void UpdateStatusBar();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skills")
   bool ActiveEffectHandleEqualityCheck(const FActiveGameplayEffectHandle& handle1, const FActiveGameplayEffectHandle& handle2) const;

   FLinearColor GetEffectColor(const FGameplayTagContainer&) const;

   int32 GetMaxNumberSlotsToDisplay();

   const struct FGameplayEffectSlotData* GetEffectSlotData(const FGameplayTag& effectName) const;

   /** Hide all the effect slots being shown currently from viewport */
   void ResetShownEffects();

   UPROPERTY(EditDefaultsOnly, Category = "Effect Data")
   UEffectIcons* effectIconDatabase;

   /**Stores all the active effects on the current focusedUnit*/
   TArray<FActiveGameplayEffectHandle> effects;

   //Maximum number of effects that can be displayed... for now
   static const int maxNumDisplaySlots = 11;

   FTimerHandle effectBarTimer;

   class AUserInput* CPCRef;
};
