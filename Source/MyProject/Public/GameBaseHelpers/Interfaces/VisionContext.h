// Created 9/28/20 5:11 PM

#pragma once
#include "VisionContext.generated.h"

class AUnit;

UINTERFACE(MinimalAPI)
class UVisionContext : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IVisionContext
{
   GENERATED_IINTERFACE_BODY()

 public:
   FORCEINLINE virtual const TSet<AUnit*>& GetVisibleEnemies() const     = 0;
   FORCEINLINE virtual const TSet<AUnit*>& GetVisiblePlayerUnits() const = 0;
};
