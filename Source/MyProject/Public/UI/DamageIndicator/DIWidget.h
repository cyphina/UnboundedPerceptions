// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserWidget.h"
#include "DIWidget.generated.h"

class AUnit;

/**
 * C++ class for the widget that the widget component holds when
 */
UCLASS()
class MYPROJECT_API UDIWidget : public UUserWidget
{
   GENERATED_BODY()

 public:
   UPROPERTY(BlueprintReadWrite, Category = "Target")
   AUnit* target;

   UPROPERTY(BlueprintReadWrite, Category = "Damage")
   int damage;

   virtual void NativeConstruct() override;
};
