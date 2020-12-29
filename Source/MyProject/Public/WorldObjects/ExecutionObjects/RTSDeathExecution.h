#pragma once

#include "RTSExecution.h"
#include "URTSDeathExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEPLAYABILITIES_API URTSDeathExecution : public UObject, public IRTSExecution
{
   GENERATED_UCLASS_BODY()

 public:
   UFUNCTION(BlueprintNativeEvent, Category = "Attack Effect")
   void Execute() override;
   void Execute_Implementation() override;
};
