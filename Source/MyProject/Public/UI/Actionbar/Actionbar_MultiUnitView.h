#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actionbar_MultiUnitView.generated.h"

class UActionbar_MultiUnitPortrait;
class UActionbar_TacticsInterface;
class UUnitSelectionSlot;

/**
 * When we have multiple allies selected and no focused unit, then we will see this view.
 * We can setup group tactics and selection individual units.
 * As with other actionbar views, this widget lives within the action bar.
 * Currently we cannot select multiple enemies in any way. This may be subject to change.
 * TODO: Add some kind of widget pool so we don't have to keep adding/removing widgets on the fly
 */
UCLASS()
class MYPROJECT_API UActionbar_MultiUnitView : public UUserWidget
{
   GENERATED_BODY()

public:
   void OnWidgetShown();
   
protected:
   UPROPERTY(Meta=(BindWidget))
   UActionbar_MultiUnitPortrait* unitPortraits;
   
   UPROPERTY(Meta=(BindWidget))
   UActionbar_TacticsInterface* tacticsInterface;
};
