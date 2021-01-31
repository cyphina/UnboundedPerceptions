#pragma once

class AUnit;
class ABaseHero;
class UMySpell;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnSpellSlotReplaced, AUnit*, TSubclassOf<UMySpell>, int);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFocusedUnitSpellCasted, AUnit*, int);
DECLARE_EVENT_TwoParams(USpellBook, FOnSpellLearned, const ABaseHero&, TSubclassOf<UMySpell>);
DECLARE_EVENT_TwoParams(USpellBook, FOnSpellUpgraded, const ABaseHero&, TSubclassOf<UMySpell>);

namespace SpellGameContext
{
   inline FOnSpellSlotReplaced      OnSpellSlotReplacedEvent;
   inline FOnFocusedUnitSpellCasted OnFocusedUnitSpellCastedEvent;
   inline FOnSpellLearned           OnSpellLearnedEvent;
   inline FOnSpellUpgraded          OnSpellUpgradedEvent;
}
