#include "MyProject.h"
#include "ChasingState.h"

#include "BrainComponent.h"

#include "Unit.h"
#include "AIStuff/AIControllers/UnitController.h"

void ChasingState::Enter(AUnit& unit)
{
   if(unit.GetTargetUnit() && !unit.GetTargetUnit()->IsVisible()) {
      unit.GetUnitController()->StopMovement();
   } else
      unit.state->ChangeState(EUnitState::STATE_IDLE);
}

void ChasingState::Exit(AUnit& unit)
{
}

void ChasingState::Update(AUnit& unit, float deltaSeconds)
{
   if(unit.GetTargetUnit()->IsVisible())
      unit.GetUnitController()->BeginAttack(unit.GetTargetUnit());
   else if(LIKELY(currentChaseTime < 5)) {
      currentChaseTime += deltaSeconds;
   } else {
      // Abandon chase since it took too long
      currentChaseTime = 0;
      FAIMessage msg(AUnit::AIMessage_TargetLoss, &unit);
      FAIMessage::Send(unit.unitController, msg);
   }
}
