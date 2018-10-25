// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableBase.h"
#include "GameFramework/Actor.h"
#include "SavePoint.generated.h"

class UInteractableActorDecoratorBase;

UCLASS()
class MYPROJECT_API ASavePoint : public AInteractableBase
{
   GENERATED_BODY()

 public:
   // Sets default values for this actor's properties
   ASavePoint();

   UPROPERTY(EditAnywhere)
   UInteractableActorDecoratorBase* decoratedInteractable;

   virtual void Interact_Implementation(ABaseHero* hero) override;

 protected:
   virtual void BeginPlay() override;

 public:
   virtual void Tick(float DeltaTime) override;
};
