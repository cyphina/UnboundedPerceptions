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
<<<<<<< HEAD
   void Follow(const ABaseHero* heroToFollow);
=======
   void Follow(const AActor* actorToFollow);
>>>>>>> componentrefactor

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Patrol();

   UFUNCTION(BlueprintCallable, Category = "NPCActions")
   void Stop();

 protected:
   void OnPossess(APawn* inPawn) override;
   void BeginPlay() override;

 private:
<<<<<<< HEAD
   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBehaviorTree* followTree;

   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBehaviorTree* patrolTree;

   UPROPERTY(VisibleAnywhere, Category = "AIData")
   UBlackboardComponent* blackboardComp;

   UPROPERTY(EditDefaultsOnly, Category = "AIData")
   UBlackboardData* npcBB;

=======
   const AActor* followTarget;
   FTimerHandle followLoopTimer;

>>>>>>> componentrefactor
   static const FName targetKeyName;
};
