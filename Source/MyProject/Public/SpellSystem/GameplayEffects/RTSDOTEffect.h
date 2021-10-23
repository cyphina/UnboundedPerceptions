#pragma once

#include "RTSDamageEffect.h"
#include "RTSDOTEffect.generated.h"

/**
 * Base class for all DOT effects
 */
UCLASS()
class MYPROJECT_API URTSDOTEffect : public URTSDamageEffect
{
   GENERATED_BODY()

 public:
   URTSDOTEffect();
};
