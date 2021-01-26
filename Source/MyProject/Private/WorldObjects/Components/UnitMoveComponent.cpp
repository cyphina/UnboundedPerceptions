#include "MyProject.h"
#include "UnitMoveComponent.h"
#include "MyAttributeSet.h"
#include "Unit.h"
#include "UpStatComponent.h"

void UUnitMoveComponent::BeginPlay()
{
   Cast<AUnit>(GetOwner())->GetStatComponent()->OnStatsUpdated().AddUObject(this, &UUnitMoveComponent::OnMoveSpeedChanged);
}

void UUnitMoveComponent::OnMoveSpeedChanged(const FGameplayAttribute& att, float newVal, AUnit* unitRef)
{
   if(att == UMyAttributeSet::GetMovementSpeedAttribute()) { 
      MaxWalkSpeed = newVal;
   }
}

