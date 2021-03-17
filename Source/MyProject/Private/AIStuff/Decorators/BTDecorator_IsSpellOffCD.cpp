// Created 3/16/21 1:58 AM

#include "MyProject.h"
#include "BTDecorator_IsSpellOffCD.h"

#include "AIController.h"
#include "MySpell.h"
#include "Unit.h"

bool UBTDecorator_IsSpellOffCD::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   AUnit* unitRef = Cast<AUnit>(OwnerComp.GetAIOwner()->GetPawn());
   if(unitRef && spellClass)
   {
      if(unitRef->GetAbilitySystemComponent()->GetAbilities().Find(spellClass))
      {
         return !spellClass.GetDefaultObject()->IsOnCD(unitRef->GetAbilitySystemComponent());
      }
   }
   return false;
}

FString UBTDecorator_IsSpellOffCD::GetStaticDescription() const
{
   if(spellClass)
   {
      if(!IsInversed())
      {
         return FString::Printf(TEXT("Off CD: %s"), *spellClass->GetName());
      }
      else
      {
         return FString::Printf(TEXT("On CD: %s"), *spellClass->GetName());
      }
   }
   return "";
}
