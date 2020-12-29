// Created 8/30/20 8:25 PM

#include "UpGameClock.h"

#include "GameSpeedContext.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "Clock"

double FUpTime::TimeToClockwork() const
{
   return static_cast<double>(seconds + minutes * 60 + hours * 3600);
}

FUpGameClock::FUpGameClock(const IGameSpeedContext& gameSpeedContext, const FUpTime& clockStartTime, const FUpDate& clockStartDate) : gameSpeedContext(gameSpeedContext)
{
   calendar  = clockStartDate;
   clockwork = clockStartTime.TimeToClockwork();
   gameSpeedContext.OnGameSpeedUpdated().AddWeakLambda(this, [this](float newSpeedMultiplier) { speedMultiplier = newSpeedMultiplier; });
}

void FUpGameClock::TickClock(float deltaTime)
{
   clockwork += speedMultiplier * deltaTime;

   if(UNLIKELY(clockwork > hours * 3600)) {
      clockwork = 0;
      IncrementHour();
   }
}

void FUpGameClock::AddGameTime(FUpTime timeToAdd, FUpDate daysToAdd)
{
}

void FUpGameClock::IncrementHour()
{
   ++hours;
   if(UNLIKELY(hours == 24)) {
      hours = 1;
      IncrementDay();
   }
   OnHourPassedEvent.Broadcast();
}

void FUpGameClock::IncrementMonth()
{
   ++calendar.months;
   OnMonthPassedEvent.Broadcast();
   if(calendar.months > 12) {
      calendar.months = 1;
      IncrementYear();
   }
}

void FUpGameClock::IncrementDay()
{
   ++calendar.days;
   OnDayPassedEvent.Broadcast();
   if(UNLIKELY(calendar.days > UKismetMathLibrary::DaysInMonth(calendar.years, calendar.months))) {
      calendar.days = 1;
      IncrementMonth();
   }
}

void FUpGameClock::IncrementYear()
{
   ++calendar.years;
   OnYearPassedEvent.Broadcast();
}

FText FUpGameClock::GetSeconds() const
{
   return FText::AsNumber(floor(fmod(clockwork / 60, 60)));
}

FText FUpGameClock::GetMinute() const
{
   const float minutes = floor(fmod(clockwork / 60, 60));
   return minutes > 10 ? FText::AsNumber(minutes) : FText::FromString("0" + FString::FromInt(minutes));
}

FText FUpGameClock::GetHour() const
{
   return FText::AsNumber(hours);
}

FText FUpGameClock::GetAmPm() const
{
   return isPm() ? FText(LOCTEXT("TimeOfDay", "Pm")) : FText(LOCTEXT("TimeOfDay", "Am"));
}

FText FUpGameClock::GetDay() const
{
   return FText::AsNumber(calendar.days);
}

FText FUpGameClock::GetMonth() const
{
   switch(time.months) {
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

FText FUpGameClock::GetYear() const
{
   return FText::AsNumber(calendar.years, &FNumberFormattingOptions::DefaultNoGrouping());
}

#undef LOCTEXT_NAMESPACE
