// Created 1/17/21 01:01 AM

#pragma once
#include "LocalPlayerSubsystem.h"
#include "GameplayDelegateContext.generated.h"

class AUnit;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDieGlobal, AUnit*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNamedInteractableInteracted, const FText&);

UCLASS()
class MYPROJECT_API UGameplayDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()

public:
   FOnUnitDieGlobal& OnUnitDieGlobal() { return OnUnitDieGlobalEvent; }
   FOnNamedInteractableInteracted& OnInteracted() { return OnInteractedEvent; }
   
private:
   FOnUnitDieGlobal OnUnitDieGlobalEvent;
   FOnNamedInteractableInteracted OnInteractedEvent;
};
