#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffect.h"
#include "EffectStatusBar.generated.h"

class UEffectIcons;

/**
 * Part of the actionbar, shown in single unit views denoting the status effects they have
 */
UCLASS()
class MYPROJECT_API UEffectStatusBar : public UUserWidget
{
   GENERATED_BODY()

public:

   /** Add an effect to be shown in the viewport*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Effect Data")
   void ToggleGameplayEffectIcon(FActiveGameplayEffectHandle activeEffectHandle, FGameplayTag effectName, UTexture2D* effectIcon, FLinearColor color, int index);

   /**Remove all the effects being shown currently from viewport*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void ResetShownEffects();
   
   static const int maxNumDisplaySlots = 11; //Maximum number of effects that can be displayed... for now

protected:
   void NativeConstruct() override;
   
private:
   /**Updates the number of status effects displayed.  Called after status effect bar is displayed*/
   UFUNCTION(BlueprintCallable, Category = "Skills")
   void UpdateStatusBar();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skills")
   bool ActiveEffectHandleEqualityCheck(const FActiveGameplayEffectHandle& handle1, const FActiveGameplayEffectHandle& handle2) const;
   
   UPROPERTY(EditAnywhere, Category = "Effect Data")
   UEffectIcons* effectIconDatabase;

   /**Stores all the active effects on the current focusedUnit*/
   UPROPERTY(BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess="true"))
   TArray<FActiveGameplayEffectHandle> effects;

   FTimerHandle effectBarTimer;
   
   const FGameplayTagContainer nameFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.EffectName"));
   const FGameplayTagContainer elemFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"));

   class AUserInput* CPCRef;
};
