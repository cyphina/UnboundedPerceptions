// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

/*
 * Class to manage level loading functionality
 */

UCLASS()
class MYPROJECT_API ALevelManager : public AActor
{
   GENERATED_BODY()

 public:
   ALevelManager();

 protected:
   virtual void BeginPlay() override;

 public:
   // Called every frame
   virtual void Tick(float DeltaTime) override;
};
