// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ActionSlot.h"

#include "TextBlock.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "UI/UserWidgets/RTSIngameWidget.h"
#include "UI/UserWidgets/ToolTipWidget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Button.h"

TSubclassOf<UToolTipWidget> UActionSlot::toolTipWidgetClass = nullptr;

UActionSlot::UActionSlot(const FObjectInitializer& o) : UUserWidget(o)
{
   ConstructorHelpers::FClassFinder<UToolTipWidget> tooltipClass(TEXT("/Game/RTS_Tutorial/HUDs/HelpUI/GameIndicators/BP_ToolTipBox"));
   toolTipWidgetClass = tooltipClass.Class;
}

void UActionSlot::SetImage(UTexture2D* image)
{
   actionImage->SetBrushFromTexture(image, false);
}

void UActionSlot::SetInfo(FText newInfo)
{
   infoText->SetText(newInfo);
}

void UActionSlot::SetImageFromMaterial(UMaterialInstanceDynamic* image)
{
   actionImage->SetBrushFromMaterial(image);
}

UTexture2D* UActionSlot::GetImage() const
{
   return Cast<UTexture2D>(actionImage->Brush.GetResourceObject());
}

void UActionSlot::NativeConstruct()
{
   Super::NativeConstruct();
   btnAction->OnClicked.AddDynamic(this, &UActionSlot::OnBtnClick);
   btnAction->OnHovered.AddDynamic(this, &UActionSlot::OnBtnHover);
   CPCRef                           = Cast<AUserInput>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());
}

void UActionSlot::OnBtnHover()
{
   TWeakObjectPtr<UToolTipWidget> tooltipWidgetRef = CreateWidget<UToolTipWidget>(CPCRef, toolTipWidgetClass);
   ShowDesc(tooltipWidgetRef.Get());
   btnAction->SetToolTip(tooltipWidgetRef.Get());
}
