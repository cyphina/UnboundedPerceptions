// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BTTask_CastSpellByTags.h"

#include "MySpell.h"
#include "RemoveIf.h"
#include "WorldObjects/Unit.h"
#include "UnitController.h"
#include "UnitMessages.h"
#include "UpAIHelperLibrary.h"

#include "UpResourceManager.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "EnvironmentQuery/EnvQueryManager.h"

UBTTask_CastSpellByTags::UBTTask_CastSpellByTags()
{
   NodeName    = "CastSpellByTags";
   bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_CastSpellByTags::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
   AUnitController* unitControllerRef = Cast<AUnitController>(ownerComp.GetAIOwner());
   if(TryCastSpellsWithTag(filterTags, *unitControllerRef->GetUnitOwner())) {
      WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellCasted);
      WaitForMessage(ownerComp, UnitMessages::AIMessage_SpellInterrupt);
      WaitForMessage(ownerComp, UnitMessages::AIMessage_TargetLoss);
      return EBTNodeResult::InProgress;
   }
   return EBTNodeResult::Failed;
}

void UBTTask_CastSpellByTags::OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess)
{
   bSuccess &= message == UnitMessages::AIMessage_SpellCasted & message != UnitMessages::AIMessage_SpellInterrupt & message != UnitMessages::AIMessage_TargetLoss;
   Super::OnMessage(ownerComp, nodeMemory, message, requestID, bSuccess);
}

bool UBTTask_CastSpellByTags::TryCastSpellsWithTag(const FGameplayTagContainer& spellRequirementTags, const AUnit& ownerRef)
{
   URTSAbilitySystemComponent* abilitySystemComp = ownerRef.GetAbilitySystemComponent();
   USpellCastComponent*  spellCastComp     = ownerRef.FindComponentByClass<USpellCastComponent>();

   if(!spellCastComp || !abilitySystemComp) {
      UE_LOG(LogTemp, Error, TEXT("Unit %s trying to execute spell casting task without a spell/ability component"), *ownerRef.GetGameName().ToString());
      return false;
   }
   
   const TSubclassOf<UMySpell> spellToCast = GetFirstCastableSpellMatchingTagCriteria(spellRequirementTags, ownerRef, spellCastComp, abilitySystemComp);

   if(spellToCast) {
      UEnvQuery*          targetingScheme     = nullptr;
      const FGameplayTag& spellDescriptionTag = spellToCast.GetDefaultObject()->GetSpellDefaults().descriptionTags.First();
      if(spellToTargetingMap.Find(spellDescriptionTag)) {
         targetingScheme = spellToTargetingMap[spellDescriptionTag];
      } else {
         targetingScheme = spellToCast.GetDefaultObject()->GetDefaultQueryForSpell(ownerRef.GetWorld());
      }

      UUpAIHelperLibrary::AIBeginCastSpell(targetingScheme, spellToCast, spellCastComp);
   }

   return false;
}

TSubclassOf<UMySpell> UBTTask_CastSpellByTags::GetFirstCastableSpellMatchingTagCriteria
(const FGameplayTagContainer& spellRequirementTags, const AUnit& ownerRef, const USpellCastComponent* spellCastComp, const URTSAbilitySystemComponent* abilitySystemComp)
{
   TArray<TSubclassOf<UMySpell>> equippedSpells           = abilitySystemComp->GetAbilities();
   const int32                   matchingSpellsStartIndex = Algo::RemoveIf(equippedSpells, [spellRequirementTags](const TSubclassOf<UMySpell>& spellClass) {
      return spellClass.GetDefaultObject()->AbilityTags.HasAny(spellRequirementTags);
   });

   for(int i = matchingSpellsStartIndex; i < equippedSpells.Num(); ++i) {
      if(const TSubclassOf<UMySpell> spellClass = abilitySystemComp->GetSpellAtSlot(i)) {
         if(abilitySystemComp->CanCast(spellClass)) {
            return spellClass;
         }
      }
   }

   return nullptr;
}
