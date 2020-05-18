// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSIngameWidget.h"
#include "RTSGameState.h"
#include "RTSSidebarWidget.h"
#include "UI/UserWidgets/ToolTipWidget.h"

#define LOCTEXT_NAMESPACE "MainWidget"

URTSIngameWidget::URTSIngameWidget()
{
   // set time and date to 1
   speedIndex  = 0;
   gameTime    = {1, 1, 1};
   gameDate    = {1, 1, 1};
   float arr[] = {0, 0.5, 1, 1.5, 2, 5};
   gamespeeds.Append(arr, UE_ARRAY_COUNT(arr));
}

void URTSIngameWidget::NativeConstruct()
{
   gameTime.Init(1, 3);
   gameDate.Init(1, 3);
   gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
   gameStateRef->ingameWidget = this;
   Super::NativeConstruct();
}

FText URTSIngameWidget::GetHour() const
{
   int timeHour = gameTime[2];
   if(timeHour > 12) // our hours are stored in military time but displayed as regular time
   {
      timeHour = timeHour - 12;
   }
   return FText::AsNumber(timeHour);
}

FText URTSIngameWidget::GetMinute() const
{
   int timeMinute = gameTime[1];
   if(timeMinute > 10)
      return FText::AsNumber(timeMinute);
   return FText::FromString("0" + FString::FromInt(timeMinute));
}

FText URTSIngameWidget::GetAmPm() const
{
   if(gameTime[2] < 12) {
      return FText(LOCTEXT("TimeOfDay", "Am"));
   } else {
      return FText(LOCTEXT("TimeOfDay", "Pm"));
   }
}

FText URTSIngameWidget::GetMonth() const
{
   switch(gameDate[1]) {
      case 1: return FText(LOCTEXT("TimeOfDay", "January")); break;
      case 2: return FText(LOCTEXT("TimeOfDay", "February")); break;
      case 3: return FText(LOCTEXT("TimeOfDay", "March")); break;
      case 4: return FText(LOCTEXT("TimeOfDay", "April")); break;
      case 5: return FText(LOCTEXT("TimeOfDay", "May")); break;
      case 6: return FText(LOCTEXT("TimeOfDay", "June")); break;
      case 7: return FText(LOCTEXT("TimeOfDay", "July")); break;
      case 8: return FText(LOCTEXT("TimeOfDay", "August")); break;
      case 9: return FText(LOCTEXT("TimeOfDay", "September")); break;
      case 10: return FText(LOCTEXT("TimeOfDay", "October")); break;
      case 11: return FText(LOCTEXT("TimeOfDay", "November")); break;
      case 12: return FText(LOCTEXT("TimeOfDay", "December")); break;
      default:
         UE_LOG(LogTemp, Warning, TEXT("ERROR MONTH VALUE INVALID"));
         return FText::GetEmpty();
         break;
   }
}

FText URTSIngameWidget::GetDay() const
{
   return FText::AsNumber(gameDate[0]);
}

FText URTSIngameWidget::GetYear() const
{
   return FText::AsNumber(gameDate[2], &FNumberFormattingOptions::DefaultNoGrouping());
}

FText URTSIngameWidget::GetDisplaySpeedText() const
{
   return FText::AsNumber(speedIndex);
}

void URTSIngameWidget::SetGameSpeed()
{
   gameStateRef->UpdateGameSpeedDelegate.Broadcast(gamespeeds[speedIndex]);
}

#undef LOCTEXT_NAMESPACE
