// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalActor.generated.h"

UCLASS()
class MYPROJECT_API AGoalActor : public AActor
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Goal Circle Details", Meta = (AllowPrivateAccess = true))
   bool UseRadius;

   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Goal Circle Details", Meta = (AllowPrivateAccess = true))
   float radius;

   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Goal Circle Details", Meta = (AllowPrivateAccess = true))
   FLinearColor color;

 protected:
   virtual void BeginPlay() override;

 public:
   AGoalActor();
};
