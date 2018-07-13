// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "DialogUI.generated.h"

class ANPC;

/**
 * Dialog widget for talking to NPCs, and IntimateNPCs,
 */

/**Current view for our social widget*/
UENUM(BlueprintType)
enum class ESocialHUDState : uint8
{
	/**View when talking to a random*/
	Conversation,
	/**View when talking to someone who you can gain relationship points*/
	Intimate,
	/**View when talking to a shopkeeper*/
	Shop
};

UCLASS()
class MYPROJECT_API UDialogUI : public UMyUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "References", Meta=(AllowPrivateAccess=true))
	ANPC*								npcRef = nullptr;

	/**Storage of the current view since after we press talk, we hide the view and eventually will need to get back to it.*/
	ESocialHUDState						socialHUDState;

public:

	void								Construct_Implementation() override;

	/** Called after greeting the IntimateNPC */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Callbacks")
	void								PostGreeting();

	/** View for NPCs which you can't gain friends from, but you can ask about topics from */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callbacks")
	void								SetConversationView();
	void								SetConversationView_Implementation() { socialHUDState = ESocialHUDState::Conversation; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callbacks")
	void								SetIntimateView();
	void								SetIntimateView_Implementation() { socialHUDState = ESocialHUDState::Intimate; };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callbacks")
	void								SetShopView();
	void								SetShopView_Implementation() { socialHUDState = ESocialHUDState::Shop; };

	UFUNCTION(BlueprintCallable, Category = "Callbacks")
	void								SetMainView();

	bool								OnWidgetAddToViewport_Implementation() override;
	void								SetNPC(ANPC* newNpcRef) { this->npcRef = newNpcRef; }
	ANPC*								GetNPC() { return npcRef; }
};
