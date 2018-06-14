// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/Interactable.h"
#include "EventSystem/RTSConditional.h"
#include "RTSDoor.generated.h"

class ARTSGameMode;
class AUserInput;

UCLASS()
class MYPROJECT_API ARTSDoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
	FTimeline				tL;
	
	/**Is this door open or closed?*/
	bool					isOpen = false;

	/**Store initial rotation for when door should go to closed state*/
	FRotator				initialRotation;
	/**Store this value so we don't have to keep calculating a temporary*/
	FRotator				finalRotation;

	ARTSGameMode*			gameModeRef;
	AUserInput*				cpcRef;

	UFUNCTION()
	void					HandleProgress(float Value);

public:	

	/**Curve representing door rotation rate*/
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat*					progressCurve;

	UPROPERTY(EditAnywhere)
	USceneComponent*				scene;

	/*Mesh of the door itself*/
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*			doorMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent*					doorCollision;

	/**List of conditional requirements to open*/
	UPROPERTY(EditAnywhere)
	TArray<FConditionData>			useConditions;

	ARTSDoor();

	void					BeginPlay() override;
	void					Tick(float DeltaTime) override;

	void					Interact_Implementation(ABaseHero* hero) override;

	FVector					GetInteractableLocation_Implementation() override;

	bool 					CanInteract_Implementation() override;
};
