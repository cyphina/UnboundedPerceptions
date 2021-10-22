// Created 5/29/21 5:46 PM

#include "MyProjectEditor.h"
#include "UnitControllerDetails.h"
#include "DetailLayoutBuilder.h"
#include "Kismet/Private/BlueprintEditorCommands.h"
#include "Commands/Commands.h"

TSharedRef<IDetailCustomization> FUnitControllerDetails::MakeInstance()
{
   return MakeShareable(new FUnitControllerDetails);
}

void FUnitControllerDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
   DetailBuilder.HideCategory("Input");
   DetailBuilder.HideCategory("LOD");
   DetailBuilder.HideCategory("Cooking");
   DetailBuilder.HideCategory("Actor");
   DetailBuilder.HideCategory("Tick");                                                                                                    
}
