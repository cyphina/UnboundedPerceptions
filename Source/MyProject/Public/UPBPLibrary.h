// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UPBPLibrary.generated.h"

/**
 * Library of important functionality 
 */

class UWorldDataObject;
class IWorldDataObject;

UCLASS(MinimalAPI)
class UUPBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/*Wrapper function for construct object from class.  Make sure class inherits from UWorldDataObject*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create WorldDataObject", BlueprintInternalUseOnly = "true"), Category = "Object Creation")
	static class UWorldDataObject*			CreateWorldDataObject(TSubclassOf<UWorldDataObject> objectClass, UObject* outer, FName name);
};
