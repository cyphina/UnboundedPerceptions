// Created 3/12/21 08:42 AM

#pragma once
#include "BehaviorTree/BTCompositeNode.h"
#include "BTComposite_ForwardSequence.generated.h"

/**
 * A sequence that continues to execute all its child nodes even if one fails.
 */
UCLASS()
class MYPROJECT_API UBTComposite_ForwardSequence : public UBTCompositeNode
{
   GENERATED_BODY()

public:
   UBTComposite_ForwardSequence();

protected:
   virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;

#if WITH_EDITOR
   virtual bool  CanAbortLowerPriority() const override;
   virtual FName GetNodeIconName() const override;
#endif
};
