// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "UnitQueryGenerator.generated.h"

/**
 * Generator that returns all the visible units that should be checked.  Not really relevant... yet.  Probably more efficient to test the items manually at
 * this point
 */

class AUnit;

UCLASS()
class MYPROJECT_API UUnitQueryGenerator : public UEnvQueryGenerator
{
	GENERATED_BODY()

	UUnitQueryGenerator();
	 
	/**Generates the items*/
	virtual void GenerateItems(FEnvQueryInstance& queryInstance) const override;

	///**What type of units should be checked*/
	//UPROPERTY(EditAnywhere, Category = "UnitProperties")
	//TSubclassOf<AUnit>			unitClass;
};
