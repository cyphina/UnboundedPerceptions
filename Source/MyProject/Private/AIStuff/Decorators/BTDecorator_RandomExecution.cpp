// Created 3/8/21 9:18 AM

#include "MyProject.h"
#include "BTDecorator_RandomExecution.h"

UBTDecorator_RandomExecution::UBTDecorator_RandomExecution()
{
   NodeName              = "Chance to Execute";
   bAllowAbortNone       = false;
   bAllowAbortLowerPri   = false;
   bAllowAbortChildNodes = false;
}

bool UBTDecorator_RandomExecution::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   const int randomVal = FMath::RandRange(0, 100);
   if(chanceToExecute > randomVal)
   {
      return true;
   }
   return false;
}

FString UBTDecorator_RandomExecution::GetStaticDescription() const
{
   if(IsInversed())
   {
      return FString::Printf(TEXT("Chance: %d%%"), 100 - chanceToExecute);
   }
   else
   {
      return FString::Printf(TEXT("Chance: %d%%"), chanceToExecute);
   }
}
