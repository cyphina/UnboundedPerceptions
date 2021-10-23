// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Up_AlliesQueryGenerator.h"
#include "EnvQueryItemType_Unit.h"
#include "RTSGameState.h"
#include "UnitController.h"
#include "WorldObjects/Ally.h"

UUp_AlliesQueryGenerator::UUp_AlliesQueryGenerator()
{
   ItemType = UEnvQueryItemType_Unit::StaticClass();
}

void UUp_AlliesQueryGenerator::GenerateItems(FEnvQueryInstance& queryInstance) const
{
   AUnit* queryOwnerUnit = Cast<AUnit>(queryInstance.Owner.Get());

   if(filterRadius > 0)
   {
      auto pred = [queryOwnerUnit, this](const AUnit* unit) {
         return FVector::Dist2D(queryOwnerUnit->GetActorLocation(), unit->GetActorLocation()) < filterRadius * filterRadius;
      };
      TArray<AUnit*> unitsInRange = queryOwnerUnit->GetAllies().FilterByPredicate(pred);

      for(auto& item : unitsInRange)
      {
         queryInstance.AddItemData<UEnvQueryItemType_Unit>(item);
      }
   }
   else
   {
      for(auto& item : queryOwnerUnit->GetAllies())
      {
         queryInstance.AddItemData<UEnvQueryItemType_Unit>(item);
      }
   }
}

FText UUp_AlliesQueryGenerator::GetDescriptionTitle() const
{
   return NSLOCTEXT("EnvQueryGenerator", "TeammateGeneratorTitle", "Generates Set of Friendly Units (optionally filtered by distance)");
}

FText UUp_AlliesQueryGenerator::GetDescriptionDetails() const
{
   return FText::Format(NSLOCTEXT("EnvQueryGenerator", "TeammateGeneratorDetails", "Radius: {0} units"), FText::AsNumber(filterRadius));
}
