#include "MyProject.h"
#include "EnvQueryTest_DamageScore.h"

#include "SpellDataLibrary.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Algo/RemoveIf.h"
#include "UpResourceManager.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

#include "Unit.h"
#include "UpStatComponent.h"
#include "SpellSystem/MySpell.h"

namespace
{
   FORCEINLINE float CalcDistance2D(const FVector& PosA, const FVector& PosB) { return (PosB - PosA).Size2D(); }
}

UEnvQueryTest_DamageScore::UEnvQueryTest_DamageScore()
{
   Cost          = EEnvTestCost::Low;
   ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
}

void UEnvQueryTest_DamageScore::RunTest(FEnvQueryInstance& QueryInstance) const
{
   // make sure we have an owner for this test instance
   UObject* QueryOwner = QueryInstance.Owner.Get();
   if(QueryOwner == nullptr)
   {
      return;
   }

   // Setup min and max test values allowed.  Values are bound for faster access
   FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
   const float MinThresholdValue = FloatValueMin.GetValue();

   FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
   const float MaxThresholdValue = FloatValueMax.GetValue();

   for(FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
   {
      AUnit*      unit        = Cast<AUnit>(GetItemActor(QueryInstance, It));
      const float threatValue = FindNumOffensiveAbilities(unit) + GetHighestOffensiveAttribute(unit);
      It.SetScore(TestPurpose, FilterType, threatValue, MinThresholdValue, MaxThresholdValue);
   }
}

FText UEnvQueryTest_DamageScore::GetDescriptionTitle() const
{
   return NSLOCTEXT("EQTest", "DamageFilter", "Get unit with highest damage potential");
}

FText UEnvQueryTest_DamageScore::GetDescriptionDetails() const
{
   return DescribeFloatTestParams();
}

int UEnvQueryTest_DamageScore::FindNumOffensiveAbilities(AUnit* unit) const
{
   int numOffensiveAbilities = 0;

   for(const auto& spellClass : unit->GetAbilitySystemComponent()->GetAbilities())
   {
      if(spellClass.GetDefaultObject()->AbilityTags.HasAny(USpellDataLibrary::GetOffensiveTags()))
      {
         UMySpell* spell = spellClass.GetDefaultObject();
         {
            // Only increase score against spells that hurt you a decent amount
            const FDamageScalarStruct damageStruct = spell->GetDamage(unit->GetAbilitySystemComponent());
            if(damageStruct.intelligence + damageStruct.agility + damageStruct.strength + damageStruct.understanding > 150) ++numOffensiveAbilities;
         }
      }
   }
   return numOffensiveAbilities;
}

int UEnvQueryTest_DamageScore::GetHighestOffensiveAttribute(AUnit* unit) const
{
   int maxValue = 0;
   // TODO: Maybe check affinity but its costly to loop through all that
   for(auto x : TEnumRange<EAttributes>())
   {
      const int attValue = unit->GetStatComponent()->GetAttributeAdjValue(x);
      if(attValue > maxValue) maxValue = attValue;
   }

   for(uint8 i = STAT_AFF_START_INDEX; i < STAT_AFF_END_INDEX; ++i)
   {
      const float skillAffValue = unit->GetStatComponent()->GetSkillAdjValue(static_cast<EUnitScalingStats>(i));
      if(skillAffValue > maxValue) maxValue = skillAffValue;
   }

   return maxValue;
}
