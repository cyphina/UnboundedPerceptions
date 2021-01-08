// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

class ABaseHero;

/**
 * Controller for more active NPCs.  Some NPCs just stand around and talk, but NPCs with this controller are moving about, and maybe following you for some reasons
 */
UCLASS()
class MYPROJECT_API ANPCAIController : public AAIController
{
   GENERATED_BODY()

 public:
   ANPCAIController();

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Follow(const ABaseHero* heroToFollow);

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Patrol();

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Stop();

 protected:
   void OnPossess(APawn* inPawn) override;
   void BeginPlay() override;

 private:
   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBehaviorTree* followTree;

   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBehaviorTree* patrolTree;

   UPROPERTY(VisibleAnywhere, Category = "AIData")
   UBlackboardComponent* blackboardComp;

   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBlackboardData* npcBB;

   static const FName targetKeyName;
};
