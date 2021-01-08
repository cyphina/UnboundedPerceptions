#pragma once

#pragma once

class ABaseHero;
class ANPC;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNPCTalked, ANPC*, const FGameplayTag&);

namespace NPCEvents
{
   inline FOnNPCTalked OnNPCTalkedEvent;
}
