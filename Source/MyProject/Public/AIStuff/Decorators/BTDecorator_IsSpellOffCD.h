// Created 3/16/21 1:59 AM

#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsSpellOffCD.generated.h"

/**
 * @brief Checks to see if a certain spell is on cooldown
 */
UCLASS()
class MYPROJECT_API UBTDecorator_IsSpellOffCD : public UBTDecorator
{
   GENERATED_BODY()

 protected:
   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

   FString GetStaticDescription() const override;

   UPROPERTY(EditAnywhere)
   TSubclassOf<class UMySpell> spellClass;
};
