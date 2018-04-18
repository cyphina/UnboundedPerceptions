// Fill out your copyright notice in the Description page of Project Settings.

//Widget Component used to attach our Healthbar widget to our Character
#pragma once
#include "Components/WidgetComponent.h"
#include "HealthbarComp.generated.h"

class UHealthbar;
class AUnit;

/**
 * Class so we can interact with this component in C++
 */
UCLASS()
class MYPROJECT_API UHealthbarComp : public UWidgetComponent
{
	GENERATED_BODY()
	AUnit* unitRef;
	float healthPercentage;
	UHealthbar* healthBar;
	TSubclassOf<UUserWidget> widgetClass;
public:
	UPROPERTY(BlueprintReadWrite)
		UUserWidget* widgetRef;

	UHealthbarComp();
		void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;		
	void EndPlay(const EEndPlayReason::Type EPR) override;
};
