// Created 3/2/21 11:07 PM

#include "MyProject.h"
#include "Unit.h"
#include "EnvQueryItemType_Unit.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UEnvQueryItemType_Unit::SetContextHelper(FEnvQueryContextData& ContextData, const AUnit* SingleUnit)
{
   ContextData.ValueType = UEnvQueryItemType_Unit::StaticClass();
   ContextData.NumValues = 1;
   ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr));

   SetValue(ContextData.RawData.GetData(), SingleUnit);
}

void UEnvQueryItemType_Unit::SetContextHelper(FEnvQueryContextData& ContextData, const TArray<AUnit*> MultipleUnits)
{
   ContextData.ValueType = UEnvQueryItemType_Unit::StaticClass();
   ContextData.NumValues = MultipleUnits.Num();
   ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr) * MultipleUnits.Num());

   uint8* RawData = static_cast<uint8*>(ContextData.RawData.GetData());
   for(int32 ActorIndex = 0; ActorIndex < MultipleUnits.Num(); ActorIndex++)
   {
      SetValue(RawData, MultipleUnits[ActorIndex]);
      RawData += sizeof(FWeakObjectPtr);
   }
}

FVector UEnvQueryItemType_Unit::GetItemLocation(const uint8* RawData) const
{
   AActor* MyActor = UEnvQueryItemType_Unit::GetValue(RawData);
   return MyActor ? MyActor->GetActorLocation() : FAISystem::InvalidLocation;
}
