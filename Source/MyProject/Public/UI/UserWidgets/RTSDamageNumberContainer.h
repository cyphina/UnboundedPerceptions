#pragma once

#include "CoreMinimal.h"
#include "UserWidgetPool.h"
#include "Blueprint/UserWidget.h"
#include "RTSDamageNumberContainer.generated.h"

class AUnit;
class UOverlay;
class URTSDamageNumberWidget;
struct FUpDamage;

/**
 * Widget that appears over the unit's head when a new damage number needs to be spawned 
 */
UCLASS()
class MYPROJECT_API URTSDamageNumberContainer : public UUserWidget
{
   GENERATED_BODY()

public:
   URTSDamageNumberContainer(const FObjectInitializer& Initializer);

   void SetOwningUnit(AUnit* newOwningUnit);

   /** Called after damage animation ends and damage number needs to be cleaned up */
   void ReleaseDamageNumberWidget(URTSDamageNumberWidget* destroyedDamageNumberWidget);
   
protected:
   void NativeOnInitialized() override;
   void ReleaseSlateResources(bool bReleaseChildren) override;
   
   UPROPERTY(EditDefaultsOnly)
   TSubclassOf<URTSDamageNumberWidget> damageNumberWidgetClass;

   UPROPERTY(Meta=(BindWidget))
   UOverlay* damageNumberContainerWidget;

private:
   void OnDamageDealt(const FUpDamage& damageReceived);
   void OnHealingReceived(const FUpDamage& damageReceived);

   UPROPERTY(Transient)
   FUserWidgetPool widgetPool;

   AUnit*          owningUnit;
};
