#pragma once

#include "CursorClickFunctionality.h"
#include "Engine/EngineTypes.h"

class UManualSpellComponent;
class USpellCastComponent;
class ARTSPawn;
class AUserInput;
class AAlly;

enum class ECursorStateEnum : uint8;

/*
 * Holds functionality for clicking during different click states when playing the base game.
 * There's not much proper null checking in this class because this class will get swapped out if we are in a different click mode (like a debug mode where we can actually select enemies)
 */
class DefaultCursorClickFunctionality : public CursorClickFunctionalityBase
{
public:
   DefaultCursorClickFunctionality(ARTSPawn* pawnRef, AUserInput* controllerRef);
   ~DefaultCursorClickFunctionality() = default;

   /** Used to talk, interact, confirm spell usage, confirm item usage, and create a selection rectangle */
   void HandleLeftClick() final;

   /** Handles changing some state variables pertaining to mouse controls but doesn't actually do anything on its own */
   void HandleLeftClickRelease() final;

   /** Used for attacking and moving */
   void HandleRightClick() final;
   
   /** Can queue item usage, attack moves, and spell casting*/
   void HandleShiftLeftClick() final;
   
   /** Leads to queueing a move action or an attack */
   void HandleShiftRightClick() final;

private:
   void ToggleSingleAllySelection();
   
   void AttackMoveQueue();
   
   void SpellCastQueue();
   
   void ItemUsageQueue();
   
   /** Select an ally or enemy when we have the select cursor (no units are selected)*/
   void SelectSingleUnitUnderClick();
   
   /** Select an enemy unit when we have attack cursor (one ally selected)*/
   void SelectEnemy();
   void ClickInteract();
   void ClickTalk();
   void ClickUseItem();
   void ClickCastSpell();
   void ClickAttackMove();

   void MoveInFormation(FVector newLocation);
   
   ECursorStateEnum      GetCursorState() const;
   static bool           CheckWantToCast(USpellCastComponent* spellCastComp);
};
