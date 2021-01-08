#include "MyProject.h"
#include "FindTargetAllyContext.h"

#include "TargetComponent.h"
#include "../AIControllers/UnitController.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "WorldObjects/Unit.h"

void UFindTargetAllyContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
   //TODO: What the heck does this do over FindEnemyQueryContext? 
   Super::ProvideContext(QueryInstance, ContextData);

   // Get owner of this Query and cast it to an actor
   // Then get the actor's controller and cast it to AIController
   AUnitController* AICon = Cast<AUnitController>(Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController());
   if (AUnit* targetData = AICon ? AICon->GetUnitOwner()->GetTargetComponent()->GetTargetUnit() : nullptr) {
      // Set the context SeeingPawn to provided context data
      UEnvQueryItemType_Actor::SetContextHelper(ContextData, targetData);
      // GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Wee") + AICon->GetSeeingPawn()->GetActorLocation().ToString());
   }
}
