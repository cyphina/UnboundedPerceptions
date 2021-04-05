
#pragma once
#include "ManualTargetingControl.generated.h"

<<<<<<< HEAD
struct FUpSpellTargeting;
=======
class UUpSpellTargeting;
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   virtual void FinalizeSpellTargeting(const FUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&)       = 0;
=======
   virtual void FinalizeSpellTargeting(const UUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&)       = 0;
>>>>>>> componentrefactor
   virtual bool IsTargetingSelf()                                                                                                          = 0;
};
