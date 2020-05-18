// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ToolTipWidget.h"
#include "UMG/Public/Components/TextBlock.h"

void UToolTipWidget::SetTitleText(const FText& newText)
{
   titleText->Text = newText;
}

void UToolTipWidget::SetDescText(const FText& newText)
{
   descText->Text = newText;
}

void UToolTipWidget::SetMoreInfoText(const FText& newText)
{
   moreInfoText->Text = newText;
}

void UToolTipWidget::SetEvenMoreInfoText(const FText& newText)
{
   evenMoreInfoText->Text = newText;
}

void UToolTipWidget::SetMetaDataText(const FText& newText)
{
   metaDataText->Text = newText;
}

void UToolTipWidget::SetupTTBoxText(const FText& name, const FText& ttText, const FText& ttText2, const FText& ttText3, const FText& ttText4)
{
   SetTitleText(name);
   SetDescText(ttText);
   SetMoreInfoText(ttText2);
   SetEvenMoreInfoText(ttText3);
   SetMetaDataText(ttText4);
}
