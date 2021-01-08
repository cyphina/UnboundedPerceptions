#pragma once

namespace UnitMessages
{
   static const inline FName AIMessage_SpellCasted    = TEXT("SpellCasted!");
   static const inline FName AIMessage_SpellCastFail    = TEXT("SpellCastFailed!"); // When AI searches for a target and finds one
   static const inline FName AIMessage_Hit            = TEXT("AttackReady!");
   static const inline FName AIMessage_SpellInterrupt = TEXT("SpellInterrupted!");
   static const inline FName AIMessage_Stunned        = TEXT("Stunned!");
   static const inline FName AIMessage_Silenced       = TEXT("Silenced!");
   static const inline FName AIMessage_TargetLoss     = TEXT("TargetLoss");  // When target dies or we stopped chasing it
   static const inline FName AIMessage_FoundTarget    = TEXT("FoundTarget"); // When AI searches for a target and finds one
};
