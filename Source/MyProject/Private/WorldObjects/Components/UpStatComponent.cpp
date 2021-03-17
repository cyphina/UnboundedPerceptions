#include "MyProject.h"
#include "UpStatComponent.h"
#include "WorldObjects/Unit.h"
#include "DamageStructs.h"
#include "UnitController.h"
#include "SpellSystem/RTSAbilitySystemComponent.h"

UUpStatComponent::UUpStatComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
}

void UUpStatComponent::UpdateStats(const FGameplayAttribute& updatedStat) const
{
   // There's one time at the start this can be called without a base character since this function is called when we initialize the base attribute values in baseC
   if(baseC)
   {
      baseC->StatUpdate(updatedStat);
   }
}

void UUpStatComponent::BeginPlay()
{
   Super::BeginPlay();
   AUnit* unitOwnerRef = Cast<AUnit>(GetOwner());

   baseC = MakeUnique<FBaseCharacter>(*const_cast<UMyAttributeSet*>(unitOwnerRef->GetAbilitySystemComponent()->AddSet<UMyAttributeSet>()));
}

void UUpStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FOnStatsUpdated& UUpStatComponent::OnBaseStatsUpdated() const
{
   return baseC->GetAttSet()->baseStatUpdatedEvent;
}

FOnStatsUpdated& UUpStatComponent::OnStatsUpdated() const
{
   return baseC->GetAttSet()->statUpdatedEvent;
}

float UUpStatComponent::GetElementalStatValueFromElemTag(const FGameplayTag& elemTag, bool bGetAff, bool bGetBaseValue)
{
   static const TMap<FGameplayTag, EUnitScalingStats> elemToAffMap = {{FGameplayTag::RequestGameplayTag("Combat.Element.None"), EUnitScalingStats::Physical_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Arcane"), EUnitScalingStats::Arcane_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Blood"), EUnitScalingStats::Blood_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Chaos"), EUnitScalingStats::Chaos_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Dark"), EUnitScalingStats::Darkness_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Earth"), EUnitScalingStats::Earth_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Electric"), EUnitScalingStats::Electric_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Ethereal"), EUnitScalingStats::Ethereal_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Fire"), EUnitScalingStats::Fire_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Force"), EUnitScalingStats::Physical_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Light"), EUnitScalingStats::Light_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Poison"), EUnitScalingStats::Poison_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Water"), EUnitScalingStats::Water_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Wind"), EUnitScalingStats::Wind_Aff}};

   static const TMap<FGameplayTag, EUnitScalingStats> elemToDefMap = {{FGameplayTag::RequestGameplayTag("Combat.Element.None"), EUnitScalingStats::Physical_Aff},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Arcane"), EUnitScalingStats::Arcane_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Blood"), EUnitScalingStats::Blood_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Chaos"), EUnitScalingStats::Chaos_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Dark"), EUnitScalingStats::Darkness_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Earth"), EUnitScalingStats::Earth_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Electric"), EUnitScalingStats::Electric_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Ethereal"), EUnitScalingStats::Ethereal_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Fire"), EUnitScalingStats::Fire_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Force"), EUnitScalingStats::Physical_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Light"), EUnitScalingStats::Light_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Poison"), EUnitScalingStats::Poison_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Water"), EUnitScalingStats::Water_Resist},
                                                                      {FGameplayTag::RequestGameplayTag("Combat.Element.Wind"), EUnitScalingStats::Wind_Resist}};

   const auto& map = bGetAff ? elemToAffMap : elemToDefMap;

   if(bGetBaseValue)
   {
      return GetSkillBaseValue(map[elemTag]);
   }
   else
   {
      return GetSkillAdjValue(map[elemTag]);
   }
}
