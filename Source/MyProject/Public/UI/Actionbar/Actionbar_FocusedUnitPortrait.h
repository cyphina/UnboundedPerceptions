// Created 1/15/21 5:17 PM

#pragma once
#include "UserWidget.h"
#include "Actionbar_FocusedUnitPortrait.generated.h"

class UESkillContainer;
class UTextBlock;
class AUnit;

UCLASS()
class MYPROJECT_API UActionbar_FocusedUnitPortrait : public UUserWidget
{
   GENERATED_BODY()

public:
   void OnWidgetShown(AUnit* focusedUnit);
   
protected:
   UPROPERTY(meta=(BindWidget))
   UTextBlock* Text_Name;

   UPROPERTY(meta=(BindWidget))
   UTextBlock* Text_Level;

   UPROPERTY(meta=(BindWidget))
   UESkillContainer* skillWheel;
};
