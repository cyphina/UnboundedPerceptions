// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "SkillSlot.generated.h"

class UMySpell;
class UESkillContainer;
class UActionSlot;

UCLASS()
class MYPROJECT_API USkillSlot : public UUserWidget
{
	GENERATED_BODY()

	FTimeline						cdTimeline;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int								cdTimer;

public:
	UPROPERTY(BlueprintReadWrite) //Reference to skill container which holds this slot
	UESkillContainer*				eSkillContainer;

	UPROPERTY(BlueprintReadWrite) //make sure to set this up in bp -- cooldown shadow material
	UMaterialInstanceDynamic*		cdDMatInst;

	UPROPERTY(BlueprintReadWrite) //make sure to set this up in bp -- image material
	UMaterialInstanceDynamic*		imageDMatInst;

	UPROPERTY(BlueprintReadWrite) // a reference so we can use the widget's properties in our C++ code.  There's two references, one in BP which we use to set this one
	UActionSlot*					actionSlotRef;

	USkillSlot(const FObjectInitializer& o);

	UFUNCTION()
	void							NativeConstruct() override;

	UFUNCTION()
	void							NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/**Function that is called when this slot is activated*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void							PerformAction();

	/**Used to create the effect of the skillslot being filled as it goes off cooldown*/
	UFUNCTION(BlueprintCallable)
	void							TickTimeline();

	/**Can be used if this slot is replaced with another skill to update the timeline length*/
	UFUNCTION(BlueprintCallable, Category = "Binding Functions")
	void							UpdateTimelineCD(float newDuration);

	/**When the cd visuals need to be hided (time and image mostly)*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Binding Functions")
	void							HideCDVisuals();

	/**When the cd visuals need to be shown (time and image mostly)*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void							ShowCDVisuals();

	/*Special playtimeline function used to keep playing at the point at which a spell is on cd.  Used if skill on cd swapped to new slot*/
	UFUNCTION(BlueprintCallable, Category = "Timeline Exposure")
	void							PlayTimeline(float startingPoint);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void							SetImage(UTexture2D* image);

	/**Used wehn changing the skill to a new one*/
	UFUNCTION(BlueprintCallable, Category = "Update skills")
	void							UpdateSkillSlot(TSubclassOf<UMySpell> spell);
};
