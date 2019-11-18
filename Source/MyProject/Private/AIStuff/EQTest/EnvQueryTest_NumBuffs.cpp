// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "EQTest/EnvQueryTest_NumBuffs.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "WorldObjects/Unit.h"
#include "AbilitySystemComponent.h"

UEnvQueryTest_NumBuffs::UEnvQueryTest_NumBuffs()
{
   Cost          = EEnvTestCost::Low;
   ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
   debuffTags = FGameplayTag::RequestGameplayTag("Combat.Effect.Debuff");
}

void UEnvQueryTest_NumBuffs::RunTest(FEnvQueryInstance& QueryInstance) const
{
   UObject* QueryOwner = QueryInstance.Owner.Get();
   if (QueryOwner == nullptr) { return; }

   FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
   float MinThresholdValue = FloatValueMin.GetValue();

   FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
   float MaxThresholdValue = FloatValueMax.GetValue();

   for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It) {
      const AActor* itemActor = GetItemActor(QueryInstance, It.GetIndex());
      if (const AUnit* unitRef = Cast<AUnit>(itemActor)) {
         const float numDebuffs = unitRef->GetAbilitySystemComponent()->GetActiveEffects(FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer(debuffTags))).Num();
         It.SetScore(TestPurpose, FilterType, numDebuffs, MinThresholdValue, MaxThresholdValue);
      }
   }
}

FText UEnvQueryTest_NumBuffs::GetDescriptionTitle() const
{
   return NSLOCTEXT("EQTest", "NumBuffsDesc", "Number of debuffs on unit");
}

FText UEnvQueryTest_NumBuffs::GetDescriptionDetails() const
{
   return DescribeFloatTestParams();
}
