// Created 6/14/21 7:3 PM

#pragma once
#include "AttributeSet.h"
#include "DialogStructs.h"
#include "RTSTrigger.h"
#include "HUDTypes.h"
#include "ItemManager.h"

#include "RTSTriggerTypes.generated.h"

class ANPC;
class AQuest;
class ABaseHero;
class AUnit;

/** A trigger which allows us to modify a unit's stat */
UCLASS()
class MYPROJECT_API URTSTriggerStatModify : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   FGameplayAttribute attributeToModify;

   /** Can find unit's stat to modify by type in which the first unit found of this type will have its stat modified */
   UPROPERTY(EditAnywhere)
   TSubclassOf<AUnit> unitType;

   UPROPERTY(EditAnywhere, Meta = (EditCondition = "UnitType == nullptr"))
   FName unitName;
};

UCLASS()
class MYPROJECT_API URTSTriggerOpenHUD : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   EHUDs hudToOpen;

   /** Should we try to open or close this hud? */
   UPROPERTY(EditAnywhere)
   bool bOpen = true;
};

UCLASS()
class MYPROJECT_API URTSTriggerChangeParty : public URTSTrigger
{
   GENERATED_BODY()

 public:
   URTSTriggerChangeParty() { newHeroesInParty.SetNum(4); }
   virtual void TriggerEvent_Implementation() override;

 protected:
   /** Will add up to 4 heroes to the party.
    * Any less will cause empty slots within the party.
    * Extra references are ignored.
    */
   UPROPERTY(EditAnywhere, EditFixedSize)
   TArray<TSubclassOf<ABaseHero>> newHeroesInParty;
};

UCLASS()
class MYPROJECT_API URTSTriggerAddQuest : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   TSubclassOf<AQuest> questToAdd;
};

UCLASS()
class MYPROJECT_API URTSTriggerDisplayDialog : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere, Meta = (TitleProperty = "dialog"))
   TArray<FDialogData> DialogToDisplay;
};

UCLASS()
class MYPROJECT_API URTSTriggerConversationDialog : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   // UPROPERTY(EditAnywhere)
   // TODO: Create this class
   // TSoftObjectPtr<ClassWithDialog> ConversationToLoad;
};

/** Move an NPC that is spawned into the level */
UCLASS()
class MYPROJECT_API URTSTriggerMoveNPC : public URTSTrigger
{
   GENERATED_BODY()
 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   TSubclassOf<ANPC> npcClass;

   /** Use the NPC string table to pick a name please! */
   UPROPERTY(EditAnywhere)
   FText npcName;

   /** Should we let the NPC walk to the destination (true) or teleport them there? (false) */
   UPROPERTY(EditAnywhere)
   bool bWalkToDestination;

   UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
   FVector npcMoveLocation;
};

/** Move an NPC that exists as part of the level (instead of spawned in) */
UCLASS()
class MYPROJECT_API URTSTriggerMoveNPCInLevel : public URTSTrigger
{
   GENERATED_BODY()
 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   ANPC* npc;

   /** Should we let the NPC walk to the destination (true) or teleport them there? (false) */
   UPROPERTY(EditAnywhere)
   bool bWalkToDestination;

   UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
   FVector npcMoveLocation;
};

UCLASS()
class MYPROJECT_API URTSTriggerSetNPCFollow : public URTSTrigger
{
   GENERATED_BODY()
 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   TSubclassOf<ANPC> npcClass;

   /** Use the NPC string table to pick a name please! */
   UPROPERTY(EditAnywhere)
   FText npcName;

   /** If set to -1, will follow the hero that is currently blocking interactions (the one talking) */
   UPROPERTY(EditAnywhere, Meta = (ClampMin = "-1", ClampMax = "3"))
   int heroToFollowIndex;
};

UCLASS()
class MYPROJECT_API URTSTriggerDestroyWorldObject : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   TSubclassOf<AActor> actorClass;

   UPROPERTY(EditAnywhere)
   FText objectName;
};

UCLASS()
class MYPROJECT_API URTSTriggerAddItem : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere, meta = (RowType = ItemLookupRow))
   FDataTableRowHandle itemToAddRowRef;

   UPROPERTY(EditAnywhere, Meta = (ClampMin = "1", ClampMax = "999"))
   int itemCount;

   UPROPERTY(EditAnywhere)
   TSubclassOf<ABaseHero> heroToAddItemToClass;
};

UCLASS()
class MYPROJECT_API URTSTriggerLearnDialogTopic : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere, meta = (Categories = "Dialog"))
   FGameplayTag dialogTopicToLearn;
};

UCLASS()
class MYPROJECT_API URTSPlaySequence : public URTSTrigger
{
   GENERATED_BODY()

 public:
   virtual void TriggerEvent_Implementation() override;

 protected:
   UPROPERTY(EditAnywhere)
   class ULevelSequence* sequenceToPlay;
};
