// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESkillContainer.generated.h"

class USkillSlot;
class UMySpell;
class AAlly;
class UManualSpellComponent;

/**
 * @brief Widget to display a skill ring when a unit is selected. Players can select skills from the ring ot cast, and swap positions on the ring
 * to control what skill binds to what hotkey.
 */
UCLASS()
class MYPROJECT_API UESkillContainer : public UUserWidget
{
   GENERATED_BODY()

 public:
   void NativeConstruct() override;

   UPROPERTY(BlueprintReadWrite, Category = "Properties")
   TArray<USkillSlot*> skillSlots;

   /**Get a skillslot from this container*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
   USkillSlot* GetSkillSlot(int index) const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors") AAlly* GetAllyRef() const { return allyRef; }

   UFUNCTION(BlueprintSetter, Category = "Accessors")
   void SetManualSpellComponentRef(UManualSpellComponent* spellComp) { manualSpellComponent = spellComp; }

   UFUNCTION(BlueprintCallable)
   void UseSkill(int skillToUseIndex);

 private:
   UPROPERTY(BlueprintSetter = SetAllyRef)
   UManualSpellComponent* manualSpellComponent;
};
