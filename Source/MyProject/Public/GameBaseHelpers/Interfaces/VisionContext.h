// Created 9/28/20 5:11 PM

#pragma once
#include "VisionContext.generated.h"

class AUnit;

UINTERFACE(MinimalAPI)
class UVisionContext : public UInterface
{
   GENERATED_BODY()
};

class IVisionContext
{
   GENERATED_BODY()

 public:
   FORCEINLINE virtual const TArray<AUnit*>& GetVisibleEnemies() const     = 0;
   FORCEINLINE virtual const TArray<AUnit*>& GetVisiblePlayerUnits() const = 0;

   virtual void StartVisionChecks() = 0;
   virtual void StopVisionChecks()  = 0;
};
