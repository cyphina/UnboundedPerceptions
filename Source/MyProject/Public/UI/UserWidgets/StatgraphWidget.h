// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/RTSBrowserWidgetBase.h"
#include "MyAttributeSet.h"
#include "StatgraphWidget.generated.h"

/**
 * Widget that displays our stats in graph format
 */

class AUnit;

UCLASS()
class MYPROJECT_API UStatgraphWidget : public URTSBrowserWidgetBase
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Category = "References", Meta = (AllowPrivateAccess="true"))
   class AUserInput* cpcRef;

 protected:
   void NativeConstruct() override final;

   UPROPERTY(EditDefaultsOnly)
   int width;

   UPROPERTY(EditDefaultsOnly)
   int height;

   // Listens to get updates when the hero whose character menu is currently displayed has their stat changed. First value is for adj val updates, second value is for base val updates
   TPair<FDelegateHandle, FDelegateHandle> statListeners;

 public:
   /**
    * Sends data to the browser relating to elemental affinities
    */
   UFUNCTION(BlueprintCallable)
   void ShowElementalOffensive();

   /**
    * Sends data to the browser relating to elemental defense
    */
   UFUNCTION(BlueprintCallable)
   void ShowElementalDefense();

   /**
    * Sends data to the browser relating to mechanics
    */
   UFUNCTION(BlueprintCallable)
   void ShowMechanics();

   /**
    * Sends data to the browser relating to vitals
    */
   UFUNCTION(BlueprintCallable)
   void ShowVitals();

   /**
    * Changes the active hero to show information for. Called when the character menu initially opens. Currently not used since I removed the title to save space
    */
   UFUNCTION(BlueprintCallable)
   void SwapHero();

    /**
    * Call this to display browser when widget starts up and to setup listener to hero stats
    */
    UFUNCTION(BlueprintCallable)
   void Startup();

   /**
    * Call this to unregister listener to hero when the widget is closed
    */
   UFUNCTION(BlueprintCallable)
   void Cleanup();
 private:
   /**
    * Sends data to the browser to update one of the stats. If the stat is on screen, it will be updated
    */
   void UpdateStat(const FGameplayAttribute& attributeModified, float& newAttributeValue, AUnit* unitAffected);

    /**
    * Sends data to the browser to update one of the base stats. If the stat is on screen, it will be updated
    */
   void UpdateBaseStat(const FGameplayAttribute& attributeModified, float& newAttributeValue, AUnit* unitAffected);

   /**
    * Helper function for Updating and sending Stats to Browser
    */
   void CreateAndSendStatUpdate(const FGameplayAttribute& attributeModified, float& newAttributeValue, AUnit* unitAffected, const FString& keyName);

   FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override final;

   /**
    * A check to see if the focused unit is a hero. In theory this UI will only be open if a hero is focused but ya never know
    */
   FORCEINLINE const AUnit* CheckIfFocusedUnitHero() const;
};
