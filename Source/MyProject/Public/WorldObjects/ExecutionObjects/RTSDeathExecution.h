#pragma once

#include "RTSExecution.h"
#include "RTSDeathExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
<<<<<<< HEAD
class URTSDeathExecution : public UObject, public IRTSExecution
{
   GENERATED_BODY()

 public:
   void Execute_Implementation() override;
=======
class URTSDeathExecution : public URTSExecution
{
   GENERATED_BODY()
>>>>>>> componentrefactor
};
