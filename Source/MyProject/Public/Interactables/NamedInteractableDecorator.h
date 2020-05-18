// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "NamedInteractableDecorator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNamedInteractableInteracted);

/**
 * Decorator that allows us to give a name property to an interactable
 */

UCLASS()
class MYPROJECT_API UNamedInteractableDecorator : public UInteractableActorDecoratorBase
{
   GENERATED_BODY()

   /**Used to identify interactables in quests*/
   UPROPERTY(EditAnywhere, Category = "Interactable Settings")
   FText name;

   /**Wrapper function that calls quest manager update when we interact with this object*/
   UFUNCTION()
   void OnInteractedQuestManagerWrapper();

 public:
   void Init() override;
   bool Interact() override;

   UFUNCTION(BlueprintCallable, BlueprintPure)
   FORCEINLINE FText GetName() const override { return name; }

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnNamedInteractableInteracted OnInteracted;
};
