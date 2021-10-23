#pragma once

#include "Components/WidgetComponent.h"
#include "DamageNumberWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UDamageNumberWidgetComponent : public UWidgetComponent
{
   GENERATED_BODY()

public:
   UDamageNumberWidgetComponent();

protected:
	virtual void BeginPlay() override;
};
