// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "BreakMenu.generated.h"

/**
 * Widget class for widget that appears when we want to do things like save or quit
 */

class ARTSGameMode;
class UMyGameInstance;

UCLASS()
class MYPROJECT_API UBreakMenu : public UMyDraggableWidget
{
	GENERATED_BODY()
	
public: 

	void						Construct_Implementation() override;

private:

	ARTSGameMode*				gameModeRef;
	UMyGameInstance*			gameInstanceRef;

	UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
	void						Resume();

	UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
	void						SaveLoad();

	UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
	void						Options();

	UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
	void						Exit();
					
};
