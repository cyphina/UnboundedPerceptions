#include "MyProject.h"
#include "Up_UnitQueryGenerator.h"
#include "RTSGameState.h"
#include "WorldObjects/Unit.h"
#include "UnitController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

UUp_UnitQueryGenerator::UUp_UnitQueryGenerator()
{
   // unitClass = AUnit::StaticClass();
}

void UUp_UnitQueryGenerator::GenerateItems(FEnvQueryInstance& queryInstance) const
{
   IAllUnitsContext* allUnitsContext = Cast<IAllUnitsContext>(queryInstance.Owner.Get()->GetWorld()->GetGameState());

   TArray<AActor*> matchingActors;

   for(AUnit* unit : allUnitsContext->GetAllAllyUnits()) {
      matchingActors.Add(unit);
   }

   for(AUnit* unit : allUnitsContext->GetAllEnemyUnits()) {
      matchingActors.Add(unit);
   }

   queryInstance.AddItemData<UEnvQueryItemType_Actor>(matchingActors);
}
