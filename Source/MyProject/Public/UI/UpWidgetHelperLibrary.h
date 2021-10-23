// Created 1/19/21 04:10 AM

#pragma once
#include "Widget.h"
#include "WidgetTree.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpWidgetHelperLibrary.generated.h"

UCLASS()
class MYPROJECT_API UUpWidgetHelperLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

 public:
   /** Displays an obvious message at the top of the screen the player can see. */
   UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
   static void DisplayHelpText(UObject* WorldContextObject, const FText& helpText);

   /** Draws a box but uses a FSlateBrush over a Slate Brush Asset. */
   UFUNCTION(BlueprintCallable, Category = "Painting", meta = (DisplayName = "Draw Box from Brush"))
   static void Up_DrawBox(UPARAM(ref) FPaintContext& Context, FVector2D Position, FVector2D Size, FSlateBrush Brush, FLinearColor Tint = FLinearColor::White);

   /** Draw a B-Spline given points which define 3 straight lines (see https://www.malinc.se/m/MakingABezierSpline.php) */
   UFUNCTION(BlueprintCallable, Category = "Painting")
   static void DrawSpline(UPARAM(ref) FPaintContext& Context, FVector2D Position, FVector2D Size, FVector2D P0, FVector2D P1, FVector2D P2, FVector2D P3, float Thickness,
                          FLinearColor Tint);

   /** Draw a B-Spline given draw-space points which define 3 straight lines (see https://www.malinc.se/m/MakingABezierSpline.php) */
   UFUNCTION(BlueprintCallable, Category = "Painting")
   static void DrawDrawSpaceSpline(UPARAM(ref) FPaintContext& Context, FVector2D P0, FVector2D P1, FVector2D P2, FVector2D P3, float Thickness,
                          FLinearColor Tint);

   /**
    * Looks for the first user widget in the hierarchy that contains the passed in widget.
    * ! For this to work make sure you don't set the Owning Player when creating a widget.
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
      int            searchDepth   = 0;

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
