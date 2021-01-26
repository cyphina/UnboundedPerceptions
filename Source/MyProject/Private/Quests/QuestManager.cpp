#include "MyProject.h"
#include "UserWidget.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "UI/Minimap.h"
#include "WorldObjects/BaseHero.h"
#include "QuestManager.h"

#include "GameplayDelegateContext.h"
#include "UI/QuestList.h"
#include "UI/QuestListSlot.h"
#include "UI/QuestJournal.h"
#include "Quest.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "WorldObjects/NPC.h"
#include "UserInput.h"
#include "GoalActor.h"
#include "ItemDelegateContext.h"
#include "RTSIngameWidget.h"
#include "Interactables/NamedInteractableDecorator.h"
#include "Items/ItemManager.h"
#include "ItemDelegateContext.h"
#include "DialogSystem/NPCDelegateStore.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"

#if WITH_EDITOR
void UQuestManager::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
   Super::PostEditChangeProperty(PropertyChangedEvent);
   UpdateQuestClassList();
}
#endif

void UQuestManager::UpdateQuestClassList()
{
   UE_LOG(LogTemp, Display, TEXT("Quest Class Map Updated"));

   FAssetRegistryModule& assetReg        = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
   IAssetRegistry&       assetRegistry   = assetReg.Get();
   const FString         questFolderPath = TEXT("/Game/RTS_Tutorial/Blueprints/Quest/Blueprints/Quests");

   assetRegistry.AddPath(questFolderPath);
   assetRegistry.ScanPathsSynchronous({questFolderPath});
   TArray<FAssetData> questAssets;
   assetRegistry.GetAssetsByPath(*questFolderPath, questAssets, true);
   for(FAssetData& asset : questAssets)
   {
      const UBlueprint* questBP = Cast<UBlueprint>(asset.GetAsset());
      if(questBP)
      {
         AQuest* quest = Cast<AQuest>(questBP->GeneratedClass->GetDefaultObject(false));
         if(quest)
         {
            TSubclassOf<AQuest> questClass = questBP->GeneratedClass.Get();
            questClassList.Add(quest->questInfo.id, questClass);
         }
      }
   }
}

void UQuestManager::Init()
{
   controllerRef = Cast<AUserInput>(GetOuter()->GetWorld()->GetFirstPlayerController());

   // Widgets may not be initialized due to begin play order so setup next frame.
   GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UQuestManager::SetupWidgetReferences);

   completedQuests.Reserve(4);
   failedQuests.Reserve(4);
   quests.Reserve(4);

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemPickedUp().AddUObject(this, &UQuestManager::OnItemPickedUp);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemDropped().AddUObject(this, &UQuestManager::OnItemPickedUp);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemPurchased().AddUObject(this, &UQuestManager::OnItemPurchased);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemUsed().AddUObject(this, &UQuestManager::OnItemPickedUp);

   // TODO: One more listener for when item rewards are given from quests
   NPCDelegateContext::OnNPCConversationEvent.AddUObject(this, &UQuestManager::OnTalkNPC);
   NPCDelegateContext::OnNPCTalkedEvent.AddUObject(this, &UQuestManager::OnTalkNPC, FGameplayTag::EmptyTag);

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnInteracted().AddUObject(this, &UQuestManager::OnInteracted);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnUnitDieGlobal().AddUObject(this, &UQuestManager::OnEnemyDie);
}

void UQuestManager::SetupWidgetReferences()
{
   if(questListRef && questJournalRef)
   {
      questListRef    = controllerRef->GetHUDManager()->GetIngameHUD()->GetQuestList();
      questJournalRef = controllerRef->GetHUDManager()->GetIngameHUD()->GetQuestJournal();
   }
}

void UQuestManager::SelectNewQuest(AQuest* quest)
{
   if(IsValid(quest))
   {
      questListRef->currentlySelectedQuest = quest;
      questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)
                  ->OnQuestSelected(); // this goes first or else OnQuestSelected can't remove the old quest slots selected goal
   }
}

