// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Blueprint/UserWidget.h"
#include "DragDropOperation.h"
#include "MyUserWidget.generated.h"


class AUserInput;

/**
* Parent from this widget so we can get some callbacks and drag it around
* Make sure to make dragabble parts visible, and non draggable parts self-hit-test-invisible
* Widgets that derive from this widget should be togglable by player controls and/or stored in the HUDManager
*/

UCLASS(abstract)
class MYPROJECT_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "References")
	AUserInput*				CPC;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callback")
	bool					OnWidgetAddToViewport();
	virtual bool			OnWidgetAddToViewport_Implementation(); //Callback whenever toggled on and off screen

	/*We overwrite construct instead of overriding NativeConstruct because it runs first... Construct actually calls NativeConstruct, or at least it goes before...*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "User Interface", meta = (Keywords = "Begin Play")) //blueprintcosmetic = not run on any servers
	void Construct();
	virtual void Construct_Implementation(); //Override CPCRef to the CameraPawnController Class
};
