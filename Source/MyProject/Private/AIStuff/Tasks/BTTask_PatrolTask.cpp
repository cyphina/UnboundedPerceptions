#include "MyProject.h"
#include "BTTask_PatrolTask.h"

#include "UnitMessages.h"
#include "../AIControllers/UnitController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/PatrolComponent.h"
#include "WorldObjects/Unit.h"

UBTTask_Patrol::UBTTask_Patrol(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   NodeName    = "Patrol";
   bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTPatrolTaskMemory* myMemory = (FBTPatrolTaskMemory*)nodeMemory;
   myMemory->AICon               = Cast<AAIController>(ownerComp.GetAIOwner());
   myMemory->patrolComp          = myMemory->AICon->FindComponentByClass<UPatrolComponent>();

   if(myMemory->AICon && myMemory->patrolComp->GetPatrolPoints().Num() > 0) {
      UBlackboardComponent* blackboardComp = ownerComp.GetBlackboardComponent();
      if(blackboardComp) {
         if(myMemory->patrolComp->GetPatrolPoints().Num() > 0) {
            return PatrolToNextLocation(ownerComp, nodeMemory);
         }
      }
   }
   return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_Patrol::PatrolToNextLocation(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FBTPatrolTaskMemory* myMemory = (FBTPatrolTaskMemory*)nodeMemory;

   if(myMemory->patrolComp->Patrol()) {
      const FAIRequestID requestId = myMemory->AICon->GetCurrentMoveRequestID();

      // Without a request ID, the new move request creation will somehow trigger the message instantly, infinitely looping OnMessage and PatrolToNextLocation
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_MoveFinished, requestId.GetID());
      WaitForMessage(ownerComp, UBrainComponent::AIMessage_RepathFailed, requestId.GetID());
      WaitForMessage(ownerComp, UnitMessages::AIMessage_FoundTarget);
      return EBTNodeResult::InProgress;
   }
   return EBTNodeResult::Failed;
}

uint16 UBTTask_Patrol::GetInstanceMemorySize() const
{
   return sizeof(FBTPatrolTaskMemory);
}

void UBTTask_Patrol::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
   // Line 84 - BTTaskNoded.cpp shows that bSuccess is the message status for whathever message triggers this
   if(!bSuccess)
   {
      UE_LOG(LogBehaviorTree, Warning, TEXT("Problem patrolling to location"));
      FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
   }

   // Found a target so move on from this task
   if(Message == UnitMessages::AIMessage_FoundTarget) { // Move finishes on
      
      Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
      FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
      return;
   }

   // Didn't find a target so keep patrolling
   //! DO NOT CALL THIS OR IT CALLS PatrolToNextLocation twice => Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
   PatrolToNextLocation(OwnerComp, NodeMemory);
}