bool UQuestManager::AddNewQuest(TSubclassOf<AQuest> questClassToSpawn, bool forcedStart)
{
   auto condition = [questClassToSpawn](AQuest* quest)
   {
      return questClassToSpawn == quest->GetClass();
   };
   if(IsValid(questClassToSpawn) && !quests.ContainsByPredicate(condition))
   {
      AQuest* quest =
      questListRef->GetWorld()->SpawnActorDeferred<AQuest>(questClassToSpawn.Get(), FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

      quest->SetQuestManagerRef(this);
      quest->FinishSpawning(FTransform());
      quests.Add(quest);
      quest->SetupStartingGoals();
      questListRef->AddToQuestList(quest);
      questListRef->GetQuestListSlot(quest)->UpdateQuestEntry();
      questJournalRef->AddEntry(quest);
      if(forcedStart)
      {
         SelectNewQuest(quest);
         questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)->SelectSubGoal(0);
      }
      if(!controllerRef->GetHUDManager()->IsWidgetOnScreen(EHUDs::HS_QuestList)) { hudManagerRef->AddHUD(static_cast<int>(EHUDs::HS_QuestList)); }
      return true;
   }
   return false;
}

void UQuestManager::OnSwitchSubGoal()
{
   if(questListRef->currentlySelectedQuest)
   {
      if(currentGoalActor) { currentGoalActor->Destroy(); }
   }

   if(questListRef->currentlySelectedQuest->questInfo.subgoals[questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)->GetSelectedGoalIndex()]
     .goalLocation != FGoalInfo::invalidGoalLocation)
   {
      currentGoalActor = controllerRef->GetWorld()->SpawnActorAbsolute<AGoalActor>(
      goalActorClass,
      FTransform(
      questListRef->currentlySelectedQuest->questInfo.subgoals[questListRef->GetQuestListSlot(questListRef->currentlySelectedQuest)->GetSelectedGoalIndex()]
     .goalLocation),
      FActorSpawnParameters());
   }

   currentDistance = GetDistanceToGoal();
   questListRef->SetDistanceText(currentDistance);
   controllerRef->GetWidgetProvider()->GetIngameHUD()->GetMinimap()->UpdateDirectionArrow(FMath::RadiansToDegrees(
   FMath::Acos(FVector::ForwardVector.CosineAngle2D(controllerRef->GetBasePlayer()->GetHeroes()[0]->GetActorLocation() - currentGoalActor->GetActorLocation()))));
   questListRef->ToggleDistanceIndicatorVisibility(true);
   if(currentDistance > 10)
      controllerRef->GetWidgetProvider()->GetIngameHUD()->GetMinimap()->ToggleDirectionArrowVisibility(true);
   else
      controllerRef->GetWidgetProvider()->GetIngameHUD()->GetMinimap()->ToggleDirectionArrowVisibility(false);
}

int UQuestManager::GetDistanceToGoal()
{
   const FVector partyLeaderLocationXY =
   FVector(controllerRef->GetBasePlayer()->GetHeroes()[0]->GetActorLocation().X, controllerRef->GetBasePlayer()->GetHeroes()[0]->GetActorLocation().Y, 0);
   const FVector goalActorLocationXY = FVector(currentGoalActor->GetActorLocation().X, currentGoalActor->GetActorLocation().Y, 0);
   return FMath::RoundToInt((partyLeaderLocationXY - goalActorLocationXY).Size2D() / 100);
}

void UQuestManager::EndQuest(AQuest* questToEnd)
{
   quests.Remove(questToEnd);
   switch(questToEnd->GetQuestState())
   {
      case EQuestState::currentQuests: break;
      case EQuestState::failedQuests: failedQuests.Add(questToEnd);
         break;
      case EQuestState::completedQuests: completedQuests.Add(questToEnd);
         break;
   }

   // add our quest to the quest journal under its new category
   questJournalRef->AddEntry(questToEnd);

   // remove from quest list if we added it to the quest list
   if(questListRef->GetQuestListSlot(questToEnd))
      questListRef->RemoveFromQuestList(questToEnd);

   // If it is selected in the questlistwidget
   if(questToEnd == questListRef->currentlySelectedQuest)
   {
      questListRef->currentlySelectedQuest = nullptr;

      // Also remove the goal actor from the minimap
      if(IsValid(currentGoalActor))
      {
         currentGoalActor->Destroy();
         questListRef->ToggleDistanceIndicatorVisibility(false);
         controllerRef->GetWidgetProvider()->GetIngameHUD()->GetMinimap()->ToggleDirectionArrowVisibility(false);
      }
   }

   // questJournalRef->RemoveFromQuestJournal(questToEnd);

   // if we have another quest, then select it
   if(quests.Num() > 0 && IsValid(quests[0]))
      SelectNewQuest(quests[0]);

   // if we selected this quest in the journal as it ends, deselect4 it
   if(questJournalRef->GetSelectedQuest() == questToEnd)
      questJournalRef->OnQuestEntryClicked(nullptr, nullptr);

   // if this quest was completed sucessfully, give us the rewards!
   if(questToEnd->GetQuestState() == EQuestState::completedQuests)
   {
      controllerRef->GetBasePlayer()->UpdateEXP(questToEnd->questInfo.questReward.exp);
      controllerRef->GetBasePlayer()->UpdateGold(questToEnd->questInfo.questReward.gold);
      OnQuestCompletedDelegate.Broadcast();
   }
}

