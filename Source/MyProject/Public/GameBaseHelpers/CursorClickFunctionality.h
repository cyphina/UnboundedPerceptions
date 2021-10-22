#pragma once

/**
 * Abstract base class for a class that controls click functionality depending on the cursor state
 */

class ARTSPawn;
class AUserInput;
class AUnit;
class ABaseHero;

DECLARE_LOG_CATEGORY_EXTERN(Up_Log_PlayerActions, Log, All);

class MYPROJECT_API CursorClickFunctionalityBase
{
 public:
   virtual void HandleLeftClick()        = 0;
   virtual void HandleRightClick()       = 0;
   virtual void HandleShiftLeftClick()   = 0;
   virtual void HandleShiftRightClick()  = 0;
   virtual void HandleLeftClickRelease() = 0;

   virtual ~CursorClickFunctionalityBase() = default;

 protected:
   void IssueMoveToSelectedUnits(FVector moveLocation);
   void IssueMoveToFocusedUnit(FVector moveLocation);

   void IssueAttackToSelectedUnits(AUnit* attackTarget);
   void IssueAttackToFocusedUnit(AUnit* attackTarget);

   void IssueAttackMoveToSelectedUnits(FVector attackLocation);
   void IssueAttackMoveToFocusedUnit(FVector attackLocation);

   void CancelSelectedUnitsSelectedSpell();
   void SelectionRectSetup() const;
   void QueueActionToSelectedUnits(const TFunction<void(AUnit*)>& queuedAction);
   void IssueInteractCommandToSelectedHeroes();
   void IssueTalkComandToSelectedHeroes();
   void IssueItemUseCommandToHeroWithInventory();
   void ResetSecondaryCursorState();

   ABaseHero* GetHeroUsingInventory() const;

   ARTSPawn*   pawnRef       = nullptr;
   AUserInput* controllerRef = nullptr;
   FHitResult  clickHitResult;
};
