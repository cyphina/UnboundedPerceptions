// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUserWidget.h"
#include "UserInput.h"
#include <any>

#include "PanelWidget.h"

struct FMyWidgetEventParams
{
   TArray<std::any> params;
   FReply           reply;
};

void UMyUserWidget::OnWidgetRemovedFromViewport_Implementation()
{
}

void UMyUserWidget::DispatchEvent(FName eventName)
{
   // UWidget* widget = this;
   // do
   // {
   //    UFunction* function = widget->FindFunction(*(FString("On") + eventName.ToString()));
   //    if(!function)
   //    {
   //       return;
   //    }
   //
   //    // TODO: Update params for every widget that handles this
   //    FMyWidgetEventParams params;
   //    ProcessEvent(function, &params);
   //
   //    if(!params.reply.IsEventHandled())
   //    {
   //       widget = GetParent();
   //    }
   //    else
   //    {
   //       return;
   //    }
   // }
   // while(widget);
}

void UMyUserWidget::SetVisibility(ESlateVisibility newVisibility)
{
   Super::SetVisibility(newVisibility);
   OnMyWidgetVisibilityChanged(newVisibility);
}

void UMyUserWidget::NativeConstruct()
{
   CPC = Cast<AUserInput>(GetOwningPlayer());
   Super::NativeConstruct();
}

bool UMyUserWidget::OnWidgetAddToViewport_Implementation()
{
   return true;
}
