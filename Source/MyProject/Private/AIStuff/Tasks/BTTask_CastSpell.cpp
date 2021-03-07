#include "MyProject.h"

#include "AIStuff/AIControllers/UnitController.h"
#include "SpellCastComponent.h"

#include "SpellSystem/MySpell.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "WorldObjects/Unit.h"

#include "BTTask_CastSpell.h"

#include "SingleUnitTargeting.h"
#include "AOETargeting.h"
#include "NoTargeting.h"
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
   USpellCastComponent* spellCastComponent = unitController->FindComponentByClass<USpellCastComponent>();

   if(!CheckPreconditions(unitController, spellCastComponent))
   {
      return EBTNodeResult::Failed;
   }

   if(spellCastComponent->CanCast(spellToCast))
   {
      WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellFinished);
      WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellCastFail);
      WaitForMessage(ownerComp, UnitMessages::AIMessage_Stunned);
      WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellInterrupt);
      WaitForMessage(ownerComp, UnitMessages::AIMessage_TargetLoss);

      if(targetFindingLogic && !spellToCast.GetDefaultObject()->GetSpellDefaults().Targeting->IsChildOf(UUpSpellTargeting_None::StaticClass()))
      {
         UUpAIHelperLibrary::AIBeginCastSpell(targetFindingLogic, spellToCast, spellCastComponent);     
      }
      else
      {
         spellCastComponent->BeginCastSpell(spellToCast);
      }
      return EBTNodeResult::InProgress;
   }
   return EBTNodeResult::Failed;
}

void UBTTask_CastSpell::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   bSuccess = message != UnitMessages::AIMessage_SpellInterrupt & message != UnitMessages::AIMessage_TargetLoss & message != UnitMessages::AIMessage_SpellCastFail &
              message != UnitMessages::AIMessage_Stunned;
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
         if(UEnvQueryOption* queryOption = targetFindingLogic->GetOptionsMutable()[0])
         {
            if(UEnvQueryGenerator* queryGenerator = queryOption->Generator)
            {
               if(queryGenerator->ItemType->IsChildOf(UEnvQueryItemType_Point::StaticClass()))
               {
                  if(spell->GetTargeting()->IsChildOf(UUpSpellTargeting_SingleUnit::StaticClass()))
                  {
                     UE_LOG(LogTemp, Error, TEXT("%s tried to cast a spell %s with the wrong targeting!"), *GetName(), *spell->GetSpellName().ToString());
                  }
               }
               else if(queryGenerator->ItemType->IsChildOf(UEnvQueryItemType_ActorBase::StaticClass()))
               {
                  if(spell->GetTargeting()->IsChildOf(UUpSpellTargeting_Area::StaticClass()))
                  {
                     UE_LOG(LogTemp, Error, TEXT("%s tried to cast a spell %s with the wrong targeting!"), *GetName(), *spell->GetSpellName().ToString());
                  }
               }
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
