#pragma once
#include "Components/WidgetComponent.h"
#include "HealthbarComp.generated.h"

class UHealthbar;
class AUnit;
struct FUpDamage;

/**
 * WidgetComponent class containing a healthbar widget.  This component is attached ot our unit to display its health
 */
UCLASS()
class MYPROJECT_API UHealthbarComp : public UWidgetComponent
{
   GENERATED_BODY()

<<<<<<< HEAD
   UPROPERTY()
   AUnit*                   unitRef;

   float                    healthPercentage;

   UPROPERTY()
   UHealthbar*              healthBar;

   TSubclassOf<UUserWidget> widgetClass;

   void OnDamageReceived(const FUpDamage& damage);

public:
=======
 public:
>>>>>>> componentrefactor
   UPROPERTY(BlueprintReadWrite)
   UUserWidget* widgetRef;

   UHealthbarComp();

 protected:
   void BeginPlay() override final;
   void EndPlay(const EEndPlayReason::Type EPR) override final;

 private:
   UPROPERTY()
   AUnit* unitRef;

   float healthPercentage;

   UPROPERTY()
   UHealthbar* healthBar;

   TSubclassOf<UUserWidget> widgetClass;

   void OnDamageReceived(const FUpDamage& damage);
   void OnHealingReceived(const FUpDamage& damage);
};
