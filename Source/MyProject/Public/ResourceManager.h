// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"

/** Singleton class for some important variables */
namespace ResourceManager 
{
	extern TMap<FGameplayTag, FColor>			elementalMap;


	void										InitResourceManager();
	void										InitElementalMap();
	void										InitSupportTags();

	//Make sure when you pass a world reference to this class, you pass it one from an actor or from a function that has (meta=WorldContextObject)
	template<class T>
	T*											FindTriggerObjectInWorld(FString nameToMatch, UWorld* worldRef)
	{
		for (TActorIterator<T> actItr(worldRef); actItr; ++actItr)
		{
			if (UKismetSystemLibrary::GetDisplayName(*actItr) == nameToMatch)
			{
				return *actItr;
			}
		}
#if UE_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Could not find object named %s"), *nameToMatch);
#endif
		return nullptr;
	}

	//Tags that make an ability considered a support ability
	extern FGameplayTagContainer				supportTags; 
}
