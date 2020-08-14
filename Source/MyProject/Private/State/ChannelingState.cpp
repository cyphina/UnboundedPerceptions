#include "ChannelingState.h"
#include "Unit.h"
#include "GameplayAbilityTypes.h"
#include "MyAbilitySystemComponent.h"
#include "MySpell.h"
#include "UnitController.h"

ChannelingState::ChannelingState()
{
}

void ChannelingState::Enter(AUnit& unit)
{
   //TODO: Incantation animation
   //TODO: Setup ChannelTime max from CurrentSpell inn AbilityComponent (maybe there's a better way)
   currentChannelTime = 0.f;
   channelTime        = unit.GetAbilitySystemComponent()->GetCurrentSpell().GetDefaultObject()->GetCastTime(unit.GetAbilitySystemComponent());
}

void ChannelingState::Exit(AUnit& unit)
{
   // Typically we exit a channeling state when the user presses the confirm spell (a 2 part spell cast).
   // Then we trigger the event for the confirm spell and pass this information to it.
   FGameplayEventData eD = FGameplayEventData();
   eD.EventTag           = FGameplayTag::RequestGameplayTag("Skill.Confirm");

   //Moves targetData so now unit.targetData will be null.  Must get targetData from the event handler, and regular confirm has no targetData since its a None type target.
   // TODO: Do we need this? eD.TargetData = unit.targetData.spellTargetData;

   //Handle the end of channeling in the spell itself by having some action taken when the event is heard.
   //Chanelled spell should call stop when it finishes or activated early.
   unit.GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Skill.Confirm"), &eD);
   currentChannelTime = 0;
}

void ChannelingState::Update(AUnit& unit, float deltaSeconds)
{
   if(currentChannelTime < channelTime)
      currentChannelTime += deltaSeconds;
}

ChannelingState::~ChannelingState()
{
}