void UQuestManager::CompleteGoals()
{
   for(int& i : questListRef->currentlySelectedQuest->GetCurrentGoalIndices())
   {
      questListRef->currentlySelectedQuest->CompleteSubGoal(i, false);
   }
}

void UQuestManager::OnPartyLeaderMove()
{
   if(IsValid(currentGoalActor))
   {
      currentDistance = GetDistanceToGoal();
      questListRef->SetDistanceText(currentDistance);
      if(currentDistance < 10)
      {
         controllerRef->GetWidgetProvider()->GetIngameHUD()->GetMinimap()->ToggleDirectionArrowVisibility(false);
      } else
      {
         controllerRef->GetWidgetProvider()->GetIngameHUD()->GetMinimap()->UpdateDirectionArrow(FMath::RadiansToDegrees(
         FMath::Acos(FVector::ForwardVector.CosineAngle2D(controllerRef->GetBasePlayer()->GetHeroes()[0]->GetActorLocation() - currentGoalActor->GetActorLocation()))));
         controllerRef->GetWidgetProvider()->GetIngameHUD()->GetMinimap()->ToggleDirectionArrowVisibility(true);
      }
   }
}

void UQuestManager::OnEnemyDie(AUnit* deadUnit)
{
   if(AEnemy* enemy = Cast<AEnemy>(deadUnit))
   {
      // For updating UI, we need to use TaskGraphMainThread
      auto dieFuture = Async(EAsyncExecution::TaskGraphMainThread, [this, enemy]()
      {
         for(AQuest* quest : quests)
         {
            for(const int& goalIndex : quest->currentGoalIndices)
            {
               if(const FGoalInfo& goal = quest->questInfo.subgoals[goalIndex]; goal.goalType == EGoalType::Hunt)
               {
                  if(enemy->GetGameName().EqualTo(goal.additionalNames[0]))
                  {
                     if(goal.amount < 2 || quest->currentAmounts[goalIndex] + 1 >= goal.amount)
                     {
                        quest->CompleteSubGoal(goalIndex, false);
                        return;
                     }
                     ++quest->currentAmounts[goalIndex];
                     questListRef->GetQuestListSlot(quest)->UpdateQuestEntry();
                  }
               }
            }

            // Regardless if whether this goal finishes the quest or not, update the quest journal
            if(quest == questJournalRef->GetSelectedQuest()) { questJournalRef->UpdateDetailWindow(); }
         }
      });
   }
}

void UQuestManager::OnTalkNPC(ANPC* talkedToNPC, FGameplayTag conversationTopic)
{
   auto talkFuture = Async(EAsyncExecution::TaskGraphMainThread, [this, talkedToNPC, conversationTopic]()
   {
      for(AQuest* quest : quests)
      {
         for(const int& goalIndex : quest->currentGoalIndices)
         {
            if(const FGoalInfo& goal = quest->questInfo.subgoals[goalIndex];
               goal.goalType == EGoalType::Talk && talkedToNPC->GetGameName().EqualTo(goal.additionalNames[0]))
            {
               if(goal.additionalNames.Num() == 1 || conversationTopic.GetTagName() == *goal.additionalNames[1].ToString())
               {
                  quest->CompleteSubGoal(goalIndex, false);
                  if(quest == questJournalRef->GetSelectedQuest()) { questJournalRef->UpdateDetailWindow(); }
                  return;
               }
            } else if(goal.goalType == EGoalType::Find && quest->currentAmounts[goalIndex] >= goal.amount)
            {
               quest->CompleteSubGoal(goalIndex, false);
               if(quest == questJournalRef->GetSelectedQuest()) { questJournalRef->UpdateDetailWindow(); }
               return;
            }
         }
      }
   });
}

