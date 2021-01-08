// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDManager.h"
#include "Input/Reply.h"
#include "Trigger.h"
#include "IPropertyTypeCustomization.h"

class IDetailPropertyRow;

/**
 * A custom panel for trigger type objects (FTriggerData)
 * Different slate widgets show depending on what trigger type is selected
 * The slate widgets values are then used to populate the TriggerValues and TriggerObjects string arrays inside the TriggerData property
 */
class FTriggerDataCustomization : public IPropertyTypeCustomization
{
 public:
   /** Used when setting up editor module to create an instance of this custom panel class and register it to display whenever we have a trigger type object being viewed*/
   static TSharedRef<IPropertyTypeCustomization> MakeInstance();
   /** Handles the drop down thing that displays the name of the category for the struct*/
   void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
   /** Handles extra rows (as in the case of an array. Called again when compiling a blueprint*/
   void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

 private:
   TArray<TWeakObjectPtr<UObject>> selectedObjects;

   // Property handles that are part of TriggerData
   TSharedPtr<IPropertyHandle> triggerType;            // Reference to TriggerType property so we can reuse in the delegate
   TSharedPtr<IPropertyHandle> triggerValues;          // Reference to TriggerValues property so we can set this depending on the custom slate widgets
   TSharedPtr<IPropertyHandle> triggerObjects;         // Reference to TriggerObjects property so we can set this depending on the custom slate widgets
   uint8                       triggerHandleValue = 0; // This is the property value of the TriggerType property reference

   // References to the custom widget rows we make
   IDetailPropertyRow* tagIDRow   = nullptr;
   FDetailWidgetRow*   booleanRow = nullptr;
   FDetailWidgetRow*   enumRow    = nullptr;
   int                 enumIndex  = 0; // State used in enumRow to display the correct default value for the combobutton

   IDetailChildrenBuilder*          currentStructBuilder = nullptr;
   IPropertyTypeCustomizationUtils* currentUtilities     = nullptr;
   TSharedPtr<IPropertyHandle>      currentStructHandle;

   // Creates all the custom slate widgets that are conditionally displayed based on the trigger type selected
   void CreateAllTriggerWidgets();
   // Helper to get the name of the enum being displayed in the enum widget's content menu and button
   FText GetEnumName(uint8 enumNameIndex, wchar_t* enumName = TEXT("HUDs")) const;
   // Sets up the content menu that opens when we click on the enum's combo button
   TSharedRef<SWidget> SetupEnumCbtnContent();

   // Visibility functions are checked once per frame and change depending on what kind of trigger we are trying to make
   EVisibility GetVisibilityTag() const;
   EVisibility GetVisibilityBRow() const;
   EVisibility GetVisibilityHUDEnumRow() const;

   // When modifying our custom widgets, call these functions to modify the actual FTriggerData
   void OnTagUpdated();
   void OnCheckboxChanged(ECheckBoxState e);
   void OnEnumChanged(uint8 newEnumIndex);

   // Used to convert the data in our struct into data for the slate widgets
   FName GetTagNameFromValues();
   bool  GetTriggerBoolFromValues();
   // Ensures the value in triggervalues and the selected enum value in the combobox is equivalent
   FText GetDefaultEnumVal() const;

   // Used to set initial values for any custom widgets granted we have the correct trigger type selected and already have default trigger values or trigger objects
   void SetupCustomRowDefaultValues();
};
