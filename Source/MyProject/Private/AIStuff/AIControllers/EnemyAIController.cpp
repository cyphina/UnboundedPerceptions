// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EnemyAIController.h"

#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"


void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& updatedActors)
{
   ////No stimuli around
   /////sightPerceptionInfo = GetAIPerceptionComponent()->GetFreshestTrace(UAISense::GetSenseID<UAISense_Sight>());
   // percievedActors.Empty();
   // GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(sight->Implementation, percievedActors);
   // if (!percievedActors.Num())
   //{
   //	blackboardComp->SetValueAsObject(blackboardEnemyKey, nullptr);
   //	currentClosest = TNumericLimits<int>::Max();
   //	return;
   //}
   // else
   //{
   //	/*if(GetPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID<UAISense_Sight>()) != nullptr)
   //	{
   //		//if(sightPerceptionInfo != nullptr && GetAIPerceptionComponent()->HasActiveStimulus(*sightPerceptionInfo->Target, UAISense::GetSenseID<UAISense_Sight>()))
   //		//{
   //			FVector sightLocation = sightPerceptionInfo->GetStimulusLocation(UAISense::GetSenseID<UAISense_Sight>());
   //			FVector direction = GetPawn()->GetActorLocation() - sightLocation;
   //			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Distance Calc ") + percievedActors[0]->GetName() + TEXT(" "));
   //			UE_LOG(LogTemp, Warning, TEXT("%s"), *sightPerceptionInfo->LastSensedStimuli[0].Tag.ToString());
   //			if(int sqrDistance = FVector::DotProduct(direction, direction) < currentClosest)
   //			{
   //				blackboardComp->SetValueAsObject(blackboardEnemyKey, sightPerceptionInfo->Target.Get());
   //				currentClosest = sqrDistance;
   //			}
   //		}

   //	}
   //	*/
   //
   //	currentClosest = TNumericLimits<int>::Max();
   //	for (AActor* actor : percievedActors)
   //	{
   //		if (actor->IsA<AAlly>())
   //		{
   //			AUnit* targetUnit = Cast<AUnit>(actor);
   //			if (targetUnit->GetCanTarget() && GetUnitOwner()->GetIsEnemy() != targetUnit->GetIsEnemy())
   //			{
   //				FVector direction = GetPawn()->GetActorLocation() - targetUnit->GetActorLocation();
   //				int sqrDistance = FVector::DotProduct(direction, direction);
   //				if (sqrDistance < currentClosest)
   //				{
   //					blackboardComp->SetValueAsObject(blackboardEnemyKey, targetUnit);
   //					currentClosest = sqrDistance;
   //				}

   //			}
   //		}
   //	}
   //}
}

AEnemyAIController::AEnemyAIController()
{
   // Component Init
   AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("PerceptionComp"));

   //Create a sight sense
   sight                               = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
   sight->SightRadius                  = 1000.f;
   sight->LoseSightRadius              = 1500.f;
   sight->PeripheralVisionAngleDegrees = 145.f;

   //Tell sight to detect everything
   sight->DetectionByAffiliation.bDetectEnemies    = true;
   sight->DetectionByAffiliation.bDetectFriendlies = true;
   sight->DetectionByAffiliation.bDetectNeutrals   = true;

   //Register the sense to our Perception Component
   AIPerceptionComponent->ConfigureSense(*sight);
   currentClosest = INT_MAX;
}

void AEnemyAIController::BeginPlay()
{
   Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
   if (behaviorTree) {
      // Initialize blackboard and start attached behavior tree
      UseBlackboard(behaviorTree->BlackboardAsset, blackboardComp);
      behaviorTreeComp->StartTree(*behaviorTree);
   }
   // register onperceptionupdated to fire whenever aiperception gets updated
   AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);

   GetAIPerceptionComponent()->GetActorsPerception(GetPawn(), info);

   if(idleMoveTree)
      behaviorTreeComp->SetDynamicSubtree(FGameplayTag::RequestGameplayTag("Combat.AI.InjectTreeTag"), idleMoveTree);
}

AActor* AEnemyAIController::GetSeeingPawn()
{
   // return seeing pawn
   UObject* object = blackboardComp->GetValueAsObject(blackboardEnemyKey);
   return object ? Cast<AActor>(object) : nullptr;
}
