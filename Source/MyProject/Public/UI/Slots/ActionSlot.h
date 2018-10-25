// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionSlot.generated.h"

/**
 * Designed for composition
 */

class AUserInput;

UCLASS()
class MYPROJECT_API UActionSlot : public UUserWidget
{
   GENERATED_BODY()

 public:
   UPROPERTY(BlueprintReadOnly)
   AUserInput* CPCRef;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action")
   UTexture2D* actionImage;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Action", Meta = (ExposeOnSpawn = true))
   int slotIndex;

   UPROPERTY(BlueprintReadWrite, Category = "Action")
   FText info;

   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action")
   void SetImage(UTexture2D* image);

   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Action")
   void SetImageFromMaterial(UMaterialInstanceDynamic* image);

   void NativeConstruct() override;
};
