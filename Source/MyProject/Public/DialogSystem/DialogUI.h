// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "DialogUI.generated.h"

class ANPC;

/**
 * Dialog widget for talking to NPCs, and IntimateNPCs,
 */

UCLASS()
class MYPROJECT_API UDialogUI : public UMyUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "References", Meta=(AllowPrivateAccess=true))
	ANPC* npcRef;

	/** True for conversation, false for intimate*/
	bool bConvView;

public:

	/** Called after greeting the IntimateNPC*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Callbacks")
	void								PostGreeting();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callbacks")
	void								SetConversationView();
	void								SetConversationView_Implementation() { bConvView = true; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callbacks")
	void								SetIntimateView();
	void								SetIntimateView_Implementation() { bConvView = false; }

	UFUNCTION(BlueprintCallable, Category = "Callbacks")
	void								SetMainView();

	bool								OnWidgetAddToViewport_Implementation() override;
	void								SetNPC(ANPC* newNpcRef) { this->npcRef = newNpcRef; }
	ANPC*								GetNPC() { return npcRef; }
};
