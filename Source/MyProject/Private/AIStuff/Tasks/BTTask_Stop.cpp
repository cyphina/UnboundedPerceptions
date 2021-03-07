#include "MyProject.h"
#include "BTTask_Stop.h"
#include "UnitController.h"

UBTTask_Stop::UBTTask_Stop()
{
   NodeName    = "Stop Unit";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_Stop::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitC = Cast<AUnitController>(ownerComp.GetAIOwner());
   checkf(unitC, TEXT("Called Stop on a non-unit or UnitController is missing"));
   unitC->StopCurrentAction();
   return EBTNodeResult::Succeeded;
}
