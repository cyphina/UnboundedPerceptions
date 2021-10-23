// Created 3/2/21 11:02 PM

#pragma once
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvQueryItemType_Unit.generated.h"

class AUnit;

UCLASS()
class MYPROJECT_API UEnvQueryItemType_Unit : public UEnvQueryItemType_Actor
{
   GENERATED_BODY()

 public:
   static void SetContextHelper(FEnvQueryContextData& ContextData, const AUnit* SingleUnit);
   static void SetContextHelper(FEnvQueryContextData& ContextData, const TArray<AUnit*> MultipleUnits);

 private:
   virtual FVector GetItemLocation(const uint8* RawData) const override;
   using UEnvQueryItemType_Actor::SetContextHelper;
};
