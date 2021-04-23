#pragma once

#include "Ally.h"
#include "Items/Backpack.h"
#include "Stats/StatEnums.h"
#include "BaseHero.generated.h"

class ABasePlayer;
class ABaseHero;
class Inventory;
class IInteractable;
class UGameplayAbility;
class USpellBook;
class UEquipmentContainer;
class UTriggerManager;
class AHeroAIController;

using Equip_Slot_Arr = TStaticArray<int, 7>;

DECLARE_EVENT(ABaseHero, FOnLevelUp);
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnPickupItem, FMyItem&);

UCLASS(HideCategories = ("Clothing", "Mobile", "Replication"), Blueprintable)
class MYPROJECT_API ABaseHero : public AAlly
{
   GENERATED_BODY()

   friend class AHeroAIController;
   friend class UTriggerManager;
   friend void USaveLoadClass::SetupAlliedUnits();

 public:
   ABaseHero(const FObjectInitializer& oI);

   /**
    * Equip an item
    * @param backpackIndex - Index of the slot holding the item we want to equip
    */
   UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
   void Equip(const int backpackIndex);

   /**
    * Unequip an item
    * @param unequipSlot - Equipment slot index of the item to unequip
    */
   UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
   void Unequip(const int unequipSlot) const;

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stats")
   int GetCurrentExp() const;

   UFUNCTION(BlueprintCallable, Category = "Stats")
   void SetCurrentExp(int amount);

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stats")
   int GetExpToLevel() const;

   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats")
   void LevelUp();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Items")
   int GetCurrentItemId() { return currentItemId.IsSet() ? currentItemId.GetValue() : INDEX_NONE; }

   /**Set the item set to be used currently by this hero*/
   UFUNCTION(BlueprintCallable, Category = "Items")
   void SetCurrentItem(int newCurrentItemID, int newCurrentItemSlotIndex);

   /**Get the interactable this hero is targeted to interact with*/
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Interactable")
   AActor* GetCurrentInteractable() const { return Cast<AActor>(currentInteractable); }

   /** Allows us to change this character's base attributes (PERMANENTLY), which can modify their other stats which scale off that attribute */
   void OnAttributePointAllocated(ABaseHero* heroWithAttChange, EAttributes att, bool isIncrementing);

   /** Get a copy of our spell book which holds all the spells in our skill tree*/
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   USpellBook* GetSpellBook() const { return spellbook; }

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   int GetSkillPoints() const { return skillPoints; }

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   int GetAttPoints() const { return attPoints; }

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AI")
   AHeroAIController* GetHeroController() const { return heroController; }

   UFUNCTION(BlueprintPure, BlueprintCallable)
   APlayerController* GetOwningPlayer() const;

   void SetCurrentInteractable(AActor* newInteractable);

   /** Gets the item set to be used currently by this hero */
   TOptional<int> GetCurrentItem() const { return currentItemId.GetValue(); }

   void SetUnitSelected(bool value) override;

   FOnLevelUp& OnLevelUp() { return OnLevelUpEvent; }

   FOnPickupItem& OnPickupItem() { return OnPickupItemEvent; }

   const UEquipmentContainer* GetEquipment() const;

   UBackpack& GetBackpack() const { return *backpack; }

   int  GetHeroIndex() const { return heroIndex; }
   void SetHeroIndex(int newHeroIndex) { heroIndex = newHeroIndex; }

   void SetEnabled(bool bEnabled) override final;

   /**
    * @brief Range of how close we have to be to an object to interact with it.
    */
   static const int INTERACT_RANGE = 150;

   /**
    * @brief Multiplier to increase amount of xp needed for next level
    */
   static const float NEXT_EXP_MULTIPLIER;

 protected:
   void BeginPlay() override final;
   void Tick(float deltaSeconds) override final;
   void EndPlay(const EEndPlayReason::Type epr) override final;
   void PossessedBy(AController* newController) override final;
   void UnPossessed() override;
   void CheckGameOverOnDeath() const;

   /**Index inside party.  -1 means we're not in the party*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero Props")
   int heroIndex = -1;

   /**Attribute points to divy up*/
   UPROPERTY(EditAnywhere, Category = "Hero Props")
   int attPoints = 100;

   /**Skill (spellbook) points to divy up*/
   UPROPERTY(EditAnywhere, Category = "Hero Props")
   int skillPoints = 5;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<USpellBook> spellbookClass;

 private:
   /**Functions for adding and removing bonuses when equipping and unequipping.  Set isEquip to true when equipping, and to false when unequipping
    *@param equipID - ID of the equipment being added/removed
    *@param isEquip - Are we equipping or removing equipment?*/
   UFUNCTION()
   void OnEquipped(int equipID, bool isEquip) const;

   /**
    * @brief Removes a single instance of an item. Called after the controller completes a UseItem action.
    * @param itemID ItemID of the item we want to use
    */
   void OnSpellCasted(TSubclassOf<UMySpell> spellCasted);

   void OnSpellLearned(const ABaseHero& heroThatLearnedSpell, TSubclassOf<UMySpell> spellLearned);
   void OnSpellUpgraded(const ABaseHero& heroThatLearnedSpell, TSubclassOf<UMySpell> spellLearned);

   /**
    * @brief Activates triggers that were supposed to fire off on this hero if it were alive
    */
   void LoadSavedTriggers() const;

   /**
    * @brief Gives the hero all abilities corresponding to items so that they can use any item
    */
   void GiveItemAbilities() const;

   bool OnItemPickup(FMyItem& itemPickedUp) const;

   UFUNCTION(BlueprintCallable)
   void OnItemDropped(int droppedItemSlot);

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
   USpellBook* spellbook;

   UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
   UBackpack* backpack;

   UPROPERTY()
   ABasePlayer* player; // reference to our player class, which has information on our team

   UPROPERTY()
   AActor* currentInteractable; // reference to the interactable which we are trying to interact with

   UPROPERTY()
   AHeroAIController* heroController;

   UPROPERTY()
   UEquipmentContainer* equipment;

   TOptional<int> currentItemId;        // id of the item that is going to be used by this character
   TOptional<int> currentItemSlotIndex; // index in our inventory slots of the item that is going be used

   FOnPickupItem OnPickupItemEvent;

   int        currentExp  = 0;   // how much xp we have
   int        expForLevel = 100; // how much we need for next level
   FOnLevelUp OnLevelUpEvent;
};
