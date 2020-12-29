#pragma once

#include "Components/ActorComponent.h"
#include "SpellCastComponent.generated.h"

class UManualSpellComponent;
class AUnitController;
class AUnit;
class UMySpell;

DECLARE_EVENT_OneParam(USpellCastComponent, FOnSpellCasted, TSubclassOf<UMySpell>);

/**
 * This component is placed on the CONTROLLER of any unit that can cast spells.
 * 1. Call BeginCastSpell() with a spell in mind. The unit which gets this function called on must have their target set already (via AI or maybe a manual spellcast component)
 * 2. We call AdjustPosition() if our spell requires us to move towards our target.
 * 3. If there's some kind of cast time we call IncantationCheck() which represents the unit saying the magic words
 * 4. Once the magic words are said we use CastSpell() to activate the ability. At this point, if our spell can be channeled we will move onto the channeling state.
 * 5. While in the channeling state our spell will be replaced by a proxy spell which we can press again to cancel the channel (typically representing a charging effect like solar beam).
 * 6. Else the channeled skill will deactivate itself after the channeling time ends and our spell pipeline ends.
 * If the owner does not have a RTSStateComponent it will not be able to move in range to cast spells. Perhaps this is wanted -
 * if we have a stationary tower or never cast targeted spells. Maybe we want AI to handle moving to position - it's up to the client code.
 * ! Relies on us having a state component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API USpellCastComponent : public UActorComponent
{
   GENERATED_BODY()

   friend AUnitController;
   friend UManualSpellComponent;

 public:
   /** Currently spell being casted/channeled */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE TSubclassOf<UMySpell> GetCurrentSpell() const { return currentSpell; }

   /**Initiate spell casting procedure without any input triggers.  Used for AI spellcasting.  Returns if we successfully transitioned to CASTING state*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual bool BeginCastSpell(TSubclassOf<UMySpell> spellToCastIndex);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   float GetCurrentIncantationTime() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   float GetCurrentChannelingTime() const;

   /** Activates the ability when we're in position. Some abilities are casted in phases, which means they get replaced by another spell.  
    * Also used for item usage (which also triggers abilities).
    * @param spellToCast - Spell we want to cast.  Left as a parameter because we can cast 
    * @return  - Returns true if successful cast, or false on unsuccessful cast. In multiplayer, the cast may still fail due to some desync between server.
    */
   void CastSpell(TSubclassOf<UMySpell> spellToCast);

   void CancelIncantation();

   void CancelChanneling();

   /**Setup a move towards a position so we're in range to cast our spell (Requires currentSpell be set)*/
   void AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation);

   /**Setup a move towards an actor so we're in range to cast our spell (Requires currentSpell be set)*/
   void AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor);

   FOnSpellCasted& OnSpellCasted() { return OnSpellCastedEvent; }

   /** Name used to notify AI observers that a spell has been casted*/
   inline static const FName AIMessage_SpellCasted = TEXT("SpellCasted!");

 protected:
   void BeginPlay() override;

   /** This is a test if the spell casted should break the user's invisibility.
    *It can be overriden in a child spell cast component such that a unit can
    *cast spells without ever breaking invisibility
    */
   virtual bool CheckSpellCastBreakInvis(TSubclassOf<UMySpell> spellToCast) const;

   UPROPERTY(EditDefaultsOnly)
   UAnimMontage* incantationAnimation = nullptr;

   UPROPERTY(EditDefaultsOnly)
   UAnimMontage* castAnimation = nullptr;

 private:
   void OnUnitStopped();

   void NotifyAIAboutSpellCast() const;

   int GetRange(TSubclassOf<UMySpell> spellClass) const;

   /**Checks to see if spell has a cast time. If so, it will start channeling (incantation) process.  Else it will just cast the spell*/
   void IncantationCheck(TSubclassOf<UMySpell> spellToCast);

   void BeginUnitChaneling(TSubclassOf<UMySpell> spellToCast);

   /**If the spell has the "Skill.Channeling" tag then it requires us to channel after the incantation aka unit has to focus energy into the spell*/
   virtual void SpellChanneling(TSubclassOf<UMySpell> spellToCast);

   void HandleVeryCloseTarget();

   bool SpellRepositionCheckAndFix(TSubclassOf<UMySpell> spellClass);

   void TestAndPlayAnimation(UAnimMontage* montageToPlay, float animPlayTime);

   TSubclassOf<class UMySpell>       currentSpell;
   class AUnit*                      unitOwnerRef;
   class URTSAbilitySystemComponent* abilityComponentRef;

   FOnSpellCasted OnSpellCastedEvent;

   FTimerHandle incantationTimer;
   FTimerHandle channelingTimer;
};
