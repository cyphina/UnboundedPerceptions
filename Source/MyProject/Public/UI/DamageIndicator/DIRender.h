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
UCLASS()
class MYPROJECT_API UDIRender : public UTextRenderComponent
{
   GENERATED_BODY()

   FTimeline tL;

 public:
   FVector            start, end;
   static AUserInput* controllerRef;

   UDIRender();
   void BeginPlay() override;
   void TickComponent(float deltaSeconds, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

   // Function called from the timeline on a tick
   UFUNCTION()
   void TimelineEffect(float val);

   // Function called when timeline ends
   UFUNCTION()
   void OnTimelineFinished();

   UPROPERTY(BlueprintReadWrite, Category = "Target")
   AUnit* target;
};
