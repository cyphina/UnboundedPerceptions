// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "MainWidget.generated.h"

/**
 * Class for Main Overlay
 */

class ARTSGameState;

UCLASS()
class MYPROJECT_API UMainWidget : public UMyUserWidget
{
	GENERATED_BODY()
	
#pragma region CallbacksAndReferences
public:
	UMainWidget();
	void							Construct_Implementation() override;

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = true))//, category = "UIInitialParams", Meta = (ExposeOnSpawn = true))
	ARTSGameState*					gameStateRef;
#pragma endregion

#pragma region GameTime

private:
	/**Array with possible speed multipliers the game can run off of*/
	TArray<float>					gamespeeds;

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<int>						gameTime;

	UPROPERTY(BlueprintReadWrite)
	TArray<int>						gameDate;

	UPROPERTY(BlueprintReadWrite)
	FText							month;

	/**Used to index what speed to go at in our predefined game speeds*/
	UPROPERTY(BlueprintReadWrite)
	int								speedIndex;

#pragma region TimeAccessors
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText							GetHour() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText							GetMinute() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText							GetAmPm() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText							GetMonth() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText							GetDay() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText							GetYear() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText							GetDisplaySpeedText() const;
	UFUNCTION(BlueprintCallable)
	void							SetGameSpeed();
#pragma endregion

#pragma endregion

#pragma region UI
	/**FText must be passed by ref due to localization resource gathering UPARAM(ref) seems to not work*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Help") 
	void							DisplayHelpText(const FText& hText);
	
	/**tooltip box display -- ttText is description and ttText2 is another box for more situational information
	 * @param name - Title block for the tooltip window
	 * @param ttText - First block for information.  If any ttText's are empty strings, the corresponding blocks will be hidden
	 * @param ttText2 - Second block for information
	 * @param ttText3 - Third block for information
	 * @param ttText4 - Fourth block for information
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Help")
	void							DisplayTTBoxText(const FText& name, const FText& ttText, const FText& ttText2, const FText& ttText3, const FText& ttText4); 

	/**Needed to move description box when hovering over action slot*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "Accessor")
	UUserWidget*					GetDescriptionBox(); 

	/**Change color of unit text when selected -- Not used anymore*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Accessor")
	void							ChangeSelectionColors();

#pragma endregion

};
