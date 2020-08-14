#pragma once

#include "Components/ActorComponent.h"
#include "SpellCastComponent.generated.h"

class UMySpell;

DECLARE_EVENT_OneParam(USpellCastComponent, FOnSpellCasted, TSubclassOf<UMySpell>);

/** This component is placed on the CONTROLLER of any unit that can cast spells */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API USpellCastComponent : public UActorComponent
{
   GENERATED_BODY()

   virtual ~USpellCastComponent() = default;

 public:
   /** Name used to notify AI observers that a spell has been casted*/
   inline static const FName AIMessage_SpellCasted = TEXT("SpellCasted!");

   /** We can use this hook to setup phase changes or something else (like a magic wand effect in dota*/
   FOnSpellCasted onSpellCastedEvent;

   /** Currently spell being casted/channeled */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE TSubclassOf<UMySpell> GetCurrentSpell() const { return currentSpell; }

   /** Sets the spell currently being casted/channeled */
   void SetCurrentSpell(TSubclassOf<UMySpell> newCurrentSpell) { currentSpell = newCurrentSpell; }

   /**Initiate spell casting procedure without any input triggers.  Used for AI spellcasting.  Returns if we successfully transitioned to CASTING state*/
   UFUNCTION(BlueprintCallable, Category = "Action")
   virtual bool BeginCastSpell(TSubclassOf<UMySpell> spellToCastIndex);

   /** Actually activates the ability when we're in range or  and triggers abilities that
    * wait for click events to continue.
    * Also used for item usage (which also triggers abilities).
    * @param spellToCast - Spell we want to cast.  Left as a parameter because we can cast
    * @return  - Returns true if sucessful cast, or false on unsucessful cast. In multiplayer, the cast may still fail due to some desync between server.
    */
   virtual bool CastSpell(TSubclassOf<UMySpell> spellToCast);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   FORCEINLINE float GetCurrentChannelTime() const;

 protected:
   void BeginPlay() override;

   /** This is a test if the spell casted should break the user's invisibility.
    *It can be overrided in a child spell cast component such that a unit can
    *cast spells without ever breaking invisibility
    */
   virtual bool CheckSpellCastBreakInvis(TSubclassOf<UMySpell> spellToCast) const;

 private:
   TSubclassOf<class UMySpell>       currentSpell;
   class AUnit*                      unitOwnerRef;
   class URTSAbilitySystemComponent* abilityComponentRef;

   void NotifyAIAboutSpellCast() const;

   /**Checks to see if spell has a cast time, and if so, it will start channeling (incantation) process.  Else it will just cast the spell*/
   virtual void IncantationCheck(TSubclassOf<UMySpell> spellToCast) const;

   /**If the spell has the Skill.Channeling tag then it requires us to channel after the incantation aka unit has to focus energy into the spell*/
   virtual void SpellChanneling(TSubclassOf<UMySpell> spellToCast);

   /**Setup a move towards a position so we're in range to cast our spell (Requires currentSpell be set)*/
   void AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation);

   /**Setup a move towards an actor so we're in range to cast our spell (Requires currentSpell be set)*/
   void AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor);
};
