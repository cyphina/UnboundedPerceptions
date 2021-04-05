#include "MyProject.h"
#include "UnitMoveComponent.h"
<<<<<<< HEAD
#include "MyAttributeSet.h"
=======
#include "RTSAttributeSet.h"
>>>>>>> componentrefactor
#include "Unit.h"
#include "UpStatComponent.h"

void UUnitMoveComponent::BeginPlay()
{
<<<<<<< HEAD
   Cast<AUnit>(GetOwner())->GetStatComponent()->GetAttSet()->statUpdatedEvent.AddUObject(this, &UUnitMoveComponent::OnMoveSpeedChanged);
}

void UUnitMoveComponent::OnMoveSpeedChanged(const FGameplayAttribute& att, float& newVal, AUnit* unitRef)
{
   if(att == UMyAttributeSet::MovementSpeedAttribute()) { 
=======
   Cast<AUnit>(GetOwner())->GetStatComponent()->OnStatsUpdated().AddUObject(this, &UUnitMoveComponent::OnMoveSpeedChanged);
}

void UUnitMoveComponent::OnMoveSpeedChanged(const FGameplayAttribute& att, float newVal, AUnit* unitRef)
{
   if(att == URTSAttributeSet::GetMovementSpeedAttribute()) { 
>>>>>>> componentrefactor
      MaxWalkSpeed = newVal;
   }
}

