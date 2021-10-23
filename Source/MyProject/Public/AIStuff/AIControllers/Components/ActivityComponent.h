#pragma once

#include "Components/ActorComponent.h"
#include "ActivityComponent.generated.h"

/*
 * This component is used to determine things like when should this unit be active (when it is spawned in the world). Some unis may do things like move around, but
 * they will only get involved into battles when they are close.
 * Other units may not need this because they are spawned into the world
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpActivityComponent : public UActorComponent
{
   GENERATED_BODY()
};

