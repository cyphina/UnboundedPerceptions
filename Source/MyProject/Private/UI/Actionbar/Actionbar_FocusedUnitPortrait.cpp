// Created 1/15/21 5:21 PM

#include "MyProject.h"
#include "Actionbar_FocusedUnitPortrait.h"
#include "ESkillContainer.h"
#include "TextBlock.h"
#include "Unit.h"
#include "UpStatComponent.h"
#include "Image.h"

void UActionbar_FocusedUnitPortrait::OnWidgetShown(AUnit* focusedUnit)
{
   Text_Name->SetText(focusedUnit->GetGameName());
   Text_Level->SetText(FText::AsNumber(focusedUnit->GetStatComponent()->GetUnitLevel()));
   skillWheel->OnWidgetShown(focusedUnit->GetAbilitySystemComponent());
   if(UTexture2D* unitIcon = focusedUnit->GetImage())
   {
      if(UUserWidget* characterPortrait = Cast<UUserWidget>(GetWidgetFromName("CharacterPortrait")))
      {
         if(UImage* portrait = Cast<UImage>(characterPortrait->GetWidgetFromName("Portrait")))
         {
            portrait->SetBrushFromTexture(focusedUnit->GetImage());
         }
      }
   }
}
