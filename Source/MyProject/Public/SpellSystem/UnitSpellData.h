#pragma once

class UMySpell;
class UMyAbilitySystemComponent;

struct UnitSpellData {
   TSubclassOf<UMySpell>      currentSpell;
   float                      currentChannelTime = 0; // Time spent channeling by unit
   float                      channelTime        = 0; // How long unit has to channel
   UMyAbilitySystemComponent* abilitySystem;          // Skill component
};