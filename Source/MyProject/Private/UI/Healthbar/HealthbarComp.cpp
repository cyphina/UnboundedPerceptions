// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "WorldObjects/Unit.h"
#include "HealthbarComp.h"
#include "Healthbar.h"

UHealthbarComp::UHealthbarComp()
{
   SetWidgetSpace(EWidgetSpace::Screen);
   // SetRelativeLocation(FVector::ZeroVector);
}

void UHealthbarComp::BeginPlay()
{
   Super::BeginPlay();
   SetCollisionProfileName("NoCollision");
   healthBar = Cast<UHealthbar>(GetUserWidgetObject());
   unitRef   = Cast<AUnit>(GetOwner());

   // if(healthBar && unitRef)
   // GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, unitRef->GetName() + healthBar->GetName());
}

void UHealthbarComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   if (healthBar && unitRef) {
      healthPercentage = (unitRef->GetVitalCurValue(static_cast<int>(Vitals::Health)) / unitRef->GetVitalAdjValue(static_cast<int>(Vitals::Health)));
      healthBar->SetHealth(healthPercentage);
   }
}

void UHealthbarComp::EndPlay(const EEndPlayReason::Type EPR)
{
   Super::EndPlay(EPR);
   // ReleaseResources();
}
