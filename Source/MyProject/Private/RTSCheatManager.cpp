<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

=======
>>>>>>> componentrefactor
#include "MyProject.h"
#include "RTSCheatManager.h"

#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "BasePlayer.h"
<<<<<<< HEAD
#include "RTSIngameWidget.h"
#include "RTSVisionComponent.h"

#include "UpResourceManager.h"
#include "UpStatComponent.h"
=======
#include "RTSDamageCalculation.h"
#include "RTSIngameWidget.h"
#include "RTSVisionComponent.h"
#include "UnitController.h"

#include "UpResourceManager.h"
#include "UpStatComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
>>>>>>> componentrefactor
#include "Quests/QuestManager.h"
#include "Quests/Quest.h"

#include "UI/HUDManager.h"
<<<<<<< HEAD
#include "UI/UserWidgets/ActionbarInterface.h"
#include "UI/UserWidgets/ESkillContainer.h"
=======
#include "UI/Actionbar/ActionbarInterface.h"
#include "UI/Actionbar/ESkillContainer.h"
>>>>>>> componentrefactor
#include "UI/Slots/SkillSlot.h"

#include "SpellSystem/SpellDataManager.h"
#include "SpellSystem/RTSAbilitySystemComponent.h"

#include "WorldObjects/Unit.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/Enemies/Enemy.h"

#include "EventSystem/EventManager.h"

void URTSCheatManager::InitCheatManager()
{
   Super::InitCheatManager();
   userInputRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   gameModeRef  = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
}

<<<<<<< HEAD
void URTSCheatManager::LevelUp(FString heroName)
{
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Leveling Up via Cheats!"));
   if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->GetHeroes().FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) { heroRef->LevelUp(); }
}

void URTSCheatManager::LevelUpToLevel(FString heroName, int level)
{
   if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->GetHeroes().FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) {
      while (heroRef->GetStatComponent()->GetUnitLevel() < level)
         heroRef->LevelUp();
   }
}

