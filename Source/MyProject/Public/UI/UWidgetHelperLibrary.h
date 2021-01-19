// Created 1/19/21 04:10 AM

#pragma once
#include "Widget.h"
#include "WidgetTree.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UWidgetHelperLibrary.generated.h"


UCLASS()
class MYPROJECT_API UUWidgetHelperLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

   /**
    * Looks for the first user widget in the hierarchy that contains the passed in widget.
    * Casts it to the desired type and returns it if it matches the desired type.
    * Else return nullptr.
    */
   template<typename WidgetT = UUserWidget>
   static WidgetT* GetUserWidgetParent(const UWidget* widget)
   {
      static_assert(TIsDerivedFrom<WidgetT, UUserWidget>::IsDerived, "Only looks for UUserWidget subclasses in the hierarchy");

      UUserWidget* parentWidget = nullptr;
      const UWidget* currentWidget = widget;

      while(currentWidget)
      {
         if(UWidgetTree* widgetTree = Cast<UWidgetTree>(currentWidget->GetOuter()))
         {
            if(parentWidget = Cast<UUserWidget>(widgetTree->GetOuter()); parentWidget)
            {
               if(parentWidget->IsA<WidgetT>())
               {
                  return Cast<WidgetT>(parentWidget);
               }
               return nullptr;
            }
         }
      }
      return nullptr;
   }
};
