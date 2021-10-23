
#pragma once
#include "ManualTargetingControl.generated.h"

class UUpSpellTargeting;
class UMySpell;

UINTERFACE(MinimalAPI)
class UManualTargetingController : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IManualTargetingController
{
   GENERATED_IINTERFACE_BODY()

 public:
   virtual void FinalizeSpellTargeting(const UUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&)       = 0;
   virtual bool IsTargetingSelf()                                                                                                          = 0;
};
