#pragma once

UENUM(BlueprintType)
enum class ECombatType : uint8
{
   Melee,
   Ranged,
   Magic
};

struct UpCombatInfo
{
   bool        isDead       = false;
   bool        bMissLastHit = false;
   ECombatType combatStyle  = ECombatType::Magic;
};
