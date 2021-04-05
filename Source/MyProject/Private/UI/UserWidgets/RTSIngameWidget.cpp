#include "MyProject.h"
#include "RTSIngameWidget.h"

<<<<<<< HEAD
=======
#include "HUDManager.h"
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   if(worldRef) { Cast<AUserInput>(worldRef->GetFirstPlayerController())->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(hText); }
=======
   if(worldRef)
   {
      Cast<AUserInput>(worldRef->GetFirstPlayerController())->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(hText);
   }
>>>>>>> componentrefactor
}

void URTSIngameWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();
<<<<<<< HEAD
=======
   StaticClass();
>>>>>>> componentrefactor
}
