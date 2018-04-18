// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interactable.generated.h"

//Use this uobject to expose to blueprints the interface
UINTERFACE(BlueprintType)
class MYPROJECT_API UInteractable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class MYPROJECT_API IInteractable
{
	GENERATED_IINTERFACE_BODY()

public:	
	/** classes using this interface must implement this function 
	 * @param hero - The hero who interacted with this interactable
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void 				Interact(ABaseHero* hero);

	/** classes using this interface must implement this function */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	FVector 			GetInteractableLocation();
};
