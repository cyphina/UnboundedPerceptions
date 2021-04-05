// Created 7/30/20 4:53 AM

#pragma once

#include "RTSExecution.h"
#include "RTSMoveExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
<<<<<<< HEAD
class URTSMoveExecution : public UObject, public IRTSExecution
{
   GENERATED_BODY()

 public:
   void Execute_Implementation() override;
=======
class URTSMoveExecution : public URTSExecution
{
   GENERATED_BODY()

>>>>>>> componentrefactor
};
