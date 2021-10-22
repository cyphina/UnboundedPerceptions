#include "MyProject.h"
#include "EnemyAIController.h"

#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Globals/RTSGlobalCVars.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer) : AUnitController(ObjectInitializer)
{
}

void AEnemyAIController::BeginPlay()
{
   Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);

   if(GameplayModifierCVars::bEnableEnemyAI)
   {
      if(UBehaviorTreeComponent* BTComponent = FindComponentByClass<UBehaviorTreeComponent>())
      {
         BTComponent->StartLogic();
      }
   }
}
