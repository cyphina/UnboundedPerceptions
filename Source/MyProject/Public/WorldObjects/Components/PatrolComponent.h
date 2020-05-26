// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "PatrolComponent.generated.h"

class AAIController;
class UBehaviorTree;

/**Allows us to easily set patrol points for enemy and npc's because it is linked to a visualization
 * AIs that use the patrol task rely on the AIs having a PatrolComponent
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UPatrolComponent : public USceneComponent
{
   GENERATED_BODY()

   AAIController* ownerControllerRef;

 public:
   UPatrolComponent();

 protected:
   virtual void BeginPlay() override final;

 public:

   EPathFollowingRequestResult::Type moveRes;

   /**Patrolling is a behavior since it can be more complex than a single task.  Used in UnitController*/
   UPROPERTY(EditAnywhere)
   UBehaviorTree* patrolTree;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPCMovement")
   bool enabled;

   /**List of NPC patrol points.  NPC will go to each point in order and loop.*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPCMovement")
   TArray<FVector> patrolPoints;

   int currentPatrolIndex = -1;

   // Used in the patrol points visualization
   FLinearColor editorUnselectedColor = FLinearColor::Black;
   FLinearColor editorSelectedColor   = FLinearColor::Blue;

   UFUNCTION()
   void DeletePatrolPoint(int patrolIndex);

   /** Called in Patrol task to move unit to next point*/
   TPair<EPathFollowingRequestResult::Type, FAIRequestID> MoveToNextPatrolPoint();

   UFUNCTION(BlueprintCallable)
   FORCEINLINE FVector GetCurrentPatrolPoint() { return patrolPoints [ currentPatrolIndex ]; }
};
