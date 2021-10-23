#include "MyProject.h"
#include "UI/DamageIndicator/DamageNumberWidgetComponent.h"

UDamageNumberWidgetComponent::UDamageNumberWidgetComponent()
{
   SetWidgetSpace(EWidgetSpace::Screen);
}

void UDamageNumberWidgetComponent::BeginPlay()
{
   Super::BeginPlay();
   SetCollisionProfileName("NoCollision");
}
