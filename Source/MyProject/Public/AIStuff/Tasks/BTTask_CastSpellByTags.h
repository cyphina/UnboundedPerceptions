// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MySpell.h"
#include "RTSAbilitySystemComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CastSpellByTags.generated.h"

class UEnvQuery;
class AUnit;

/**
 *Casts one of the spells in the unit's equipped abilities with a certain gameplay tag.  AI handles what to target
 */
UCLASS()
class MYPROJECT_API UBTTask_CastSpellByTags : public UBTTaskNode
{
   GENERATED_BODY()

 public:
   UBTTask_CastSpellByTags();

   UPROPERTY(EditAnywhere)
   FGameplayTagContainer filterTags;

   UPROPERTY(EditAnywhere)
   TMap<FGameplayTag, UEnvQuery*> spellToTargetingMap;

 protected:
   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;

 private:
   void                  OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess) override;
   TSubclassOf<UMySpell> GetFirstCastableSpellMatchingTagCriteria(const FGameplayTagContainer& spellRequirementTags, const AUnit& ownerRef,
                                                                  const USpellCastComponent* spellCastComp, const URTSAbilitySystemComponent* abilitySystemComp);

   /**
    * @brief Attempts to cast spells that are off cooldown and ones that we have the required mana to cast IF they have the required tags
    * @param spellRequirementTags Tags that are required for us to consider these spells
    * @return Returns true if an eligible spell was found.
    */
   bool TryCastSpellsWithTag(const FGameplayTagContainer& spellRequirementTags, const AUnit& ownerRef);

   FString GetStaticDescription() const override;
};
