// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Trigger.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "RTSIngameWidget.h"
#include "UpStatComponent.h"
#include "UI/HUDManager.h"
#include "Quests/Quest.h"
#include "Quests/QuestManager.h"
#include "DialogSystem/DialogBox.h"
#include "DialogSystem/DialogManager.h"
#include "AIStuff/AIControllers/NPCAIController.h"
#include "Globals/UpResourceManager.h"
#include "WorldObjects/NPC.h"
#include "WorldObjects/Unit.h"
#include "WorldObjects/BaseHero.h"
#include "Interactables/InteractableBase.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Runtime/MovieScene/Public/MovieSceneSequencePlayer.h"

FTriggerData FTriggerData::defaultTrigger = FTriggerData();

FTriggerData::FTriggerData()
{
}

void UTriggerManager::Init()
{
   cpcRef      = Cast<AUserInput>(GetOuter()->GetWorld()->GetFirstPlayerController());
   gameModeRef = Cast<ARTSGameMode>(GetOuter());
}

void UTriggerManager::AddTriggerToRecords(FName worldObjectName, const FTriggerData& finishedTriggerActivation)
{
   triggerRecords.Add(worldObjectName, finishedTriggerActivation);
}

void UTriggerManager::ActivateTrigger(UPARAM(ref) FTriggerData& triggerData)
{
   if(triggerData.enabled) {
      if(triggerData.numCalls != 0) { TriggerEffect(triggerData); }
   }
}

void UTriggerManager::ChangeDialog(const FTriggerData& tdata)
{
   check(tdata.triggerObjects.Num() == 1 && tdata.triggerValues.Num() <= 2);
   if(ANPC* finishedTalkNPC = UpResourceManager::FindTriggerObjectInWorld<ANPC>(tdata.triggerObjects[0], cpcRef->GetWorld())) {
      if(tdata.triggerValues.Num() == 1)
         finishedTalkNPC->SetCurrentDialog(*tdata.triggerValues[0]);
      else
         finishedTalkNPC->SetSpecificDialog(FGameplayTag::RequestGameplayTag(*tdata.triggerValues[0]), *tdata.triggerValues[1]);
   } else {
      // TODO: Find a way to add to trigger records w/o knowing if there's already an entry in triggerRecords
      AddTriggerToRecords(*tdata.triggerObjects[0], tdata);
   }
}

void UTriggerManager::ModifyStats(const FTriggerData& tdata)
{
   check(tdata.triggerObjects.Num() == 1 && tdata.triggerValues.Num() <= 2);
   uint8 offset = 0;
   if(AUnit* unit = UpResourceManager::FindTriggerObjectInWorld<AUnit>(tdata.triggerObjects[0], cpcRef->GetWorld()))
      for(int i = 0; i < tdata.triggerValues.Num(); i += 2) {
         const uint8 statId      = TriggerValueToNum(tdata, i);
         const uint8 statEnumVal = UpResourceManager::statMapper[statId];
         const uint8 statValue   = TriggerValueToNum(tdata, i + 1);

         switch(statEnumVal) {
            case 0: unit->GetStatComponent()->ModifyStats<false>(statValue, static_cast<EAttributes>(statId - offset)); break;
            case 1:
               offset = CombatInfo::AttCount;
               unit->GetStatComponent()->ModifyStats<false>(statValue, static_cast<EUnitScalingStats>(statId - offset));
               break;
            case 2:
               offset = CombatInfo::AttCount + CombatInfo::StatCount;
               unit->GetStatComponent()->ModifyStats<false>(statValue, static_cast<EVitals>(statId - offset));
               break;
            case 3:
               offset = CombatInfo::AttCount + CombatInfo::StatCount + CombatInfo::VitalCount;
               unit->GetStatComponent()->ModifyStats<false>(statValue, static_cast<EMechanics>(statId - offset));
               break;
            default: break;
         }
      }
}

