// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogOfWarManager.generated.h"

UCLASS()
class MYPROJECT_API AFogOfWarManager : public AActor
{
	GENERATED_UCLASS_BODY()

	virtual ~AFogOfWarManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	//UFUNCTION(BlueprintNativeEvent)
		//void OnFowTextureUpdated(UTexture2D* currentTexture, UTexture2D* lastTexture);

protected:
	// Called when the game starts or when spawned


public:	
	// Called every frame

	
	
};
