#include "MyProject.h"
#include "BTDecorator_IsReadyToAttack.h"

#include "TargetedAttackComponent.h"
#include "UnitController.h"
#include "WorldObjects/Unit.h"

UBTDecorator_IsReadyToAttack::UBTDecorator_IsReadyToAttack()
{
   NodeName = "Is Ready to Attack";
   // For now, don't allow users to select any "Abort Observers", because it's currently not supported.
   bAllowAbortNone       = false;
   bAllowAbortLowerPri   = false;
   bAllowAbortChildNodes = false;
}

bool UBTDecorator_IsReadyToAttack::CalculateRawConditionValue(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory) const
{
   if(UTargetedAttackComponent* targetedAttackComp = ownerComp.GetAIOwner()->FindComponentByClass<UTargetedAttackComponent>())
   {
      return targetedAttackComp->GetIsReadyToAttack();
   }

   // Units without a targeted attack component should control their attack timings in the behavior tree.
   return true;
}
