// Created 11/29/20 3:10 AM

#pragma once
#include "RiskComponent.generated.h"

class AUserInput;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API URiskComponent : public UActorComponent
{
   GENERATED_BODY()

 public:

 protected:
   AUserInput* controllerRef;

   void BeginPlay() override;

 private:
   float CalculateTargetRisk();
};
