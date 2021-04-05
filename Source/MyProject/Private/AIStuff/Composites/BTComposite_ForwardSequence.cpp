// Created 3/12/21 08:41 AM

#include "MyProject.h"
#include "Composites/BTComposite_ForwardSequence.h"

UBTComposite_ForwardSequence::UBTComposite_ForwardSequence()
{
   NodeName = "Forward Sequence";
}

int32 UBTComposite_ForwardSequence::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
   int32 NextChildIdx = BTSpecialChild::ReturnToParent;

   if(PrevChild == BTSpecialChild::NotInitialized)
   {
      NextChildIdx = 0;
   }
   else if(PrevChild + 1 < GetChildrenNum())
   {
      NextChildIdx = PrevChild + 1;
   }

   return NextChildIdx;
}

#if WITH_EDITOR
bool UBTComposite_ForwardSequence::CanAbortLowerPriority() const
{
   return false;
}

FName UBTComposite_ForwardSequence::GetNodeIconName() const
{
   return FName("BTEditor.Graph.BTNode.Composite.Sequence.Icon");
}
#endif