void UTriggerManager::OpenHUDTrigger(const FTriggerData& tdata)
{
   checkf(tdata.triggerObjects.Num() == 0 && tdata.triggerValues.Num() == 1, TEXT("Incorrect parameters for OPENHUDTRIGGER"));
   checkf(tdata.triggerValues[0].IsNumeric(), TEXT("OPENHUDTRIGGER triggerValue should be numeric"));
   // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, tdata.triggerValues[0]);
   hudManagerRef->AddHUD((uint8)TriggerValueToNum(tdata, 0));
}

void UTriggerManager::OpenHUDTriggerStorage(const FTriggerData& tdata)
{
   checkf(tdata.triggerObjects.Num() == 1 && tdata.triggerValues.Num() == 2, TEXT("Incorrect parameters for OPENHUDTRIGGER"));
   checkf(tdata.triggerValues[0].IsNumeric(), TEXT("OPENHUDTRIGGER triggerValue should be numeric"));
   // if(AInteractableBase* interactable = UpResourceManager::FindTriggerObjectInWorld<AInteractableBase>(tdata.triggerObjects[0], cpcRef->GetWorld()))
   // if(ABaseHero* heroRef = UpResourceManager::FindTriggerObjectInWorld<ABaseHero>(tdata.triggerObjects[1], cpcRef->GetWorld()))
   // hudManagerRef->AddHUD(int);
}

void UTriggerManager::ChangeParty(const FTriggerData& tdata)
{
   checkf(tdata.triggerObjects.Num() <= 4, TEXT("Incorrect number of parameters %d for ChangePartyTrigger"), tdata.triggerObjects.Num());
   TArray<ABaseHero*> newHeroes = TArray<ABaseHero*>();

   for(FString heroName : tdata.triggerObjects) {
      // empty string means no hero at that slot
      if(heroName.IsEmpty()) { break; }

      ABaseHero* hero = UpResourceManager::FindTriggerObjectInWorld<ABaseHero>(heroName, cpcRef->GetWorld());
      if(hero) { newHeroes.Add(hero); }
   }
   // GetBasePlayer will return null if we don't go through persistent level setup first
   cpcRef->GetBasePlayer()->UpdateParty(newHeroes);
   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("This is the number of new members in your party!: ") + FString::FromInt(newHeroes.Num()));
}

void UTriggerManager::ActivateOtherTrigger(const FTriggerData& tdata)
{
}

void UTriggerManager::DeactivateOtherTrigger(const FTriggerData& tdata)
{
}

void UTriggerManager::ChangeTriggerType(const FTriggerData& tdata)
{
}

bool UTriggerManager::AddQuest(const FTriggerData& tdata)
{
   checkf(tdata.triggerObjects.Num() == 0 && tdata.triggerValues.Num() == 2, TEXT("Incorrect parameters for OPENHUDTRIGGER"));
   checkf(tdata.triggerValues[1].IsNumeric(), TEXT("ADDQUESTTRIGGER triggerValue 2 should be numeric"));
   if(gameModeRef->GetQuestManager()->AddNewQuest(
          gameModeRef->GetQuestManager()->questClassList[FGameplayTag::RequestGameplayTag(*(FString("QuestName.") + tdata.triggerValues[0]))],
          (bool)FCString::Atoi(*tdata.triggerValues[1]))) {
      return true;
   }
   return false;
}

bool UTriggerManager::CompleteQuestGoal(const FTriggerData& tdata)
{
   checkf(tdata.triggerValues.Num() == 3, TEXT("Complete quest goal trigger should have 3 parameters!"));
   checkf(tdata.triggerValues[1].IsNumeric(), TEXT("ADDQUESTTRIGGER triggerValue 2 should be numeric"));

   AQuest* quest = *gameModeRef->GetQuestManager()->quests.FindByPredicate(
       [&](AQuest* questToFilter) { return questToFilter->questInfo.id.ToString() == "QuestName." + tdata.triggerValues[0]; });
   int goalIndex = FCString::Atoi(*tdata.triggerValues[1]);

   if(quest->questInfo.subgoals[goalIndex].goalState == EGoalState::currentGoal) {
      quest->CompleteSubGoal(goalIndex, (bool)FCString::Atoi(*tdata.triggerValues[2]));
      return true;
   }
   return false;
}

