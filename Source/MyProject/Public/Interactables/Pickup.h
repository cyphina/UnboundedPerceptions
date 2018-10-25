// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractableBase.h"
#include "Items/Item.h"
#include "Pickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPickupDelegate); // event dispatcher for when picking up a pickup

class AUserInput;

UCLASS()
class MYPROJECT_API APickup : public AInteractableBase
{
   GENERATED_BODY()

 public:
   APickup();

   virtual void BeginPlay() override;
   virtual void Tick(float deltaSeconds) override;

   void    Interact_Implementation(ABaseHero* hero) override;
   FVector GetInteractableLocation_Implementation(ABaseHero* hero) override;

   UFUNCTION(BlueprintCallable, Category = "Interact")
   void OnPickedUp();

   /*Item that will be picked up*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
   FMyItem item;

   UPROPERTY(BlueprintReadOnly, Category = "Item")
   AUserInput* CPCRef;

   /*Event dispatcher to be called whenever picked up*/
   UPROPERTY(BlueprintAssignable, Category = "Pickup")
   FPickupDelegate OnPickupDelegate;

   void OnComponentBeginOverlap(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
   void SaveInteractable(FMapSaveInfo& mapData) override;
};
