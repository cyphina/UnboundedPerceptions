#include "MyProject.h"
#include "EnemyAIController.h"

#include "WorldObjects/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

<<<<<<< HEAD
AEnemyAIController::AEnemyAIController()
=======
AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer) : AUnitController(ObjectInitializer)
>>>>>>> componentrefactor
{
}

void AEnemyAIController::BeginPlay()
{
   Super::BeginPlay();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);
}

