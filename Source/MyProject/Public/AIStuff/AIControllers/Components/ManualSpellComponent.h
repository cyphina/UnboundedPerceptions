// Created 8/21/20 12:21 PM

#pragma once
#include "ManualTargetingControl.h"
#include "ManualSpellComponent.generated.h"

<<<<<<< HEAD
class USpellCastComponent;
class AUnit;
class AUnitController;
struct FUpSpellTargeting;
=======
class URTSAbilitySystemComponent;
class USpellCastComponent;
class AUnit;
class AUnitController;
class UUpSpellTargeting;
>>>>>>> componentrefactor
class UMySpell;

/**
 * Add this component to a unit if it should be able to cast spells
 * using keyboard and mouse targeting. Required on any actor that wants item use capabilities (at least through traditional targeting means).
 * Used in allied units and enemies when debug functionality is enabled.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UManualSpellComponent : public UActorComponent, public IManualTargetingController
{
   GENERATED_BODY()

   friend AUnitController;

 public:
   /**
    * Called right after we press our spell key and then click on a target to confirm our spell
    * From a raycast (left click) we can test to see if the target clicked on is a proper target for our spell
    * @param result - Result of the target we found with a click
<<<<<<< HEAD
    * @param spellClass - Pass in class because we can technically setup targetting for a new spell while casting a spell (and thus can't use currentSpell)
    * @return -  Returns true if the target clicked was a valid spell target
    */
   UFUNCTION(BlueprintCallable, Category = "Spells")
   bool SetupSpellTargeting(UPARAM(ref) FHitResult& result);

=======
    * @return -  Returns true if the target clicked was a valid spell target
    */
   bool OnSpellConfirmInput(const FHitResult& hitResult, TSubclassOf<UMySpell> selectedSpell);

   /**
    * Can be called after we confirm that we made a valid hit. Called separately from the hit check because when using left click to confirm our cast, we only want to cancel
    * our other actions if we clicked on a valid spell.
    */
   void StartSpellCastAction(const FHitResult& hitResult, TSubclassOf<UMySpell> selectedSpell);
   
>>>>>>> componentrefactor
   /** Gets spell that has been selected, but may not be channeled */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spells")
   FORCEINLINE TSubclassOf<UMySpell> GetCurrentlySelectedSpell() const { return currentlySelectedSpell; }

   /**
    * When spell hotkey is pressed down with this ally selected.  Returns true when spell successfully set up or casted.  Exposed to blueprints for actionbar usage.
    * @param spellToCastIndex - Index of the actionbar key pressed
    */
   bool PressedCastSpell(int spellToCastIndex);

   /**
    * Used when we activate items. Items are a variant of spell casting.
<<<<<<< HEAD
    * @param spellToCast - Ability we are trying to cast
    */
   bool PressedCastSpell(TSubclassOf<UMySpell> spellToCast);

=======
    * @param spellToCast - Ability we are trying to cast. If nullptr, will reset the currently selected spell.
    */
   bool PressedCastSpell(TSubclassOf<UMySpell> spellToCast);

   USpellCastComponent* GetSpellCastComp() const { return spellCastComp; }
   
>>>>>>> componentrefactor
   static const FText NOT_ENOUGH_MANA_TEXT;
   static const FText INVALID_TARGET_TEXT;
   static const FText ON_COOLDOWN_TEXT;

 protected:
   void BeginPlay() override;
<<<<<<< HEAD

 private:
   void OnUnitStopped();

   void FinalizeSpellTargeting(const FUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&) override;

   /** Called when we target something we're not suppsoed to. Returns false and prints a message. */
   bool InvalidTarget() const;
=======
   void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

 private:	
   void OnUnitStopped();

   void OnSkillActivated(int spellIndex);

   /** Final step in manual spell casting involves transfering the data over to the spell cast component */
   void FinalizeSpellTargeting(const UUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&) override;
>>>>>>> componentrefactor

   /** If we tap a selected spell again, we can deselect it*/
   void DeselectSpell();

   /** Callback for when casting a spell but placed in this class specifically to limit the triggering of this callback to only units that have manual spell casting */
   void OnSpellCasted(TSubclassOf<UMySpell> spellCasted);

<<<<<<< HEAD
   void OnSpellSlotReplaced(int dropSlotindex, TSubclassOf<UMySpell> spellClass);

=======
   URTSAbilitySystemComponent* GetRTSAbilityComp() const;
   
>>>>>>> componentrefactor
   bool IsTargetingSelf() override;

   /** Unlike the current spell, this is the one selected by the player, but it may not be the one being channeled */
   TSubclassOf<UMySpell> currentlySelectedSpell;

<<<<<<< HEAD
   AUnit* unitOwner;

   /** Manual spellcasting still requires us to have a spell cast component */
=======
   AUnit* unitWithPlayerControl;

   /** Manual Spellcasting still requires us to have a spell cast component */
>>>>>>> componentrefactor
   USpellCastComponent* spellCastComp;
};
