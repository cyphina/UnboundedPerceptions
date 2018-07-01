// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "MainWidget.h"
#include "Kismet/KismetMathLibrary.h"

ARTSGameState::ARTSGameState() {
	seconds = 0;
	minutes = 0;
	hours = 1;
	days = 15;
	months = 11;
	years = 2020;
	defaultGameTimeSpeed = 60;
	clockwork = 0;
}

void ARTSGameState::BeginPlay()
{
	Super::BeginPlay();
	//if there's some default settings for time, set it now
	clockwork += seconds + minutes * 60 + hours * 3600;

	//set game date
	gameDate.Init(0,arraySize);
	gameTime.Init(0,arraySize);
	gameDate[0] = days;
	gameDate[1] = months;
	gameDate[2] = years;

	//match initial speed
	timeUnit = 0; 
	UpdateGameSpeedDelegate.AddDynamic(this, &ARTSGameState::UpdateGameSpeed);
}


void ARTSGameState::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	clockwork += deltaSeconds * timeUnit;
	Clock();

	//if(floor(fmod(clockwork,SECONDS_IN_DAY) < SMALL_NUMBER))
	//{
	//	++days;
	//	Calendar();
	//}

}


void ARTSGameState::Clock()
{
	seconds = floor(fmod(clockwork, 60));
	minutes = floor(fmod(clockwork/60, 60));
	if(hours == 24)
	{
		hours = floor(fmod(clockwork/3600, 24)) + 1;
		if(hours == 0) //branching conditional faster than modulo?
		{
			++days;
			Calendar();
			clockwork = 0;
		}
	}

	gameTime[0] = seconds;
	gameTime[1] = minutes;
	gameTime[2] = hours;

	mainWidgetRef->SetClock(gameTime);
}

void ARTSGameState::Calendar()
{
	if(days > UKismetMathLibrary::DaysInMonth(years, months))
	{
		days = 1;
		++months;
		if(months > 12)
		{
			months = 1;
			++years;
		}
		gameDate[0] = days;
		gameDate[1] = months;
		gameDate[2] = years;
	}
	gameDate[0] = days;
	gameDate[1] = months;
	gameDate[2] = years;

	mainWidgetRef->SetDate(gameDate);
}

void ARTSGameState::UpdateGameSpeed(float speedMultiplier)
{
	speedModifier = speedMultiplier;
	timeUnit = speedModifier * defaultGameTimeSpeed;
}

void ARTSGameState::UpdateGameTime(int secs, int mins, int hrss)
{
	clockwork = clockwork + seconds + minutes * 60 + hours * 3600; 
}

void ARTSGameState::UpdateGameDay(int day, int month, int year)
{
	gameDate[0] = days;
	gameDate[1] = months;
	gameDate[2] = years;
}

void ARTSGameState::AddGameTime(FDateTime timeToAdd)
{
	
}







