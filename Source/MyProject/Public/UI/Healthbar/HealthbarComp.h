#pragma once
#include "Components/WidgetComponent.h"
#include "HealthbarComp.generated.h"

class UHealthbar;
class AUnit;
struct FUpDamage;

/**
 * WidgetComponent class containing a healthbar widget.  This component is attached to our unit to display its health
 */
UCLASS()
class MYPROJECT_API UHealthbarComp : public UWidgetComponent
{
   GENERATED_BODY()

 public:
   UHealthbarComp();

 protected:
   void BeginPlay() override final;
   void EndPlay(const EEndPlayReason::Type EPR) override final;

 private:
   UPROPERTY()
   AUnit* unitRef;

   UPROPERTY()
   UHealthbar* healthBar;

   void OnDamageReceived(const FUpDamage& damage);
   void OnHealingReceived(const FUpDamage& damage);
};
