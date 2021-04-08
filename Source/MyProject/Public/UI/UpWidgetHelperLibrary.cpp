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
