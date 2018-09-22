// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "NavFilter_Defensive.h"
#include "NavArea_EnemySpot.h"


UNavFilter_Defensive::UNavFilter_Defensive()
{
	FNavigationFilterArea filterArea;
	filterArea.AreaClass = UNavArea_EnemySpot::StaticClass();
	filterArea.bIsExcluded = true;
	Areas.Add(filterArea);
}
