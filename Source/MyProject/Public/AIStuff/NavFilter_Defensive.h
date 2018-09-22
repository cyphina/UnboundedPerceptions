// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavFilter_AIControllerDefault.h"
#include "NavFilter_Defensive.generated.h"

/**
 * Filter when defensively moving (actively try to avoid enemes) 
 */

UCLASS()
class MYPROJECT_API UNavFilter_Defensive : public UNavFilter_AIControllerDefault
{
	GENERATED_BODY()
	
	UNavFilter_Defensive();
};
