// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyCheatManager.h"

#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "BasePlayer.h"

#include "ResourceManager.h"
#include "Quests/QuestManager.h"
#include "Quests/Quest.h"

#include "UI/HUDManager.h"
#include "UI/UserWidgets/ActionbarInterface.h"
#include "UI/ESkillContainer.h"
#include "UI/Slots/SkillSlot.h"

#include "SpellSystem/SpellManager.h"

#include "WorldObjects/Unit.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/Enemies/Enemy.h"

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
   if (AUnit* unitRef = ResourceManager::FindTriggerObjectInWorld<AUnit>(objectID, userInputRef->GetWorld())) {
      if (toggleGodMode)
         unitRef->godMode = true;
      else
         unitRef->godMode = false;
   }
#endif
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

void UMyCheatManager::PauseGameTimer()
{
   gameStateRef->UpdateGameSpeed(0);
}

void UMyCheatManager::SetGameTime(int seconds, int minutes, int hours)
{
   gameStateRef->UpdateGameTime(seconds, minutes, hours);
}

void UMyCheatManager::SetGameDay(int day, int month, int year)
{
   gameStateRef->UpdateGameDay(day, month, year);
}

void UMyCheatManager::SetUnitCurHP(FString unitName, int hpVal)
{
   if (AUnit* unit = ResourceManager::FindTriggerObjectInWorld<AUnit>(unitName, GetWorld())) { unit->SetVitalCurValue(static_cast<uint8>(Vitals::Health), hpVal); }
}

void UMyCheatManager::SeeAll()
{
   for (AEnemy* e : gameStateRef->enemyList) {
      e->IncVisionCount();
      e->GetCapsuleComponent()->SetVisibility(true, true);
   }
}
