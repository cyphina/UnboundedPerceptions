#pragma once

class IGameSpeedContext;
DECLARE_MULTICAST_DELEGATE(FOnHourPassed);
DECLARE_MULTICAST_DELEGATE(FOnDayPased);
DECLARE_MULTICAST_DELEGATE(FOnMonthPassed);
DECLARE_MULTICAST_DELEGATE(FOnYearPassed);

/** Struct to hold units of time needed by the ingame clock */

struct FUpTime {
   FUpTime() = default;

   FUpTime(uint8 seconds, uint8 minutes, uint8 hours) : seconds(seconds), minutes(minutes), hours(hours) {}

   uint8 seconds;
   uint8 minutes;
   uint8 hours;

   double TimeToClockwork() const;
};

struct FUpDate {
   FUpDate() = default;

   FUpDate(uint8 days, uint8 months, uint8 years) : days(days), months(months), years(years) {}

   uint8 days;
   uint8 months;
   uint8 years;
};

/** A class that encapsulates a ticking clock used to have some notion of time within the game.
  * Does not actually store times (but it does cache the hour). It does however store dates. */
USTRUCT(NoExport)
struct FUpGameClock {
   FUpGameClock(const IGameSpeedContext& gameSpeedContext, const FUpTime& clockStartTime, const FUpDate& startDate);

   /** Call each frame to update the clock */
   void TickClock(float deltaSeconds);
   
   /** Used only when events occur to modfiy the date like maybe the main character goes into a coma */
   void AddGameTime(FUpTime timeToAdd, FUpDate daysToAdd);
   
   /** Completely replace the date the game takes place on */
   void SetGameTime(FUpTime newTime, FUpDate newDate);
   
   FText GetSeconds() const;
   FText GetMinute() const;
   FText GetHour() const;
   FText GetAmPm() const;
   FText GetMonth() const;
   FText GetDay() const;
   FText GetYear() const;

   FOnHourPassed  GetOnHourPassedEvent() const { return OnHourPassedEvent; }
   FOnDayPased    GetOnDayPassedEvent() const { return OnDayPassedEvent; }
   FOnMonthPassed GetOnMonthPassedEvent() const { return OnMonthPassedEvent; }
   FOnYearPassed  GetOnYearPassedEvent() const { return OnYearPassedEvent; }

 private:
   const IGameSpeedContext& gameSpeedContext;

   /* This clockwork accumulates seconds in real time
    * scaled by a certain factor to represent the flow of time in the game is faster (like harvest moon) 
    * When one day's worth of time passes, clockwork is reset. */
   double clockwork       = 0;
   float  speedMultiplier = 1.f;
   uint8  hours           = 1;
   /** We only store date values - other values are calculated from the clockwork as needed */
   FUpDate calendar;

   /** These events are used to update the widget that shows time so they don't have to query the time every frame */
   mutable FOnHourPassed  OnHourPassedEvent;
   mutable FOnDayPased    OnDayPassedEvent;
   mutable FOnMonthPassed OnMonthPassedEvent;
   mutable FOnYearPassed  OnYearPassedEvent;

   FORCEINLINE bool isPm() const { return hours >= 12; }

   void IncrementHour();
   void IncrementMonth();
   void IncrementYear();
   void IncrementDay();
};
