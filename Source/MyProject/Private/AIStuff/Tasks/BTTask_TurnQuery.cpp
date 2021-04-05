// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_TurnQuery.h"

#include "TargetComponent.h"
#include "Unit.h"
#include "UnitMessages.h"
#include "UpAIHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UBTTask_TurnQuery::UBTTask_TurnQuery()
{
   NodeName     = "Turn";
   bNotifyTick  = false;
   queryRunMode = EEnvQueryRunMode::Type::SingleResult;
}

EBTNodeResult::Type UBTTask_TurnQuery::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
   AUnitController* unitController = Cast<AUnitController>(OwnerComp.GetAIOwner());

   if(unitController)
   {
      if(actionQuery)
      {
         FEnvQueryRequest queryRequest{actionQuery, unitController->GetUnitOwner()};
         const auto       attackAfterTargetFound = FQueryFinishedSignature::CreateUObject(this, &UBTTask_TurnQuery::OnTurnFinished, unitController, &OwnerComp);

         const int  requestId = queryRequest.Execute(queryRunMode, attackAfterTargetFound);
         const bool bValid    = (requestId >= 0);

         if(bValid)
         {
            return EBTNodeResult::InProgress;
         }
      }
      else
      {
         WaitForMessage(OwnerComp, UnitMessages::AIMessage_Stunned);
         WaitForMessage(OwnerComp, UnitMessages::AIMessage_TargetLoss);

         UTargetComponent* targetComp = unitController->GetUnitOwner()->GetTargetComponent();
         if(AActor* turnTargetActor = targetComp->GetTargetActorOrUnit())
         {
            if(UUpAIHelperLibrary::IsFacingTarget(unitController->GetUnitOwner(), turnTargetActor->GetActorLocation()))
            {
               return EBTNodeResult::Succeeded;
            }

            unitController->GetUnitOwner()->GetTargetComponent()->SetTarget(turnTargetActor);
            unitController->TurnTowardsActor(turnTargetActor);
            return EBTNodeResult::InProgress;
         }
         else
         {
            const FVector turnPoint = targetComp->GetTargetLocationVisit();

            if(UUpAIHelperLibrary::IsFacingTarget(unitController->GetUnitOwner(), turnPoint))
            {
               return EBTNodeResult::Succeeded;
            }

            unitController->GetUnitOwner()->GetTargetComponent()->SetTarget(turnPoint);
            unitController->TurnTowardsPoint(turnPoint);
            return EBTNodeResult::InProgress;
         }
      }
   }
   return EBTNodeResult::Failed;
}

void UBTTask_TurnQuery::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
   bSuccess = Message != UnitMessages::AIMessage_TargetLoss & Message != UnitMessages::AIMessage_Stunned;
   Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}

void UBTTask_TurnQuery::OnTurnFinished(TSharedPtr<FEnvQueryResult> queryResult, AUnitController* unitController, UBehaviorTreeComponent* ownerComp)
{
   if(queryResult.IsValid())
   {
      AActor* targetActor = queryResult->GetItemAsActor(0);

      WaitForMessage(*ownerComp, UnitMessages::AIMessage_TurnFinished);
      WaitForMessage(*ownerComp, UnitMessages::AIMessage_Stunned);
      WaitForMessage(*ownerComp, UnitMessages::AIMessage_TargetLoss);

      if(targetActor)
      {
         if(UUpAIHelperLibrary::IsFacingTarget(unitController->GetUnitOwner(), targetActor->GetActorLocation()))
         {
            FinishLatentTask(*ownerComp, EBTNodeResult::Type::Succeeded);
            return;
         }

         unitController->GetUnitOwner()->GetTargetComponent()->SetTarget(targetActor);
         unitController->TurnTowardsActor(targetActor);
      }
      else
      {
         const FVector targetLocation = queryResult->GetItemAsLocation(0);

         if(UUpAIHelperLibrary::IsFacingTarget(unitController->GetUnitOwner(), targetLocation))
         {
            FinishLatentTask(*ownerComp, EBTNodeResult::Type::Succeeded);
            return;
         }

         unitController->GetUnitOwner()->GetTargetComponent()->SetTarget(targetLocation);
         unitController->TurnTowardsPoint(targetLocation);
      }
   }
   else
   {
      FinishLatentTask(*ownerComp, EBTNodeResult::Type::Failed);
   }
}
