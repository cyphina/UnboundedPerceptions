#pragma once

#include "UserWidget.h"
#include "DIWidget.generated.h"

class AUnit;

/**
 * DEPRECIATED: Using RTSDamageNumber classes instead
 */
UCLASS()
class MYPROJECT_API UDIWidget : public UUserWidget
{
   GENERATED_BODY()

 public:
   UPROPERTY(BlueprintReadWrite, Category = "Target")
   AUnit* target;

   UPROPERTY(BlueprintReadWrite, Category = "Damage")
   int damage;

   virtual void NativeConstruct() override;
};
