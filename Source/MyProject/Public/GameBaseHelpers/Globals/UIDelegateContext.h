// Created 1/17/21 01:13 AM

#pragma once
#include "LocalPlayerSubsystem.h"
#include "StatEnums.h"

#include "UIDelegateContext.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributePointAllocated, EAttributes, bool);

UCLASS()
class MYPROJECT_API UUIDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()
public:
   FOnAttributePointAllocated OnAttributePointAllocated() { return OnAttributePointAllocatedEvent; };

private:
   FOnAttributePointAllocated OnAttributePointAllocatedEvent;
};
