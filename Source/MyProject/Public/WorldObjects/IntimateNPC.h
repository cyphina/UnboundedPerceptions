// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventSystem/Trigger.h"
#include "GameplayTagContainer.h"
#include "WorldObjects/NPC.h"
#include "IntimateNPC.generated.h"

/**
 * NPC class for social links
 */

class ARTSGameMode;
struct FNPCIntimateSaveInfo;

UCLASS()
class MYPROJECT_API AIntimateNPC : public ANPC
{
   GENERATED_BODY()

   int           relationshipPoints            = 0;
   int           currentRelationshipEventIndex = 0;
   ARTSGameMode* gameModeRef                   = nullptr;

   /** Point values at which events occur*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   TArray<int> relationshipEventPointValues;

   /** Triggers to activate special effects when the relationshipEventPointValues are met
    * May require a special trigger struct specifically for this.
    */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   TArray<FTriggerData> relationshipTriggers;

 public:
   AIntimateNPC();
   void BeginPlay() override;

   UFUNCTION(BlueprintCallable, Category = "Relationships")
   void SetRelationshipPoints(int pointsToAdd);

   UFUNCTION(BlueprintCallable, Category = "Relationships")
   int GetRelationshipPoints() const { return relationshipPoints; }

   virtual void Interact_Implementation(ABaseHero* hero) override;

   /**
    *Sets up the dialogue UI to the proper state (conversation/intimate view) after interacting with this NPC
    */
   void            SetupAppropriateView() override;
   virtual FVector GetInteractableLocation_Implementation(ABaseHero* hero) override { return Super::GetInteractableLocation_Implementation(hero); }

   void SaveNPCData(FMapSaveInfo& mapInfo) override;
   void LoadNPCData(FNPCIntimateSaveInfo& npcSaveInfo);
};
