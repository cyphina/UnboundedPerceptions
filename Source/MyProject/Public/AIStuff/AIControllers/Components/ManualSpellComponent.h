// Created 8/21/20 12:21 PM

#pragma once
#include "ManualTargetingControl.h"
#include "ManualSpellComponent.generated.h"

class URTSAbilitySystemComponent;
class USpellCastComponent;
class AUnit;
class AUnitController;
class UUpSpellTargeting;
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
    * @return -  Returns true if the target clicked was a valid spell target
    */
   UFUNCTION(BlueprintCallable, Category = "Spells")
   bool OnSpellConfirmInput(UPARAM(ref) FHitResult& result);

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
    * @param spellToCast - Ability we are trying to cast
    */
   bool PressedCastSpell(TSubclassOf<UMySpell> spellToCast);

   static const FText NOT_ENOUGH_MANA_TEXT;
   static const FText INVALID_TARGET_TEXT;
   static const FText ON_COOLDOWN_TEXT;

 protected:
   void BeginPlay() override;
   void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

 private:	
   void OnUnitStopped();

   void OnSkillActivated(int spellIndex);

   void FinalizeSpellTargeting(const UUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&) override;

   /** Called when we target something we're not suppsoed to. Returns false and prints a message. */
   bool InvalidTarget() const;

   /** If we tap a selected spell again, we can deselect it*/
   void DeselectSpell();

   /** Callback for when casting a spell but placed in this class specifically to limit the triggering of this callback to only units that have manual spell casting */
   void OnSpellCasted(TSubclassOf<UMySpell> spellCasted);

   URTSAbilitySystemComponent* GetRTSAbilityComp() const;
   
   bool IsTargetingSelf() override;

   /** Unlike the current spell, this is the one selected by the player, but it may not be the one being channeled */
   TSubclassOf<UMySpell> currentlySelectedSpell;

   AUnit* unitWithPlayerControl;

   /** Manual Spellcasting still requires us to have a spell cast component */
   USpellCastComponent* spellCastComp;
};
