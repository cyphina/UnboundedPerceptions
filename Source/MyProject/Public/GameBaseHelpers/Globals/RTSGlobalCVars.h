#pragma once

namespace AttackCVars
{
   extern bool bPrintAttackTimings;
   extern bool bUseOldDamageNumbers;
}

namespace GameplayModifierCVars
{
   extern float interactPointMoveRange;
   extern float vectorMoveStopRange;
   extern bool  bEnableEnemyControl;
   inline bool  bEnableEnemyAI        = true;
   inline bool  bShowEnemyPerspective = false;
}
