// Created 8/29/20 11:38 PM

#include "MyProject.h"
#include "UpGameClock.h"
#include "UpGameClockWidget.h"
#include "RTSGameState.h"
#include "TextBlock.h"

void UUpGameClockWidget::NativeOnInitialized()
{
   gameClockRef = &Cast<ARTSGameState>(UGameplayStatics::GetGameState(GetOwningPlayer()))->GetGameClock();
   gameClockRef->GetOnHourPassedEvent().AddUObject(this, UpdateHour);
   gameClockRef->GetOnDayPassedEvent().AddUObject(this, UpdateDay);
   gameClockRef->GetOnMonthPassedEvent().AddUObject(this, UpdateMonth);
   gameClockRef->GetOnYearPassedEvent().AddUObject(this, UpdateYear);
   SetInitialClockDisplay();
}

void UUpGameClockWidget::SetInitialClockDisplay() const
{
   Text_Seconds->SetText(gameClockRef->GetSeconds());
   Text_Minutes->SetText(gameClockRef->GetMinute());
   Text_Hours->SetText(gameClockRef->GetHour());
   Text_Day->SetText(gameClockRef->GetDay());
   Text_Month->SetText(gameClockRef->GetMonth());
   Text_Year->SetText(gameClockRef->GetYear());
}

void UUpGameClockWidget::UpdateHour()
{
   Text_Hours->SetText(gameClockRef->GetHour());
}

void UUpGameClockWidget::UpdateDay()
{
   Text_Day->SetText(gameClockRef->GetDay());
}

void UUpGameClockWidget::UpdateMonth()
{
   Text_Month->SetText(gameClockRef->GetMonth());
}

void UUpGameClockWidget::UpdateYear()
{
   Text_Year->SetText(gameClockRef->GetHour());
}
