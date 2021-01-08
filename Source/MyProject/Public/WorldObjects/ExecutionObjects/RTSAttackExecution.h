#pragma once

#include "RTSExecution.h"
#include "RTSAttackExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class URTSAttackExecution : public UObject, public IRTSExecution
{
   GENERATED_BODY()

 public:
   void Execute_Implementation() override;
};
