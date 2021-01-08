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
   /** Start to activate a skill on selected unit. May not activate skill, but request targeting first. */
   UFUNCTION(BlueprintCallable, Category = "Skills")
   void UseSkill(int skillIndex);

   /** View when tabbing through multiple selected allies */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void SingleAllyViewIndexFree(AAlly* allyToFocus);

   /** View when the focused unit dies. */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "View")
   void DeadUnitView();

   UESkillContainer* GetSkillContainer() const { return skillContainerRef; }

 protected:
   UPROPERTY(BlueprintReadWrite, Category = "References", meta = (BindWidget))
   UESkillContainer* skillContainerRef;

   UPROPERTY(BlueprintReadWrite, Category = "References", meta = (BindWidget))
   UChannelingBar* channelingBarRef;

   /** Can we drag this action bar around or (*EVENTUALLY*) lock it from changing units? */
   UPROPERTY(BlueprintReadWrite, Category = "Properties")
   bool isLocked;

   void NativeOnInitialized() override;

 private:
   void OnFocusedUnitChanged(AUnit* newFocusedUnit);
};
