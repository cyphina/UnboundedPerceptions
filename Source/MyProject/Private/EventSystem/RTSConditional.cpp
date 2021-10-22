#include "RTSConditional.h"
#include "RTSGameMode.h"
#include "UserInput.h"

DEFINE_LOG_CATEGORY(Up_Log_Conditionals);

AUserInput* URTSConditionalBase::GetControllerRef() const
{
   if(UWorld* World = GetWorld())
   {
      if(AUserInput* UserInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
      {
         return UserInput;
      }
   }
   return nullptr;
}

ARTSGameMode* URTSConditionalBase::GetGameModeRef() const
{
   if(UWorld* World = GetWorld())
   {
      if(ARTSGameMode* GameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode()))
      {
         return GameModeRef;
      }
   }
   return nullptr;
}

bool URTSConditional::CheckIsConditionSatisfied_Implementation()
{
   const UObject* Owner = GetOuter();

   if(Owner)
   {
      UE_LOG(Up_Log_Conditionals, Log, TEXT("Conditional of type %s executed within object %s"), *GetClass()->GetName(), *Owner->GetName());
   }

   return !bReverseResult;
}

URTSConditionalSequence::URTSConditionalSequence()
{
}

void URTSConditionalSequence::AddConditional(URTSConditional* ConditionalToAdd)
{
   conditionalsInSequence.Add(ConditionalToAdd);
}

void URTSConditionalSequence::RemoveConditional(URTSConditional* ConditionalToRemove)
{
   conditionalsInSequence.Remove(ConditionalToRemove);
}

bool URTSConditionalSequence::CheckIsConditionSatisfied_Implementation()
{
   for(URTSConditional* condition : conditionalsInSequence)
   {
      if(!condition->CheckIsConditionSatisfied())
      {
         return false;
      }
   }
   return true;
}
