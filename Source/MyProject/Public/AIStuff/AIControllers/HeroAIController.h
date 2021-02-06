// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStuff/AIControllers/AllyAIController.h"
#include "HeroAIController.generated.h"

class ABaseHero;

/**
 * Controller class for heroes contains tasks specifically heroes can perform
 */
UCLASS()
class MYPROJECT_API AHeroAIController : public AAllyAIController
{
   GENERATED_BODY()

 public:
   /** Called when player clicks on an interactable */
   UFUNCTION(BlueprintCallable, Category = "Misc")
   void BeginInteract(AActor* interactable);

   /** Called when the player clicks on one of the items to use */
   UFUNCTION(BlueprintCallable, Category = "Items")
   void BeginUseItem(int itemToUseID, int slotIndex);

   void StopCurrentAction() override final;

 protected:
   virtual void OnPossess(APawn* InPawn) override final;

 private:
   UPROPERTY()
   ABaseHero* heroRef;
};
