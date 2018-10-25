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

/**To name an interactable, add a named decorator to it.  Else there will be no name*/

UCLASS()
class MYPROJECT_API AInteractableBase : public AActor, public IInteractable, public IWorldObject
{
   GENERATED_BODY()

 public:
   AInteractableBase();

 protected:
   /**An interactable decorator which extends the functionality of interactables*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
   UInteractableActorDecoratorBase* decorator;

   virtual void BeginPlay() override;

   /**Used to save the ncessary data for all interactable actors on the map*/
   FInteractableSaveInfo SaveInteractableData();

   /**Used to load the necessary data for all interactable actors on the map*/
   void LoadInteractableData(FInteractableSaveInfo& interactableInfo);

 public:
   /*Scene root so that we can transform child components without having to worry about its parent's transformations*/
   UPROPERTY(VisibleAnywhere)
   USceneComponent* sceneRoot;

   /*Scene component represents the location a character must arrive to to interact with the interactable*/
   UPROPERTY(VisibleAnywhere)
   USceneComponent* sceneLocation;

   /*Mesh of the interactable (collision and visual).  Don't put on top of scene component or else the character can never reach the interactable destination*/
   UPROPERTY(VisibleAnywhere)
   UStaticMeshComponent* interactableMesh;

   virtual void Tick(float DeltaTime) override;

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   FORCEINLINE FText GetGameName() const final override;

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void SetGameName(FText value) final override;

   void    Interact_Implementation(ABaseHero* hero) override;
   FVector GetInteractableLocation_Implementation(ABaseHero* hero) override;
   bool    CanInteract_Implementation() override;

   virtual void SaveInteractable(FMapSaveInfo& mapData);
   void         LoadInteractable(FInteractableSaveInfo& interactableInfo);
};
