// Created 8/8/20 4:11 PM

#pragma once
#include "UnitCallbacks.generated.h"

struct FUpDamage;
class URTSUnitAnimController;

DECLARE_DELEGATE_OneParam(FOnUnitDamageReceived, const FUpDamage&);
DECLARE_DELEGATE_OneParam(FOnUnitDamageDealt, const FUpDamage&);

DECLARE_EVENT_OneParam(AUnitController, FOnUnitDie, const AUnit&);
DECLARE_EVENT(AUnitController, FOnUnitStop);       //
DECLARE_EVENT(URTSUnitAnimController, FOnUnitHit); // When a unit initiates an attack (animation begins)

UINTERFACE(MinimalAPI)
class UUnitCallbacks : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IUnitCallbacks
{
   GENERATED_IINTERFACE_BODY()

 public:
   virtual FOnUnitDie&            OnUnitDie()            = 0;
   virtual FOnUnitDamageReceived& OnUnitDamageReceived() = 0;
   virtual FOnUnitDamageDealt&    OnUnitDamageDealt()    = 0;
   virtual FOnUnitStop&           OnUnitStop()           = 0;
   virtual FOnUnitHit&            OnUnitHit()            = 0;
};
