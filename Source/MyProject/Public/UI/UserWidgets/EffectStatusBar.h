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
   UPROPERTY(BlueprintReadOnly, Category = "References")
   AUserInput* CPCRef;

   void NativeConstruct() override;
   void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

   /**Updates the number of status effects displayed*/
   UFUNCTION(BlueprintCallable, Category = "Skills")
   void UpdateStatusBar();

   /**Add an effect to be shown in the viewport*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Effect Data")
   void AddGameplayEffectIcon(FActiveGameplayEffectHandle activeEffectHandle, FGameplayTag effectName, UTexture2D* effectIcon, FLinearColor color);

   /**Remove all the effects being shown currently from viewport*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void ResetShownEffects();

 private:
   UPROPERTY(EditAnywhere, Category = "Effect Data")
   UEffectIcons* effectIconDatabase;
   UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"), Category = "Effect Data")
   FGameplayTagContainer currentlyStoredEffectsList;

   TArray<FActiveGameplayEffectHandle> effects; // array used for temporary storage of all effects applied to user

   const FGameplayTagContainer nameFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.EffectName"));
   const FGameplayTagContainer elemFilter = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Element"));
};
