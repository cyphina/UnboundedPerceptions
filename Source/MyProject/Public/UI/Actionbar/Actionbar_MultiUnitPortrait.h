// Created 1/15/21 8:57 PM

#pragma once
#include "UserWidget.h"
#include "Actionbar_MultiUnitPortrait.generated.h"

class UUniformGridPanel;
class UUnitSelectionSlot;
class IWidgetCompilerLog;

/**
 * @brief Shows different units that are selected on the actionbar when more than one is selected.
 */
UCLASS()
class MYPROJECT_API UActionbar_MultiUnitPortrait : public UUserWidget
{
   GENERATED_BODY()

 public:
   void RefreshDisplayedUnitImages();

 protected:
   void NativeOnInitialized() override;

#if WITH_EDITOR
   void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif

   UPROPERTY(Meta = (BindWidget))
   UUniformGridPanel* unitGrid;

   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<UUnitSelectionSlot> unitSelectionSlotClass;

 private:
   TStaticArray<UUnitSelectionSlot*, 16> unitSlots;
};
