#pragma once

#include "Components/ActorComponent.h"
#include "SpellChoiceComponent.generated.h"

/*
 * This component contains logic about prioritizing WHAT spells it should cast and WHEN.
 * This is important for spells with no targeting, like scream of pain.
 * This can change depending on phases.
 * It specifies a table just like the ones in https://sonny.fandom.com/wiki/City_Council which denotes what spells are first choice and
 * how the spell choices change depending on the state.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpSpellChoiceComponent : public UActorComponent
{
   GENERATED_BODY()
};