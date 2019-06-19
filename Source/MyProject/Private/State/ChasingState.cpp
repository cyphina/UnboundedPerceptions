#include "MyProject.h"
#include "ChasingState.h"

#include "Unit.h"
#include "AIStuff/AIControllers/UnitController.h"


void ChasingState::Enter(AUnit& unit)
{
   if(unit.targetData.targetUnit && !unit.targetData.targetUnit->IsVisible())
   {
      unit.GetUnitController()->StopMovement();
      unit.GetUnitController()->Chase();
   }
   else
      unit.state->ChangeState(EUnitState::STATE_IDLE);
}

void ChasingState::Exit(AUnit& unit)
{
   
}

void ChasingState::Update(AUnit& unit, float deltaSeconds)
{
   if(unit.targetData.targetUnit->IsVisible())
      unit.GetUnitController()->BeginAttack(unit.targetData.targetUnit);
}
