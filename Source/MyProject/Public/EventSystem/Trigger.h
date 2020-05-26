// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Trigger.generated.h"

/**
 * Class for creating an event that changes only a few objects at a time
 * Think of triggers like function objects, but since we have some set layout for creaing triggers, we can
 * create all types of functions by changing the trigger types and arguments, allowing us to easily
 * modify them in the property editor as oppposed to function objects
 */

class AUserInput;
class ARTSGameMode;

UENUM(BlueprintType)
enum class ETriggerType : uint8 {
   /** Doesn't do anything */
   None,
   /** Given an NPC name (object 1), changes dialog of topic (value 1 (is fully qualified gameplaytag)) to dialog (value 2).  If (value2) empty, changes default dialog to dialogName
      (value1)*/
   ChangeDialog,
   /** For each object, assumes object is a unit.  Changes Stat (value N) (ordered numerically by stats, skills, vitals, mechanics) by adding (value N+1) to it*/
   ModifyStats,
   /** Takes one TriggerValue denoting the hud to be opened enum value corresponding int value.  Look at HUDManager for HUDs enum and corresponding int values */
   OpenHUDTrigger,
   /** Takes one TriggerValue denoting the hud to be opened enum value corresponding int value.  Look at HUDManager for HUDs enum and corresponding int values */
   OpenStorageHUDTrigger,
   /** Takes up to four trigger objects denoting the new party members in their new order*/
   ChangePartyTrigger,
   /** Activate another trigger (object 1) */
   ActivateOtherTrigger,
   /** Deactivate another trigger (object 1) */
   DeactivateOtherTrigger,
   /** Changes trigger (object 1) to type (data 1)*/
   ChangeTriggerType,
   /** Add quest from questlistmap with key (value1), and if (value2) is not 0, it will start automatically*/
   AddQuestTrigger,
   /** Complete's a quest goal from currentQuests with gameplay child tag id (value1) and goalIndex (value2).  (value3) denotes pass (!= 0) or fail (== 0) */
   CompleteQuestGoalTrigger,
   /** Displays the lines in TriggerValues as dialog.  Messages aren't localized, nor do they have a corresponding actor name, so better use DisplayConversationTrigger*/
   DisplayDialogTrigger,
   /** Gets the conversation from the conversation table named (value1) and display its contents*/
   DisplayConversationTrigger,
   /** Destroys the npc with name (object 1) */
   DestroyNPCTrigger,
   /** Moves the npc with name (object 1) to location X: (value 1), Y: (value2), Z: (value3)*/
   MoveNPCTrigger,
   /** Adds an item to hero (object 1) with id (value 1) and count (value 2).  If no hero is specified, then the interacting hero gets the item (if there is one) */
   AddItemTrigger,
   /** Adds the fully qualified gameplay tag name (value 1) as a new learned dialog topic*/
   LearnDialogTopic,
   /**Sets the NPC with name (object 1) to follow the hero with name (object 2).  If no hero name is specified, then the interacting hero is followed*/
   SetNPCFollow,
   /**Sets the IntimateNPC with name (object 1) to allow conversations (!= 0) or not (==0) (value1)*/
   SetNPCWantConverse,
   /**Plays sequence with filePath (relative to sequence folder)*/
   PlaySequence
};

USTRUCT(BlueprintType, NoExport)
struct FTriggerData {
   FTriggerData();
   FTriggerData(bool isEnabled, ETriggerType trigType, int numberOfCalls, TArray<FString> triggerObj, TArray<FString> triggerVals) :
       enabled(isEnabled), triggerType(trigType), numCalls(numberOfCalls), triggerObjects(triggerObj), triggerValues(triggerVals)
   {
   }

   static FTriggerData defaultTrigger; // not const so can be passed through trigger functionality but should never be modified

   /**Can this trigger be activated?  Can be toggled on and off by other triggers*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
   bool enabled = true;

   /**Type of the trigger*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
   ETriggerType triggerType = ETriggerType::None;

   /**Number of times this trigger can activate.  Set to -1 to be able to call this infinately*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
   int numCalls = 1;

   /**What objects this trigger will act on*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
   TArray<FString> triggerObjects = TArray<FString>();

   /**The new value that impacts the operated objects*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameters")
   TArray<FString> triggerValues = TArray<FString>();
};

UCLASS(Blueprintable)
class MYPROJECT_API UTriggerManager : public UObject
{
   GENERATED_BODY()

   UTriggerManager() {}
   ~UTriggerManager() {}

   /** Record for triggers on WorldObjects so they can be stored across levels
    * To store triggers on a WorldObject, just add the trigger to the map with the key being the WorldObject's name.
    * When the WorldObject is created again and calls BeginPlay(), load up the appropriate trigger records.
    */
   TMultiMap<FName, FTriggerData> triggerRecords;

   UPROPERTY()
   class AHUDManager* hudManagerRef;
 public:
   void Init();

   UPROPERTY()
   AUserInput*   cpcRef;

   UPROPERTY()
   ARTSGameMode* gameModeRef;

   TMultiMap<FName, FTriggerData> GetTriggerRecords() const { return triggerRecords; }

   UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Records")
   void AddTriggerToRecords(FName worldObjectName, const FTriggerData& finishedTriggerActivation);

   /**Activates a trigger given a trigger data.  If the objects referred to are not loaded, then we must store the trigger somewhere
   and the object will check for any triggers performed on it when it is loaded
   @param triggerData - Reference to triggerData incase we need to care about the # times activated and such
   */
   UFUNCTION(BlueprintCallable, Category = "TriggerLibrary")
   void ActivateTrigger(UPARAM(ref) FTriggerData& triggerData);

 private:
   ///---Trigger helper functions---
   void ChangeDialog(const FTriggerData& tdata);
   void ModifyStats(const FTriggerData& tdata);
   void OpenHUDTrigger(const FTriggerData& tdata);
   void OpenHUDTriggerStorage(const FTriggerData& tdata);
   void ChangeParty(const FTriggerData& tdata);
   void ActivateOtherTrigger(const FTriggerData& tdata);
   void DeactivateOtherTrigger(const FTriggerData& tdata);
   void ChangeTriggerType(const FTriggerData& tdata);
   bool AddQuest(const FTriggerData& tdata);
   bool CompleteQuestGoal(const FTriggerData& tdata);
   void DisplayDialog(const FTriggerData& tdata);
   void DisplayConversation(const FTriggerData& tdata);
   void DestroyNPC(const FTriggerData& tdata);
   void MoveNPC(const FTriggerData& tdata);
   void AddItem(const FTriggerData& tdata);
   void LearnDialogTopic(const FTriggerData& tdata);
   void SetNPCFollow(const FTriggerData& tdata);
   void SetNPCWantConverse(const FTriggerData& tdata);
   void PlaySequence(const FTriggerData& tdata);
   /**Function that triggers an effect when the trigger is activated*/
   void TriggerEffect(FTriggerData& tdata);
};
