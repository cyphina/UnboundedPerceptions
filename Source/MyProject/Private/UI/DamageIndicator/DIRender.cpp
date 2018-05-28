// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DIRender.h"
#include "UserInput.h"

AUserInput* UDIRender::controllerRef = nullptr;

UDIRender::UDIRender()
{
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curveFinder(TEXT("/Game/RTS_Tutorial/Blueprints/Effects/DamageIndicator/DICurve"));
	if (curveFinder.Succeeded())
	{
		FOnTimelineFloat animDI;
		animDI.BindUFunction(this, FName("Effect"));
		tL.AddInterpFloat(curveFinder.Object, animDI, FName(TEXT("TimelineEffect")));
		tL.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		tL.SetLooping(false);
	}
	PrimaryComponentTick.bCanEverTick = true;
}

void UDIRender::BeginPlay()
{
	Super::BeginPlay();
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

void UDIRender::TickComponent(float deltaSeconds, ELevelTick tickType, FActorComponentTickFunction * thisTickFunction)
{
	Super::TickComponent(deltaSeconds, tickType, thisTickFunction);
	SetWorldRotation(FRotator(0,-180,0) + controllerRef->GetCameraPawnRotation());
	TickTimeLine(deltaSeconds);
}

void UDIRender::TickTimeLine(float deltaTime)
{
	if (ready)
	{
		tL.PlayFromStart();
		start = RelativeLocation;
		end = start + FVector(0, 0, 100);
		
		ready = false;
	}

	if(tL.IsPlaying())
	{
		tL.TickTimeline(deltaTime);
	}
	else
	{
		DestroyComponent(false);
	}
}

void UDIRender::Effect(float val)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, RelativeLocation.ToString());
	FVector newLocation = FMath::Lerp(start, end, val);
	SetRelativeLocation(newLocation);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("EffectProgress: timeline: %f  value:%f"), tL.GetPlaybackPosition(), val));
}
