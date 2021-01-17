// Created 9/12/20 4:57 PM

#pragma once
#include "GameplayAbility.h"
#include "RTSExecution.generated.h"

class AUnit;

/**
* Allows us to create a hierarchy of purely behavioral function objects in blueprints that have a specific purpose -
* Using blueprint function objects allows designers to create plug in play custom behavior for actions like:
* Death
* Attack
* Movement
* ... and more ...
*/
UCLASS()
class URTSExecution : public UGameplayAbility
{
   GENERATED_BODY()

private:
   using UGameplayAbility::CommitAbility;
};
