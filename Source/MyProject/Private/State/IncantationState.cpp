#include "MyProject.h"
#include "ChannelingState.h"
#include "Unit.h"
#include "AIStuff/AIControllers/UnitController.h"
#include "AIModule/Classes/BrainComponent.h"
#include "MySpell.h"

IncantationState::IncantationState()
{
}

IncantationState::~IncantationState()
{
}

void IncantationState::Enter(AUnit& unit)
{
}

void IncantationState::Exit(AUnit& unit)
{
   const auto spellCastComponent = unit.GetUnitController()->FindComponentByClass<USpellCastComponent>();
   if(spellCastComponent->GetCurrentIncantationTime() > 0) unit.GetUnitController()->FindComponentByClass<USpellCastComponent>()->CancelChanneling();
}

void IncantationState::Update(AUnit& unit, float deltaSeconds)
{
}
