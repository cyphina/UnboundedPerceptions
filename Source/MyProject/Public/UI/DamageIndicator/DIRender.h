// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "DIRender.generated.h"

class AUnit;
class AUserInput;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UDIRender : public UTextRenderComponent
{
	GENERATED_BODY()

	FTimeline tL;
	bool isPlaying; //timeline bool

public:
	bool ready = false;
	FVector start, end;
	static AUserInput* controllerRef;

	UDIRender();
	void BeginPlay() override;
	void TickComponent(float deltaSeconds, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	//Function that gets called from the timer to call effect
	void TickTimeLine(float deltaTime);

	//Function called from the timeline on a tick
	UFUNCTION()
		void Effect(float val);

	UPROPERTY(BlueprintReadWrite, Category = "Target")
		AUnit* target;
};
