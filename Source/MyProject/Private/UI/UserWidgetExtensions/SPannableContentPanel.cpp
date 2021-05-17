// Created 5/10/21 6:39 PM

#include "MyProject.h"
#include "SPannableContentPanel.h"
#include "SConstraintCanvas.h"

SPannableContentPanel::SPannableContentPanel()
{
}

void SPannableContentPanel::Construct(const FArguments& InArgs)
{
   if(InArgs._OnMouseButtonDown.IsBound())
   {
      SetOnMouseButtonDown(InArgs._OnMouseButtonDown);
   }

   if(InArgs._OnMouseMove.IsBound())
   {
      SetOnMouseMove(InArgs._OnMouseMove);
   }

   // When using the UMG wrapper the setting on the UMG wrapper overrides this
   SetClipping(EWidgetClipping::ClipToBounds);

   ScrollZoomMultiplier = InArgs._ScrollZoomMultiplier;
   PanSpeed             = InArgs._PanSpeed;
   IsBounded            = InArgs._IsBounded;
   MaxPanX              = InArgs._MaxPanX;
   MaxPanY              = InArgs._MaxPanY;

   ChildSlot.Padding(InArgs._Padding)[SAssignNew(OffsetCanvas, SConstraintCanvas)];

   OffsetCanvasContentSlot = &OffsetCanvas->AddSlot().AutoSize(true).Alignment(FVector2D(0, 0));
   (*OffsetCanvasContentSlot)[InArgs._Content.Widget];
}

void SPannableContentPanel::SetContent(TSharedRef<SWidget> InContent)
{
   OffsetCanvas->ClearChildren();
   OffsetCanvas->AddSlot()[InContent].AutoSize(true).Alignment(FVector2D(0, 0)).Offset(InitialOffset);
}

const TSharedRef<SWidget>& SPannableContentPanel::GetContent() const
{
   return OffsetCanvasContentSlot->GetWidget();
}

void SPannableContentPanel::ClearContent()
{
   OffsetCanvasContentSlot->DetachWidget();
}

void SPannableContentPanel::SetPadding(const TAttribute<FMargin>& InPadding)
{
   SetAttribute(ChildSlot.SlotPadding, InPadding, EInvalidateWidgetReason::Layout);
}

FReply SPannableContentPanel::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
   return FReply::Handled();
}

FReply SPannableContentPanel::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
   if(MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
   {
      if(const TAttribute<FMargin>& PreviousPositionAtt = OffsetCanvasContentSlot->OffsetAttr; PreviousPositionAtt.IsSet())
      {
         const FMargin& PreviousPosition = PreviousPositionAtt.Get();
         const FMargin  NewPosition{PreviousPosition.Left + MouseEvent.GetCursorDelta().X * (PanSpeed / CurrentZoom),
                                   PreviousPosition.Top + MouseEvent.GetCursorDelta().Y * (PanSpeed / CurrentZoom), 0, 0};

         if(NewPosition.Left < -MaxPanX || NewPosition.Left > MaxPanX / 2 || NewPosition.Top > MaxPanY / 2 || NewPosition.Top < -MaxPanY)
         {
            return FReply::Handled();
         }

         OffsetCanvasContentSlot->Offset(NewPosition);
      }
   }
   return FReply::Handled();
}

FReply SPannableContentPanel::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
   SCompoundWidget::OnMouseWheel(MyGeometry, MouseEvent);
   const float Delta = MouseEvent.GetWheelDelta() * ScrollZoomMultiplier;
   if(TOptional<FSlateRenderTransform> Transform = OffsetCanvas->GetRenderTransform(); Transform.IsSet())
   {
      const FScale2D PreviousScale = Transform.GetValue().GetMatrix().GetScale();
      const FScale2D NewScale      = FScale2D(PreviousScale.GetVector() + Delta);

      if(NewScale.GetVector().X > 0.1f && NewScale.GetVector().X < MaxZoom)
      {
         CurrentZoom = NewScale.GetVector().X;
         OffsetCanvas->SetRenderTransform(FSlateRenderTransform(NewScale));
      }
   }
   else
   {
      const FScale2D NewScale = FScale2D(1 + Delta);
      CurrentZoom             = NewScale.GetVector().X;
      OffsetCanvas->SetRenderTransform(FSlateRenderTransform(NewScale));
   }
   return FReply::Handled();
}
