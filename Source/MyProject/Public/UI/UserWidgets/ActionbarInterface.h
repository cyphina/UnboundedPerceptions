// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UserWidgetExtensions/MyUserWidget.h"
#include "ActionbarInterface.generated.h"


class AUnit;
class UESkillContainer;
class AAlly;

/*
 * Class that represents the action bar we see when selecting a unit/enemy
 * Includes skillbar, character skills, and tactics menu
 */

UCLASS()
class MYPROJECT_API UActionbarInterface : public UMyUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "References")
	UESkillContainer*			skillContainerRef;

	/** Can we drag this actionbar around or (*EVENTUALLY*) lock it from changing units?*/
	UPROPERTY(BlueprintReadWrite, Category = "Properties")
	bool						isLocked;

	/**Start to activate a skill on selected unit.  May not activate skill, but request targetting first*/
	UFUNCTION(BlueprintCallable, Category = "Skills")
	void						UseSkill(int skillIndex);

	/**Triggered when skill actually casted*/
	UFUNCTION(BlueprintCallable, Category = "Skills") 
	void						ShowSkillVisualCD(int skillIndex);

#pragma region views

public:
	/**Actionbar view when one unit is selected*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
	void						SingleAllyViewIndexFree(AAlly* allyToFocus);

#pragma endregion
};

