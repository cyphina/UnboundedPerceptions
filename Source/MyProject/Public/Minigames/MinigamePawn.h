// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MinigameTypes.h"
#include "MinigamePawn.generated.h"

UCLASS()
class MYPROJECT_API AMinigamePawn : public APawn
{
   GENERATED_BODY()

 public:
   // Sets default values for this pawn's properties
   AMinigamePawn();

   /**StartMinigame should probably be called when the pawn is possessed*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Minigame Event")
   void StartMinigame();

   /**StartMinigame should probably be called when the game COMPLETELY FINISHES (button press, timed event, etc.).
   *Some minigames may have multiple levels which should be handled inside the pawn.  When complete done, we check to see if the objectives are met.  
   *Also cleans up after the minigame, so call even if we're aborting.*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Minigame Event")
   bool EndMinigame();

   UFUNCTION(BlueprintCallable, BlueprintPure)
   const FMinigameData& GetMinigameData() const { return mData; }

 protected:
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;
   virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
   virtual void PossessedBy(AController* newController) override;

   /**Data used to decide what to do when the minigame completes and how to construct this instance of the game*/
   UPROPERTY(EditDefaultsOnly)
   FMinigameData mData;
};
