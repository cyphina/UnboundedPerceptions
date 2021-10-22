// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "EventSystem/DEPRECATED_RTSConditional.h"
#include "TriggerInteractableDecorator.generated.h"

class UMyGameInstance;
class ARTSGameMode;
class URTSTriggerBase;

/** Decorates an InteractableBase to make an object that activates trigger(s) when interacted with */

UCLASS()
class MYPROJECT_API UTriggerInteractableDecorator : public UInteractableActorDecoratorBase
{
   GENERATED_BODY()

 public:
   UTriggerInteractableDecorator();

   void Init() override;
   bool Interact() override;

   /**Set this trigger to something when you want to interact*/
   UPROPERTY(EditAnywhere, Instanced, Category = "Trigger")
   TArray<URTSTriggerBase*> triggersActivatedOnInteract;
};
