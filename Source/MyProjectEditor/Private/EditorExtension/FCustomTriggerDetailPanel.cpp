// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProjectEditor.h"
#include "FCustomTriggerDetailPanel.h"
#include "Trigger.h"
#include "Slate/Public/Widgets/Text/STextBlock.h"
#include "Slate/Public/Widgets/Input/SCheckBox.h"
#include "Slate/Public/Widgets/Input/SComboBox.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "GameplayTagContainer.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "MultiBoxBuilder.h"

#define LOCTEXT_NAMESPACE "TriggerStructCustomization"

TSharedRef<IPropertyTypeCustomization> FTriggerDataCustomization::MakeInstance()
{
   return MakeShareable(new FTriggerDataCustomization());
}

void FTriggerDataCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> structPropertyHandle, FDetailWidgetRow& headerRow,
                                                IPropertyTypeCustomizationUtils& structCustomizationUtils)
{
   headerRow.NameContent()[structPropertyHandle->CreatePropertyNameWidget(LOCTEXT("Trigger Details", "Trigger Details"))].ValueContent().MinDesiredWidth(
       500)[SNew(STextBlock).Text(LOCTEXT("Extra info", "Some new representation")).Font(IDetailLayoutBuilder::GetDetailFont())];
}

void FTriggerDataCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> structPropertyHandle, IDetailChildrenBuilder& structBuilder,
                                                  IPropertyTypeCustomizationUtils& structCustomizationUtils)
{
   // Save the struct builder since it gets reset on compile
   currentStructBuilder = &structBuilder;
   currentUtilities     = &structCustomizationUtils;
   currentStructHandle  = structPropertyHandle;

   // Get property references
   triggerValues  = structPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FTriggerData, triggerValues));
   triggerObjects = structPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FTriggerData, triggerValues));
   triggerType    = structPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FTriggerData, triggerType));
   auto bEnabled = structPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FTriggerData, enabled));
   auto numCalls = structPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FTriggerData, numCalls));


   // Add existing properties to the struct so we get those slate widgets
   IDetailPropertyRow& triggerValueRow = structBuilder.AddProperty(triggerValues.ToSharedRef());
   IDetailPropertyRow& triggerObjectsRow = structBuilder.AddProperty(triggerObjects.ToSharedRef());
   IDetailPropertyRow& triggerTypeRow = structBuilder.AddProperty(triggerType.ToSharedRef());
   IDetailPropertyRow& bEnabledRow = structBuilder.AddProperty(bEnabled.ToSharedRef());
   IDetailPropertyRow& numCallsRow = structBuilder.AddProperty(numCalls.ToSharedRef());

   // TODO: Setup visibility for triggerValues and triggerObjects
   //triggerValueRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FTriggerDataCustomization::GetVisibilityTag)));
   //triggerObjectsRow.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FTriggerDataCustomization::GetVisibilityTag)));

   // Bind delegates to capture changing the triggertype enum which prompts the layout of this widget to change
   FSimpleDelegate onTriggerTypePropChanged;
   onTriggerTypePropChanged.BindLambda([this]() {
      // Update this value whenever we switch our trigger type to change visibilities of other widgets
      triggerType->GetValue(triggerHandleValue);
   });

   triggerType->SetOnPropertyValueChanged(onTriggerTypePropChanged);
   CreateAllTriggerWidgets();
   //TODO: Setup the custom widgets default properties based on the predefined values on TriggerData
   SetupCustomRowDefaultValues();
}

void FTriggerDataCustomization::CreateAllTriggerWidgets()
{
   if(currentStructBuilder) {
      // Setup a gameplay tag that can be used for various trigger types
      tagIDRow = currentStructBuilder->AddExternalStructure(MakeShared<FStructOnScope>(FGameplayTag::StaticStruct()), "TagId");
      tagIDRow->DisplayName(LOCTEXT("CustomTriggerDetailTag", "Tag Id"));
      tagIDRow->Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FTriggerDataCustomization::GetVisibilityTag)));

      // Setup a checkbox that can be used for various trigger types
      booleanRow = &currentStructBuilder->AddCustomRow(LOCTEXT("CustomTriggerDetailSearchBool", "triggerbool"))
                        .NameContent()[currentStructHandle->CreatePropertyNameWidget(LOCTEXT("CustomTriggerDetailCheck", "Trigger Bool"))]
                        .ValueContent()[SNew(SCheckBox).OnCheckStateChanged(FOnCheckStateChanged::CreateSP(this, &FTriggerDataCustomization::OnCheckboxChanged))];
      booleanRow->Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FTriggerDataCustomization::GetVisibilityBRow)));

      // Create a drop down with hud enum values
      enumRow = &currentStructBuilder->AddCustomRow(LOCTEXT("CustomTriggerDetailSearchEnum", "triggerenum"))
                     .NameContent()[currentStructHandle->CreatePropertyNameWidget(LOCTEXT("CustomTriggerEnum", "Trigger Enum"))]
                     .ValueContent()[SNew(SComboButton)
                                         .OnGetMenuContent(this, &FTriggerDataCustomization::SetupEnumCbtnContent)
                                         .ContentPadding(FMargin(2.0f, 2.0f))
                                          // Passing in a delegate like this will call it every frame as opposed to passing in a function which sets it once as it is created
                                         .ButtonContent()[SNew(STextBlock).Text(this, &FTriggerDataCustomization::GetDefaultEnumVal).Font(IDetailLayoutBuilder::GetDetailFont())]];
      enumRow->Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FTriggerDataCustomization::GetVisibilityHUDEnumRow)));
   }
}

