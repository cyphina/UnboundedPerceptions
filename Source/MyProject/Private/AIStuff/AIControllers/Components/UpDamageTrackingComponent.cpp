#include "WorldObjects/Components/UpDamageTrackingComponent.h"

#include "Unit.h"
#include "UpStatComponent.h"
#include "SpellSystem/Calcs/RTSDamageCallbacks.h"

UUpDamageTrackingComponent::UUpDamageTrackingComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
}

void UUpDamageTrackingComponent::BeginPlay()
{
   AUnit* unitOwner = Cast<AUnit>(GetOwner());
   unitOwner->OnUnitDamageDealtEvent //OnDamageEvent.AddUObject(this, &UUpDamageTrackingComponent::RecordDataOnDamageOrHealingEvent);
   unitOwner->OnUnitDamageReceivedEvent
}

void UUpDamageTrackingComponent::TrackNonUnitDamageEvent(const FUpDamage& d, float worldTime)
{
   if(GetOwner() == d.targetUnit) {
      if(!d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing"))) {
         damageRecieved.Insert(TPair<int, float>(d.damage, worldTime));
      } else {
         healingRecieved.Insert(TPair<int, float>(d.damage, worldTime));
      }
   }
}

void UUpDamageTrackingComponent::TrackUnitDamageEvent(const FUpDamage& d, float worldTime)
{
   if(LIKELY(!d.effects.HasTag(FGameplayTag::RequestGameplayTag("Combat.DamageEffects.Healing")))) {
      damageDealt.Insert(TPair<int, float>(d.damage, worldTime));
      damageRecieved.Insert(TPair<int, float>(d.damage, worldTime));
   } else {
      healingDealt.Insert(TPair<int, float>(d.damage, worldTime));
      healingRecieved.Insert(TPair<int, float>(d.damage, worldTime));
   }
}

void UUpDamageTrackingComponent::RecordDataOnDamageOrHealingReceivedEvent(const FUpDamage& d)
{
   // Store timestamp of when damage was taken for informative purposes
   float worldTime = GetWorld()->GetTimeSeconds();
   if(d.sourceUnit) {
      TrackUnitDamageEvent(d, worldTime);
   } else {
      TrackNonUnitDamageEvent(d, worldTime);
   }
}

void UUpDamageTrackingComponent::RecordDataOnDamageOrHealingDealtEvent(const FUpDamage& d)
{
   
}

float UUpDamageTrackingComponent::GetDPS(float timespan, float endTimePoint)
{
   unsigned int      i                   = 0;
   int               totalDamage         = 0;
   TPair<int, float> currentDamageAmount = damageDealt.GetPrevItem(i);

   while(currentDamageAmount.Value + timespan > endTimePoint && i < damageDealt.Size()) {
      totalDamage += currentDamageAmount.Key;
      currentDamageAmount = damageDealt.GetPrevItem(++i);
   }
   return totalDamage / timespan;
}

float UUpDamageTrackingComponent::GetDamageReceivedPerSecond(float timespan, float endTimePoint)
{
   unsigned int      i                   = 0;
   int               totalDamageRecieved = 0;
   TPair<int, float> currentDamageAmount = damageRecieved.GetPrevItem(i);

   while(currentDamageAmount.Value + timespan > endTimePoint && i < damageRecieved.Size()) {
      totalDamageRecieved += currentDamageAmount.Key;
      currentDamageAmount = damageDealt.GetPrevItem(++i);
   }
   return totalDamageRecieved / timespan;
}

float UUpDamageTrackingComponent::GetHealingPerSecond(float timespan, float endTimePoint)
{
   unsigned int      i                   = 0;
   int               totalHealingDealt   = 0;
   TPair<int, float> currentDamageAmount = healingDealt.GetPrevItem(i);

   while(currentDamageAmount.Value + timespan > endTimePoint && i < healingDealt.Size()) {
      totalHealingDealt += currentDamageAmount.Key;
      currentDamageAmount = healingDealt.GetPrevItem(++i);
   }
   return totalHealingDealt / timespan;
}

float UUpDamageTrackingComponent::GetHealingReceivedPerSecond(float timespan, float endTimePoint)
{
   unsigned int      i                    = 0;
   int               totalHealingRecieved = 0;
   TPair<int, float> currentDamageAmount  = healingRecieved.GetPrevItem(i);

   while(currentDamageAmount.Value + timespan > endTimePoint && i < healingRecieved.Size()) {
      totalHealingRecieved += currentDamageAmount.Key;
      currentDamageAmount = healingRecieved.GetPrevItem(++i);
   }
   return totalHealingRecieved / timespan;
}
