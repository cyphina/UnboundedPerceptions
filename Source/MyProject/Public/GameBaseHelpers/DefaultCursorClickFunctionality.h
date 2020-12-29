#pragma once

#include "CursorClickFunctionality.h"
#include "Engine/EngineTypes.h"

class ARTSPawn;
class AUserInput;
class AAlly;

enum class ECursorStateEnum : uint8;

class UDefaultCursorClickFunctionality : public ICursorClickFunctionality
{
   ARTSPawn* const   pawnRef;
   AUserInput* const controllerRef;
   FHitResult        clickHitResult;

 public:
   UDefaultCursorClickFunctionality(ARTSPawn* pawnRef, AUserInput* controllerRef);
   ~UDefaultCursorClickFunctionality() = default;

   void HandleLeftClick() final;
   void HandleLeftClickRelease() final;
   void HandleRightClick() final;
   /** Can queue item usage, attack moves, and spell casting*/
   void HandleShiftLeftClick() final;
   /** Leads to queueing a move action or an attack */
   void HandleShiftRightClick() final;

 private:
   /// Left click functionality
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

   inline ECursorStateEnum GetCursorState() const;
   inline static bool      CheckAllyWantToCast(const AAlly* ally) const;
   inline bool             AttemptAllyCastOnTarget(const AAlly* ally);
   const TArray<AAlly*>&   GetSelectedAllies() const;
};
