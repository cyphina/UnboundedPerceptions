// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

   virtual void BeginPlay() override final;
   virtual void Tick(float deltaSeconds) override final;

   void    Interact_Implementation(ABaseHero* hero) override final;
   FVector GetInteractableLocation_Implementation() const override final;

   /* Item that will be picked up */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
   FMyItem item;

   /*Event dispatcher to be called whenever picked up*/
   UPROPERTY(BlueprintAssignable, Category = "Pickup")
   FPickupDelegate OnPickupDelegate;

   void OnComponentBeginOverlap(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep,
                                const FHitResult& sweepResult);
  // Just adds the name of this pick up (one editor gives actors by default) to show it hasn't been picked up
   void SaveInteractable(FMapSaveInfo& mapData) override final;
   void LoadInteractable(FMapSaveInfo& mapData) override final;
};
