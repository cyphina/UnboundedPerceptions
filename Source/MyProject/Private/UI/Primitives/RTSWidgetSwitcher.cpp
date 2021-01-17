// Created 1/16/21 11:06 PM

#include "MyProject.h"
#include "UI/Primitives/RTSWidgetSwitcher.h"

void URTSWidgetSwitcher::SetActiveWidgetIndex(int32 index)
{
   OnWidgetSwitchedEvent.Broadcast(GetWidgetAtIndex(index), index);
   Super::SetActiveWidgetIndex(index);
}

void URTSWidgetSwitcher::SetActiveWidget(UWidget* Widget)
{
   OnWidgetSwitchedEvent.Broadcast(Widget, GetChildIndex(Widget));
   Super::SetActiveWidget(Widget);
}
