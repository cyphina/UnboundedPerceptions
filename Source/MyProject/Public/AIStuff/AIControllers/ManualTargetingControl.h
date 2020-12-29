
#pragma once
#include "ManualTargetingControl.generated.h"

struct FUpSpellTargeting;

UINTERFACE(MinimalAPI)
class UManualTargetingController : public UInterface
{
   GENERATED_UINTERFACE_BODY()
};

class IManualTargetingController
{
   GENERATED_IINTERFACE_BODY()

 public:
   virtual void FinalizeSpellTargeting(const FUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&)       = 0;
   virtual void FinalizeSpellTargetingOnSelf(const FUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&) = 0;
   virtual bool IsTargetingSelf()                                                                                                          = 0;
   virtual void CastSpellOnSelf(TSubclassOf<UMySpell> spellClass)                                                                          = 0;
};
