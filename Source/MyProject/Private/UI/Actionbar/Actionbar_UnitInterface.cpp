// Created 1/17/21 12:09 AM

#include "MyProject.h"
#include "Actionbar_UnitInterface.h"

void UActionbar_UnitInterface::OnWidgetShown(AUnit* focusedUnit)
{
   focusedUnitRef = focusedUnit;
}
