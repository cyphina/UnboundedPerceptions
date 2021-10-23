// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/RTSBrowserWidgetBase.h"
#include "RTSAttributeSet.h"
#include "RTSSidebarWidget.generated.h"

/**
 * Sidebar that shoes hero icons and hp. Is a browser widget
 */

class UBluEye;
class AUnit;
class AAlly;
class ABaseHero;

UCLASS()
class MYPROJECT_API URTSSidebarWidget : public URTSBrowserWidgetBase
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (AllowPrivateAccess = true))
   class AUserInput* cpcRef;

 public:
   UPROPERTY(EditDefaultsOnly)
   int width;

   UPROPERTY(EditDefaultsOnly)
   int height;

   void NativeOnInitialized() override final;
   void NativeConstruct() override;
   /** This updates the sidebar with information pertaining to all the heroes so it properly updates what hero is selected and the hp values and such by filling up an array
    * of JSON objects representing each hero.
    */
   UFUNCTION()
   void UpdatePartyInformation();

   UFUNCTION()
   void UpdateDeselectAllHeroes();

   /** Called when we toggle deselect on a single hero*/
   UFUNCTION()
   void UpdateHeroSelectionToggled(AAlly* deselectedHeroRef);

   /**
    * Used when a hero is selected. Handles if the hero is CTRL clicked or just regular selected.
    */
   UFUNCTION()
   void UpdateSingleHeroSelect(ABaseHero* selectedHero);

   /** Updates the hero's health current value when it senses a change*/
   UFUNCTION()
   void UpdateHeroHealth(const FGameplayAttribute& attributeModified, float newAttributeValue, AUnit* unitAffected);

   ///** Updates hero's mana value when it sees a change */
   UFUNCTION()
   void UpdateHeroMana(const FGameplayAttribute& attributeModified, float newAttributeValue, AUnit* unitAffected);

   UFUNCTION()
   void UpdateHeroVitals(const FGameplayAttribute& attributeModified, float newAttributeValue, AUnit* unitAffected);

   /** Updates hero's base health value when it sees a change (either changing base value or current value)*/
   UFUNCTION()
   void UpdateHeroMaxHealth(const FGameplayAttribute& attributeModified, float newAttributeValue, AUnit* unitAffected);

   /** Updates hero's base mana value when it sees a change (either changing base value or current value)*/
   UFUNCTION()
   void UpdateHeroMaxMana(const FGameplayAttribute& attributeModified, float newAttributeValue, AUnit* unitAffected);

   UFUNCTION()
   void UpdateHeroMaxVitals(const FGameplayAttribute& attributeModified, float newAttributeValue, AUnit* unitAffected);

 protected:
   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;
   FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;

   UFUNCTION()
   void HandleBluEvent(const FString& eventName, const FString& eventMessage);

 private:
   /** Creates a JSON string used to update the browser to tell it we selected a single hero
    * @param heroRef - A hero unit aliased by an AAlly pointer
    */
   FString MakeSingleHeroSelectedJson(const AUnit* heroRef) const;

   /** Creates a JSON string used to update the browser new vital information
    * @param heroRef - A hero unit aliased by an AAlly pointer
    */
   FString MakeHeroVitalJson(const AUnit* heroRef, const FString& vitalKey, double vitalValue) const;

   /** Handles to events we are subscribed to that should be unregistered when we change party members*/
   TArray<TPair<FDelegateHandle, FDelegateHandle>, TFixedAllocator<8>> subscribedAttributeSetDelegateHandles;

   FTimerHandle initialSetupTimer;
};
