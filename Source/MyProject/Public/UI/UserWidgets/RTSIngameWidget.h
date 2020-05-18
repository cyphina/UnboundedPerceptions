#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "RTSIngameWidget.generated.h"

class ARTSGameState;
class UToolTipWidget;
class URTSSidebarWidget;

/**
 * HUD for huds that are displayed during the core gameplay loop (excluding menu setting huds which can be seen even when the game isn't loaded or huds in other minigames)
 */
UCLASS()
class MYPROJECT_API URTSIngameWidget : public UMyUserWidget
{
   GENERATED_BODY()

   URTSIngameWidget();

   void NativeConstruct() override;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
   class AHUDManager* hudManagerRef;

   UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true)) //, category = "UIInitialParams", Meta = (ExposeOnSpawn = true))
   ARTSGameState* gameStateRef;

#pragma region GameTime

 private:
   /**Array with possible speed multipliers the game can run off of*/
   TArray<float> gamespeeds;

 public:
   UPROPERTY(BlueprintReadWrite)
   TArray<int> gameTime;

   UPROPERTY(BlueprintReadWrite)
   TArray<int> gameDate;

   UPROPERTY(BlueprintReadWrite)
   FText month;

   /**Used to index what speed to go at in our predefined game speeds*/
   UPROPERTY(BlueprintReadWrite)
   int speedIndex;

#pragma region TimeAccessors
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FText GetHour() const;
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FText GetMinute() const;
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FText GetAmPm() const;
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FText GetMonth() const;
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FText GetDay() const;
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FText GetYear() const;
   UFUNCTION(BlueprintCallable, BlueprintPure)
   FText GetDisplaySpeedText() const;
   UFUNCTION(BlueprintCallable)
   void SetGameSpeed();

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void SetClock(TArray<int> time) { gameTime = time; };

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void SetDate(TArray<int> date) { gameDate = date; }
#pragma endregion

#pragma endregion

#pragma region UI

   /**FText must be passed by ref due to localization resource gathering UPARAM(ref) seems to not work*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Help")
   void DisplayHelpText(const FText& hText);

   UFUNCTION(Blueprintcallable)
   URTSSidebarWidget* GetSidebarWidget() const { return sideBarWidget; }

 protected:

   UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
   URTSSidebarWidget* sideBarWidget;
};
