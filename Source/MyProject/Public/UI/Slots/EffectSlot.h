#pragma once

#include "ActionSlot.h"
#include "GameplayEffectTypes.h"
#include "EffectSlot.generated.h"

struct FGameplayTag;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSlotData;
class UAbilitySystemComponent;

/**
 * Slot that shows us what status effects we have in the status effect bar
 */
UCLASS()
class MYPROJECT_API UEffectSlot : public UActionSlot
{
   GENERATED_BODY()

 public:
   /**
	  * @brief By default slots are hidden, and when there are effects that are in the index corresponding to the slots, then we make the slots visible and showcase the effect icons.
	  */
   void ActivateEffectSlot(FActiveGameplayEffectHandle activeEffectHandle, const FGameplayEffectSlotData& effectSlotData, const FLinearColor& color);

   FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

 protected:
   UFUNCTION(BlueprintCallable, BlueprintPure)
   int32 GetStackCount() const;

   void SynchronizeProperties() override;

   void ShowDesc(UToolTipWidget* tooltip) override;

   UPROPERTY(Meta = (BindWidget))
   UTextBlock* Text_StackCount;

   UPROPERTY(BlueprintReadOnly)
   FActiveGameplayEffectHandle currentActiveEffectHandle;

 private:
   FText GetEffectTogglableText(const FGameplayEffectSpec& effectSpec);

   const FGameplayEffectSlotData* currentEffectSlotData;
};
