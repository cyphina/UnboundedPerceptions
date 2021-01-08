#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "MainWidget.generated.h"

class URTSIngameWidget;
class USettingsMenu;
class UBreakMenu;

/**
 * The root widget of Up. It contains:
 * - All the widgets for the base game
 * - All the widgets specific to minigames
 * - Widgets that can be accessed even within the main menu or widgets common across minigames.
 */
UCLASS()
class MYPROJECT_API UMainWidget : public UMyUserWidget
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
   class AHUDManager* hudManagerRef;

 protected:
   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   URTSIngameWidget* ingameWidget;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   USettingsMenu* settingsMenu;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   UBreakMenu* breakMenu;

#pragma endregion
};
