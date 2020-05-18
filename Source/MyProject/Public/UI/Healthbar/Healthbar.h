#pragma once

#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

/**
 * Widget used to display on the healthbar widget component
 */
UCLASS()
class MYPROJECT_API UHealthbar : public UUserWidget {
   GENERATED_BODY()

public:
   /**Prompt to redraw healthbar to correct percent*/
   UFUNCTION(BlueprintImplementableEvent, Category = "Accessors")
   void UpdateHealthbar(float value);
};
