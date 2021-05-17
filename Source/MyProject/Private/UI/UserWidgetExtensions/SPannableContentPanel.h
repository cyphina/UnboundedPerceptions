#pragma once
#include "SConstraintCanvas.h"

/**
 * @brief Used to contain content which is to large to see all of. Reveals a part of it and allows us to pan around it.
 */
class MYPROJECT_API SPannableContentPanel : public SCompoundWidget
{
 public:
   SLATE_BEGIN_ARGS(SPannableContentPanel) : _ScrollZoomMultiplier(0.2f), _MaxZoom(3.f) {}
   SLATE_DEFAULT_SLOT(FArguments, Content)
   SLATE_ARGUMENT(FMargin, Padding)
   SLATE_ARGUMENT(float, ScrollZoomMultiplier)
   SLATE_ARGUMENT(float, PanSpeed)
   SLATE_ARGUMENT(float, MaxZoom)
   SLATE_ARGUMENT(bool, IsBounded)
   SLATE_ARGUMENT(float, MaxPanX)
   SLATE_ARGUMENT(float, MaxPanY)
   SLATE_ARGUMENT(FVector2D, InitialOffset)
   SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)
   SLATE_EVENT(FPointerEventHandler, OnMouseMove)
   SLATE_END_ARGS()

   SPannableContentPanel();

   void Construct(const FArguments& InArgs);

   void                       SetContent(TSharedRef<SWidget> InContent);
   const TSharedRef<SWidget>& GetContent() const;
   void                       ClearContent();

   void SetPadding(const TAttribute<FMargin>& InPadding);

 private:
   virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
   virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
   virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

   TSharedPtr<SConstraintCanvas> OffsetCanvas;
   SConstraintCanvas::FSlot*     OffsetCanvasContentSlot;

   FVector2D InitialOffset;

   float ScrollZoomMultiplier = 0.2f;
   float PanSpeed             = 1.f;
   float CurrentZoom          = 1;
   float MaxZoom              = 3.f;

   /** How far left and right we can pan */
   float MaxPanX = 1000.f;

   /** How far up and down we can pan */
   float MaxPanY = 1000.f;

   bool IsBounded = false;
};
