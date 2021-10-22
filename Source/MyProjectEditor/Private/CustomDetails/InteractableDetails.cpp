// Created 6/15/21 1:37 AM

#include "MyProject.h"
#include "InteractableDetails.h"
#include "DetailLayoutBuilder.h"

TSharedRef<IDetailCustomization> FInteractableDetails::MakeInstance()
{
   return MakeShareable(new FInteractableDetails);
}

void FInteractableDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
   DetailBuilder.HideCategory(TEXT("Virtual Texture"));
   DetailBuilder.HideCategory("HLOD");
   DetailBuilder.HideCategory("Mobile");
   DetailBuilder.HideCategory("Input");
   DetailBuilder.HideCategory("Cooking");
   DetailBuilder.HideCategory("Activation");
   DetailBuilder.HideCategory("Tags");
   DetailBuilder.HideCategory("Material Parameters");
   DetailBuilder.EditCategory(TEXT("Interactable Base"), FText::GetEmpty(), ECategoryPriority::Variable);
}
