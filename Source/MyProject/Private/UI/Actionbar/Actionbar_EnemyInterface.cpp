// Created 1/15/21 9:05 PM

#include "MyProject.h"
#include "Actionbar_EnemyInterface.h"
#include "RTSGlobalCVars.h"
#include "ActionSlotStyle.h"
#include "Border.h"
#include "Enemy.h"
#include "MySpell.h"
#include "SpellDelegateStore.h"
#include "TileView.h"
#include "UIDelegateContext.h"
#include "WidgetBlueprintLibrary.h"

UEnemySkillData* UEnemySkillData::CreateEnemySkillData(TSubclassOf<UMySpell> spellClass, int slotIndex)
{
   UEnemySkillData* skillData = NewObject<UEnemySkillData>();
   skillData->spellClass      = spellClass;
   skillData->slotIndex       = slotIndex;
   return skillData;
}

FReply UEnemySkillSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(GetStyleCDO())
   {
      actionBorder->SetBrush(GetStyleCDO()->GetPressedBrush());
   }

   return FReply::Unhandled();
}

FReply UEnemySkillSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(GetStyleCDO())
   {
      actionBorder->SetBrush(GetStyleCDO()->GetDefaultBrush());
   }
   return FReply::Unhandled();
}

void UEnemySkillSlot::NativeOnListItemObjectSet(UObject* ListItemObject)
{
   if(UEnemySkillData* itemData = Cast<UEnemySkillData>(ListItemObject))
   {
      slotIndex = itemData->slotIndex;
      UpdateSkillSlot(itemData->spellClass);
      if(UActionbar_EnemyInterface* enemyInterface = Cast<UActionbar_EnemyInterface>(GetParentContainer()))
      {
         SetInfo(FText::AsNumber(itemData->spellClass.GetDefaultObject()->GetCost(enemyInterface->GetEnemyRef()->GetAbilitySystemComponent())));
      }
   }
}

void UEnemySkillSlot::NativeOnEntryReleased()
{
}

void UActionbar_EnemyInterface::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   TView_SkillList->OnItemClicked().AddUObject(this, &UActionbar_EnemyInterface::HandleEnemySkillClicked);
   SpellGameContext::OnFocusedUnitSpellCastedEvent.AddUObject(this, &UActionbar_EnemyInterface::OnFocusedUnitSpellCasted);
}

void UActionbar_EnemyInterface::OnWidgetShown(AUnit* focusedUnit)
{
   enemyRef = Cast<AEnemy>(focusedUnit);
   TArray<UEnemySkillData*> listData;

   int index = -1;
   for(const TSubclassOf<UMySpell> skill : focusedUnit->GetAbilitySystemComponent()->GetAbilities())
   {
      listData.Add(UEnemySkillData::CreateEnemySkillData(skill, ++index));
   }

   TView_SkillList->SetListItems(listData);
}

void UActionbar_EnemyInterface::OnFocusedUnitSpellCasted(AUnit* focusedUnit, int spellIndex)
{
   /*if(GameplayModifierCVars::bEnableEnemyControl)
   {*/
   if(UObject* dataItem = TView_SkillList->GetItemAt(spellIndex))
   {
      if(UEnemySkillSlot* EnemySkillSlot = TView_SkillList->GetEntryWidgetFromItem<UEnemySkillSlot>(dataItem))
      {
         EnemySkillSlot->ShowCooldown();
      }
   }
   //}
}

void UActionbar_EnemyInterface::HandleEnemySkillClicked(UObject* item)
{
   UWidgetBlueprintLibrary::SetFocusToGameViewport();
   if(GameplayModifierCVars::bEnableEnemyControl)
   {
      UEnemySkillData* listItem = TView_SkillList->GetSelectedItem<UEnemySkillData>();
      if(listItem)
      {
         GetOwningLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnEnemySkillSlotClicked().Broadcast(TView_SkillList->GetIndexForItem(item));
      }
   }
}
