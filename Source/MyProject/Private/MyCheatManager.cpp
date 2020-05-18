// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyCheatManager.h"

#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "BasePlayer.h"

#include "UpResourceManager.h"
#include "Quests/QuestManager.h"
#include "Quests/Quest.h"

#include "UI/HUDManager.h"
#include "UI/UserWidgets/ActionbarInterface.h"
#include "UI/ESkillContainer.h"
#include "UI/Slots/SkillSlot.h"

#include "SpellSystem/SpellManager.h"
#include "SpellSystem/MyAbilitySystemComponent.h"

#include "WorldObjects/Unit.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/Enemies/Enemy.h"

#include "EventSystem/EventManager.h"

void UMyCheatManager::InitCheatManager()
{
   Super::InitCheatManager();
   userInputRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   gameModeRef  = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
}

void UMyCheatManager::LevelUp(FString heroName)
{
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Leveling Up via Cheats!"));
   if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->heroes.FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) { heroRef->LevelUp(); }
}

void UMyCheatManager::LevelUpToLevel(FString heroName, int level)
{
   if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->heroes.FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) {
      while (heroRef->GetLevel() < level)
         heroRef->LevelUp();
   }
}

void UMyCheatManager::GodMode(FString objectID, int toggleGodMode)
{
#if UE_EDITOR
   if (AUnit* unitRef = UpResourceManager::FindTriggerObjectInWorld<AUnit>(objectID, userInputRef->GetWorld())) {
      if (toggleGodMode)
         unitRef->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"));
      else
         unitRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"));
   }
#endif
}

void UMyCheatManager::AddQuest(FString questName)
{
   auto questTag = FGameplayTag::RequestGameplayTag(*(FString("QuestName." + questName)));
   if (gameModeRef->GetQuestManager()->questClassList.Contains(questTag)) {
      FTriggerData addQuestTrigger;
      addQuestTrigger.enabled       = true;
      addQuestTrigger.numCalls      = 1;
      addQuestTrigger.triggerType   = ETriggerType::AddQuestTrigger;
      addQuestTrigger.triggerValues = {questName, "1"};
      gameModeRef->GetTriggerManager()->ActivateTrigger(addQuestTrigger);
   } else { UE_LOG(LogTemp, Error, TEXT("Quest cheat command passed invalid parameters!")); }
}

void UMyCheatManager::FinishQuest(FString questName, int isSucessful)
{
   AQuest* quest = *gameModeRef->GetQuestManager()->quests.FindByPredicate([questName](AQuest* quest) { return quest->questInfo.name.ToString() == questName; });
   if (quest) { isSucessful == 0 ? quest->CompleteQuest(false) : quest->CompleteQuest(true); }
}

void UMyCheatManager::EquipSpell(FString heroName, int spellID, int slot)
{
   if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->heroes.FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) {
      if (slot >= 0 && slot < heroRef->abilities.Num())
         userInputRef->GetHUDManager()->GetActionHUD()->skillContainerRef->skillSlots[slot]->UpdateSkillSlot(USpellManager::Get().GetSpellClass(spellID));
   }
}

void UMyCheatManager::RefreshSpells()
{
   for (AAlly* ally : userInputRef->GetBasePlayer()->allies)
      ally->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
   for (AEnemy* enemy : userInputRef->GetGameState()->enemyList)
      enemy->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
}

void UMyCheatManager::RefreshHeroSpells(FString heroName)
{
   if (!heroName.IsEmpty()) {
      if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->heroes.FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) {
         heroRef->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
      }
   } else {
      for (ABaseHero* hero : userInputRef->GetBasePlayer()->heroes)
         hero->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
   }
}

void UMyCheatManager::PauseGameTimer() { gameStateRef->UpdateGameSpeed(0); }

void UMyCheatManager::SetGameTime(int seconds, int minutes, int hours) { gameStateRef->UpdateGameTime(seconds, minutes, hours); }

void UMyCheatManager::SetGameDay(int day, int month, int year) { gameStateRef->UpdateGameDay(day, month, year); }

void UMyCheatManager::SetUnitCurHP(FString unitName, int hpVal)
{
   if (AUnit* unit = UpResourceManager::FindTriggerObjectInWorld<AUnit>(unitName, GetWorld())) { unit->SetVitalCurValue(static_cast<uint8>(EVitals::Health), hpVal); }
}

void UMyCheatManager::SeeAll()
{
   for (AEnemy* e : gameStateRef->enemyList) {
      e->IncVisionCount();
      e->GetCapsuleComponent()->SetVisibility(true, true);
   }
}

void UMyCheatManager::LearnAllTopics()
{
   TSharedPtr<FGameplayTagNode>         rootDialogNode = UGameplayTagsManager::Get().FindTagNode("Dialog");
   TSet<TSharedPtr<FGameplayTagNode>>   leafNodes{};
   TSet<TSharedPtr<FGameplayTagNode>>   newLeafNodes{};
   TArray<TSharedPtr<FGameplayTagNode>> childNodes{};

   leafNodes.Add(rootDialogNode);

   while (leafNodes.Num() > 0) {
      for (TSharedPtr<FGameplayTagNode> node : leafNodes) {
         childNodes = node->GetChildTagNodes();
         if (childNodes.Num() > 0) { newLeafNodes.Append(childNodes); } else { userInputRef->GetBasePlayer()->LearnDialogTopic(node->GetCompleteTag()); }
      }
      leafNodes = newLeafNodes;
      newLeafNodes.Empty();
   }
}

void UMyCheatManager::SetChapterAndSection(int chapter, int section) { gameModeRef->eventManager->SkipToEvent(chapter, section); }

void UMyCheatManager::SpawnEnemies(FName id, int level, int numberToSpawn, FVector spawnLocation) { for (int i = 0; i < numberToSpawn; ++i) { } }

void UMyCheatManager::EnableCSVCategories(FString csvCategories)
{
   TArray<FString> CsvCategories;
   csvCategories.ParseIntoArray(CsvCategories, TEXT(","), true);
   for (auto x : CsvCategories) {
      if (!FCsvProfiler::Get()->EnableCategoryByString(x))
      UE_LOG(LogTemp, Error, TEXT("Category %s was unsucessfully enabled!"), *x);
   }
}
