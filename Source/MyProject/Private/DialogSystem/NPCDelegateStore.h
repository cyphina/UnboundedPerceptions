#pragma once

#pragma once

class ABaseHero;
class ANPC;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNPCConversation, ANPC*, FGameplayTag);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNPCTalked, ANPC*);
   
namespace NPCDelegateContext
{
   inline FOnNPCConversation OnNPCConversationEvent;
   inline FOnNPCTalked OnNPCTalkedEvent;
}
