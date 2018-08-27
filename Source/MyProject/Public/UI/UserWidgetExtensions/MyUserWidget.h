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

	/**I used to think Construct ran before NativeConstruct(), but I didn't realize that calling the Super function of NativeConstruct called Construct() and I usually
	 *called the parent class NativeConstruct first so I thought it went before >_<*/
	virtual void NativeConstruct(); 
};
