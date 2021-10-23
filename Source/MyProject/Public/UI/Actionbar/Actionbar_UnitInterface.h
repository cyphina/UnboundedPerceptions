// Created 1/17/21 12:10 AM

#pragma once
#include "UserWidget.h"
#include "Actionbar_UnitInterface.generated.h"

class AUnit;

UCLASS()
class MYPROJECT_API UActionbar_UnitInterface : public UUserWidget
{
   GENERATED_BODY()

public:
   virtual void OnWidgetShown(AUnit* focusedUnit);

private:
   
   UPROPERTY(BlueprintReadOnly, Meta=(AllowPrivateAccess=true))
   AUnit* focusedUnitRef;
};
