#pragma once

#pragma once

class ABaseHero;
class ANPC;
struct FGameplayTag;

<<<<<<< HEAD
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNPCTalked, ANPC*, const FGameplayTag&);

namespace NPCEvents
{
=======
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNPCConversation, ANPC*, FGameplayTag);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNPCTalked, ANPC*);
   
namespace NPCDelegateContext
{
   inline FOnNPCConversation OnNPCConversationEvent;
>>>>>>> componentrefactor
   inline FOnNPCTalked OnNPCTalkedEvent;
}
