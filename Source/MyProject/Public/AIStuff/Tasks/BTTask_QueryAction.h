// Created 3/6/21 7:58 PM

#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTTask_QueryAction.generated.h"

/**
 * @brief Base class of any action that requires us to query what to do before acting.
 * Any action that takes some time to perform is recommended to have some kind of timer (to stop it if it's going too long).
 */
UCLASS(Abstract)
class MYPROJECT_API UBTTask_QueryAction : public UBTTaskNode
{
   GENERATED_BODY()

 public:
   UBTTask_QueryAction();

 protected:
   /**
    * @brief Used to find the location or actor we need to perform our action on.
    */
   UPROPERTY(EditAnywhere)
   class UEnvQuery* actionQuery;

   UPROPERTY(EditAnywhere)
   TEnumAsByte<EEnvQueryRunMode::Type> queryRunMode;

   FString      GetStaticDescription() const override;
};
