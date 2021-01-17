#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actionbar_SingleUnitView.generated.h"

class UActionbar_DeadInterface;
class AUnit;
class URTSWidgetSwitcher;
class UActionbar_FocusedUnitPortrait;
class UEffectStatusBar;
class UChannelingBar;
class UActionbar_AllyInterface;
class UActionbar_EnemyInterface;

/**
 * Showcases information about a single unit when we have a "focused unit"
 */
UCLASS()
class MYPROJECT_API UActionbar_SingleUnitView : public UUserWidget
{
   GENERATED_BODY()

public:
   void OnWidgetShown(AUnit* focusedUnit);

protected:
   void NativeOnInitialized() override;
   
   UPROPERTY(meta=(BindWidget))
   URTSWidgetSwitcher* WS_UnitTypeView;

   UPROPERTY(meta=(BindWidget))
   UActionbar_FocusedUnitPortrait* focusedUnitPortrait;

   UPROPERTY(meta=(BindWidget))
   UEffectStatusBar* statusBar;

   UPROPERTY(meta=(BindWidget))
   UChannelingBar* channelingBar;

   UPROPERTY(meta=(BindWidget))
   UActionbar_AllyInterface* allyInterface;

   UPROPERTY(meta=(BindWidget))
   UActionbar_EnemyInterface* enemyInterface;

   UPROPERTY(meta=(BindWidget))
   UActionbar_DeadInterface* deadInterface;

private:
   void OnUnitDie(AUnit* deadUnit);
   void OnUnitSlotSelected(AUnit* selectedUnit);
};
