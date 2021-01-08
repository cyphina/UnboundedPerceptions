#include "MyProject.h"
#include "ChasingState.h"

#include "BrainComponent.h"
#include "RTSStateComponent.h"
#include "RTSVisionComponent.h"
#include "SpellDataLibrary.h"
#include "TargetComponent.h"
#include "TargetedAttackComponent.h"

#include "Unit.h"
#include "UnitMessages.h"
#include "AIStuff/AIControllers/UnitController.h"

void ChasingState::Enter(AUnit& unit)
{
   if(unit.GetTargetComponent()->GetTargetUnit() && !unit.GetTargetComponent()->GetTargetUnit()->GetVisionComponent()->IsVisible()) {
      unit.GetUnitController()->StopMovement();
   } else
      unit.FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_IDLE);
}

void ChasingState::Exit(AUnit& unit)
{
}

void ChasingState::Update(AUnit& unit, float deltaSeconds)
{
   if(unit.GetTargetComponent()->GetTargetUnit()->GetVisionComponent()->IsVisible())
      unit.GetUnitController()->FindComponentByClass<UTargetedAttackComponent>()->BeginAttack(unit.GetTargetComponent()->GetTargetUnit());
   else if(LIKELY(currentChaseTime < 5)) {
      currentChaseTime += deltaSeconds;
   } else {
      // Abandon chase since it took too long
      currentChaseTime = 0;
      FAIMessage msg(UnitMessages::AIMessage_TargetLoss, &unit);
      FAIMessage::Send(unit.unitController, msg);
   }
}
