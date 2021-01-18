#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "MainWidget.generated.h"

class UKeyRemapWidget;
class USaveLoadMenu;
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

public:
   URTSIngameWidget* GetIngameWidget() const { return ingameWidget; }

   UBreakMenu* GetBreakMenu() const { return breakMenu; }

   USettingsMenu* GetSettingsMenu() const { return settingsMenu; }

   USaveLoadMenu* GetSaveLoadMenu() const { return saveLoadMenu; }

   UKeyRemapWidget* GetRemapMenu() const { return keyRemapMenu; }

   
protected:
   UPROPERTY(BlueprintReadOnly)
   class AHUDManager* hudManagerRef;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   URTSIngameWidget* ingameWidget;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   USettingsMenu* settingsMenu;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   UBreakMenu* breakMenu;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   USaveLoadMenu* saveLoadMenu;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   UKeyRemapWidget* keyRemapMenu;
   
#pragma endregion
};
