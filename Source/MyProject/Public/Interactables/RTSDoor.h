#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/InteractableBase.h"
#include "EventSystem/RTSConditional.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "RTSDoor.generated.h"

struct FDoorInteractableSaveInfo;
class ARTSGameMode;
class AUserInput;

/**Door that can be opened or closed.  Can act as a gate if it's surrounded by walls.*/

UCLASS(HideCategories = (Input, Actor, LOD))
class MYPROJECT_API ARTSDoor : public AInteractableBase
{
	GENERATED_BODY()
	
	FTimeline				tL;
	
	/**Is this door open or closed?*/
	bool					isOpen = false;

	/**Store initial rotation for when door should go to closed state*/
	FRotator				initialRotation;

	/**Store this value so we don't have to keep calculating a temporary*/
	FRotator				finalRotation;

	UFUNCTION()
	void					HandleProgress(float Value);

public:	

	/**Curve representing door rotation rate*/
	UPROPERTY(EditAnywhere, Category = "Door Info")
	UCurveFloat*					progressCurve;
	
	/**Root component*/
	UPROPERTY(EditAnywhere)
	USceneComponent*				scene;

	/*Mesh of the door itself*/
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*			doorMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent*					doorCollision;

	/**Is this door locked? (needs a key to open)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Info")
	bool					isLocked = false;

	/**Id of the key to open this door if it is locked*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Info")
	int					keyID = 0;

	ARTSDoor();

	void					BeginPlay() override;
	void					Tick(float DeltaTime) override;

	void					Interact_Implementation(ABaseHero* hero) override;
	FVector					GetInteractableLocation_Implementation() override;
	bool					CanInteract_Implementation() override;

	void					SaveInteractable(FMapSaveInfo& mapData) override;
	void					LoadInteractable(FDoorInteractableSaveInfo& doorData);
};
