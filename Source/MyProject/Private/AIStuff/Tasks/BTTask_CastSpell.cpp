// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "AIStuff/AIControllers/UnitController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "SpellSystem/MySpell.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WorldObjects/Unit.h"

#include "BTTask_CastSpell.h"

UBTTask_CastSpell::UBTTask_CastSpell(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   NodeName = "Cast Spell";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_CastSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitController = Cast<AUnitController>(ownerComp.GetAIOwner());
   UMySpell* spell = spellToCast.GetDefaultObject();

   if (spell->GetTargetting() == FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))
   {
      FGameplayAbilityTargetingLocationInfo tInfo;
      tInfo.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
      tInfo.LiteralTransform = FTransform(ownerComp.GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName));
      auto targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromLocations(tInfo, tInfo);
      if (unitController->BeginCastSpell(spellToCast, targetData))
      {
         WaitForMessage(ownerComp, AUnit::AIMessage_SpellCasted);
         WaitForMessage(ownerComp, AUnit::AIMessage_SpellInterrupt);
         return EBTNodeResult::InProgress;
      }
   }
   else if (spell->GetTargetting() == FGameplayTag::RequestGameplayTag("Skill.Targetting.None"))
   {
      if (unitController->BeginCastSpell(spellToCast, FGameplayAbilityTargetDataHandle()))
      {
         WaitForMessage(ownerComp, AUnit::AIMessage_SpellCasted);
         WaitForMessage(ownerComp, AUnit::AIMessage_SpellInterrupt);
         return EBTNodeResult::InProgress;
      }
   }
   else if (spell->GetTargetting() == FGameplayTag::RequestGameplayTag("Skill.Targetting.Single"))
   {
      auto targetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(
         Cast<AActor>(ownerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName)));
      if (unitController->BeginCastSpell(spellToCast, targetData))
      {
         WaitForMessage(ownerComp, AUnit::AIMessage_SpellCasted);
         WaitForMessage(ownerComp, AUnit::AIMessage_SpellInterrupt);
         WaitForMessage(ownerComp, AUnit::AIMessage_TargetLoss);
         return EBTNodeResult::InProgress;
      }
   }
    return EBTNodeResult::Failed;
}

void UBTTask_CastSpell::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   bSuccess = message != AUnit::AIMessage_Stunned & message != AUnit::AIMessage_TargetLoss;
   Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
}

