// Created 1/17/21 01:01 AM

#pragma once
#include "LocalPlayerSubsystem.h"
#include "GameplayDelegateContext.generated.h"

class AUnit;
class ARTSPawn;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDieGlobal, AUnit*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoneyGained, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnExpGained, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNamedInteractableInteracted, const FText&);
DECLARE_EVENT_OneParam(ARTSPawn, FOnSkillActivated, int);

UCLASS()
class MYPROJECT_API UGameplayDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()

public:
   FOnUnitDieGlobal& OnUnitDieGlobal() { return OnUnitDieGlobalEvent; }
   FOnNamedInteractableInteracted& OnInteracted() { return OnInteractedEvent; }
   FOnMoneyGained& OnMoneyGained() { return OnMoneyGainedEvent; }
   FOnExpGained& OnExpGained() { return OnExpGainedEvent; }
   FOnSkillActivated& OnSkillActivated() { return OnSkillActivatedEvent; }

   
private:
   FOnUnitDieGlobal OnUnitDieGlobalEvent;
   FOnNamedInteractableInteracted OnInteractedEvent;
   FOnExpGained OnExpGainedEvent;
   FOnMoneyGained OnMoneyGainedEvent;
   FOnSkillActivated OnSkillActivatedEvent;

};
