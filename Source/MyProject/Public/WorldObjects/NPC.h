 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "WorldObject.h"
#include "Interactables/Interactable.h"
#include "GameplayTagContainer.h"
#include "EventSystem/Trigger.h"
#include "NPC.generated.h"

class ABaseHero;
class AUserInput;

//NPC Base Class for people we might be able to talk to
UCLASS()
class MYPROJECT_API ANPC : public ACharacter, public IWorldObject, public IInteractable
{
	GENERATED_BODY()
		               
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
	FText 					name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
	UTexture2D* 			image = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
	bool 					canTarget = true; 

	/** Does this NPC allow us to have a conversation (open up social menu) or does he/she just say something then walk away*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
	bool					bWantsToConverse = false; 

	/** Exposed name of default conversation to be quickly set in object properties.  This conversation is whatever they NPC will tell you before your 
	 * conversation digresses, so it may change when new events occur.  Rather than caching dialog, loading it form our table is fine
	 * because our table is a map (constant time searches) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
	FName 					conversationStarterName;
	/** Name of the conversation the NPC will have if it doesn't know the topic asked about */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
	FName 					defaultResponseName;
	/*Maps DialogTopics to names in the dialogTable*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
	TMap<FGameplayTag, FName>	conversationTopics;

	FTriggerData				onDialogEndTriggerData;

protected:
	AUserInput*					controllerRef;

public:
	ANPC();

	void 						BeginPlay() override;
	virtual void 				Tick(float DeltaTime) override;	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPCMovement")
	bool						doesPatrol;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NPCMovement")
	TArray<FVector>				patrolPoints;
	UPROPERTY(BlueprintReadWrite, Category = "NPCMovement")
	int							currentPatrolIndex;

#pragma region informationAccess

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	virtual void 				SetGameName(FText value) override { name = value; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	virtual FText 				GetGameName() const override { return name; };
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	virtual void 				SetImage(UTexture2D* value) override { image = value; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	virtual UTexture2D* 		GetImage() const override { return image; };

	UFUNCTION(BlueprintCallable, Category = "Accessors")
	virtual void 				SetSelected(bool value) override { isSelected = value; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	virtual bool 				GetSelected() const override { return isSelected; };
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	virtual void 				SetObjectID(int value) override { objectID = value; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	virtual int 				GetObjectID() const override { return objectID; };
	UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
	virtual void 				SetCanTarget(bool value) override { canTarget = value; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
	virtual bool 				GetCanTarget() const override { return canTarget; }

	/**Get the dialog key to lookup in dialog table for what the NPC will say when we begin to talk to it*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
	FName						GetCurrentDialog() const { return conversationStarterName; }
	/**Set the dialog key to lookup in table for what the NPC will say when we begin talking to it*/
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	virtual void 				SetCurrentDialog(FName conversationName) { conversationStarterName = conversationName; }
	/**Sets the dialog key to lookup in the table what the NPC will say when we talk to it about a specific topic*/
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	virtual void 				SetSpecificDialog(FName key, FName conversationName) { conversationTopics[FGameplayTag::RequestGameplayTag(key)] = conversationName; }
	/** Used when trying to talk to NPC about some topic.  If NPC knows nothing about topic, return defaultConversationName */
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FName						GetConversationName(FGameplayTag conversationTopic) const;
	/**Accessor to get default response when asked about a topic not known*/
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FName						GetDefaultResponseName() const { return defaultResponseName; }
	/**Accessor to grab response when first talked to*/
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FName						GetStartingConversationName() const { return conversationStarterName; }
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	bool						GetWantsToConverse() const { return bWantsToConverse; }
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	FTriggerData&				GetOnDialogFinishedTrigger() { return onDialogEndTriggerData; }
	
	UFUNCTION(BlueprintCallable, Category = "Accessors")
	int							GetCurrentPatrolIndex() const { return currentPatrolIndex; }
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "NPCMovement")
	void						MoveToCurrentPatrolPoint();
#pragma endregion
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void 						Interact(ABaseHero* hero);
	virtual void 				Interact_Implementation(ABaseHero* hero) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	FVector 					GetInteractableLocation();
	virtual FVector				GetInteractableLocation_Implementation() override;
};
