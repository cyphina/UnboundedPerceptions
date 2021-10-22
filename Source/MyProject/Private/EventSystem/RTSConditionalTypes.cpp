#include "RTSConditionalTypes.h"

#include "BaseHero.h"
#include "NPC.h"
#include "Quest.h"
#include "QuestManager.h"
#include "RTSGameMode.h"
#include "UpResourceManager.h"
#include "UserInput.h"

bool URTSConditionalHasQuest::CheckIsConditionSatisfied_Implementation()
{
   if(ARTSGameMode* GameMode = GetGameModeRef())
   {
      if(UQuestManager* QuestManager = GameMode->GetQuestManager())
      {
         if(QuestManager->FindObtainedQuestByClass(questPlayerShouldHaveClass))
         {
            return true;
         }
      }
   }
   return false;
}

bool URTSConditionalQuestCompleted::CheckIsConditionSatisfied_Implementation()
{
   if(ARTSGameMode* GameMode = GetGameModeRef())
   {
      if(UQuestManager* QuestManager = GameMode->GetQuestManager())
      {
         if(QuestManager->FindObtainedQuestByQuestStateAndClass(completedQuestClass, EQuestState::CompletedQuests))
         {
            return true;
         }
      }
   }
   return false;
}

bool URTSConditionalQuestGoalComplete::CheckIsConditionSatisfied_Implementation()
{
   if(ARTSGameMode* GameMode = GetGameModeRef())
   {
      if(UQuestManager* QuestManager = GameMode->GetQuestManager())
      {
         if(QuestManager->FindObtainedQuestByClass(questWithGoalClass))
         {
            return true;
         }
      }
   }
   return true;
}

bool URTSConditionalOwnsItem::CheckIsConditionSatisfied_Implementation()
{
   if(AUserInput* PC = GetControllerRef())
   {
      for(const ABaseHero* hero : PC->GetBasePlayer()->GetHeroes())
      {
         if(hero && hero->GetBackpack().FindItem(itemToOwn.id) != -1)
         {
            return true;
         }
      }
   }
   return false;
}

bool URTSConditionalHadConversation::CheckIsConditionSatisfied_Implementation()
{
   if(UWorld* World = GetWorld())
   {
      if(ANPC* NPC = UpResourceManager::FindTriggerObjectInWorld(World, npcToCheckConversationClass))
      {
         return NPC->GetAlreadyConversedDialogs().HasTagExact(conversationTag);
      }
   }
   return false;
}

bool URTSConditionalFulfilledStoryTag::CheckIsConditionSatisfied_Implementation()
{
   // TODO: Implement
   return true;
}
