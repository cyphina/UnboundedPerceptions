#include "MyProject.h"
#include "EnvQueryTest_FastOverlapEnemy.h"
#include "AIStuff/UpAIHelperLibrary.h"
#include "AIStuff/AI_Globals.h"
#include "Unit.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"

UEnvQueryTest_FastOverlapEnemy::UEnvQueryTest_FastOverlapEnemy() : Super()
{
   Cost          = EEnvTestCost::Low;
   TestPurpose   = EEnvTestPurpose::FilterAndScore;
   ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void UEnvQueryTest_FastOverlapEnemy::RunTest(FEnvQueryInstance& QueryInstance) const
{
   AUnit* owningUnit = Cast<AUnit>(QueryInstance.Owner.Get());

   float range = 0;
   if(QueryInstance.NamedParams.Contains(AIGlobals::EQS_RADIUS_PARAM))
   {
      range = QueryInstance.NamedParams[AIGlobals::EQS_RADIUS_PARAM];
   }
   else
   {
      range = defaultOverlapRadius;
   }

   const TArray<AUnit*>& enemyUnits = owningUnit->GetVisibleEnemies();

   /*Setup min and max test values allowed.  Values are bound for faster access*/
   FloatValueMin.BindData(owningUnit, QueryInstance.QueryID);
   const float MinThresholdValue = FloatValueMin.GetValue();

   FloatValueMax.BindData(owningUnit, QueryInstance.QueryID);
   const float MaxThresholdValue = FloatValueMax.GetValue();

   for(FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
   {
      int           score             = 0;
      const FVector generatedLocation = GetItemLocation(QueryInstance, It);
      for(AUnit* enemyUnit : enemyUnits)
      {
         const bool bInRange = UUpAIHelperLibrary::IsTargetInRange(enemyUnit, generatedLocation, range);
         if(bInRange)
         {
            ++score;
         }
      }
      It.SetScore(TestPurpose, FilterType, score, MinThresholdValue, MaxThresholdValue);
   }
}

FText UEnvQueryTest_FastOverlapEnemy::GetDescriptionDetails() const
{
   return FText::Format(NSLOCTEXT("EnvQueryTest", "FastOverlapDescription", "Number of enemies overlapped: {0}"), DescribeFloatTestParams());
}
