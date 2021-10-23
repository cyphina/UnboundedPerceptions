// Created 1/15/21 9:0 PM

#pragma once
#include "Actionbar_UnitInterface.h"
#include "IUserObjectListEntry.h"
#include "SkillSlot.h"
#include "SlotContainer.h"

#include "Actionbar_EnemyInterface.generated.h"

class UMySpell;
class UTileView;
class AEnemy;
class UTextBlock;
class AUnit;

UCLASS(BlueprintType)
class UEnemySkillData : public UObject
{
   GENERATED_BODY()

 public:
   static UEnemySkillData* CreateEnemySkillData(TSubclassOf<UMySpell> spellClass, int slotIndex);

   UPROPERTY(Transient, BlueprintReadOnly)
   TSubclassOf<UMySpell> spellClass;

   int slotIndex;
};

/**
 * @brief A more restrictive skill slot that allows us to cast enemy spells when debugging or see what spells the enemy has in the actual game.
 */
UCLASS()
class UEnemySkillSlot : public USkillSlot, public IUserObjectListEntry
{
   GENERATED_BODY()

 protected:
   void   NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) final {}
   bool   NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) final { return false; }
   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

   void NativeOnListItemObjectSet(UObject* ListItemObject) override;
   void NativeOnEntryReleased() override;

 private:
};

/** This UI shows up on our actionbar when we select an enemy */
UCLASS()
class MYPROJECT_API UActionbar_EnemyInterface : public UActionbar_UnitInterface
{
   GENERATED_BODY()

 public:
   void OnWidgetShown(AUnit* focusedUnit) override;
   void OnFocusedUnitSpellCasted(AUnit* focusedUnit, int spellIndex);

   AEnemy* GetEnemyRef() const { return enemyRef; }

 protected:
   void NativeOnInitialized() override;

   UPROPERTY(Meta = (BindWidget))
   UTextBlock* Text_EnemyDescription;

   UPROPERTY(Meta = (BindWidget))
   UTileView* TView_SkillList;

 private:
   void HandleEnemySkillClicked(UObject* item);

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
   AEnemy* enemyRef;
};
