#pragma once

#include "RTSExecution.h"
#include "RTSAttackExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class GAMEPLAYABILITIES_API URTSAttackExecution : public UObject, public IRTSExecution
{
   GENERATED_UCLASS_BODY()

 public:
   void Execute() override;
   void Execute_Implementation() override;
};