void UTriggerManager::DisplayDialog(const FTriggerData& tdata)
{
   TArray<FDialogData> dialog;

   if(tdata.triggerValues.Num() > 1) {
      for(int i = 0; i < tdata.triggerValues.Num() - 1; ++i) {
         TArray<int> nextIndices = {i + 1};
         dialog.Emplace(nextIndices, FText::FromString(tdata.triggerValues[i]), "");
      }
   }
   dialog.Emplace(TArray<int>(), FText::FromString(tdata.triggerValues[tdata.triggerValues.Num() - 1]), "");
   hudManagerRef->ShowDialogCustomLines(dialog, EDialogBoxCloseCase::finishedTriggerActivation);
}

void UTriggerManager::DisplayConversation(const FTriggerData& tdata)
{
   hudManagerRef->ShowDialogWithSource(*tdata.triggerValues[0], EDialogBoxCloseCase::finishedTriggerActivation);
}

void UTriggerManager::DestroyNPC(const FTriggerData& tdata)
{
   hudManagerRef->GetIngameHUD()->GetDialogBox()->SetDialogSource(EDialogBoxCloseCase::none); // ensure we don't go back to any social menus afterwards
   UpResourceManager::FindTriggerObjectInWorld<ANPC>(*tdata.triggerObjects[0], cpcRef->GetWorld())->Destroy();
   // in case this npc destroyed itself while we're talking, set the interactedHero to nullptr
   cpcRef->GetBasePlayer()->heroInBlockingInteraction = nullptr;
}

void UTriggerManager::MoveNPC(const FTriggerData& tdata)
{
   FVector newLocation{FCString::Atof(*tdata.triggerValues[0]), FCString::Atof(*tdata.triggerValues[1]), FCString::Atof(*tdata.triggerValues[2])};
   UpResourceManager::FindTriggerObjectInWorld<ANPC>(*tdata.triggerObjects[0], cpcRef->GetWorld())->SetActorLocation(newLocation);
}

void UTriggerManager::AddItem(const FTriggerData& tdata)
{
   if(ensure(tdata.triggerValues.Num() == 2)) {
      FMyItem itemToAdd{FCString::Atoi(*tdata.triggerValues[0]), FCString::Atoi(*tdata.triggerValues[1])};
      //TODO: Handle cases when we don't have inventory space?
      if(tdata.triggerObjects.Num() > 0) {
         UpResourceManager::FindTriggerObjectInWorld<ABaseHero>(*tdata.triggerObjects[0], cpcRef->GetWorld())->backpack->AddItem(itemToAdd);
      } else {
         // If there's a hero using something, add the item to him/her
         if(ABaseHero* activeHero = cpcRef->GetBasePlayer()->heroInBlockingInteraction; activeHero) activeHero->backpack->AddItem(itemToAdd);
      }
   }
}

void UTriggerManager::LearnDialogTopic(const FTriggerData& tdata)
{
   cpcRef->GetBasePlayer()->LearnDialogTopic(FGameplayTag::RequestGameplayTag(*tdata.triggerValues[0]));
}

void UTriggerManager::SetNPCFollow(const FTriggerData& tdata)
{
   checkf(tdata.triggerObjects.Num() > 0 && tdata.triggerObjects.Num() < 3, TEXT("Set NPC follow trigger should have 1-2 parameters!"));
   ANPC* npcRef = UpResourceManager::FindTriggerObjectInWorld<ANPC>(*tdata.triggerObjects[0], cpcRef->GetWorld());
   if(npcRef) {
      const ABaseHero* heroRef = nullptr;
      if(tdata.triggerObjects.Num() > 1) {
         int         heroIndex = FCString::Atoi(*tdata.triggerObjects[0]);
         const auto& heroes    = cpcRef->GetBasePlayer()->GetHeroes();
         if(heroIndex > 0 && heroIndex < heroes.Num()) heroRef = heroes[FCString::Atoi(*tdata.triggerObjects[0])];
      } else
         heroRef = cpcRef->GetBasePlayer()->heroInBlockingInteraction;

      if(heroRef) Cast<ANPCAIController>(npcRef->GetController())->Follow(heroRef);
   }
}

