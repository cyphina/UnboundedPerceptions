#pragma once

#include "BTTask_QueryAction.h"
#include "BTTask_CastSpell.generated.h"

class UMySpell;
class UEnvQuery;
class USpellCastComponent;

/**
 * Spell casting task where the spell and target is chosen by the designer.  The spell is a property where the target is the blackboard key
 */
UCLASS()
class MYPROJECT_API UBTTask_CastSpell : public UBTTask_QueryAction
{
   GENERATED_BODY()

 public:
   UBTTask_CastSpell();

 protected:
   UPROPERTY(EditAnywhere)
   TSubclassOf<UMySpell> spellToCast;

   EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) override;
   virtual void        OnMessage(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, FName message, int32 requestID, bool bSuccess) override;
   void                InitializeFromAsset(UBehaviorTree& Asset) override;
   FString             GetStaticDescription() const override;

 private:
   bool CheckPreconditions(AUnitController* unitController, USpellCastComponent* spellCastComponent);
};
