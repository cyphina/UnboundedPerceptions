// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectEditor.h"
#include "FUnitDetails.h"

#include "DetailLayoutBuilder.h"

TSharedRef<IDetailCustomization> FUnitDetails::MakeInstance()
{
   return MakeShareable(new FUnitDetails);
}

void FUnitDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
   DetailBuilder.HideCategory("Interaction");
   DetailBuilder.HideCategory("Materials");
   DetailBuilder.HideCategory("User Interface");
   DetailBuilder.HideCategory("Virtual Texture");
   DetailBuilder.HideCategory("Shape");
   DetailBuilder.HideCategory("Tags");
   DetailBuilder.HideCategory("Skills");
   DetailBuilder.HideCategory("Decal");
   DetailBuilder.HideCategory("Materials");
   DetailBuilder.HideCategory("Pawn");
   DetailBuilder.HideCategory("Material Parameters");
   DetailBuilder.HideCategory("HLOD");
   DetailBuilder.HideCategory("LOD");
   DetailBuilder.HideCategory("Mobile");
   DetailBuilder.HideCategory("Asset User Data");
   DetailBuilder.HideCategory("Collision");
   DetailBuilder.HideCategory("Replication");
   DetailBuilder.HideCategory("Cooking");

   DetailBuilder.EditCategory(TEXT("World Object Classification"), FText::GetEmpty(), ECategoryPriority::Variable);
}
