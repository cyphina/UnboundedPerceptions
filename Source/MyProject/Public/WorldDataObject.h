// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "WorldDataObject.generated.h"

/**
 * Subclass of object to get an initialization function.  Is an interface so we don't have to rewrite source code that already inherits from UObject
 */

UCLASS(BlueprintType)
class MYPROJECT_API UWorldDataObject : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init() PURE_VIRTUAL(UWorldDataObject::Init,);	
};

