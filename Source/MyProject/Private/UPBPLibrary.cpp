// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UPBPLibrary.h"
#include "WorldDataObject.h"

UWorldDataObject* UUPBPLibrary::CreateWorldDataObject(TSubclassOf<UWorldDataObject> objectClass, UObject* outer, FName name)
{

	UWorldDataObject* obj = NewObject<UWorldDataObject>(outer, objectClass, name);
	if(obj)
	{
		obj->Init();
		return obj;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Called CreateWorldDataObject on a non UWorldDataObject!"));
		return nullptr;
	}
}
