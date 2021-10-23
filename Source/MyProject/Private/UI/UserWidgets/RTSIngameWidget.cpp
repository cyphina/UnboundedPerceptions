#include "MyProject.h"
#include "RTSIngameWidget.h"

#include "HUDManager.h"
#include "HUDProvider.h"
#include "RTSGameState.h"
#include "RTSSidebarWidget.h"
#include "UserInput.h"
#include "UI/UserWidgets/ToolTipWidget.h"

URTSIngameWidget::URTSIngameWidget()
{
}

void URTSIngameWidget::NativeDisplayHelpText(UWorld* worldRef, const FText& hText)
{
   if(worldRef)
   {
      Cast<AUserInput>(worldRef->GetFirstPlayerController())->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(hText);
   }
}

void URTSIngameWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   StaticClass();
}
