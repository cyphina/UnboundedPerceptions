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
class MYPROJECT_API AHeroAIController : public AAllyAIController {
   GENERATED_BODY()

   UPROPERTY()
   ABaseHero* heroRef;

public:
   virtual void OnPossess(APawn* InPawn) override final;

   /** Interact with some interactable type object in the world*/
   UFUNCTION(BlueprintCallable, Category = "Misc")
   void BeginInteract(AActor* interactor);

   /** Use an item on something */
   UFUNCTION(BlueprintCallable, Category = "Items")
   void BeginUseItem(int itemToUseID);

   void Stop() override final;
};
