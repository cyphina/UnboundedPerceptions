#pragma once

class AUnit;
class UMySpell;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnSpellSlotReplaced, AUnit*, TSubclassOf<UMySpell>, int);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFocusedUnitSpellCasted, AUnit*, int);
DECLARE_EVENT_OneParam(USpellBook, FOnSpellLearned, TSubclassOf<UMySpell>);
DECLARE_EVENT_OneParam(USpellBook, FOnSpellUpgraded, TSubclassOf<UMySpell>);

namespace SpellHUDEvents
{
   inline FOnSpellSlotReplaced OnSpellSlotReplacedEvent;
   inline FOnFocusedUnitSpellCasted OnFocusedUnitSpellCastedEvent;
   inline FOnSpellLearned  OnSpellLearnedEvent;
   inline FOnSpellUpgraded OnSpellUpgradedEvent;
}
