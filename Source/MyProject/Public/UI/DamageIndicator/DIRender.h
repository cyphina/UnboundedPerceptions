// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "DIRender.generated.h"

class AUnit;
class AUserInput;

/**
 * Text render component displays text somewhere relative to its parent actor.  This subclass is specifically used to show texts that represent damage amounts and combat
 * occurences.
 */
UCLASS(BlueprintType, Blueprintable)
class MYPROJECT_API UDIRender : public UTextRenderComponent
{
   GENERATED_BODY()

   FTimeline tL;

 public:
   FVector            start, end;
   TWeakObjectPtr<APawn> cameraPawnRef;

   UPROPERTY(EditDefaultsOnly)
   float jumpHeight = 80.f;

   UPROPERTY(EditDefaultsOnly)
   float textSize = 35.f;

   UDIRender();
   void BeginPlay() override final;
   void TickComponent(float deltaSeconds, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override final;

   // Function called from the timeline on a tick
   UFUNCTION()
   void TimelineEffect(float val);

   // Function called when timeline ends
   UFUNCTION()
   void OnTimelineFinished();
};
