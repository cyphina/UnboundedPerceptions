// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyEnvQueryTest_DamageScore.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Algo/RemoveIf.h"
#include "UpResourceManager.h"
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
   const int offensiveSpellsIndexEnd = Algo::RemoveIf(unit->abilities, [](TSubclassOf<UMySpell> spellClass) {
      return spellClass.GetDefaultObject()->AbilityTags.HasAny(UpResourceManager::offensiveTags);
   });

   for (int i = 0; i < offensiveSpellsIndexEnd; ++ i) {
      UMySpell* spell = unit->abilities[i].GetDefaultObject(); {
         // Only increase score against damaing spells
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
   for (auto x : TEnumRange<EAttributes>()) {
      int attValue = unit->GetAttributeAdjValue(x);
      if (attValue > maxValue) maxValue = attValue;
   }

   for (uint8 i = STAT_AFF_START_INDEX; i < STAT_AFF_END_INDEX; ++i) {
      float skillAffValue = unit->GetSkillAdjValue(static_cast<EUnitScalingStats>(i));
      if (skillAffValue > maxValue) maxValue = skillAffValue;
   }

   return maxValue;
}
