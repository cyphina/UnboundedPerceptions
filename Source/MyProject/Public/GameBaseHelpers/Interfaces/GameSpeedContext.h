// Created 8/31/20 5:48 PM

#pragma once
#include "GameSpeedContext.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateGameSpeed, float, speedMultiplier);

UINTERFACE(MinimalAPI)
class UGameSpeedContext : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IGameSpeedContext
{
   GENERATED_IINTERFACE_BODY()

 public:
   virtual float            GetGameSpeed() const       = 0;
   virtual FUpdateGameSpeed OnGameSpeedUpdated() const = 0;
};
