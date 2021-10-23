#include "MyProject.h"
#include "EnvQueryTest_IsEnemy.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Unit.h"
#include "WorldObjects/Enemies/Enemy.h"

DECLARE_FLOAT_COUNTER_STAT(TEXT("EnvQuery"), Stat_EnvQuery_IsEnemy, STATGROUP_AI_EQS);

UEnvQueryTest_IsEnemy::UEnvQueryTest_IsEnemy()
{
   Cost          = EEnvTestCost::Low;
   TestPurpose   = EEnvTestPurpose::Filter;
   FilterType    = EEnvTestFilterType::Match;
   ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
}

void UEnvQueryTest_IsEnemy::RunTest(FEnvQueryInstance& QueryInstance) const
{
   SCOPE_CYCLE_COUNTER(Stat_EnvQuery_IsEnemy);

   AUnit* unitOwner = Cast<AUnit>(QueryInstance.Owner.Get());
   if(!unitOwner)
   {
      return;
   }

   const bool isUnitOwnerEnemy = unitOwner->GetIsEnemy();

   if(isUnitOwnerEnemy)
   {
      for(FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
      {
         const AActor* itemActor = GetItemActor(QueryInstance, It.GetIndex());

         if(itemActor->GetClass()->IsChildOf(AEnemy::StaticClass()))
         {
            It.SetScore(TestPurpose, FilterType, false, true);
         }
         else
         {
            It.SetScore(TestPurpose, FilterType, true, true);
         }
      }
   }

   else
   {
      for(FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
      {
         const AActor* itemActor = GetItemActor(QueryInstance, It.GetIndex());

         if(itemActor->GetClass()->IsChildOf(AEnemy::StaticClass()))
         {
            It.SetScore(TestPurpose, FilterType, true, true);
         }
         else
         {
            It.SetScore(TestPurpose, FilterType, false, true);
         }
      }
   }
}

FText UEnvQueryTest_IsEnemy::GetDescriptionTitle() const
{
   return NSLOCTEXT("EQTest", "IsEnemyDesc", "Is Unit An Enemy of Mine?");
}

FText UEnvQueryTest_IsEnemy::GetDescriptionDetails() const
{
   return DescribeFloatTestParams();
}
