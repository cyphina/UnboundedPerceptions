// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyEnvQueryTest_DamageScore.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

#include "Unit.h"
#include "SpellSystem/MySpell.h"

namespace
{
   FORCEINLINE float CalcDistance2D(const FVector& PosA, const FVector& PosB) { return (PosB - PosA).Size2D(); }
} // namespace

UMyEnvQueryTest_DamageScore::UMyEnvQueryTest_DamageScore()
{
   Cost          = EEnvTestCost::Low;
   ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
}

void UMyEnvQueryTest_DamageScore::RunTest(FEnvQueryInstance& QueryInstance) const
{
   // make sure we have an owner for this test instance
   UObject* QueryOwner = QueryInstance.Owner.Get();
   if (QueryOwner == nullptr) { return; }

   /*Setup min and max test values allowed.  Values are bound for faster access*/
   FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
   float MinThresholdValue = FloatValueMin.GetValue();

   FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
   float MaxThresholdValue = FloatValueMax.GetValue();

   for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It) {
      AUnit*      unit        = Cast<AUnit>(GetItemActor(QueryInstance, It));
      const float threatValue = FindNumOffensiveAbilities(unit) + GetHighestOffensiveAttribute(unit);
      It.SetScore(TestPurpose, FilterType, threatValue, MinThresholdValue, MaxThresholdValue);
   }
}

FText UMyEnvQueryTest_DamageScore::GetDescriptionTitle() const
{
   return NSLOCTEXT("EQTest", "DamageFilter", "Get unit with highest damage potential");
}

FText UMyEnvQueryTest_DamageScore::GetDescriptionDetails() const
{
   return DescribeFloatTestParams();
}

int UMyEnvQueryTest_DamageScore::FindNumOffensiveAbilities(AUnit* unit) const
{
   int numOffensiveAbilities = 0;
   for (TSubclassOf<UMySpell> spellClass : unit->abilities) {
      UMySpell* spell = spellClass.GetDefaultObject();
      if (!spell->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing"))) {
         FDamageScalarStruct damageStruct = spell->GetDamage(unit->GetAbilitySystemComponent());
         if (damageStruct.intelligence + damageStruct.agility + damageStruct.strength + damageStruct.understanding > 150) ++numOffensiveAbilities;
      }
   }
   return numOffensiveAbilities;
}

int UMyEnvQueryTest_DamageScore::GetHighestOffensiveAttribute(AUnit* unit) const
{
   int maxValue = 0;
   // TODO: Maybe check affinity but its costly to loop through all that
   for (int i = 0; i < CombatInfo::AttCount; ++i) {
      int attValue = unit->GetAttributeAdjValue(i);
      if (attValue > maxValue) maxValue = attValue;
   }

   for (int i = STAT_AFF_START_INDEX; i < STAT_AFF_END_INDEX; ++i) {
      int attValue = unit->GetAttributeAdjValue(i);
      if (attValue > maxValue) maxValue = attValue;
   }

   return maxValue;
}
