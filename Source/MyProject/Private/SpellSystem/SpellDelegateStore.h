#pragma once

class UMySpell;

DECLARE_DELEGATE_TwoParams(FOnSpellSlotReplaced, int, TSubclassOf<UMySpell>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFocusedUnitSpellCasted, AUnit*, int);

namespace SpellHUDEvents
{
   inline FOnSpellSlotReplaced OnSpellSlotReplacedEvent;
   inline FOnFocusedUnitSpellCasted OnFocusedUnitSpellCastedEvent;
}
