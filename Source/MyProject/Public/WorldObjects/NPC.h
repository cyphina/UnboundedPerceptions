// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EventSystem/Trigger.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Interactables/Interactable.h"
#include "WorldObject.h"
#include "NPC.generated.h"

class ABaseHero;
class AUserInput;
class ANPCAIController;

struct FMapSaveInfo;
struct FNPCSaveInfo;

// NPC Base Class for people we might be able to talk to
UCLASS(HideCategories = (Rendering, Replication, Clothing, "Character Movement", Physics))
class MYPROJECT_API ANPC : public ACharacter, public IWorldObject, public IInteractable
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true))
   FText name;

   static class AHUDManager* hudManagerRef;

   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))
   ANPCAIController* npcAIController;

   /** Exposed name of default conversation to be quickly set in object properties.  This conversation is whatever they NPC will tell you before your
    * conversation digresses, so it may change when new events occur.  Rather than caching dialog, loading it form our table is fine
    * because our table is a map (constant time searches) */
   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
   FName conversationStarterName;

   /**
    *Name of the conversation the NPC will have if it doesn't know the topic asked about
    */
   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
   FName defaultResponseName = "Default";

   /**
    *Maps DialogTopics to names in the dialogTable. For names not in the map, the NPC will not have a detailed conversation when asked about that topic
    */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true), Meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
   TMap<FGameplayTag, FName> conversationTopics;

   FTriggerData onDialogEndTriggerData;

   /**
    *TODO: Move this to CPC and have a single map <NPCNAME,DialogAlreadyConversedSet> for global access
    * List of dialogs (not topics) that we've already talked to this NPC about.  Sometimes used in conditionals
    */
   TSet<FName> dialogAlreadyConversed;

   /**Number of available quests to partake currently
    * This can be determined by looking at the number of conversationTopics + defaultConversationTopics to see which topics start with Quest
    * since topics which start with Quest will have a quest trigger.  A quest trigger is always followed by a conversation change trigger which will
    * change the topic to a conversation that doesn't involve accepting a quest.
    */
   int numAvailableQuests;

   /**Counts how many dialogs we can have with this NPC currently that gives us quests*/
   void CountQuestDialogs();

   /**Checks if a dialog is a quest dialog by checking if its name starts with Quest*/
   bool IsQuestDialog(FName conversationName);

   /**Checks to see if this topic is already learned by the player*/
   inline bool IsTopicLearned(FGameplayTag topic);

   /**Bound to OnTopicLearned delegate in BasePlayer to check if the questCount changes*/
   UFUNCTION()
   void OnTopicLearned(FGameplayTag topicLearned);

   /**We need this if we're trying to debug a level in the editor.
    *We don't need this for the real game since they start at the starting level where things are initialized
    *If we don't have this we have a problem with the system that determines if an NPC has a quest to give*/
#if UE_EDITOR
   FTimerHandle BeginPlayDelayTimer;
#endif

 protected:
   AUserInput* controllerRef;
   void        MakeNPCData(FNPCSaveInfo& npcSaveInfo);
   void        ReloadNPCData(const FNPCSaveInfo& npcSaveInfo);

 public:
   ANPC();

   void         BeginPlay() override;
   virtual void Tick(float DeltaTime) override final;

   static void SetHUDManagerRef(AHUDManager* newHudManager) { hudManagerRef = newHudManager; }

   /**
    *Removed since we store this as the interactedHero in BasePlayer settings
    */
   // UPROPERTY(BlueprintReadWrite, Category = "NPCConversationSettings")
   // ABaseHero* currentlyTalkingHero = false;

   /**
    * Does this NPC allow us to have a conversation (open up social menu) or does he/she just say something then walk away
    */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "NPCConversationSettings")
   bool bWantsToConverse = false;

#pragma region informationAccess

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   virtual void SetGameName(FText value) override { name = value; };

   /** Returns the name displayed when this character is talking*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   virtual FText GetGameName() const override { return name; };

   /**
    *Get the dialog key to lookup in dialog table for what the NPC will say when we begin to talk to it
    */

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FName GetCurrentDialog() const { return conversationStarterName; }

   /**
    *Set the dialog key to lookup in table for what the NPC will say when we begin talking to it
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   virtual void SetCurrentDialog(FName conversationName) { conversationStarterName = conversationName; }

   /**Sets the dialog key to lookup in the table what the NPC will say when we talk to it about a specific topic
    * @param topic - The dialogTopic the player talks to the NPC about that will have a different response
    * @param newConversationName - The name of the new conversation that the NPC will talk about when asked about this topic
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   virtual void SetSpecificDialog(FGameplayTag topic, FName newConversationName);

   /**
    *Used when trying to talk to NPC about some topic.  If NPC knows nothing about topic, return defaultConversationName
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   FName GetConversationName(FGameplayTag conversationTopic) const;

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   TArray<FGameplayTag> GetConversationTopics() const;

   /**
    *Accessor to get default response when asked about a topic not known
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   FName GetDefaultResponseName() const { return defaultResponseName; }

   /**
    *Accessor to grab response when first talked to
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   FName GetStartingConversationName() const { return conversationStarterName; }

   /**
    *Accessor to see if NPC wants to have a conversation
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   bool GetWantsToConverse() const { return bWantsToConverse; }

   /**
    *Accessor to see what happens after finishing talking with an NPC
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   FTriggerData& GetOnDialogFinishedTrigger() { return onDialogEndTriggerData; }

   /**
    *Get a list of dialogs already talked about with this NPC
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   TArray<FName> GetAlreadyConversedDialogs() { return dialogAlreadyConversed.Array(); }

   /**
    *Adds a new DIALOG to the list of DIALOGS alredy talked about
    */
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void AddConversedDialog(FName conversedDialog) { dialogAlreadyConversed.Add(conversedDialog); }

#pragma endregion

   /**
    * Logic to handle once the intro dialog with an NPC is finished
    */
   virtual void OnDoneInitialTalk();

   /**
    * Logic to handle once a dialog with an NPC is finished
    */
   virtual void OnDoneTalking();

   virtual void Interact_Implementation(ABaseHero* hero) override;

   /**
    * Sets up the dialogue UI to the proper state (conversation/intimate view) after interacting with this NPC
    */
   virtual void SetupAppropriateView();

   FVector GetInteractableLocation_Implementation() const override final;
   /**
    *NPCs can always be interacted with.  To create an NPC that doesn't interact at all, just create some blank humanoid static mesh (Persona Like)
    */
   bool CanInteract_Implementation() const override;

   virtual void SaveNPCData(FMapSaveInfo& mapInfo);
   virtual void LoadNPCData(FMapSaveInfo& mapInfo);
};

FORCEINLINE uint8 GetTypeHash(const ANPC& npc)
{
   return GetTypeHash(npc.GetActorLocation() + GetTypeHash(npc.GetGameName().ToString()));
}