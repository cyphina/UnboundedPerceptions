// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableBase.h"
#include "GameFramework/Actor.h"
#include "Items/Item.h"
#include "Pickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickupDelegate); //event dispatcher for when picking up a pickup

class AUserInput;

UCLASS()
class MYPROJECT_API APickup : public AInteractableBase
{
	GENERATED_BODY()

public:

		APickup();
		
		virtual void 				BeginPlay() override;
		virtual void 				Tick(float deltaSeconds) override;
		
		/**Still need this to override interactable implementation in BP*/
		UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		void 						Interact(ABaseHero* hero);
		void 						Interact_Implementation(ABaseHero* hero) override;

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
		FVector 					GetInteractableLocation();
		FVector 					GetInteractableLocation_Implementation() override;

		UFUNCTION(BlueprintCallable, Category = "Interact")
		void						OnPickedUp();

		///---Properties---
		/*Assume all pickups have a static mesh, also making things have properties besides visibleanywhere makes them not editable in blueprints window for some odd reason*/
		UPROPERTY(VisibleAnywhere)
		USceneComponent* 			sceneComponent;

		/*Mesh of the pickup*/
		UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* 		interactableMesh;

		/*Item that will be picked up*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FMyItem 					item;

		UPROPERTY(BlueprintReadOnly, Category = "Item")
		AUserInput* 				CPCRef;

		/*Event dispatcher to be called whenever picked up*/
		UPROPERTY(BlueprintAssignable, Category = "Pickup")
		FPickupDelegate 			OnPickupDelegate;

		void						OnComponentBeginOverlap(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
		void						SaveInteractable(FMapSaveInfo& mapData) override;
};
