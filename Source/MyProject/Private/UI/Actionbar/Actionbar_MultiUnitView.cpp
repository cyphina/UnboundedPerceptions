#include "MyProject.h"
#include "Actionbar_MultiUnitView.h"
#include "Actionbar_MultiUnitPortrait.h"

void UActionbar_MultiUnitView::OnWidgetShown()
{
   unitPortraits->RefreshDisplayedUnitImages();
}
