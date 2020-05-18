#include "MyProject.h"
#include "WorldObjects/Unit.h"
#include "HealthbarComp.h"
#include "Healthbar.h"
#include "UpDamageComponent.h"

UHealthbarComp::UHealthbarComp()
{
   SetWidgetSpace(EWidgetSpace::Screen);
}

void UHealthbarComp::BeginPlay()
{
   Super::BeginPlay();
   SetCollisionProfileName("NoCollision");
   healthBar = Cast<UHealthbar>(GetUserWidgetObject());
   unitRef   = Cast<AUnit>(GetOwner());
   unitRef->damageComponent->OnDamageTaken.AddUObject(this, &UHealthbarComp::SetWidgetHealth);
}

void UHealthbarComp::SetWidgetHealth(const FUpDamage& damage)
{
   healthBar->UpdateHealthbar(unitRef->GetVitalCurValue(EVitals::Health) / unitRef->GetVitalBaseValue(EVitals::Health));
}

void UHealthbarComp::EndPlay(const EEndPlayReason::Type EPR)
{
   Super::EndPlay(EPR);
}
