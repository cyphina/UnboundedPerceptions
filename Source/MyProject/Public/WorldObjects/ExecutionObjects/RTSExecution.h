// Created 9/12/20 4:57 PM

#pragma once
#include "RTSExecution.generated.h"

UINTERFACE(MinimalAPI)
class URTSExecution : public UInterface
{
   GENERATED_BODY()
};

/**Allows us to create a hierarchy of purely behavioral function objects in blueprints that have a specific purpose -
 * Using blueprint function objects allows designers to create plug in play custom behavior for actions like:
 * Death
 * Attack
 * Movement
 * ... and more ...
 */
class IRTSExecution
{
   GENERATED_BODY()

public:
   UFUNCTION(BlueprintNativeEvent)
   void Execute();
};
