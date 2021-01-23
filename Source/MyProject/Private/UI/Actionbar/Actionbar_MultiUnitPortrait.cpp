// Created 1/15/21 9:05 PM

#include "MyProject.h"
#include "Actionbar_MultiUnitPortrait.h"
#include "Ally.h"
#include "BasePlayer.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "UnitSelectionSlot.h"
#include "UserInput.h"
#include "WidgetCompilerLog.h"

void UActionbar_MultiUnitPortrait::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   for(int i = 0; i < unitSlots.Num(); ++i) {
      unitSlots[i] = CreateWidget<UUnitSelectionSlot>(this, unitSelectionSlotClass);  
      UUniformGridSlot* gridSlot = unitGrid->AddChildToUniformGrid(unitSlots[i]);
      gridSlot->SetRow(i / 4);
      gridSlot->SetColumn(i % 4);
   }
}

void UActionbar_MultiUnitPortrait::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
   Super::ValidateCompiledDefaults(CompileLog);
   if(!unitSelectionSlotClass)
   {
      CompileLog.Error(FText::Format(FText::FromString("{0} does not have a valid unit selection slot class set."), FText::FromString(GetName())));
   }
}

void UActionbar_MultiUnitPortrait::RefreshDisplayedUnitImages()
{
   if(AUserInput* PC = GetOwningPlayer<AUserInput>())
   {
      const TArray<AAlly*>& selectedAllies = PC->GetBasePlayer()->selectedAllies;
      const int             numSlotsToShow = FMath::Min(selectedAllies.Num(), unitSlots.Num());
      for(int i = 0; i < numSlotsToShow; ++i)
      {
         unitSlots[i]->SetUnitInformation(selectedAllies[i]);
         unitSlots[i]->SetVisibility(ESlateVisibility::Visible);
      }
      for(int i = numSlotsToShow; i < unitSlots.Num(); ++i)
      {
         unitSlots[i]->SetVisibility(ESlateVisibility::Hidden);
      }
   }
}
