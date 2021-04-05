#pragma once

namespace UnitMessages
{
<<<<<<< HEAD
   static const inline FName AIMessage_SpellCasted    = TEXT("SpellCasted!");
   static const inline FName AIMessage_SpellCastFail    = TEXT("SpellCastFailed!"); // When AI searches for a target and finds one
   static const inline FName AIMessage_Hit            = TEXT("AttackReady!");
   static const inline FName AIMessage_SpellInterrupt = TEXT("SpellInterrupted!");
   static const inline FName AIMessage_Stunned        = TEXT("Stunned!");
   static const inline FName AIMessage_Silenced       = TEXT("Silenced!");
   static const inline FName AIMessage_TargetLoss     = TEXT("TargetLoss");  // When target dies or we stopped chasing it
   static const inline FName AIMessage_FoundTarget    = TEXT("FoundTarget"); // When AI searches for a target and finds one
=======
   static const inline FName AIMessage_SpellFinished = TEXT("SpellActivated"); // After spell casted succesfully (our resources used)

   static const inline FName AIMessage_SpellCastFail =
       TEXT("SpellCastFailed!"); // When AI searches for a target and finds one but maybe the target turns invunlnerable during cast
   static const inline FName AIMessage_SpellInterrupt = TEXT("SpellInterrupted"); // When you get stunned or stop during the cast.

   static const inline FName AIMessage_AttackExecuted = TEXT("AttackFinished"); // After a successful attack execution

   static const inline FName AIMessage_Stunned        = TEXT("Stunned!");
   static const inline FName AIMessage_Silenced       = TEXT("Silenced!");
   static const inline FName AIMessage_TargetLoss     = TEXT("TargetLoss");     // When target dies or it got outta vision.
   static const inline FName AIMessage_FoundTarget    = TEXT("FoundTarget");    // When AI searches for a target and finds one
   static const inline FName AIMessage_ActionFinished = TEXT("ActionFinished"); // For some multiprocess action (like casting spells, attacking, or using items).
   static const inline FName AIMessage_TurnFinished = TEXT("TurnFinished");
>>>>>>> componentrefactor
};
