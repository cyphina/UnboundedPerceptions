// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "PopupWidget.generated.h"

/**
 * Base class for any popup windows that ask for user input
 */

UCLASS()
class MYPROJECT_API UPopupWidget : public UMyUserWidget
{
	GENERATED_BODY()

public:

	/**Changes header of the UI*/
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void				SetTitle(const FText& newText);

	/**Changes text description of popup UI*/
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void				SetDesc(const FText& newDesc);

	/**Calls the function when the confirmation is made*/
	UFUNCTION(BlueprintCallable)
	virtual void		Confirm() PURE_VIRTUAL(UPopupWidget::Confirm, );

};
