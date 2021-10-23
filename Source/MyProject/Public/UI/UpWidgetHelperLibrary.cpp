// Created 4/7/21 8:46 PM

#include "MyProject.h"
#include "UpWidgetHelperLibrary.h"

#include "HUDProvider.h"
#include "RTSIngameWidget.h"
#include "UserInput.h"

void UUpWidgetHelperLibrary::DisplayHelpText(UObject* WorldContextObject, const FText& helpText)
{
   Cast<AUserInput>(WorldContextObject->GetWorld()->GetFirstPlayerController())->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(helpText);
}

void UUpWidgetHelperLibrary::Up_DrawBox(FPaintContext& Context, FVector2D Position, FVector2D Size, FSlateBrush Brush, FLinearColor Tint)
{
   Context.MaxLayer++;
   FSlateDrawElement::MakeBox(Context.OutDrawElements, Context.MaxLayer, Context.AllottedGeometry.ToPaintGeometry(Position, Size), &Brush, ESlateDrawEffect::None, Tint);
}

void UUpWidgetHelperLibrary::DrawSpline(FPaintContext& Context, FVector2D Position, FVector2D Size, FVector2D P0, FVector2D P1, FVector2D P2, FVector2D P3,
                                        float Thickness, FLinearColor Tint)
{
   Context.MaxLayer++;
   FSlateDrawElement::MakeCubicBezierSpline(Context.OutDrawElements, Context.MaxLayer, Context.AllottedGeometry.ToPaintGeometry(Position, Size), P0, P1, P2, P3,
                                            Thickness, ESlateDrawEffect::None, Tint);
}

void UUpWidgetHelperLibrary::DrawDrawSpaceSpline(FPaintContext& Context, FVector2D P0, FVector2D P1, FVector2D P2, FVector2D P3,
                                                 float Thickness, FLinearColor Tint)
{
   Context.MaxLayer++;
   FSlateDrawElement::MakeDrawSpaceSpline(Context.OutDrawElements, Context.MaxLayer, P0, P1, P2, P3, Thickness, ESlateDrawEffect::None, Tint);
}
