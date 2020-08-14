#pragma once

#include "Components/ActorComponent.h"
#include "UUpPhaseComponent.generated.h"

/*
 * This component is very important for more complex AI
 * It allows us to store functionality about units drastically changing their AI
 * This means changing behavior and/or the spells they prioritize.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpPhaseComponent : public UActorComponent
{
   GENERATED_BODY()
};