// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ally.h"
#include "Items/Backpack.h"
#include "BaseHero.generated.h"

class ABasePlayer;
class Inventory;
class IInteractable;
class UGameplayAbility;
class USpellBook;
class UEquipment;
class AHeroAIController;

DECLARE_EVENT(ABaseHero, FOnLevelUp);

UCLASS(HideCategories = ("Clothing", "Mobile", "Replication"), Blueprintable)
class MYPROJECT_API ABaseHero : public AAlly
{
   GENERATED_BODY()

   friend class AHeroAIController;

 public:
   static const int interactRange = 150; // range of how close we have to be to an object to interact with it.
   static const int shadowRange =
       600; // range of how far we cast in front to see if shadows are blocking us along the direction of the directional light parameter set in our shader
   static const float nextExpMultiplier; // multiplier to increase amount of xp needed for next level

   /** Constructors set default values for this character's properties */
   ABaseHero(const FObjectInitializer& oI);

   FOnLevelUp      onLevelUp;

#pragma region Callbacks

   void BeginPlay() override final;
   void Tick(float deltaSeconds) override final;
   void EndPlay(const EEndPlayReason::Type epr) override final;
   void PossessedBy(AController* newController) override final;
   void SetEnabled(bool bEnabled) override final;
   void Die_Implementation() override final;

#pragma endregion

#pragma region HeroCharacterStuff

   /**Equip an item
    * @param equipItem - ID of the item to equip */
   UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
   void Equip(int inventorySlotWeEquip);

   /**Unequip an item
    * @unequipSlot - Equipment slot index of the item to unequip*/
   UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
   void Unequip(int unequipSlot);

   /**Swap an item from one slot to another
    * @param equipSlot1 - First slot to be swapped
    * @param equipSlot2 - Second slot to be swapped*/
   UFUNCTION(BlueprintCallable, Category = "Interfacing Equipment")
   void SwapEquip(int equipSlot1, int equipSlot2);

   /**Levelup functionality mostly scripted in blueprints so we can add some effects*/
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stats")
   void LevelUp();

   /**Interface to add points to some category*/
   UFUNCTION(BlueprintCallable)
   void ChangeAttribute(EAttributes att, bool isIncrementing);

   /**Like CastSpell in Ally, but checks to see if the spell was casted via an item*/
   bool CastSpell(TSubclassOf<UMySpell> spellToCast) override final;
#pragma endregion

#pragma region Accessors

   void SetSelected(bool value) override;

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stats")
   int GetCurrentExp() const;

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Stats")
   int GetExpToLevel() const;

   UFUNCTION(BlueprintCallable, Category = "Stats")
   void SetCurrentExp(int amount);

   /**Get the item set to be used currently by this hero*/
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Items")
   int GetCurrentItem() const { return currentItem; }

   /**Set the item set to be used currently by this hero*/
   UFUNCTION(BlueprintCallable, Category = "Items")
   void SetCurrentItem(int itemID) { currentItem = itemID; }

   /**Get the interactable this hero is targetted to interact with*/
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Interactable")
   AActor* GetCurrentInteractable() const;

   /**Get a copy of our spellbook which holdls all the spells in our skilltree*/
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   FORCEINLINE USpellBook* GetSpellBook() const { return spellbook; }

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   TArray<int> GetEquipment() const;

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   int GetSkillPoints() const { return skillPoints; }

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AI")
   FORCEINLINE AHeroAIController* GetHeroController() const { return heroController; }

#pragma endregion

   /**Default material color*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FLinearColor baseColor;

   /**Color of the hero currently (Depreciated - Back when I first started this project, instead of using a selection box, I changed the colors on selection*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FLinearColor currentColor;

   /**Dynamic material instance reference used to change material's color*/
   UPROPERTY()
   UMaterialInstanceDynamic* dMat;

   /**Index inside party.  -1 means we're not in the party*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int heroIndex = -1;

   /**Attribute points to divy up*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int attPoints = 100;

   /**Skill (spellbook) points to divy up*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int skillPoints = 5;

   /**Reference to our character's backpack*/
   UPROPERTY(BlueprintReadWrite)
   UBackpack* backpack;

   /**Reference to our character's spellbook*/
   UPROPERTY(BlueprintReadOnly)
   USpellBook* spellbook;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<USpellBook> spellbookClass;

 private:
   /**Function to imitate cheaply effects of shadows on our character.  See if some object is in front of us so we know its casting a shadow upon us*/
   void CheckShadows();
   /**Using some kind of item, consumeable, utility, etc when the item ability is finally activated (spell is cast).  Needs parameter since currentItem is reset on spellcast.*/
   void UseItem(int itemID);

   /**Functions for adding and removing bonuses when equipping and unequipping.  Set isEquip to true when equipping, and to false when unequipping
    *@param equipID - ID of the equipment being added/removed
    *@param isEquip - Are we equipping or removing equipment?*/
   UFUNCTION()
   void OnEquipped(int equipID, bool isEquip);

   ///References
   UPROPERTY()
   ABasePlayer* player; // reference to our player class, which has information on our team

   UPROPERTY()
   AActor* currentInteractable; // reference to the interactable which we are trying to interact with

   int     currentItem = 0;     // id of the item that is going to be used by this character

   UPROPERTY()
   AHeroAIController* heroController;

   ///Lighting effect
   UPROPERTY()
   UMaterialParameterCollection* lightSource = nullptr; // We need this parameter to figure out light direction based on our directional light we made
   UPROPERTY()
   UMaterialParameterCollectionInstance* lightDirectionPV = nullptr;
   FVector                               lightVector      = FVector::ZeroVector; // A reference we use in calculating if there's shadows

   ///Leveling information
   int currentExp  = 0;   // how much xp we have
   int expForLevel = 100; // how much we need for next level

   ///< summary>
   ///Equipment information
   ///0 - Head, 1 - Body, 2 - Legs, 3 - Acc1, 4 - Codex, 5-9 - Codex Weapons
   ///</summary>
   UPROPERTY()
   UEquipment* equipment; // a container of what we have equipped

   friend void USaveLoadClass::SetupAlliedUnits();
   friend void InteractState::Update(AUnit& unit, float deltaSeconds);
};
