// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "SavePoint.generated.h"

UCLASS()
class MYPROJECT_API ASavePoint : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASavePoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void Interact(ABaseHero* hero);
		virtual void Interact_Implementation(ABaseHero* hero) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		FVector GetInteractableLocation();
		virtual FVector GetInteractableLocation_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
