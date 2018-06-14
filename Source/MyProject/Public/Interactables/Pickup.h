// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Items/Item.h"
#include "Pickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickupDelegate); //event dispatcher for when picking up a pickup

class AUserInput;

UCLASS()
class MYPROJECT_API APickup : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
		//Functions
		APickup();
		
		virtual void 				BeginPlay() override;
		virtual void 				Tick(float deltaSeconds) override;
		
		//Override Interactable Implementation
		UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void 						Interact(ABaseHero* hero);
		void 						Interact_Implementation(ABaseHero* hero) override;

		//Override Interactable Location
		UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		FVector 					GetInteractableLocation();
		FVector 					GetInteractableLocation_Implementation() override;

		/*Called when item is picked up and needs to be cleaned up from the level and added to inventory*/
		UFUNCTION(BlueprintCallable, Category = "Interact")
		void						OnPickedUp();

		/**Can this interactable be used?*/
		bool 						CanInteract_Implementation() override;
		//Properties
		/*assume all pickups have a static mesh, also making things have properties besides visibleanywhere makes them not editable in blueprints window for some odd reason*/
		UPROPERTY(VisibleAnywhere)
		USceneComponent* 			sceneComponent;

		/*Mesh of the pickup*/
		UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* 		interactableMesh;

		/*Sphere collider to check if hero is in range or such*/
		UPROPERTY(VisibleAnywhere)
		USphereComponent* 			sphereCollider;

		/*Item that will be picked up*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		UMyItem* 					item;

		UPROPERTY(BlueprintReadOnly, Category = "Item")
		AUserInput* 				CPCRef;

		/*Event dispatcher to be called whenever picked up*/
		UPROPERTY(BlueprintAssignable, Category = "Pickup")
		FPickupDelegate 			OnPickupDelegate;

		void OnComponentBeginOverlap(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};