void URTSCheatManager::GodMode(FString objectID, int toggleGodMode)
{
#if UE_EDITOR
   if (AUnit* unitRef = UpResourceManager::FindTriggerObjectInWorld<AUnit>(objectID, userInputRef->GetWorld())) {
      if (toggleGodMode)
         unitRef->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"));
      else
         unitRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"));
=======
void URTSCheatManager::Up_LevelUp(FString heroName)
{
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Leveling Up via Cheats!"));
   if(ABaseHero* heroRef = *userInputRef->GetBasePlayer()->GetHeroes().FindByPredicate([heroName](ABaseHero* hero) {
         return hero->GetGameName().ToString() == heroName;
      }))
   {
      heroRef->LevelUp();
   }
}

void URTSCheatManager::Up_LevelUpToLevel(FString heroName, int level)
{
   if(ABaseHero* heroRef = *userInputRef->GetBasePlayer()->GetHeroes().FindByPredicate([heroName](ABaseHero* hero) {
         return hero->GetGameName().ToString() == heroName;
      }))
   {
      while(heroRef->GetStatComponent()->GetUnitLevel() < level)
      {
         heroRef->LevelUp();
      }
   }
}

void URTSCheatManager::Up_BuffAllHeroesStats()
{
   for(ABaseHero* hero : userInputRef->GetBasePlayer()->GetHeroes())
   {
      for(int i = 0; i < static_cast<int>(EAttributes::Count); ++i)
      {
         hero->GetStatComponent()->ModifyStats<true, EAttributes>(999, static_cast<EAttributes>(i));
      }
      hero->GetStatComponent()->ModifyStats<true>(999, EMechanics::MovementSpeed);
   }
}

void URTSCheatManager::Up_BuffAllEnemyStats()
{
   for(AUnit* unit : gameStateRef->GetAllEnemyUnits())
   {
      for(int i = 0; i < static_cast<int>(EAttributes::Count); ++i)
      {
         unit->GetStatComponent()->ModifyStats<true, EAttributes>(999, static_cast<EAttributes>(i));
      }
      unit->GetStatComponent()->ModifyStats<true>(999, EMechanics::MovementSpeed);
   }
}

void URTSCheatManager::Up_GodMode(FString objectID, int toggleGodMode)
{
#if UE_EDITOR
   if(AUnit* unitRef = UpResourceManager::FindTriggerObjectInWorld<AUnit>(objectID, userInputRef->GetWorld()))
   {
      if(toggleGodMode)
      {
         unitRef->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"));
      }
      else
      {
         unitRef->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Buff.GodMode"));
      }
>>>>>>> componentrefactor
   }
#endif
}

<<<<<<< HEAD
void URTSCheatManager::AddQuest(FString questName)
{
   auto questTag = FGameplayTag::RequestGameplayTag(*(FString("QuestName." + questName)));
   if (gameModeRef->GetQuestManager()->questClassList.Contains(questTag)) {
=======
void URTSCheatManager::Up_AddQuest(FString questName)
{
   const auto questTag = FGameplayTag::RequestGameplayTag(*(FString("QuestName." + questName)));
   if(gameModeRef->GetQuestManager()->questClassList.Contains(questTag))
   {
>>>>>>> componentrefactor
      FTriggerData addQuestTrigger;
      addQuestTrigger.enabled       = true;
      addQuestTrigger.numCalls      = 1;
      addQuestTrigger.triggerType   = ETriggerType::AddQuestTrigger;
      addQuestTrigger.triggerValues = {questName, "1"};
      gameModeRef->GetTriggerManager()->ActivateTrigger(addQuestTrigger);
<<<<<<< HEAD
   } else { UE_LOG(LogTemp, Error, TEXT("Quest cheat command passed invalid parameters!")); }
}

void URTSCheatManager::FinishQuest(FString questName, int isSucessful)
{
   AQuest* quest = *gameModeRef->GetQuestManager()->quests.FindByPredicate([questName](AQuest* quest) { return quest->questInfo.name.ToString() == questName; });
   if (quest) { isSucessful == 0 ? quest->CompleteQuest(false) : quest->CompleteQuest(true); }
}

void URTSCheatManager::EquipSpell(FString heroName, int spellID, int slot)
{
   if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->GetHeroes().FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) {
      if (slot >= 0 && slot < heroRef->GetAbilitySystemComponent()->GetAbilities().Num())
         userInputRef->GetWidgetProvider()->GetIngameHUD()->GetActionHUD()->GetSkillContainer()->skillSlots[slot]->UpdateSkillSlot(USpellDataManager::GetData().GetSpellClass(spellID));
   }
}

void URTSCheatManager::RefreshSpells()
{
   for (AUnit* ally : userInputRef->GetGameState()->GetAllFriendlyUnits())
      ally->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
   for (AUnit* enemy : userInputRef->GetGameState()->GetAllEnemyUnits())
      enemy->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
}

void URTSCheatManager::RefreshHeroSpells(FString heroName)
{
   if (!heroName.IsEmpty()) {
      if (ABaseHero* heroRef = *userInputRef->GetBasePlayer()->GetHeroes().FindByPredicate([heroName](ABaseHero* hero) { return hero->GetGameName().ToString() == heroName; })) {
         heroRef->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
      }
   } else {
      for (ABaseHero* hero : userInputRef->GetBasePlayer()->GetHeroes())
=======
   }
   else
   {
      UE_LOG(LogTemp, Error, TEXT("Quest cheat command passed invalid parameters!"));
   }
}

void URTSCheatManager::Up_FinishQuest(FString questName, int isSucessful)
{
   auto pred = [questName](AQuest* quest) {
      return quest->GetQuestInfo().name.ToString() == questName;
   };
   AQuest* quest = *gameModeRef->GetQuestManager()->activeQuests.FindByPredicate(pred);
   if(quest)
   {
      quest->CompleteQuest();
   }
}

void URTSCheatManager::Up_EquipSpell(FString spellNameTag, int slot)
{
   if(AUnit* focusedUnit = userInputRef->GetBasePlayer()->GetFocusedUnit())
   {
      if(slot >= 0 && slot < focusedUnit->GetAbilitySystemComponent()->GetAbilities().Num())
      {
         focusedUnit->GetAbilitySystemComponent()->SetSpellAtSlot(
             USpellDataManager::GetData().GetSpellClass(FGameplayTag::RequestGameplayTag(*(FString("Skill.Name.") + spellNameTag))), slot);
      }
   }
}

void URTSCheatManager::Up_RefreshSpells()
{
   for(AUnit* ally : userInputRef->GetGameState()->GetAllAllyUnits())
      ally->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
   for(AUnit* enemy : userInputRef->GetGameState()->GetAllEnemyUnits())
      enemy->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
}

void URTSCheatManager::Up_RefreshHeroSpells(FString heroName)
{
   if(!heroName.IsEmpty())
   {
      if(ABaseHero* heroRef = *userInputRef->GetBasePlayer()->GetHeroes().FindByPredicate([heroName](ABaseHero* hero) {
            return hero->GetGameName().ToString() == heroName;
         }))
      {
         heroRef->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
      }
   }
   else
   {
      for(ABaseHero* hero : userInputRef->GetBasePlayer()->GetHeroes())
>>>>>>> componentrefactor
         hero->GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Skill.Name")));
   }
}

<<<<<<< HEAD
void URTSCheatManager::PauseGameTimer() { gameStateRef->UpdateGameSpeed(0); }

void URTSCheatManager::SetGameTime(int seconds, int minutes, int hours) { gameStateRef->SetGameTime(FUpTime(seconds, minutes, hours), FUpDate()); }

void URTSCheatManager::SetGameDay(int day, int month, int year) { gameStateRef->SetGameTime(FUpTime(), FUpDate(day, month, year)); }

void URTSCheatManager::SetUnitCurHP(FString unitName, int hpVal)
{
   if (AUnit* unit = UpResourceManager::FindTriggerObjectInWorld<AUnit>(unitName, GetWorld())) { unit->GetStatComponent()->ModifyStats(hpVal, EVitals::Health); }
}

void URTSCheatManager::SeeAll()
{
   for (AUnit* e : gameStateRef->GetAllEnemyUnits()) {
      e->GetVisionComponent()->IncVisionCount();
      e->GetCapsuleComponent()->SetVisibility(true, true);
   }
}

void URTSCheatManager::LearnAllTopics()
{
   const TSharedPtr<FGameplayTagNode>         rootDialogNode = UGameplayTagsManager::Get().FindTagNode("Dialog");
=======
void URTSCheatManager::Up_PauseGameTimer()
{
   gameStateRef->UpdateGameSpeed(0);
}

void URTSCheatManager::Up_SetGameTime(int seconds, int minutes, int hours)
{
   gameStateRef->SetGameTime(FUpTime(seconds, minutes, hours), FUpDate());
}

void URTSCheatManager::Up_SetGameDay(int day, int month, int year)
{
   gameStateRef->SetGameTime(FUpTime(), FUpDate(day, month, year));
}

void URTSCheatManager::Up_SetUnitCurHP(FString unitName, int hpVal)
{
   if(AUnit* unit = UpResourceManager::FindTriggerObjectInWorld<AUnit>(unitName, GetWorld()))
   {
      unit->GetStatComponent()->ModifyStats(hpVal, EVitals::Health);
   }
}

void URTSCheatManager::Up_SeeAll()
{
   gameStateRef->StopVisionChecks();

   for(AUnit* e : gameStateRef->GetAllEnemyUnits())
   {
      e->SetIsUnitHidden(false);
   }

   for(AUnit* a : gameStateRef->GetAllAllyUnits())
   {
      a->SetIsUnitHidden(false);
   }
}

void URTSCheatManager::Up_LearnAllTopics()
{
   const TSharedPtr<FGameplayTagNode>   rootDialogNode = UGameplayTagsManager::Get().FindTagNode("Dialog");
>>>>>>> componentrefactor
   TSet<TSharedPtr<FGameplayTagNode>>   leafNodes{};
   TSet<TSharedPtr<FGameplayTagNode>>   newLeafNodes{};
   TArray<TSharedPtr<FGameplayTagNode>> childNodes{};

   leafNodes.Add(rootDialogNode);

<<<<<<< HEAD
   while (leafNodes.Num() > 0) {
      for (TSharedPtr<FGameplayTagNode> node : leafNodes) {
         childNodes = node->GetChildTagNodes();
         if (childNodes.Num() > 0) { newLeafNodes.Append(childNodes); } else { userInputRef->GetBasePlayer()->LearnDialogTopic(node->GetCompleteTag()); }
=======
   while(leafNodes.Num() > 0)
   {
      for(TSharedPtr<FGameplayTagNode> node : leafNodes)
      {
         childNodes = node->GetChildTagNodes();
         if(childNodes.Num() > 0)
         {
            newLeafNodes.Append(childNodes);
         }
         else
         {
            userInputRef->GetBasePlayer()->LearnDialogTopic(node->GetCompleteTag());
         }
>>>>>>> componentrefactor
      }
      leafNodes = newLeafNodes;
      newLeafNodes.Empty();
   }
}

<<<<<<< HEAD
void URTSCheatManager::SetChapterAndSection(int chapter, int section) { gameModeRef->eventManager->SkipToEvent(chapter, section); }

void URTSCheatManager::SpawnEnemies(FName id, int level, int numberToSpawn, FVector spawnLocation) { for (int i = 0; i < numberToSpawn; ++i) { } }

void URTSCheatManager::EnableCSVCategories(FString csvCategories)
{
   TArray<FString> CsvCategories;
   csvCategories.ParseIntoArray(CsvCategories, TEXT(","), true);
   for (auto x : CsvCategories) {
      if (!FCsvProfiler::Get()->EnableCategoryByString(x))
      UE_LOG(LogTemp, Error, TEXT("Category %s was unsucessfully enabled!"), *x);
=======
void URTSCheatManager::Up_SetChapterAndSection(int chapter, int section)
{
   gameModeRef->eventManager->SkipToEvent(chapter, section);
}

void URTSCheatManager::Up_SpawnEnemies(FName id, int level, int numberToSpawn, FVector spawnLocation)
{
   for(int i = 0; i < numberToSpawn; ++i)
   {
   }
}

void URTSCheatManager::Up_ToggleEnemyAI()
{
   static int AI_IS_ACTIVE = 1;
   if(AI_IS_ACTIVE)
   {
      for(AUnit* unit : gameStateRef->GetAllEnemyUnits())
      {
         if(UBehaviorTreeComponent* BTComp = unit->GetUnitController()->FindComponentByClass<UBehaviorTreeComponent>())
         {
            BTComp->StopTree();
         }
      }
      AI_IS_ACTIVE = 0;
   }
   else
   {
      for(AUnit* unit : gameStateRef->GetAllEnemyUnits())
      {
         if(UBehaviorTreeComponent* BTComp = unit->GetUnitController()->FindComponentByClass<UBehaviorTreeComponent>())
         {
            BTComp->StartLogic();
         }
      }
   }
   AI_IS_ACTIVE = 1;
}

void URTSCheatManager::Up_ShowDebugCapsules(bool bShouldShow)
{
   if(!bShouldShow)
   {
      for(AUnit* unit : gameStateRef->GetAllEnemyUnits())
      {
         unit->GetCapsuleComponent()->SetHiddenInGame(false);
      }

      for(AUnit* unit : gameStateRef->GetAllAllyUnits())
      {
         unit->GetCapsuleComponent()->SetHiddenInGame(false);
      }
   }
   else
   {
      for(AUnit* unit : gameStateRef->GetAllEnemyUnits())
      {
         unit->GetCapsuleComponent()->SetHiddenInGame(true);
      }

      for(AUnit* unit : gameStateRef->GetAllAllyUnits())
      {
         unit->GetCapsuleComponent()->SetHiddenInGame(true);
      }
   }
}

void URTSCheatManager::Up_ShowVisionSpheres(bool bShouldShow)
{
   if(bShouldShow)
   {
      for(AUnit* unit : gameStateRef->GetAllEnemyUnits())
      {
         unit->GetVisionComponent()->SetHiddenInGame(false);
      }

      for(AUnit* unit : gameStateRef->GetAllAllyUnits())
      {
         unit->GetVisionComponent()->SetHiddenInGame(false);
      }
   }
   else
   {
      for(AUnit* unit : gameStateRef->GetAllEnemyUnits())
      {
         unit->GetVisionComponent()->SetHiddenInGame(true);
      }

      for(AUnit* unit : gameStateRef->GetAllAllyUnits())
      {
         unit->GetVisionComponent()->SetHiddenInGame(true);
      }
   }
}

void URTSCheatManager::Up_DrawSphere(FString unitName, float sphereRadius, float duration)
{
   if(!unitName.IsEmpty())
   {
      if(AUnit* unit = UpResourceManager::FindTriggerObjectInWorld<AUnit>(unitName, GetWorld()))
      {
         DrawDebugSphere(GetWorld(), unit->GetActorLocation(), sphereRadius, 25, FColor::Magenta, false, duration, 0, 2);
      }
   }
   else
   {
      if(AUnit* unit = userInputRef->GetBasePlayer()->GetFocusedUnit())
      {
         DrawDebugSphere(GetWorld(), unit->GetActorLocation(), sphereRadius, 25, FColor::Magenta, false, duration, 0, 2);
      }
   }
}

void URTSCheatManager::Up_DamageUnit(FString unitName, int damageAmount, bool bCrit)
{
   if(AUnit* unit = UpResourceManager::FindTriggerObjectInWorld<AUnit>(unitName, GetWorld()))
   {
      UUpStatComponent* statComp = unit->GetStatComponent();
      const FUpDamage   mockDamage(nullptr, unit, 0, damageAmount, 0, FGameplayTag::RequestGameplayTag("Combat.Element.Fire"), FGameplayTag::EmptyTag, false);
      unit->OnUnitDamageReceived().Broadcast(mockDamage);
      // URTSDamageCalculation::ShowDamageDealt(mockDamage);
   }
}

void URTSCheatManager::Up_EnableCSVCategories(FString csvCategories)
{
   TArray<FString> CsvCategories;
   csvCategories.ParseIntoArray(CsvCategories, TEXT(","), true);
   for(auto x : CsvCategories)
   {
      if(!FCsvProfiler::Get()->EnableCategoryByString(x)) UE_LOG(LogTemp, Error, TEXT("Category %s was unsucessfully enabled!"), *x);
>>>>>>> componentrefactor
   }
}
