#pragma once

#include "Components/ActorComponent.h"
#include "UUpPositioningComponent.generated.h"

/*
 * This component holds information about how units move around while they are fighting. Some units may just attack, but others may try to annoy us by
 * teleporting about and such.
 * Standard movement without this component would just be moving based off of attacking behaviors, whereas with this component a unit could try to reposition in between
 * attacks.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpPositioningComponent : public UActorComponent
{
   GENERATED_BODY()

   /** Environment query to find the best place to escape to.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestDefensivePosition;

   /** Environment query to find the best place to deal damage.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   UEnvQuery* findBestOffensivePosition;
};
