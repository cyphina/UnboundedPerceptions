// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableBase.generated.h"

class UInteractableActorDecoratorBase;

UCLASS()
class MYPROJECT_API AInteractableBase : public AActor, public IInteractable
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
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void 					Interact(ABaseHero* hero);
	void					Interact_Implementation(ABaseHero* hero) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	FVector 				GetInteractableLocation();
	FVector					GetInteractableLocation_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	bool 					CanInteract();
	bool 					CanInteract_Implementation() override;

};
