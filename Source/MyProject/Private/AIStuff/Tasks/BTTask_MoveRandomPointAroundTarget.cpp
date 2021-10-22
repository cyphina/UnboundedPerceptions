#include "MyProject.h"
#include "BTTask_MoveRandomPointAroundTarget.h"

#include "TargetComponent.h"
#include "Unit.h"
#include "UnitMessages.h"
#include "UpStatComponent.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"

UBTTask_MoveRandomPointAroundTarget::UBTTask_MoveRandomPointAroundTarget()
{
   NodeName    = "Random Move Around Last Target";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_MoveRandomPointAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory)
{
   const AUnit* Owner = Cast<AUnit>(OwnerComp.GetAIOwner()->GetPawn());
   if(Owner && Owner->GetTargetComponent()->IsTargetingUnit())
   {
      const AUnit* Target = Owner->GetTargetComponent()->GetTargetUnit();
      if(Target)
      {
         const FVector targetLocation = Target->GetActorLocation();

         if(moveRadius < 0)
         {
            const float attackRange = Owner->GetStatComponent()->GetMechanicAdjValue(EMechanics::AttackRange);
            moveRadius              = attackRange;
         }
         const FVector PointInRange = FVector(FMath::RandPointInCircle(moveRadius), 0);
         const FVector EndPoint     = targetLocation + PointInRange;

         // Ensure this path is actually closeby since there could be obstacles making the path long even if the point is close
         if(AUnitController* UnitController = Owner->GetUnitController())
         {
            FAIMoveRequest MoveReq(EndPoint);
            MoveReq.SetAllowPartialPath(false);
            MoveReq.SetProjectGoalLocation(true);

            FPathFindingQuery Query;
            UnitController->BuildPathfindingQuery(MoveReq, Query);
            const FNavAgentProperties& NavAgentProps = UnitController->GetNavAgentPropertiesRef();
            UNavigationSystemV1*       NavSys        = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Owner->GetWorld());

            const FAIRequestID RequestId = NavSys->FindPathAsync(
                NavAgentProps, Query,
                FNavPathQueryDelegate::CreateUObject(this, &UBTTask_MoveRandomPointAroundTarget::OnFoundRandomPathablePoint, &OwnerComp, UnitController, MoveReq));

            if(RequestId != FAIRequestID::InvalidRequest)
            {
               return EBTNodeResult::InProgress;
            }
         }
      }
   }
   return EBTNodeResult::Failed;
}

void UBTTask_MoveRandomPointAroundTarget::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
   bSuccess = Message != UBrainComponent::AIMessage_RepathFailed;
   Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}

void UBTTask_MoveRandomPointAroundTarget::OnFoundRandomPathablePoint(uint32 QueryID, ENavigationQueryResult::Type QueryType, FNavPathSharedPtr ResultPath,
                                                                     UBehaviorTreeComponent* OwnerComp, AUnitController* UnitController, FAIMoveRequest MoveReq)
{
   if(OwnerComp)
   {
      if(ResultPath)
      {
         const float Cost = ResultPath->GetCost();
         if(Cost < moveRadius * 2)
         {
            UnitController->StopCurrentAction();
            const FAIRequestID RequestID = UnitController->RequestMove(MoveReq, ResultPath);
            if(RequestID.IsValid())
            {
               WaitForMessage(*OwnerComp, UBrainComponent::AIMessage_MoveFinished, RequestID.GetID());
               WaitForMessage(*OwnerComp, UBrainComponent::AIMessage_RepathFailed, RequestID.GetID());
               return;
            }
         }
      }
      FinishLatentTask(*OwnerComp, EBTNodeResult::Type::Failed);
   }
}

FString UBTTask_MoveRandomPointAroundTarget::GetStaticDescription() const
{
   return moveRadius >= 0 ? FString::Printf(TEXT("Random Point Find Radius: %f"), moveRadius) : FString::Printf(TEXT("Random Point Find Radius set to AttackRange"));
}
