// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "WorldObjects/WorldObject.h"
#include "InteractableBase.generated.h"

class UInteractableActorDecoratorBase;

/**To name an interactable, add a named decorator to it.  Else there will be no name*/
UCLASS()
class MYPROJECT_API AInteractableBase : public AActor, public IInteractable, public IWorldObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UInteractableActorDecoratorBase* decorator;

public:	

	AInteractableBase();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FORCEINLINE FText		GetGameName() const final override;

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	void					SetGameName(FText value) final override;

	void 					Interact_Implementation(ABaseHero* hero) override;
	FVector 				GetInteractableLocation_Implementation() override;
	bool 					CanInteract_Implementation() override;

};
