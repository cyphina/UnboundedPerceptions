#include "MyProject.h"

#include "AIStuff/AIControllers/UnitController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SpellCastComponent.h"

#include "SpellSystem/MySpell.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WorldObjects/Unit.h"

#include "BTTask_CastSpell.h"

#include "SingleUnitTargeting.h"
#include "AOETargeting.h"
#include "TargetComponent.h"

#include "UnitMessages.h"
#include "UpAIHelperLibrary.h"

#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "EnvironmentQuery/EnvQueryOption.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

UBTTask_CastSpell::UBTTask_CastSpell(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
   NodeName           = "Cast Spell";
   bNotifyTick        = false;
   spellToCast        = nullptr;
   targetFindingLogic = nullptr;
}

EBTNodeResult::Type UBTTask_CastSpell::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController*     unitController     = Cast<AUnitController>(ownerComp.GetAIOwner());
   USpellCastComponent* spellCastComponent = unitController->GetUnitOwner()->FindComponentByClass<USpellCastComponent>();

   if(!CheckPreconditions(unitController, spellCastComponent))
   {
      return EBTNodeResult::Failed;
   }

   WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellCasted);
   WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellCastFail);
   WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellInterrupt);
   WaitForMessage(ownerComp, UnitMessages::AIMessage_TargetLoss);

   if(targetFindingLogic)
   {
      UUpAIHelperLibrary::AIBeginCastSpell(targetFindingLogic, spellToCast, spellCastComponent);
   }
   else
   {
      HandleTargetSetFromPrevQuery(ownerComp, unitController, spellCastComponent);
   }

   return EBTNodeResult::InProgress;
}

void UBTTask_CastSpell::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   bSuccess = message != UnitMessages::AIMessage_SpellInterrupt & message != UnitMessages::AIMessage_TargetLoss & message != UnitMessages::AIMessage_SpellCastFail;
   Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
}

void UBTTask_CastSpell::InitializeFromAsset(UBehaviorTree& Asset)
{
   Super::InitializeFromAsset(Asset);

   if(spellToCast)
   {
      UMySpell* spell = spellToCast.GetDefaultObject();
      if(targetFindingLogic)
      {
         if(targetFindingLogic->GetOptionsMutable()[0]->Generator->ItemType->IsChildOf(UEnvQueryItemType_Point::StaticClass()))
         {
            if(spell->GetTargeting()->IsChildOf(UUpSpellTargeting_SingleUnit::StaticClass()))
            {
               UE_LOG(LogTemp, Error, TEXT("%s tried to cast a spell %s with the wrong targeting!"), *GetName(), *spell->GetSpellName().ToString());
            }
         }
         else if(targetFindingLogic->GetOptionsMutable()[0]->Generator->ItemType->IsChildOf(UEnvQueryItemType_ActorBase::StaticClass()))
         {
            if(spell->GetTargeting()->IsChildOf(UUpSpellTargeting_Area::StaticClass()))
            {
               UE_LOG(LogTemp, Error, TEXT("%s tried to cast a spell %s with the wrong targeting!"), *GetName(), *spell->GetSpellName().ToString());
            }
         }
      }
   }
}

FString UBTTask_CastSpell::GetStaticDescription() const
{
   if(spellToCast)
   {
      FString spellName = spellToCast.GetDefaultObject()->GetSpellDefaults().Name.ToString();
      if(spellName.IsEmpty())
      {
         spellName = "Missing Spell Name...";
      }

      if(targetFindingLogic)
      {
         return FString::Printf(TEXT("%s: %s using query\n%s to find target"), *Super::GetStaticDescription(), *spellName,
                                *targetFindingLogic->GetQueryName().ToString());
      }
      else
      {
         return FString::Printf(TEXT("%s: %s\nUsing target from prior task"), *Super::GetStaticDescription(), *spellName);
      }
   }
   return Super::GetStaticDescription();
}

bool UBTTask_CastSpell::CheckPreconditions(AUnitController* unitController, USpellCastComponent* spellCastComponent)
{
   if(!spellCastComponent)
   {
      UE_LOG(LogTemp, Error, TEXT("%s No Spell Cast Component on Unit %s trying to perform task involving casting spells!"),
             *unitController->GetUnitOwner()->GetGameName().ToString());
      return false;
   }

   if(!spellToCast)
   {
      UE_LOG(LogTemp, Error, TEXT("%s Unit %s has spell task without any spell set!"), *unitController->GetUnitOwner()->GetGameName().ToString());
      return false;
   }
   return true;
}

void UBTTask_CastSpell::HandleTargetSetFromPrevQuery(UBehaviorTreeComponent& ownerComp, AUnitController* unitController, USpellCastComponent* spellCastComponent) const
{
   const TSubclassOf<UUpSpellTargeting> spellTargeting = spellToCast.GetDefaultObject()->GetSpellDefaults().Targeting;
   if(spellTargeting->IsChildOf(UUpSpellTargeting_SingleUnit::StaticClass()))
   {
      const FVector targetLocation = ownerComp.GetBlackboardComponent()->GetValueAsVector("targetLocation");
      unitController->GetUnitOwner()->GetTargetComponent()->SetTarget(targetLocation);
   }
   else if(spellTargeting->IsChildOf(UUpSpellTargeting_Area::StaticClass()))
   {
      AUnit* targetUnit = Cast<AUnit>(ownerComp.GetBlackboardComponent()->GetValueAsObject("target"));
      unitController->GetUnitOwner()->GetTargetComponent()->SetTarget(targetUnit);
   }
   spellCastComponent->BeginCastSpell(spellToCast);
}
