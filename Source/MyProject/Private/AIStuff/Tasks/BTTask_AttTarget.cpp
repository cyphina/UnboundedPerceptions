// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_AttTarget.h"
#include "SpellDataLibrary.h"
<<<<<<< HEAD
=======
#include "TargetComponent.h"
>>>>>>> componentrefactor
#include "TargetedAttackComponent.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllers/UnitController.h"
#include "UnitMessages.h"
<<<<<<< HEAD

UBTTask_AttTarget::UBTTask_AttTarget()
{
   NodeName    = "Attack Target";
   bNotifyTick = false;
=======
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UBTTask_AttTarget::UBTTask_AttTarget()
{
   NodeName     = "Attack Target";
   bNotifyTick  = false;
>>>>>>> componentrefactor
}

EBTNodeResult::Type UBTTask_AttTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
<<<<<<< HEAD
   AUnitController* AICon = Cast<AUnitController>(ownerComp.GetAIOwner());
   if(AICon) {
      AUnit* target = Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target"));
      if(target && USpellDataLibrary::IsAttackable(target->GetAbilitySystemComponent())) {
         AICon->FindComponentByClass<UTargetedAttackComponent>()->BeginAttack(target);
         WaitForMessage(ownerComp, UnitMessages::AIMessage_TargetLoss);
         WaitForMessage(ownerComp, UnitMessages::AIMessage_Stunned);
         return EBTNodeResult::InProgress;
      }
   } else {
      ownerComp.GetBlackboardComponent()->SetValueAsObject("target", nullptr);
=======
   AUnitController* unitController = Cast<AUnitController>(ownerComp.GetAIOwner());

   if(unitController)
   {
      if(actionQuery)
      {
         FEnvQueryRequest queryRequest{actionQuery, unitController->GetUnitOwner()};
         const auto       attackAfterTargetFound = FQueryFinishedSignature::CreateUObject(this, &UBTTask_AttTarget::OnTargetFindingFinished, unitController, &ownerComp);

         const int  requestId = queryRequest.Execute(queryRunMode, attackAfterTargetFound);
         const bool bValid    = (requestId >= 0);

         if(bValid)
         {
            return EBTNodeResult::InProgress;
         }
      }
      else
      {
         UTargetComponent* targetComp = unitController->GetUnitOwner()->GetTargetComponent();
         if(AActor* targetActor = targetComp->GetTargetActorOrUnit())
         {
            if(AUnit* existingTarget = Cast<AUnit>(targetActor))
            {
               if(UTargetedAttackComponent* targetedAttackComp = unitController->FindComponentByClass<UTargetedAttackComponent>())
               {
                  targetedAttackComp->BeginAttack(existingTarget);
                  return EBTNodeResult::InProgress;
               }
            }
         }
      }
>>>>>>> componentrefactor
   }

   return EBTNodeResult::Failed;
}

void UBTTask_AttTarget::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
<<<<<<< HEAD
   bSuccess = Message != UnitMessages::AIMessage_Stunned & Message != UnitMessages::AIMessage_TargetLoss;
=======
   bSuccess = Message != UnitMessages::AIMessage_TargetLoss & Message != UnitMessages::AIMessage_Stunned;
>>>>>>> componentrefactor
   Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}

void UBTTask_AttTarget::OnTargetFindingFinished(TSharedPtr<FEnvQueryResult> queryResult, AUnitController* AICon, UBehaviorTreeComponent* ownerComp)
{
   if(queryResult.IsValid())
   {
      AUnit* targetResult = Cast<AUnit>(queryResult->GetItemAsActor(0));
      if(targetResult)
      {
         if(USpellDataLibrary::IsAttackable(targetResult->GetAbilitySystemComponent()))
         {
            AICon->FindComponentByClass<UTargetedAttackComponent>()->BeginAttack(targetResult);
            WaitForMessage(*ownerComp, UnitMessages::AIMessage_TargetLoss);
            WaitForMessage(*ownerComp, UnitMessages::AIMessage_Stunned);
            return;
         }
      }
   }
   FinishLatentTask(*ownerComp, EBTNodeResult::Type::Failed);
}
