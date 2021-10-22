#include "MyProjectEditor.h"
#include "UnitDetails.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "SButton.h"
#include "Unit.h"
#include "UnitController.h"
#include "Editor/UnrealEd/Public/Toolkits/AssetEditorManager.h"

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
   DetailBuilder.HideCategory("Tick");
   DetailBuilder.EditCategory(TEXT("World Object Classification"), FText::GetEmpty(), ECategoryPriority::Variable);

   TArray<TWeakObjectPtr<UObject> > Objects;
   DetailBuilder.GetObjectsBeingCustomized(Objects);

   if(Objects.Num() != 1)
   {
      return;
   }

   TWeakObjectPtr<AUnit> CustomizedUnit     = Cast<AUnit>(Objects[0].Get());
   auto                  ControllerShortcut = [CustomizedUnit]()
   {
      if(CustomizedUnit.IsValid())
      {
         UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
         if(IsValid(CustomizedUnit.Get()->GetUnitControllerClass()))
         {
            AssetEditorSubsystem->OpenEditorForAsset(CustomizedUnit.Get()->GetUnitControllerClass().Get()->ClassGeneratedBy);
         }
      }
      return FReply::Handled();
   };

   IDetailCategoryBuilder& Cat = DetailBuilder.EditCategory(TEXT("Unit"));
   Cat.AddCustomRow(NSLOCTEXT("Unit Editor Rows", "Controller Shortcut", "Go To Controller"))
       .WholeRowContent()[SNew(SButton)
                              .ToolTipText(
                                  TAttribute<FText>(NSLOCTEXT("Unit Editor Rows", "Controller Shortcut TT", "Click to open Asset Editor for Unit's set controller.")))
                              .Text(NSLOCTEXT("Unit Editor Rows", "Controller Shortcut Button", "Go to Controller"))
                              .HAlign(EHorizontalAlignment::HAlign_Center)
                              .OnClicked_Lambda(ControllerShortcut)];
   // TODO: Add custom button to easily access the corresponding controller and animation blueprints
}
