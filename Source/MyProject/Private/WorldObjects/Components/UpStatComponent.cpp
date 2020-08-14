#include "MyProject.h"
#include "UpStatComponent.h"
#include "WorldObjects/Unit.h"
#include "DamageStructs.h"
#include "UnitController.h"
#include "SpellSystem/RTSAbilitySystemComponent.h"

UUpStatComponent::UUpStatComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
}

void UUpStatComponent::BeginPlay()
{
   Super::BeginPlay();
   AUnit* unitOwnerRef = Cast<AUnit>(GetOwner());

   baseC = MakeUnique<FBaseCharacter>(*const_cast<UMyAttributeSet*>(unitOwnerRef->GetAbilitySystemComponent()->AddSet<UMyAttributeSet>()));
}

void UUpStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
