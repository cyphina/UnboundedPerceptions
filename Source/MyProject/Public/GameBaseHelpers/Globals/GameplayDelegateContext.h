// Created 1/17/21 01:01 AM

#pragma once
#include "LocalPlayerSubsystem.h"
#include "GameplayDelegateContext.generated.h"

class AUnit;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDie, AUnit*);

UCLASS()
class MYPROJECT_API UGameplayDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()

public:
   FOnUnitDie& OnUnitDieGlobal() { return OnUnitDieGlobalEvent; }

private:
   FOnUnitDie OnUnitDieGlobalEvent;
};
