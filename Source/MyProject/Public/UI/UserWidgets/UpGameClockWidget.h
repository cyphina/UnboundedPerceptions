// Created 8/29/20 9:49 PM

#pragma once
#include "UserWidget.h"
#include "UpGameClockWidget.generated.h"

class UTextBlock;
struct FUpGameClock;

/** Displays an in game clock representing the time the story takes place (kinda of like Persona's date/time)
 * Might add things like weather or season in the future.
 */

UCLASS()
class MYPROJECT_API UUpGameClockWidget : public UUserWidget
{
   GENERATED_BODY()

 protected:
   void NativeOnInitialized() override;

 private:
   const FUpGameClock* gameClockRef = nullptr;

   void SetInitialClockDisplay() const;

   /** Seconds and minutes get property bindings because they update too frequently for callbacks to improve much*/
   void UpdateHour();
   void UpdateDay();
   void UpdateMonth();
   void UpdateYear();

   UPROPERTY(meta = (BindWidget))
   UTextBlock* Text_Seconds;

   UPROPERTY(meta = (BindWidget))
   UTextBlock* Text_Minutes;

   UPROPERTY(meta = (BindWidget))
   UTextBlock* Text_Hours;

   UPROPERTY(meta = (BindWidget))
   UTextBlock* Text_Day;

   UPROPERTY(meta = (BindWidget))
   UTextBlock* Text_Month;

   UPROPERTY(meta = (BindWidget))
   UTextBlock* Text_Year;
};
