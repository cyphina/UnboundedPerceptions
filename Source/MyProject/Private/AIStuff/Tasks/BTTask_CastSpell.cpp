// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"

#include "AIStuff/AIControllers/UnitController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "SpellSystem/MySpell.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WorldObjects/Unit.h"

#include "BTTask_CastSpell.h"


#include "SingleUnitTargeting.h"
#include "UnitMessages.h"
#include "UpAIHelperLibrary.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "EnvironmentQuery/EnvQueryOption.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

UBTTask_CastSpell::UBTTask_CastSpell(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   NodeName    = "Cast Spell";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_CastSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController*     unitController     = Cast<AUnitController>(ownerComp.GetAIOwner());
   USpellCastComponent* spellCastComponent = unitController->GetUnitOwner()->FindComponentByClass<USpellCastComponent>();

   if(!spellCastComponent) {
      UE_LOG(LogTemp, Error, TEXT("%s No Spell Cast Component on Unit %s trying to perform task involving casting spells!"),
             *unitController->GetUnitOwner()->GetGameName().ToString());
      return EBTNodeResult::Failed;
   }

   if(!spellToCast) {
      UE_LOG(LogTemp, Error, TEXT("%s Unit %s has spell task without any spell set!"),
             *unitController->GetUnitOwner()->GetGameName().ToString());
      return EBTNodeResult::Failed;
   }

   UMySpell* spell = spellToCast.GetDefaultObject();

   if(targetFindingLogic->GetOptionsMutable()[0]->Generator->ItemType == UEnvQueryItemType_VectorBase::StaticClass()) {
      if(spell->GetTargeting()->IsChildOf(TSubclassOf<UUpSpellTargeting_SingleUnit>())) {
         UE_LOG(LogTemp, Error, TEXT("%s tried to cast a spell %s with the wrong targeting!"), *unitController->GetUnitOwner()->GetGameName().ToString(),
                *spell->GetSpellName().ToString());
         return EBTNodeResult::Failed;
      }
   }

   WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellCasted);
   WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellCastFail);
   WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellInterrupt);
   WaitForMessage(ownerComp, UnitMessages::AIMessage_TargetLoss);

   if(!targetFindingLogic)
      targetFindingLogic = spell->GetDefaultQueryForSpell(unitController->GetWorld());

   UUpAIHelperLibrary::AIBeginCastSpell(targetFindingLogic, spellToCast, spellCastComponent);

   return EBTNodeResult::InProgress;
}

void UBTTask_CastSpell::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   bSuccess = message != UnitMessages::AIMessage_SpellInterrupt & message != UnitMessages::AIMessage_TargetLoss & message != UnitMessages::AIMessage_SpellCastFail;
   Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
}
