// Created 6/14/21 12:57 PM

#include "MyProject.h"
#include "RTSTrigger.h"

#include "RTSConditional.h"
#include "RTSGameMode.h"
#include "UserInput.h"

DEFINE_LOG_CATEGORY(Up_Log_Triggers);

URTSTriggerBase::URTSTriggerBase()
{
   triggerName = *GetName();
}

void URTSTriggerBase::SetConditions(TArray<URTSConditional*> NewConditions)
{
   activationConditions = MoveTemp(NewConditions);
}

bool URTSTriggerBase::CheckActivationConditionsSatisfied()
{
   for(URTSConditional* Conditional : activationConditions)
   {
      if(!Conditional->CheckIsConditionSatisfied())
      {
         return false;
      }
   }
   return true;
}

void URTSTriggerBase::SetEnabled(bool bNewEnabled)
{
   bEnabled = bNewEnabled;
}

void URTSTriggerBase::SetTriggerLimit(int NewTriggerLimit)
{
   triggerLimit = NewTriggerLimit;
}

AUserInput* URTSTriggerBase::GetControllerRef() const
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

ARTSGameMode* URTSTriggerBase::GetGameModeRef() const
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

void URTSTrigger::TriggerEvent_Implementation()
{
   if(GetEnabled())
   {
      if(CheckActivationConditionsSatisfied())
      {
         UE_LOG(Up_Log_Triggers, Log, TEXT("Initiating trigger %s with %d activations left."), *GetTriggerName().ToString());
      }
   }
}

URTSTriggerSequence::URTSTriggerSequence()
{
}

void URTSTriggerSequence::AddTrigger(URTSTriggerBase* TriggerToAdd)
{
   triggersInSequence.Add(TriggerToAdd);
}

void URTSTriggerSequence::RemoveTrigger(URTSTriggerBase* TriggerToRemove)
{
   triggersInSequence.Remove(TriggerToRemove);
}

void URTSTriggerSequence::TriggerEvent_Implementation()
{
   if(GetEnabled())
   {
      if(CheckActivationConditionsSatisfied())
      {
         UE_LOG(Up_Log_Triggers, Log, TEXT("Initiating trigger sequence %s with %d activations left."), *GetTriggerName().ToString());
         for(URTSTriggerBase* Trigger : triggersInSequence)
         {
            if(Trigger->GetEnabled())
            {
               if(Trigger->CheckActivationConditionsSatisfied())
               {
                  Trigger->TriggerEvent();
               }
            }
         }
      }
   }
}
