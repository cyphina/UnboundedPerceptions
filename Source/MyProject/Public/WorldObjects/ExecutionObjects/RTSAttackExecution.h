#pragma once

#include "UObject/Object.h"
#include "RTSAttackExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEPLAYABILITIES_API URTSAttackExecution : public UObject
{
   GENERATED_UCLASS_BODY()

 public:
   UFUNCTION(BlueprintNativeEvent, Category = "Attack Effect")
   void         ExecuteAttack();
   virtual void ExecuteAttack_Implementation();
};
