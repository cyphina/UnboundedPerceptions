// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "NamedInteractableDecorator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNamedInteractableInteracted);

/**
 * Base clase for interactables that are actors placed in the world for the sole purpose of being interacted with
 */

UCLASS()
class MYPROJECT_API UNamedInteractableDecorator : public UInteractableActorDecoratorBase
{
	GENERATED_BODY()

	/**Used to identify interactables in quests*/
	UPROPERTY(EditAnywhere, Category = "Interactable Settings")
	FText									name;

	/**Wrapper function that calls quest manager update when we interact with this object*/
	UFUNCTION()
	void									OnInteractedQuestManagerWrapper(); 

public:	
	void									Init() override;
	bool									Interact() override;				

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FText						GetName() override { return name; }

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
	FOnNamedInteractableInteracted			OnInteracted;

};
