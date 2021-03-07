#include "MyProject.h"

#include "BTTask_BroadcastAIMessage.h"

#include "UnitController.h"

namespace BroadcastAIMessage
{
   bool                           bLogAIBroadcastMessages;
   static FAutoConsoleVariableRef CVarEnableEnemyControl(TEXT("up.enableEnemyControl"), bLogAIBroadcastMessages,
                                                         TEXT("When this flag is set to 1, we print log information for all AI Broadcast Message Tasks"));

}

UBTTask_BroadcastAIMessage::UBTTask_BroadcastAIMessage()
{
   NodeName       = "Send AI Message";
   bNotifyTick    = false;
   messageNametag = FGameplayTag::RequestGameplayTag("Combat.AI.Message");
}

EBTNodeResult::Type UBTTask_BroadcastAIMessage::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   FString tagName = messageNametag.ToString();
   if(tagName.RemoveFromStart(tagPrefix))
   {
      const FAIMessage msg(*tagName, this);
      if(BroadcastAIMessage::bLogAIBroadcastMessages)
      {
         GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, tagName);
      }

      // TODO: Foreach through enemies in range
      FAIMessage::Send(ownerComp.GetAIOwner(), msg);
      return EBTNodeResult::Succeeded;
   }
   return EBTNodeResult::Failed;
}
