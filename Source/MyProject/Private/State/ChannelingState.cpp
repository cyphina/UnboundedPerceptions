#include "ChannelingState.h"
#include "Unit.h"
#include "GameplayAbilityTypes.h"
#include "RTSAbilitySystemComponent.h"
#include "MySpell.h"
#include "UnitController.h"

ChannelingState::ChannelingState()
{
}

void ChannelingState::Enter(AUnit& unit)
{
}

void ChannelingState::Exit(AUnit& unit)
{
   const auto spellCastComponent = unit.GetUnitController()->FindComponentByClass<USpellCastComponent>();
   if(spellCastComponent->GetCurrentChannelingTime() > 0) unit.GetUnitController()->FindComponentByClass<USpellCastComponent>()->CancelChanneling();
}

void ChannelingState::Update(AUnit& unit, float deltaSeconds)
{
}

ChannelingState::~ChannelingState()
{
}
