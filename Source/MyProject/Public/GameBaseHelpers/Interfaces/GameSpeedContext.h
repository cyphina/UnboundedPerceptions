// Created 8/31/20 5:48 PM

#pragma once
#include "GameSpeedContext.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateGameSpeed, float, speedMultiplier);

UINTERFACE(MinimalAPI)
class UGameSpeedContext : public UInterface
{
   GENERATED_BODY()
};

class IGameSpeedContext
{
   GENERATED_BODY()

 public:
   virtual float             GetGameSpeed() const       = 0;
   virtual FUpdateGameSpeed& OnGameSpeedUpdated() const = 0;
};
