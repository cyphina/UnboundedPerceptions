#pragma once

#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "ActionbarInterface.generated.h"

class UCheckBox;
class UActionbar_MultiUnitView;
class UActionbar_SingleUnitView;
class AUnit;
class URTSWidgetSwitcher;
class UChannelingBar;
class AAlly;
class FOnSlotSelected;

/*
 * Class that represents the action bar we see when selecting a unit/enemy
 * Includes skillbar, character skills, and tactics menu
 */

UCLASS(meta=(DisableNativeTick))
class MYPROJECT_API UActionbarInterface : public UMyUserWidget
{
   GENERATED_BODY()
   
 public:
   /** View when tabbing through multiple selected allies */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void SingleAllyViewIndexFree(AAlly* allyToFocus);

   /** View when the focused unit dies. */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void DeadUnitView();

   FOnSlotSelected& OnSlotSelected();
   
 protected:
   UPROPERTY(meta=(BindWidget))
   URTSWidgetSwitcher* WS_UnitTypeView;

   UPROPERTY(meta=(BindWidget))
   UActionbar_SingleUnitView* singleUnitView;

   UPROPERTY(meta=(BindWidget))
   UActionbar_MultiUnitView* multiUnitView;

   UPROPERTY(meta=(BindWidget))
   UCheckBox* Chk_Lock;
   
   /** Can we drag this action bar around or (*EVENTUALLY*) lock it from changing units? */
   UPROPERTY(BlueprintReadWrite, Category = "Properties")
   bool isLocked;

   void NativeOnInitialized() override;

 private:
   UFUNCTION()
   void OnUnitSelected();
   
   UFUNCTION()
   void OnUnitDeselected();
   
   UFUNCTION()
   void OnAllAlliesCleared();

   /** Handles scenario when a new unit is left clicked on, or if we tab to focus on a different unit in the selection group */
   void OnFocusedUnitChanged(AUnit* newFocusedUnit);
};
