// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "BTTask_AIMessage.h"

#include "UnitController.h"

UBTTask_AIMessage::UBTTask_AIMessage()
{
   NodeName = "Send AI Message";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_AIMessage::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FString tagName = messageNametag.ToString();
   if (tagName.RemoveFromStart(tagPrefix))
   {
      FAIMessage msg(*tagName, this);
      GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, tagName);
      FAIMessage::Send(ownerComp.GetAIOwner(), msg);
      return EBTNodeResult::Succeeded;
   }
   return EBTNodeResult::Failed;
}