void UQuestManager::OnItemPickedUp(const ABaseHero* heroPickingItem, const FBackpackUpdateResult& itemUpdateResult)
{
   // TODO: Split off quests types into their own objects? Maybe make it so that each quest type has its own listeners to the delegates as opposed to having just the manager listen?
   if(itemUpdateResult.numUpdatedItemsRequested != itemUpdateResult.numUpdatedItemsRemaining)
   {
      const int     numItemsPickedUp = itemUpdateResult.numUpdatedItemsRequested - itemUpdateResult.numUpdatedItemsRemaining;
      const FMyItem newItem{itemUpdateResult.itemId, numItemsPickedUp};

      auto itemPickupFuture = Async(EAsyncExecution::TaskGraphMainThread, [this, newItem]() { RecalculateItemCountsForGoals(newItem); });
   }
}

void UQuestManager::OnItemPurchased
(const ABaseHero* purchasingHero, const FBackpackUpdateResult& addPurchasedItemResult, const TArray<FBackpackUpdateResult>& removePaymentItemsResults)
{
   OnItemPickedUp(purchasingHero, addPurchasedItemResult);
   for(const FBackpackUpdateResult& backpackUpdateResult : removePaymentItemsResults)
   {
      OnItemPickedUp(purchasingHero, backpackUpdateResult);  
   }
}

void UQuestManager::RecalculateItemCountsForGoals(const FMyItem item)
{
   int  questIndex = 0;
   bool changed    = false; // Flag to know if we modified this goal's item or completed this goal so update it in the quest entry

   for(AQuest* quest : quests)
   {
      for(const int& goalIndex : quest->currentGoalIndices)
      {
         const FGoalInfo& goal = quest->questInfo.subgoals[goalIndex];
         if(goal.goalType == EGoalType::Find && goal.amount > 1 && item.id == FCString::Atoi(*goal.additionalNames[0].ToString()))
         {
            quest->currentAmounts[goalIndex] = FMath::Min(goal.amount, item.count);
            if(goal.additionalNames.Num() == 1) // If we don't have to turn in the items
            {
               if(quest->currentAmounts[goalIndex] == goal.amount)
               {
                  quest->CompleteSubGoal(goalIndex, false);
               }
            }
            changed = true;
         }
      }

      if(changed)
      {
         questListRef->GetQuestListSlot(quest)->UpdateQuestEntry();
         questJournalRef->UpdateDetailWindow();
         changed = false;
      }

      ++questIndex;
   }
}

void UQuestManager::OnInteracted(const FText& decoratorName)
{
   auto itemPickupFuture = Async(EAsyncExecution::TaskGraphMainThread, [this, decoratorName]()
   {
      for(AQuest* quest : quests)
      {
         for(const int& goalIndex : quest->currentGoalIndices)
         {
            // If this goal is to interact with something, and the interactable's name matches the name in this goal
            if(const FGoalInfo& goal = quest->questInfo.subgoals[goalIndex];
               goal.goalType == EGoalType::Interact && decoratorName.EqualTo(goal.additionalNames[0]))
            {
               // If we have interacted with enough of these objects, complete the goal
               if(goal.amount < 2)
               {
                  quest->CompleteSubGoal(goalIndex, false);
                  if(quest == questJournalRef->GetSelectedQuest()) { questJournalRef->UpdateDetailWindow(); }
                  return;
               }
               // If we haven't just update the count
               // Check to see if we haven't interacted with this interactable yet by finding the
               if(quest->interactedActors[goalIndex].Find(&decoratorName) == INDEX_NONE)
               {
                  if(quest->currentAmounts[goalIndex] + 1 >= goal.amount)
                  {
                     quest->CompleteSubGoal(goalIndex, false);
                     if(quest == questJournalRef->GetSelectedQuest()) { questJournalRef->UpdateDetailWindow(); }
                     return;
                  }
                  ++quest->currentAmounts[goalIndex];
                  questListRef->GetQuestListSlot(quest)->UpdateQuestEntry();
                  quest->interactedActors[goalIndex].Add(&decoratorName);
               }
            }
         }
      }
   });
}
