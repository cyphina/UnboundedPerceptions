// Created 3/9/21 2:32 AM

#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_LoopLatent.generated.h"

struct FBTLoopLatentDecoratorMemory
{
   int32 SearchId;
   uint8 RemainingExecutions;
   float TimeStarted;
};

/**
 * Like the loop deorator but can handle latent tasks. Keeps looping if the node it decorates succeeds.
 */
UCLASS()
class MYPROJECT_API UBTDecorator_LoopLatent : public UBTDecorator
{
   GENERATED_BODY()

   UBTDecorator_LoopLatent();

 public:
   /** number of executions */
   UPROPERTY(Category = Decorator, EditAnywhere, meta = (EditCondition = "!bInfiniteLoop", ClampMin = "0"))
   int32 MinNumLoops;

   /** random number of loops to add or remove */
   UPROPERTY(Category = Decorator, EditAnywhere, meta = (EditCondition = "!bInfiniteLoop", ClampMin = "0"))
   int32 MaxNumLoops;

   /** infinite loop */
   UPROPERTY(Category = Decorator, EditAnywhere)
   bool bInfiniteLoop;

   /** timeout (when looping infinitely, when we finish a loop we will check whether we have spent this time looping, if we have we will stop looping). A negative value means loop forever. */
   UPROPERTY(Category = Decorator, EditAnywhere, meta = (EditCondition = "bInfiniteLoop"))
   float InfiniteLoopTimeoutTime;

   virtual uint16  GetInstanceMemorySize() const override;
   virtual void    DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity,
                                         TArray<FString>& Values) const override;
   virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
   virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

 protected:
   virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
   virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
};
