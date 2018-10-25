// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "ActionbarInterface.generated.h"

class AUnit;
class UESkillContainer;
class UChannelingBar;
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
   UESkillContainer* skillContainerRef;

   UPROPERTY(BlueprintReadWrite, Category = "References")
   UChannelingBar* channelingBarRef;

   /** Can we drag this actionbar around or (*EVENTUALLY*) lock it from changing units?*/
   UPROPERTY(BlueprintReadWrite, Category = "Properties")
   bool isLocked;

   /**Start to activate a skill on selected unit.  May not activate skill, but request targetting first*/
   UFUNCTION(BlueprintCallable, Category = "Skills")
   void UseSkill(int skillIndex);

   /**Triggered when skill actually casted*/
   UFUNCTION(BlueprintCallable, Category = "Skills")
   void ShowSkillVisualCD(int skillIndex);

#pragma region views

 public:
   /**View when tabbing through multiple selected allies.  (Index free since we don't tab based off the index in the party, but rather their selection index)*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void SingleAllyViewIndexFree(AAlly* allyToFocus);

#pragma endregion
};
