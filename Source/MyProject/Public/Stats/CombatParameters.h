#pragma once

UENUM(BlueprintType)
enum class ECombatType : uint8 { Melee, Ranged, Magic };

struct UPAICombatParameters {
   bool        isDead  = false;
   bool        isEnemy = false;
   ECombatType combatStyle; // type of attack we autoattack with
};
