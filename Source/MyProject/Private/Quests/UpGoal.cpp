// Created 2/11/21 09:51 PM

#include "MyProject.h"
#include "UpGoal.h"

#include "Enemy.h"
#include "ItemManager.h"
#include "NPC.h"

const FText UUpGoal::EmptyGoalArgText = NSLOCTEXT("Quest", "EmptyGoalArg", "(Empty-CDO-Placeholder)");
const FVector FGoalStyleInfo::invalidGoalLocation = FVector(-66666, -66666, -66666);

UUpGatherGoal::UUpGatherGoal()
{
   const ConstructorHelpers::FObjectFinder<UDataTable> loadedItemTable(TEXT("/Game/RTS_Tutorial/Tables/ItemList"));
   checkf(loadedItemTable.Object, TEXT("Data table for item goal row handle not found!"));

   itemToGatherId.DataTable = loadedItemTable.Object;
}

UUpInteractGoal::UUpInteractGoal()
{
   interactableClass = AInteractableBase::StaticClass();
}

UUpTalkGoal::UUpTalkGoal()
{
   npcToTurnInItemsTo = ANPC::StaticClass();
}

FText UUpHuntingGoal::MakeGoalText() const
{
   FFormatOrderedArguments args;
   args.Add(enemyToHunt.GetDefaultObject()->GetGameName());
   if(GetNumberOfEnemiesToHunt() > 1)
   {
      args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
      args.Add(GetNumCurrentlyHunted());
      args.Add(GetNumberOfEnemiesToHunt());
      return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Hunt {0}{1}: {2}/{3}"), args);
   }
   return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Hunt a {0}"), args);
}

FText UUpGatherGoal::MakeGoalText() const
{
   FFormatOrderedArguments args;
   args.Add(UItemManager::Get().GetItemInfo(GetItemToGatherId())->name);
   if(GetNPCToTurnInItemsTo() != nullptr)
   {
      if(GetNumberOfItemToGather() > 1)
      {
         args.Add(NSLOCTEXT("Quests", "Suffix", "s"));
         args.Add(GetNumCurrentlyGathered());
         args.Add(GetNumberOfItemToGather());
         args.Add(GetNPCToTurnInItemsTo().GetDefaultObject()->GetGameName());
         return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring: {0}{1} {2}/{3} to {4}!"), args);
      }
      if(!GetNPCToTurnInItemsTo().GetDefaultObject()->GetGameName().EqualTo(FText::GetEmpty()))
      {
         args.Add(GetNPCToTurnInItemsTo().GetDefaultObject()->GetGameName());
      }
      else
      {
         args.Add(EmptyGoalArgText);
      }
      return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring a {0} to {1}"), args);
   }
   else
   {
      args.Add(GetNumberOfItemToGather() - GetNumCurrentlyGathered());
      return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Bring {1} {0}(s)"), args);
   }
}

FText UUpTalkGoal::MakeGoalText() const
{
   FFormatOrderedArguments args;
   if(!npcToTurnInItemsTo.GetDefaultObject()->GetGameName().EqualTo(FText::GetEmpty()))
   {
      args.Add(npcToTurnInItemsTo.GetDefaultObject()->GetGameName());
   }
   else
   {
      args.Add(EmptyGoalArgText);
   }
   if(topicToTalkAbout != FGameplayTag::EmptyTag)
   {
      args.Add(FText::FromName(topicToTalkAbout.GetTagName()));
      return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Talk to {0} about {1}"), args);
   }
   return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Talk to {0}"), args);
}

FText UUpInteractGoal::MakeGoalText() const
{
   FFormatOrderedArguments args;
   if(!interactableClass.GetDefaultObject()->GetGameName().EqualTo(FText::GetEmpty()))
   {
      args.Add(interactableClass.GetDefaultObject()->GetGameName());
   }
   else
   {
      args.Add(EmptyGoalArgText);
   }
   return FText::Format(NSLOCTEXT("Quests", "SubGoalWidget", "Interact with the {0}"), args);
}

FText UUpCustomGoal::MakeGoalText() const
{
   return goalDescription;
}
