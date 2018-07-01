// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Interactable.generated.h"

/**Interfaces are good when we want different objects in different hierarchies to be able to communicate
 * Interfaces take no memory because of empty base optimization (or so I assume)
 */

/**Use this uobject to expose the interface to blueprints*/
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

	/**Can this interactable be used?*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	bool 				CanInteract();
};
