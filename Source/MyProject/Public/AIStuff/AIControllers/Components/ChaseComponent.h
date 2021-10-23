#pragma once

#include "Components/ActorComponent.h"
#include "ChaseComponent.generated.h"

/*
 * This component has logic the unit can use to chase an enemy around. Some enemies may decide not to chase, others may use the standard follow the enemy position
 * for a few seconds before giving up.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpChaseComponent : public UActorComponent
{
   GENERATED_BODY()

   /** Used to chase after enemies that leave the vision radius*/
   virtual void Chase();

   /** Condition used to indicate chasing failure.  Chasing sucess is finding the enemy again*/
   virtual bool ChasingQuit();
};

