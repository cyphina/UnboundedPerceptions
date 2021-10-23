// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_Action.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIStuff/AIControllers/UnitController.h"

UBTTask_Action::UBTTask_Action()
{
   bNotifyTick = false;
   NodeName = "Random Pick";
}

EBTNodeResult::Type UBTTask_Action::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitControllerRef = Cast<AUnitController>(ownerComp.GetAIOwner());
   int randInt = FMath::RandRange(0,100);

   for(int i = 0; i < numberCap.Num(); ++i)
   {
      if(randInt < numberCap[i])
      {
         ownerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKey.SelectedKeyName, i);
         return EBTNodeResult::Succeeded;
      }
   }

   ownerComp.GetBlackboardComponent()->SetValueAsInt(BlackboardKey.SelectedKeyName, numberCap.Num());
   return EBTNodeResult::Succeeded;
}
