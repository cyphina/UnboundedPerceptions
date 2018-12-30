// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffect.h"
#include "EffectStatusBar.generated.h"

/**
 * Part of the actionbar, shown in single unit views denoting the status effects they have
 */

class AUserInput;

UCLASS()
class MYPROJECT_API UEffectIcons : public UDataAsset
{
   GENERATED_BODY()

 public:
   UPROPERTY(EditAnywhere)
   TMap<FGameplayTag, UTexture2D*> effectIconMap;
};

UCLASS()
class MYPROJECT_API UEffectStatusBar : public UUserWidget
{
   GENERATED_BODY()

 public:

   static const int maxNumDisplaySlots = 11; //Maximum number of effects that can be displayed... for now

   UPROPERTY(BlueprintReadOnly, Category = "References")
   AUserInput* CPCRef;

   void NativeConstruct() override;
   void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

   /**Add an effect to be shown in the viewport*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Effect Data")
   void ToggleGameplayEffectIcon(FActiveGameplayEffectHandle activeEffectHandle, FGameplayTag effectName, UTexture2D* effectIcon, FLinearColor color, int index);

   /**Remove all the effects being shown currently from viewport*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void ResetShownEffects();

 private:

   UPROPERTY(EditAnywhere, Category = "Effect Data")
   UEffectIcons* effectIconDatabase;

   /**Stores all the active effects on the current focusedUnit*/
   UPROPERTY(BlueprintReadOnly, Category = "Skills", meta = (AllowPrivateAccess="true"))
   TArray<FActiveGameplayEffectHandle> effects; 

   const FGameplayTagContainer nameFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.EffectName"));
   const FGameplayTagContainer elemFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"));

   /**Updates the number of status effects displayed.  Called after status effect bar is displayed*/
   UFUNCTION(BlueprintCallable, Category = "Skills")
   void UpdateStatusBar();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Skills")
   bool ActiveEffectHandleEqualityCheck(const FActiveGameplayEffectHandle& handle1, const FActiveGameplayEffectHandle& handle2) const;
};
