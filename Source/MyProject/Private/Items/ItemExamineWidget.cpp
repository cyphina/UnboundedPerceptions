// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ItemExamineWidget.h"

#include "Button.h"
#include "HUDManager.h"

void UItemExamineWidget::NativeConstruct()
{
   btnClose->OnClicked.AddDynamic(this, &UItemExamineWidget::CloseMenu);
}

bool UItemExamineWidget::OnWidgetAddToViewport_Implementation()
{
   if(itemToDisplayID != -1) return true;
   return false;
}

void UItemExamineWidget::CloseMenu()
{
   itemToDisplayID = 0;
   hudManagerRef->AddHUD(EHUDs::HS_ExamineMenu);
}
