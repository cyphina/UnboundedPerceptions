// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "WorldObjects/WorldObject.h"
#include "InteractableBase.generated.h"

class ABaseHero;
class UInteractableActorDecoratorBase;
struct FMapSaveInfo;
struct FInteractableSaveInfo;
struct FInteractableSaveInfoWrapper;

/**To name an interactable, add a named decorator to it.  Else there will be no name*/

UCLASS()
class MYPROJECT_API AInteractableBase : public AActor, public IInteractable, public IWorldObject
{
   GENERATED_BODY()

public:
   AInteractableBase();

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   FORCEINLINE FText GetGameName() const final override;

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void SetGameName(FText value) final override;

   void    Interact_Implementation(ABaseHero* hero) override;
   FVector GetInteractableLocation_Implementation() const override;
   bool    CanInteract_Implementation() const override;

   virtual void SaveInteractable(FMapSaveInfo& mapData); // Saves data about interactable on a map when transitioning to another level or when game is saving
   virtual void LoadInteractable
   (FMapSaveInfo& mapData); // Loads data about an interactable when transitioning to the map with the interactable.  Specific implementation in every subclass

protected:
   virtual void Tick(float DeltaTime) override;
   void         BeginPlay() override;

   /**An interactable decorator which extends the functionality of interactables*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
   UInteractableActorDecoratorBase* decorator;

   /*Scene root so that we can transform child components without having to worry about its parent's transformations*/
   UPROPERTY(VisibleAnywhere)
   USceneComponent* sceneRoot;

   /*Scene component represents the location a character must arrive to to interact with the interactable*/
   UPROPERTY(VisibleAnywhere)
   USceneComponent* sceneLocation;

   /*Mesh of the interactable (collision and visual).  Don't put on top of scene component or else the character can never reach the interactable destination*/
   UPROPERTY(VisibleAnywhere)
   UStaticMeshComponent* interactableMesh;

   /**Used internally to save the necessary data for all interactable actors on the map which has to be done when leaving a level*/
   FInteractableSaveInfoWrapper SaveInteractableData();

   /**Used internally to load the necessary data for all interactable actors on the map.  Used when restoring a level's state*/
   void LoadInteractableData(FInteractableSaveInfo& interactableInfo);
};

FORCEINLINE uint8 GetTypeHash(const AInteractableBase& interactable)
{
   return GetTypeHash(interactable.GetActorLocation());
}