void UTriggerManager::SetNPCWantConverse(const FTriggerData& tdata)
{
   checkf(tdata.triggerObjects.Num() > 0 && tdata.triggerObjects.Num() < 3, TEXT("Set NPC want converse trigger should have 2 parameters!"));
   ANPC* npcRef = UpResourceManager::FindTriggerObjectInWorld<ANPC>(*tdata.triggerObjects[0], cpcRef->GetWorld());
   if(npcRef) { npcRef->bWantsToConverse = (bool)FCString::Atoi(*tdata.triggerValues[0]); }
}

void UTriggerManager::PlaySequence(const FTriggerData& tdata)
{
   checkf(tdata.triggerValues.Num() == 1, TEXT("Missing sequence filepath parameter for triggerValue"));
   FString               filePath = "/Game/RTS_Tutorial/Sequencer/" + tdata.triggerValues[0];
   FStringAssetReference sequencetoLoad{filePath};
   ALevelSequenceActor*  sequenceActorRef;
   ULevelSequencePlayer* sequencePlayer =
       ULevelSequencePlayer::CreateLevelSequencePlayer(cpcRef, Cast<ULevelSequence>(sequencetoLoad.TryLoad()), FMovieSceneSequencePlaybackSettings(), sequenceActorRef);
   sequencePlayer->Play();
}

void UTriggerManager::TriggerEffect(FTriggerData& triggerData)
{
#if UE_EDITOR
   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(static_cast<uint8>(triggerData.triggerType)) + " Type Trigger Activated");
#endif

   // If the trigger activation fails somehow, the call count will not be decreased.
   switch(triggerData.triggerType) {
      case ETriggerType::None: return; break;
      case ETriggerType::OpenHUDTrigger: OpenHUDTrigger(triggerData); break;
      case ETriggerType::ModifyStats: ModifyStats(triggerData); break;
      case ETriggerType::ActivateOtherTrigger: ActivateTrigger(UPARAM(ref) triggerData); break;
      case ETriggerType::ChangeDialog: ChangeDialog(triggerData); break;
      case ETriggerType::ChangePartyTrigger: ChangeParty(triggerData); break;
      case ETriggerType::AddQuestTrigger:
         if(!AddQuest(triggerData)) return;
         break;
      case ETriggerType::CompleteQuestGoalTrigger:
         if(!CompleteQuestGoal(triggerData)) return;
         break;
      case ETriggerType::DisplayConversationTrigger: DisplayConversation(triggerData); break;
      case ETriggerType::DisplayDialogTrigger: DisplayDialog(triggerData); break;
      case ETriggerType::DestroyNPCTrigger: DestroyNPC(triggerData); break;
      case ETriggerType::MoveNPCTrigger: MoveNPC(triggerData); break;
      case ETriggerType::AddItemTrigger: AddItem(triggerData); break;
      case ETriggerType::LearnDialogTopic: LearnDialogTopic(triggerData); break;
      case ETriggerType::SetNPCFollow: SetNPCFollow(triggerData); break;
      case ETriggerType::SetNPCWantConverse: SetNPCWantConverse(triggerData); break;
      case ETriggerType::PlaySequence: PlaySequence(triggerData); break;
      default: UE_LOG(LogTemp, Warning, TEXT("Unknown Trigger type attempted to be activated!"));
   }

   if(triggerData.numCalls != -1) --triggerData.numCalls;
}
