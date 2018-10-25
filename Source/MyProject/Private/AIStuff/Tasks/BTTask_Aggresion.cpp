// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_Aggresion.h"

#include "ResourceManager.h"
#include "UserInput.h"

#include "WorldObjects/Unit.h"
#include "UnitController.h"

#include "BehaviorTree/BTFunctionLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"

#include "SpellSystem/MySpell.h"
#include "GameplayAbilityTargetTypes.h"

UBTTask_Aggresion::UBTTask_Aggresion()
{
   NodeName    = "Aggression";
   bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Aggresion::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitControllerRef = Cast<AUnitController>(ownerComp.GetAIOwner());

   FBTAggresssionTaskMemory* myMemory = (FBTAggresssionTaskMemory*)nodeMemory;

   myMemory->attackSpell       = nullptr;
   myMemory->attackSpellIndex  = -1;
   myMemory->unitControllerRef = unitControllerRef;
   myMemory->unitRef           = myMemory->unitControllerRef->GetUnitOwner();

   for (int i = 0; i < myMemory->unitRef->abilities.Num(); ++i) {
      if (IsValid(myMemory->unitRef->abilities[i])) {
         if (myMemory->unitRef->abilities[i].GetDefaultObject()->AbilityTags.HasAny(ResourceManager::offensiveTags) && myMemory->unitRef->CanCast(myMemory->unitRef->abilities[i])) {
            myMemory->attackSpell      = myMemory->unitRef->abilities[i].GetDefaultObject();
            myMemory->attackSpellIndex = i;
            break;
         }
      }
   }

   if (IsValid(myMemory->attackSpell)) {
      // all the eqs targetting calls abort this task if they fail
      if (myMemory->attackSpell->GetTargetting() == FGameplayTag::RequestGameplayTag("Skill.Targetting.Area")) {
         myMemory->unitControllerRef->FindBestAOELocation(myMemory->attackSpellIndex, false);
      } else if (myMemory->attackSpell->GetTargetting() == FGameplayTag::RequestGameplayTag("Skill.Targetting.None")) {
         myMemory->unitRef->BeginCastSpell(myMemory->attackSpellIndex, FGameplayAbilityTargetDataHandle());
      } else {
         myMemory->unitControllerRef->FindWeakestTarget(myMemory->attackSpellIndex, false);
      }
      return EBTNodeResult::InProgress;
   }

   // if there's no spell to be casted
   return EBTNodeResult::Failed;
}

void UBTTask_Aggresion::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* nodeMemory, float DeltaSeconds)
{
   // if spell finally casted...
   FBTAggresssionTaskMemory* myMemory = (FBTAggresssionTaskMemory*)nodeMemory;
   if (myMemory->attackSpell->isOnCD(myMemory->unitControllerRef->GetUnitOwner()->GetAbilitySystemComponent())) { FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); }
}

uint16 UBTTask_Aggresion::GetInstanceMemorySize() const
{
   return sizeof(FBTAggresssionTaskMemory);
}
