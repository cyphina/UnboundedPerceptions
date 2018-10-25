// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "EventSystem/Trigger.h"
#include "EventSystem/RTSConditional.h"
#include "TriggerInteractableDecorator.generated.h"

class UMyGameInstance;
class ARTSGameMode;

/** Decorates an InteractableBase to make an object that activates a trigger when interacted with */

UCLASS()
class MYPROJECT_API UTriggerInteractableDecorator : public UInteractableActorDecoratorBase
{
   GENERATED_BODY()

   ARTSGameMode* gameModeRef;

 public:
   UTriggerInteractableDecorator();

   void Init() override;
   bool Interact() override;

   /**Set this trigger to something when you want to interact*/
   UPROPERTY(EditAnywhere, Category = "Trigger")
   TArray<FTriggerData> triggersActivatedOnInteract;
};
