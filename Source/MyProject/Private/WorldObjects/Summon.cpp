#include "MyProject.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Summon.h"
#include "DIRender.h"

void ASummon::BeginPlay()
{
   Super::BeginPlay();
   AAlly::SetEnabled(true); // Will add this unit to the proper bookkeeping data structures
   OnUnitDie().AddUObject(this, &ASummon::HandleOnUnitDie);
}

void ASummon::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   auto basePlayerRef = controllerRef->GetBasePlayer();
   bEnabled ? basePlayerRef->summons.Add(this) : basePlayerRef->summons.Remove(this);
}

void ASummon::HandleOnUnitDie()
{
   TArray<UDIRender*> damageComponents;
   GetComponents<UDIRender>(damageComponents);
   for(auto x : damageComponents) {
      x->SetVisibility(true);
   }

   // Delay destruction to show damage numbers
   SetLifeSpan(2.f);
}
