#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/InteractableBase.h"
#include "EventSystem/RTSConditional.h"
#include "RTSDoor.generated.h"

class ARTSGameMode;
class AUserInput;

/**Door that can be opened or closed.  Can act as a gate if it's surrounded by walls.*/

UCLASS()
class MYPROJECT_API ARTSDoor : public AInteractableBase
{
	GENERATED_BODY()
	
	FTimeline				tL;
	
	/**Is this door open or closed?*/
	bool					isOpen = false;

	/**Store initial rotation for when door should go to closed state*/
	FRotator		
	initialRotation;
	/**Store this value so we don't have to keep calculating a temporary*/
	FRotator				finalRotation;

	UFUNCTION()
	void					HandleProgress(float Value);

public:	

	/**Curve representing door rotation rate*/
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat*					progressCurve;
	
	/**Root component*/
	UPROPERTY(EditAnywhere)
	USceneComponent*				scene;

	/*Mesh of the door itself*/
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent*			doorMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent*					doorCollision;

	ARTSDoor();

	void					BeginPlay() override;
	void					Tick(float DeltaTime) override;

	void					Interact_Implementation(ABaseHero* hero) override;

	FVector					GetInteractableLocation_Implementation() override;
};
