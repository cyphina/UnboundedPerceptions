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
class MYPROJECT_API ANPCAIController : public AAIController {
   GENERATED_BODY()

public:

   ANPCAIController();
   void OnPossess(APawn* inPawn) override;
   void BeginPlay() override;

private:

   static const FName targetKeyName;

   UPROPERTY(VisibleAnywhere, Category = "AIData")
   UBlackboardComponent* blackboardComp;

   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBlackboardData* npcBB;

#pragma region actions

public:
   ///<summary>
   ///Actions call upon behavior tree logic rather than trying to write out logic in code
   ///</summary>

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Follow(ABaseHero* heroToFollow);

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Patrol();

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Stop();

private:
   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBehaviorTree* followTree;

   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBehaviorTree* patrolTree;
#pragma endregion 
};
