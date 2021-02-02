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

 public:
   /**
    * Looks for the first user widget in the hierarchy that contains the passed in widget.
    * Casts it to the desired type and returns it if it matches the desired type.
    * Else return nullptr.
    * Looks up to 10 widgets up in the hierarchy, and it only finds the parent if it is in the widget tree of the parent as seen in the designer
    * (that means if we dynamically add it to one of the widget's in the parent's tree it won't count).
    */
   template <typename WidgetT = UUserWidget>
   static WidgetT* GetUserWidgetParent(const UWidget* widget)
   {
      static_assert(TIsDerivedFrom<WidgetT, UUserWidget>::IsDerived, "Only looks for UUserWidget subclasses in the hierarchy");

      UUserWidget*   parentWidget  = nullptr;
      const UWidget* currentWidget = widget;
      int searchDepth = 0;

      while(currentWidget && searchDepth < 10)
      {
         if(UWidgetTree* widgetTree = Cast<UWidgetTree>(currentWidget->GetOuter()))
         {
            if(parentWidget = Cast<UUserWidget>(widgetTree->GetOuter()); parentWidget)
            {
               if(parentWidget->IsA<WidgetT>())
               {
                  return Cast<WidgetT>(parentWidget);
               }
               currentWidget = parentWidget;
            }
         }
         ++searchDepth;
      }
      return nullptr;
   }
};
