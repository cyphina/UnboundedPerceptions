#include "MyProject.h"
#include "BTTask_QueryMove.h"

#include "Unit.h"
#include "UnitController.h"
#include "UnitMessages.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UBTTask_QueryMove::UBTTask_QueryMove()
{
   NodeName    = "Move";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_QueryMove::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitController = Cast<AUnitController>(ownerComp.GetAIOwner());

   if(unitController)
   {
      FEnvQueryRequest queryRequest{actionQuery, unitController->GetUnitOwner()};
      const auto       moveAfterPositionFound = FQueryFinishedSignature::CreateUObject(this, &UBTTask_QueryMove::OnMovePositionFound, unitController, &ownerComp);

      const int  requestId = queryRequest.Execute(queryRunMode, moveAfterPositionFound);
      const bool bValid    = (requestId >= 0);

      if(bValid)
      {
         return EBTNodeResult::InProgress;
      }
   }
   return EBTNodeResult::Failed;
}

void UBTTask_QueryMove::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   bSuccess = message != UBrainComponent::AIMessage_RepathFailed;
   Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
}

void UBTTask_QueryMove::OnMovePositionFound(TSharedPtr<FEnvQueryResult> queryResult, AUnitController* AICon, UBehaviorTreeComponent* ownerComp)
{
   if(queryResult.IsValid())
   {
      AActor*                           targetActor = queryResult->GetItemAsActor(0);
      EPathFollowingRequestResult::Type moveRequestResult;

      if(targetActor)
      {
         moveRequestResult = AICon->MoveActor(targetActor, stopRange);
      }
      else
      {
         const FVector targetLocation = queryResult->GetItemAsLocation(0);
         moveRequestResult            = AICon->Move(targetLocation, stopRange);
      }

      if(moveRequestResult == EPathFollowingRequestResult::Type::RequestSuccessful)
      {
         WaitForMessage(*ownerComp, UBrainComponent::AIMessage_MoveFinished, AICon->GetCurrentMoveRequestID());
         WaitForMessage(*ownerComp, UBrainComponent::AIMessage_RepathFailed);
      }
      else
      {
         FinishLatentTask(*ownerComp, EBTNodeResult::Type::Failed);
      }
   }
}
