#pragma once

#include "AIController.h"
#include "NPCAIController.generated.h"

class ABaseHero;

/**
 * NPC's that do more than just stand still need an NPCAIController.
 * This gives them the options of patrolling around or following the player.
 */
UCLASS()
class MYPROJECT_API ANPCAIController : public AAIController
{
   GENERATED_BODY()

 public:
   ANPCAIController();

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Follow(const AActor* actorToFollow);

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Patrol();

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Stop();

 protected:
   void OnPossess(APawn* inPawn) override;
   void BeginPlay() override;

 private:
   const AActor* followTarget;
   FTimerHandle followLoopTimer;

   static const FName targetKeyName;
};
