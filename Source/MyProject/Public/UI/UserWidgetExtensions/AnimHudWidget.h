// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyUserWidget.h"
#include "WidgetAnimation.h"
#include "AnimHudWidget.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UAnimHudWidget : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   UWidgetAnimation* anim;
};
