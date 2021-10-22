// Created 6/14/21 7:14 PM

#include "MyProject.h"
#include "RTSTriggerTypes.h"
#include "BasePlayer.h"
#include "DialogBox.h"
#include "HUDManager.h"
#include "LevelSequencePlayer.h"
#include "NPC.h"
#include "NPCAIController.h"
#include "QuestManager.h"
#include "Quest.h"
#include "RTSGameMode.h"
#include "Unit.h"
#include "UpResourceManager.h"

void URTSTriggerStatModify::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(UWorld* World = GetWorld())
   {
      if(AUnit* Unit = UpResourceManager::FindTriggerObjectInWorld<AUnit>(unitName.ToString(), World))
      {
      }
   }
}

void URTSTriggerOpenHUD::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(UWorld* World = GetWorld())
   {
      if(AUserInput* UserInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
      {
         if(bOpen)
         {
            UserInput->GetHUDManager()->AddHUD(hudToOpen);
         }
         else
         {
            UserInput->GetHUDManager()->HideHUD(hudToOpen);
         }
      }
   }
}

void URTSTriggerChangeParty::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   TArray<ABaseHero*> NewHeroes = TArray<ABaseHero*>();

   if(UWorld* World = GetWorld())
   {
      for(TSubclassOf<ABaseHero> HeroClass : newHeroesInParty)
      {
         if(IsValid(HeroClass))
         {
            UE_LOG(Up_Log_Triggers, Warning, TEXT("Invalid Hero Ref in Trigger %s"), *GetTriggerName().ToString());
            return;
         }

         if(ABaseHero* Hero = UpResourceManager::FindTriggerObjectInWorld<ABaseHero>(HeroClass.GetDefaultObject()->GetGameName().ToString(), World))
         {
            NewHeroes.Add(Hero);
         }
      }

      if(NewHeroes.Num() > 0)
      {
         if(AUserInput* UserInput = Cast<AUserInput>(World->GetFirstPlayerController()))
         {
            if(ABasePlayer* BasePlayer = UserInput->GetBasePlayer())
            {
               BasePlayer->UpdateActiveParty(NewHeroes);
            }
         }
      }
   }
}

void URTSTriggerAddQuest::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(IsValid(questToAdd))
   {
      if(UWorld* World = GetWorld())
      {
         if(ARTSGameMode* GameModeRef = Cast<ARTSGameMode>(World->GetAuthGameMode()))
         {
            GameModeRef->GetQuestManager()->AddNewQuest(questToAdd);
         }
      }
   }
}

void URTSTriggerDisplayDialog::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(AUserInput* UserInput = GetControllerRef())
   {
      UserInput->GetHUDManager()->ShowDialogCustomLines(DialogToDisplay, EDialogBoxCloseCase::finishedTriggerActivation);
   }
}

void URTSTriggerConversationDialog::TriggerEvent_Implementation()
{
}

void URTSTriggerMoveNPC::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(UWorld* World = GetWorld())
   {
      if(ANPC* NPC = UpResourceManager::FindTriggerObjectInWorld(World, npcClass, npcName))
      {
         if(bWalkToDestination)
         {
            if(AAIController* AIController = Cast<AAIController>(NPC->GetController()))
            {
               AIController->MoveToLocation(npcMoveLocation);
            }
         }
         else
         {
            NPC->SetActorLocation(npcMoveLocation);
         }
      }
   }
}

void URTSTriggerDestroyWorldObject::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(UWorld* World = GetWorld())
   {
      if(AActor* ActorToDestroy = UpResourceManager::FindTriggerObjectInWorld(World, actorClass, objectName))
      {
         ActorToDestroy->Destroy(true);
      }
   }
}

void URTSTriggerMoveNPCInLevel::TriggerEvent_Implementation()
{
   if(ANPC* NPC = npc)
   {
      if(bWalkToDestination)
      {
         if(AAIController* AIController = Cast<AAIController>(NPC->GetController()))
         {
            AIController->MoveToLocation(npcMoveLocation);
         }
      }
      else
      {
         NPC->SetActorLocation(npcMoveLocation);
      }
   }
}

void URTSTriggerSetNPCFollow::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(UWorld* World = GetWorld())
   {
      if(ANPC* NPC = UpResourceManager::FindTriggerObjectInWorld(World, npcClass, npcName))
      {
         if(ANPCAIController* npcAIController = Cast<ANPCAIController>(NPC->GetController()))
         {
            if(AUserInput* PC = GetControllerRef())
            {
               if(ABasePlayer* BasePlayer = PC->GetBasePlayer())
               {
                  if(heroToFollowIndex > 0 && heroToFollowIndex < BasePlayer->GetHeroes().Num())
                  {
                     npcAIController->Follow(BasePlayer->GetHeroes()[heroToFollowIndex]);
                  }
                  else
                  {
                     npcAIController->Follow(BasePlayer->GetHeroBlockingInteraction());
                  }
               }
            }
         }
      }
   }
}

void URTSTriggerAddItem::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(itemCount > 0)
   {
      const FMyItem ItemToAdd{FCString::Atoi(*itemToAddRowRef.RowName.ToString()), itemCount};
      if(heroToAddItemToClass)
      {
         if(AUserInput* PC = GetControllerRef())
         {
            if(ABasePlayer* BasePlayer = PC->GetBasePlayer())
            {
               if(const ABaseHero* AffectedHero = UpResourceManager::FindTriggerObjectInWorld<ABaseHero>(GetWorld(), heroToAddItemToClass))
               {
                  AffectedHero->GetBackpack().AddItem(ItemToAdd);
               }
            }
         }
      }
   }
}

void URTSTriggerLearnDialogTopic::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(AUserInput* PC = GetControllerRef())
   {
      if(ABasePlayer* BasePlayer = PC->GetBasePlayer())
      {
         BasePlayer->LearnDialogTopic(dialogTopicToLearn);
      }
   }
}

void URTSPlaySequence::TriggerEvent_Implementation()
{
   Super::TriggerEvent_Implementation();

   if(UWorld* World = GetWorld())
   {
      ALevelSequenceActor*  NewLevelSequnceActor = nullptr;
      ULevelSequencePlayer* SequencePlayer =
          ULevelSequencePlayer::CreateLevelSequencePlayer(World, sequenceToPlay, FMovieSceneSequencePlaybackSettings(), NewLevelSequnceActor);
      SequencePlayer->Play();
   }
}
