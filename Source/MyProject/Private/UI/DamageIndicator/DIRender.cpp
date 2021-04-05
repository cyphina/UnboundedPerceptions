// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DIRender.h"
#include "UserInput.h"
#include "Unit.h"
#include "RTSPawn.h"

UDIRender::UDIRender()
{
   PrimaryComponentTick.bCanEverTick = true;
   bAllowConcurrentTick              = true;
   bCanEverAffectNavigation          = false;
}

void UDIRender::BeginPlay()
{
   Super::BeginPlay();

   if(jumpCurve)
   {
      FOnTimelineFloat animDI;
      animDI.BindUFunction(this, FName("TimelineEffect"));
      tL.AddInterpFloat(jumpCurve, animDI, FName(TEXT("TimelineEffect")));
      tL.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
      tL.SetLooping(false);
      FOnTimelineEvent e;
      e.BindUFunction(this, "OnTimelineFinished");
      tL.SetTimelineFinishedFunc(e);
   }

   if(defaultTextMaterial)
   {
      SetTextMaterial(defaultTextMaterial);
   }

   cameraPawnRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
   tL.PlayFromStart();
   start = FVector::UpVector * (GetOwner()->GetSimpleCollisionHalfHeight() - 20) + FVector(FMath::RandRange(-20, 20), FMath::RandRange(-20, 20), 0);
   end   = start + FVector(0, 0, jumpHeight);
}

void UDIRender::TickComponent(float deltaSeconds, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
   // TODO: Make this look better and remove tick and replace with timer?
   Super::TickComponent(deltaSeconds, tickType, thisTickFunction);
   // Ensure damage values always facing camera even when camera is rotated
   SetWorldRotation(rotationOffset + cameraPawnRef->GetActorRotation());
   tL.TickTimeline(deltaSeconds);
}

void UDIRender::TimelineEffect(float val)
{
   const FVector newLocation = FMath::Lerp(start, end, val);
   SetRelativeLocation(newLocation);
}

void UDIRender::OnTimelineFinished()
{
   DestroyComponent(false);
}
