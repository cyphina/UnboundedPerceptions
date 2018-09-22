// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "WorldObject.h"

#if UE_EDITOR
#include <type_traits>
#endif

/** Singleton class for some important variables */
namespace ResourceManager
{
	extern TMap<FGameplayTag, FColor>			elementalMap;

	void										InitResourceManager();
	void										InitElementalMap();
	void										InitSupportTags();
	void										InitOffensiveTags();

	///<summary>
	///These template can generate alot of code if you use too many different types.  Probably not a problem considering how large codebase already is but just a note
	///</summary>

	/**Make sure when you pass a world reference to this class, you pass it one from an actor or from a function that has (meta=WorldContextObject)
	 *@param nameToMatch - Name of an object that implements IWorldObject
	 *@param worldRef - Reference to the world (from an actor or uobject with an actor outer)
	 */
	template<class T>
	T*											FindTriggerObjectInWorld(FString nameToMatch, UWorld* worldRef)
	{
#if UE_EDITOR
		static_assert(std::is_base_of<IWorldObject, T>::value, "Template parameter should derive from IWorldObject");
#endif

		for (TActorIterator<T> actItr(worldRef); actItr; ++actItr)
		{
			if ((*actItr)->GetGameName().ToString() == nameToMatch)
			{
				return *actItr;
			}
		}

#if UE_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Could not find object named %s"), *nameToMatch);
#endif
		return nullptr;
	}

	/**Executes a function inside an arbitrary UObject using UObject Reflection (granted T is a subclass of UObject)*/
	void ExecuteFunctionFromWorldObject(UObject* objectRef, FName functionToExecute, UWorld* worldRef);

	template<typename T>
	struct GetPropertyFromType;

	template<>
	struct GetPropertyFromType<float>
	{
		using value = UFloatProperty;	
	};

	template<>
	struct GetPropertyFromType<int>
	{
		using value = UIntProperty;
	};

	/**Executes a function inside an arbitrary UObject using UObject Reflection (granted T is a subclass of UObject)*/
	template<typename T>
	bool GetObjectVariable(UObject* objectRef, FName propertyToRead, T& outValue, UWorld* worldRef)
	{
		if (objectRef)
		{
			T foundVar;
			using Property = GetPropertyFromType<T>::value;
			Property p = FindField<Property>(wo->GetClass(), propertyToRead);
			if(p)
			{
				outValue = p->GetPropertyValue_InContainer(objectRef);
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool SetObjectVariable(UObject* objectRef, FName propertyToRead, T newValue, UWorld* worldRef)
	{
		if (objectRef)
		{
			T foundVar;
			using Property = GetPropertyFromType<T>::value;
			Property p = FindField<Property>(wo->GetClass(), propertyToRead);
			if(p)
			{
				outValue = p->SetPropertyValue_InContainer(objectRef, newValue);
				return true;
			}
		}
		return false;
	}

	//Tags that make an ability considered a support ability
	extern FGameplayTagContainer				supportTags;
	extern FGameplayTagContainer				offensiveTags;
}
