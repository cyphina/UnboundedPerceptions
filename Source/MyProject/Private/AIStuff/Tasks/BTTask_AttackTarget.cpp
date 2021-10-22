#include "MyProject.h"
#include "BTTask_AttackTarget.h"
#include "SpellDataLibrary.h"
#include "TargetComponent.h"
#include "TargetedAttackComponent.h"
#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllers/UnitController.h"
#include "UnitMessages.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
   NodeName     = "Attack Target";
   bNotifyTick  = false;
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitController = Cast<AUnitController>(ownerComp.GetAIOwner());

   if(unitController)
   {
      if(actionQuery)
      {
         FEnvQueryRequest queryRequest{actionQuery, unitController->GetUnitOwner()};
         const auto       attackAfterTargetFound = FQueryFinishedSignature::CreateUObject(this, &UBTTask_AttackTarget::OnTargetFindingFinished, unitController, &ownerComp);

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
   }

   return EBTNodeResult::Failed;
}

void UBTTask_AttackTarget::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
   bSuccess = Message != UnitMessages::AIMessage_TargetLoss & Message != UnitMessages::AIMessage_Stunned;
   Super::OnMessage(OwnerComp, NodeMemory, Message, RequestID, bSuccess);
}

void UBTTask_AttackTarget::OnTargetFindingFinished(TSharedPtr<FEnvQueryResult> queryResult, AUnitController* AICon, UBehaviorTreeComponent* ownerComp)
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