FText FTriggerDataCustomization::GetEnumName(uint8 enumNameIndex, wchar_t* enumName) const
{
   UEnum* hudEnum = FindObject<UEnum>(ANY_PACKAGE, enumName, true);
   check(hudEnum);
   // Safely returns INDEX_NONE of the index is not part of the enum
   return hudEnum->GetDisplayNameTextByValue(enumNameIndex);
}

TSharedRef<SWidget> FTriggerDataCustomization::SetupEnumCbtnContent()
{
   // Build a menu item for each value in the hud enum that modifies the values array when we select it
   FMenuBuilder MenuBuilder(true, NULL);
   for(HUDs val : TEnumRange<HUDs>()) {
      // When changing our item, modify the corresponding value in our value array
      FUIAction itemAction(FExecuteAction::CreateSP(this, &FTriggerDataCustomization::OnEnumChanged, static_cast<uint8>(val)));
      MenuBuilder.AddMenuEntry(GetEnumName(static_cast<uint8>(val)), TAttribute<FText>(), FSlateIcon(), itemAction);
   }
   return MenuBuilder.MakeWidget();
}

void FTriggerDataCustomization::OnTagUpdated()
{
}

void FTriggerDataCustomization::OnCheckboxChanged(ECheckBoxState e)
{
}

void FTriggerDataCustomization::OnEnumChanged(uint8 newEnumIndex)
{
   // Update the trigger values array with the correct values
   uint32 numChildren = 0;
   triggerValues->GetNumChildren(numChildren);
   if(numChildren > 0) {
      triggerValues->AsArray()->GetElement(0)->SetValue(FString::FromInt(newEnumIndex));
   }
}

FName FTriggerDataCustomization::GetTagNameFromValues()
{
   return "";
}

bool FTriggerDataCustomization::GetTriggerBoolFromValues()
{
   return false;
}

FText FTriggerDataCustomization::GetDefaultEnumVal() const
{
   // When loading up the enum widget in the details panel (by choosing a triggertype that shows an enum), we need to
   // reload its value based on the first value of TriggerValues. If there is no value in TriggerValues, then we'll make one
   // to keep the mapping between value and widget.

   if(GetVisibilityHUDEnumRow() == EVisibility::Visible) {
      uint32 numChildren = 0;
      triggerValues->GetNumChildren(numChildren);
      // Restore the widget value from the value of TriggerData
      FString enumValue;

      if(numChildren > 0) {
         // If there's an element in triggerValues, set this enum to correspond with that value (index can be out of bounds if
         triggerValues->GetChildHandle(0)->GetValue(enumValue);
         if(enumValue.IsNumeric())
            return GetEnumName(FCString::Atoi(*enumValue));
         return LOCTEXT("TriggerEnumError", "Error With Populating Enum Value");
      } else {
         // If trigger values is empty, add a slot and set it to the default
         auto res = triggerValues->AsArray()->AddItem();
         if(res == FPropertyAccess::Result::Success) {
            // If we successfully created a new value in the default trigger values
            triggerValues->AsArray()->GetElement(0)->SetValue(FString("0"));
            return GetEnumName(0);
         } else {
            UE_LOG(LogTemp, Error, TEXT("Problem with Custom Trigger Detail being unable to set a default value handling custom enum combobutton"));
            return LOCTEXT("TriggerEnumError", "Error With Populating Enum Value");
         }
      }
   }
   return FText::GetEmpty();
}

void FTriggerDataCustomization::SetupCustomRowDefaultValues()
{
   // Get this when opening the menu to set the trigger type to be the initial value loaded from the struct
   triggerType->GetValue(triggerHandleValue);
   tagIDRow->GetPropertyHandle()->GetChildHandle("TagName")->SetValue(FString("Combat"));
}

EVisibility FTriggerDataCustomization::GetVisibilityTag() const
{
   ETriggerType trigPropVal = static_cast<ETriggerType>(triggerHandleValue);
   if(trigPropVal == ETriggerType::AddQuestTrigger || trigPropVal == ETriggerType::CompleteQuestGoalTrigger || trigPropVal == ETriggerType::LearnDialogTopic)
      return EVisibility::Visible;
   return EVisibility::Hidden;
}

EVisibility FTriggerDataCustomization::GetVisibilityBRow() const
{
   ETriggerType trigPropVal = static_cast<ETriggerType>(triggerHandleValue);
   if(trigPropVal == ETriggerType::AddQuestTrigger || trigPropVal == ETriggerType::SetNPCWantConverse || trigPropVal == ETriggerType::SetNPCFollow)
      return EVisibility::Visible;
   return EVisibility::Hidden;
}

EVisibility FTriggerDataCustomization::GetVisibilityHUDEnumRow() const
{
   ETriggerType trigPropVal = static_cast<ETriggerType>(triggerHandleValue);
   if(trigPropVal == ETriggerType::OpenHUDTrigger)
      return EVisibility::Visible;
   return EVisibility::Hidden;
}
