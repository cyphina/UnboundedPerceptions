// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESkillContainer.generated.h"

class USkillSlot;
class AAlly;

UCLASS()
class MYPROJECT_API UESkillContainer : public UUserWidget
{
	GENERATED_BODY()
	UPROPERTY(BlueprintSetter=SetAllyRef)
	AAlly* allyRef;
public:
	void NativeConstruct() override;

	FText manaHelpText;

	UPROPERTY(BlueprintReadWrite, Category = "Properties")
		TArray<USkillSlot*> skillSlots;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
		USkillSlot* GetSkill(int index) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Accessors")
		AAlly* GetAllyRef() const { return allyRef; } 

	UFUNCTION(BlueprintSetter, Category = "Accessors")
		void SetAllyRef(AAlly* ally) { allyRef = ally; }

	UFUNCTION(BlueprintCallable)
		void UseSkill(int skillToUseIndex);
};
