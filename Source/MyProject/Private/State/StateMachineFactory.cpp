#include "StateMachineFactory.h"
#include "HeroStateMachine.h"
#include "Unit.h"
#include "BaseHero.h"

TUniquePtr<RTSStateMachine> StateMachineFactory::BuildStateMachine(AUnit* unitToBuildMachineFor)
{
   if(unitToBuildMachineFor->GetClass() == ABaseHero::StaticClass()) {
      return MakeUnique<HeroStateMachine>(Cast<ABaseHero>(unitToBuildMachineFor));
   } else if(unitToBuildMachineFor->GetClass()->IsChildOf(AUnit::StaticClass())) {
      return MakeUnique<RTSStateMachine>(unitToBuildMachineFor);
   }
   return nullptr;
}
