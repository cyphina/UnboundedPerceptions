// Fill out your copyright notice in the Description page of Project Settings.

// Widget used for our healthbar
#pragma once

#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UHealthbar : public UUserWidget
{
   GENERATED_BODY()

   float healthPercent;

 public:
   UFUNCTION(BlueprintCallable, Category = "Accessors")
   void SetHealth(float value);

   UFUNCTION(BlueprintCallable, Category = "Accessors")
   float GetHealth() const;
};
