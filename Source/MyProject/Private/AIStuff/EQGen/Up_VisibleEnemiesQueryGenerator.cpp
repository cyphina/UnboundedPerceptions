#include "MyProject.h"
#include "Up_VisibleEnemiesQueryGenerator.h"
#include "EnvQueryItemType_Unit.h"
#include "UnitController.h"
#include "WorldObjects/Enemies/Enemy.h"

UUp_VisibleEnemiesQueryGenerator::UUp_VisibleEnemiesQueryGenerator()
{
   ItemType = UEnvQueryItemType_Unit::StaticClass();
}

void UUp_VisibleEnemiesQueryGenerator::GenerateItems(FEnvQueryInstance& queryInstance) const
{
   AUnit* queryOwnerUnit = Cast<AUnit>(queryInstance.Owner.Get());

   if(filterRadius >= 0)
   {
      auto pred = [queryOwnerUnit, this](const AUnit* unit) {
         return FVector::Dist2D(queryOwnerUnit->GetActorLocation(), unit->GetActorLocation()) < filterRadius * filterRadius;
      };

      TArray<AUnit*> unitsInRange = queryOwnerUnit->GetEnemies().FilterByPredicate(pred);
      for(auto& item : unitsInRange)
      {
         queryInstance.AddItemData<UEnvQueryItemType_Unit>(item);
      }
   }
   else
   {
      for(auto& item : queryOwnerUnit->GetVisibleEnemies())
      {
         queryInstance.AddItemData<UEnvQueryItemType_Unit>(item);
      }
   }
}

FText UUp_VisibleEnemiesQueryGenerator::GetDescriptionTitle() const
{
   return NSLOCTEXT("EnvQueryGenerator", "VisibleEnemiesGenerator", "Generates Enemies in Range");
}

FText UUp_VisibleEnemiesQueryGenerator::GetDescriptionDetails() const
{
   FText rangeText =
       filterRadius >= 0 ? FText::AsNumber(filterRadius) : NSLOCTEXT("EnvQueryGenerator", "EnemyGeneratorVisionRange", "(Size Determined by Vision Radius)");
   return FText::Format(NSLOCTEXT("EnvQueryGenerator", "EnemyGeneratorDetails", "Radius: {0} units"), rangeText);
}
