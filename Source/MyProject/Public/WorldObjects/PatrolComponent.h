// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "PatrolComponent.generated.h"

class AAIController;

/**Allows us to easily set patrol points for enemy and npc*/

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UPatrolComponent : public USceneComponent
{
   GENERATED_BODY()

   AAIController* ownerControllerRef;

   UFUNCTION()
   void OnMoveCompleted(FAIRequestID requestID, EPathFollowingResult::Type result);

 public:
   UPatrolComponent();

 protected:
   virtual void BeginPlay() override;

 public:
   virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPCMovement")
   bool enabled;

   /**List of NPC patrol points.  NPC will go to each point in order and loop.*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPCMovement")
   TArray<FVector> patrolPoints;

   int currentPatrolIndex = 0;

   FLinearColor editorUnselectedColor = FLinearColor::Black;
   FLinearColor editorSelectedColor   = FLinearColor::Blue;

   UFUNCTION()
   void DeletePatrolPoint(int patrolIndex);

   UFUNCTION(BlueprintCallable)
   void MoveToNextPatrolPoint();
};
