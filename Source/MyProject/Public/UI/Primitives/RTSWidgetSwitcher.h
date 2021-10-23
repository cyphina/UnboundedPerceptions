// Created 1/16/21 10:56 PM

#pragma once
#include "WidgetSwitcher.h"
#include "RTSWidgetSwitcher.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWidgetSwitched, UWidget*, widgetSwitchedFrom, int32, widgetSwitchedFromIndex);

UCLASS()
class MYPROJECT_API URTSWidgetSwitcher : public UWidgetSwitcher
{
   GENERATED_BODY()

public:
   void SetActiveWidgetIndex(int32 index) override;
   void SetActiveWidget(UWidget* Widget) override;

   UPROPERTY(BlueprintAssignable, Category = "Switcher Callbacks")
   FOnWidgetSwitched OnWidgetSwitchedEvent;
};
