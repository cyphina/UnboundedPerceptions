#pragma once

UENUM(BlueprintType)
enum class ECombatType : uint8
{
   Melee,
   Ranged,
   Magic
};

<<<<<<< HEAD
struct UpCombatInfo {
   bool        isDead  = false;
=======
struct UpCombatInfo
{
   bool            isDead       = false;
   bool            bMissLastHit = false;
   FThreadSafeBool isUnitHidden = false;
>>>>>>> componentrefactor
   ECombatType combatStyle = ECombatType::Magic;
};
