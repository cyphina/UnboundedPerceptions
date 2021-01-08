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
   UWidgetAnimation* GetAnim() const { return anim; }

 protected:
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   UWidgetAnimation* anim;

   void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
