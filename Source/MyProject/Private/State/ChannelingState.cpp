#include "ChannelingState.h"
#include "Unit.h"
#include "GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "UnitController.h"

ChannelingState::ChannelingState()
{
}

void ChannelingState::Enter(AUnit& unit)
{
   //TODO: Incantation animation
}

void ChannelingState::Exit(AUnit& unit)
{
   FGameplayEventData eD = FGameplayEventData();
   eD.EventTag = FGameplayTag::RequestGameplayTag("Skill.Confirm");

   //Moves targetData so now unit.targetData will be null.  Must get targetData from the event handler, and regular confirm has no targetData since its a None type target.
   eD.TargetData = unit.targetData.spellTargetData;

   //Handle the end of channeling in the spell itself by having some action taken when the event is heard.
   //Chanelled spell should call stop when it finishes or activated early.
   unit.GetAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Skill.Confirm"), &eD);
   unit.unitSpellData.currentChannelTime = 0;
}

void ChannelingState::Update(AUnit& unit, float deltaSeconds)
{
   if (unit.unitSpellData.currentChannelTime < unit.unitSpellData.channelTime)
      unit.unitSpellData.currentChannelTime += deltaSeconds;
}

ChannelingState::~ChannelingState()
{
}
