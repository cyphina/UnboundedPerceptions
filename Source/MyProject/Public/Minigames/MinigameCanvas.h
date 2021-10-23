// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinigameCanvas.generated.h"

UCLASS()
class MYPROJECT_API AMinigameCanvas : public AActor
{
	GENERATED_BODY()
	
public:	
	AMinigameCanvas();

protected:
	virtual void BeginPlay() override;

public:	

	const static int pixelCountProximityAmt = 1000;

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* canvasMesh;

	//Render target we'll be drawing on
	UPROPERTY(BlueprintReadWrite, Category = "Canvas Refs")
	UTextureRenderTarget2D* renderTarget;

	//Initial image of render target
	UPROPERTY(BlueprintReadWrite, Category = "Canvas Refs")
	UMaterialInstanceDynamic* canvasMaterial;

	//Brush texture
	UPROPERTY(BlueprintReadWrite, Category = "Canvas Refs")
	UMaterialInstanceDynamic* brushMaterial;

	//Check to see player draw a similar picture
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CheckDrawing();
};
