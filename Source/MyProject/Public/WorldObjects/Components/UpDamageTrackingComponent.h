#pragma once

#include "Components/ActorComponent.h"
#include "Stats/RTSCircularBuffer.h"
#include "UpDamageTrackingComponent.generated.h"

struct FUpDamage;

/** A component used for tracking damage of a target over time. Can be use this data to display some stats, or use it for AI scoring systems */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpDamageTrackingComponent : public UActorComponent
{
   GENERATED_BODY()

   UUpDamageTrackingComponent();

   ///<remarks>
   ///These buffers are used to hold damage/healing events so that we can query how much damage/healing a unit took over a certain period of time
   ///</remarks>
   RTSCircularBuffer<TPair<int, float>> damageDealt     = RTSCircularBuffer<TPair<int, float>>(30);
   RTSCircularBuffer<TPair<int, float>> healingDealt    = RTSCircularBuffer<TPair<int, float>>(30);
   RTSCircularBuffer<TPair<int, float>> healingRecieved = RTSCircularBuffer<TPair<int, float>>(30);
   RTSCircularBuffer<TPair<int, float>> damageRecieved  = RTSCircularBuffer<TPair<int, float>>(30);

 protected:
   void BeginPlay() override;

 public:
   /** Reads from a circular buffer which stores damage information and a timestamp for each damage event. Gathers damage events that are within the specified timespan
    * @param timespan - Timespan we want to grab damage information for
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetDPS(float timespan, float endTimePoint);

   /** Reads from a circular buffer which stores information about hits we've received and a timestep for each heal event.
    * @param timespan - Timespan we want to grab damage information for
   */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetDamageReceivedPerSecond(float timespan, float endTimePoint);

   /** Reads from a circular buffer which stores information about healing we've dealt and a timestep for each heal event.
    * @param timespan - Timespan we want to grab damage information for
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetHealingPerSecond(float timespan, float endTimePoint);

   /** Reads from a circular buffer which stores information about healing we've received and a timestep for each heal event.
    * @param timespan - Timespan we want to grab damage information for
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   float GetHealingReceivedPerSecond(float timespan, float endTimePoint);

   //TODO: Create another buffer in ability component to hold how much a unit is getting/stunning so we can attack disablers

 private:
   // Used to insert information into this damage tracking component when we take damage
   void RecordDataOnDamageOrHealingDealtEvent(const FUpDamage& d);
   void RecordDataOnDamageOrHealingReceivedEvent(const FUpDamage& d);
   void TrackNonUnitDamageEvent(const FUpDamage& d, float worldTime);
   void TrackUnitDamageEvent(const FUpDamage& d, float worldTime);
};
