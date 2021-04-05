// Created 3/9/21 2:36 AM

#include "MyProject.h"
#include "BTDecorator_LoopLatent.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"

UBTDecorator_LoopLatent::UBTDecorator_LoopLatent()
{
   NodeName                = "Latent Loop";
   MaxNumLoops             = 3;
   MinNumLoops             = 0;
   InfiniteLoopTimeoutTime = -1.f;
   bNotifyActivation       = true;
   bNotifyDeactivation     = true;

   bAllowAbortNone       = false;
   bAllowAbortLowerPri   = false;
   bAllowAbortChildNodes = false;
}

void UBTDecorator_LoopLatent::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
   FBTLoopLatentDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTLoopLatentDecoratorMemory>(SearchData);
   FBTCompositeMemory*           ParentMemory    = GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData);
   const bool                    bIsSpecialNode  = GetParentNode()->IsA(UBTComposite_SimpleParallel::StaticClass());

   // Before the first activation these values won't be initialized, so we can use them to know if it's the first time this loop will be run.
   if((bIsSpecialNode && ParentMemory->CurrentChild == BTSpecialChild::NotInitialized) || (!bIsSpecialNode && ParentMemory->CurrentChild != ChildIndex))
   {
      int numLoops                         = FMath::RandRange(MinNumLoops, MaxNumLoops);
      DecoratorMemory->RemainingExecutions = numLoops;
      DecoratorMemory->TimeStarted         = GetWorld()->GetTimeSeconds();
   }
}

void UBTDecorator_LoopLatent::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
   if(NodeResult == EBTNodeResult::Type::Succeeded)
   {
      FBTLoopLatentDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTLoopLatentDecoratorMemory>(SearchData);

      bool bShouldLoop = false;
      if(bInfiniteLoop)
      {
         // Protect from truly infinite loop within single search (some parallel node thing)
         // SearchId seems to just be a unique id for each node to differentiate nodes between searches (searching the next child to be executed)
         if(SearchData.SearchId != DecoratorMemory->SearchId)
         {
            if((InfiniteLoopTimeoutTime < 0.f) || ((DecoratorMemory->TimeStarted + InfiniteLoopTimeoutTime) > GetWorld()->GetTimeSeconds()))
            {
               bShouldLoop = true;
            }
         }

         DecoratorMemory->SearchId = SearchData.SearchId;
      }
      else
      {
         DecoratorMemory->RemainingExecutions--;
         bShouldLoop = DecoratorMemory->RemainingExecutions > 0;
      }

      if(bShouldLoop)
      {
         GetParentNode()->SetChildOverride(SearchData, ChildIndex);
      }
   }
}

uint16 UBTDecorator_LoopLatent::GetInstanceMemorySize() const
{
   return sizeof(FBTLoopLatentDecoratorMemory);
}

void UBTDecorator_LoopLatent::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity,
                                                    TArray<FString>& Values) const
{
   Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

   if(!bInfiniteLoop)
   {
      FBTLoopLatentDecoratorMemory* DecoratorMemory = (FBTLoopLatentDecoratorMemory*)NodeMemory;
      Values.Add(FString::Printf(TEXT("loops remaining: %d"), DecoratorMemory->RemainingExecutions));
   }
   else if(InfiniteLoopTimeoutTime > 0.f)
   {
      FBTLoopLatentDecoratorMemory* DecoratorMemory = (FBTLoopLatentDecoratorMemory*)NodeMemory;

      const float TimeRemaining = InfiniteLoopTimeoutTime - (GetWorld()->GetTimeSeconds() - DecoratorMemory->TimeStarted);
      Values.Add(FString::Printf(TEXT("time remaining: %s"), *FString::SanitizeFloat(InfiniteLoopTimeoutTime)));
   }
}

FString UBTDecorator_LoopLatent::GetStaticDescription() const
{
   // basic info: infinite / num loops
   if(bInfiniteLoop)
   {
      if(InfiniteLoopTimeoutTime < 0.f)
      {
         return FString::Printf(TEXT("Loops Infinitely"));
      }
      else
      {
         return FString::Printf(TEXT("Loops for %s seconds"), *FString::SanitizeFloat(InfiniteLoopTimeoutTime));
      }
   }
   else
   {
      return FString::Printf(TEXT("Loops %d-%d times"), MinNumLoops, MaxNumLoops);
   }
}

FName UBTDecorator_LoopLatent::GetNodeIconName() const
{
   return FName("BTEditor.Graph.BTNode.Decorator.Loop.Icon");
}
