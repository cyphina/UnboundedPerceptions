// Created 7/30/20 4:5 AM

#pragma once

#include "RTSExecution.h"
#include "RTSMoveExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class MYPROJECT_API URTSMoveExecution : public public UObject, public IRTSExecution
{
   GENERATED_UCLASS_BODY()

 public:
   void Execute() override;
   void Execute_Implementation() override;
};
