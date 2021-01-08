// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "NPC.h"
#include "BaseHero.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "RTSGameState.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogBox.h"
#include "DialogSystem/NPCSocialMenu.h"
#include "Quests/QuestManager.h"
#include "LevelSaveStructs.h"
#include "RTSIngameWidget.h"
#include "AIStuff/AIControllers/NPCAIController.h"
#include "AIModule/Classes/BrainComponent.h"

AHUDManager* ANPC::hudManagerRef = nullptr;

// Sets default values
ANPC::ANPC()
{
   PrimaryActorTick.bCanEverTick      = true;
   onDialogEndTriggerData.triggerType = ETriggerType::OpenHUDTrigger;
   onDialogEndTriggerData.enabled     = true;

   TArray<FString> triggerValues;
   triggerValues.Add(FString::FromInt(static_cast<uint8>(EHUDs::HS_Social)));
   onDialogEndTriggerData.triggerValues = triggerValues;
   onDialogEndTriggerData.numCalls      = -1;

   GetCapsuleComponent()->SetCollisionProfileName("NPC");
   GetMesh()->SetCollisionProfileName("NoCollision");
   auto arrowComp = FindComponentByClass<UArrowComponent>();
   if(arrowComp)
      arrowComp->DestroyComponent();

   GetMesh()->SetRenderCustomDepth(false);
   GetMesh()->CustomDepthStencilValue = 255;

   AIControllerClass = ANPCAIController::StaticClass();
}

void ANPC::BeginPlay()
{
   Super::BeginPlay();
   controllerRef   = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   npcAIController = Cast<ANPCAIController>(GetController());

#if UE_EDITOR
   // Figure out how many quests this NPC can give.  Delay so we can test individual levels without this failing
   GetWorld()->GetTimerManager().SetTimer(BeginPlayDelayTimer, this, &ANPC::CountQuestDialogs, 2.f, false);
#endif

   // Callback to help maintain quest count even when new topics are learned
   Cast<ABasePlayer>(controllerRef->PlayerState)->OnDialogLearned.AddDynamic(this, &ANPC::OnTopicLearned);

   // Load up all triggers that tried to activate on this object but this object wasn't loaded at the time
   TArray<FTriggerData> savedTriggers;
   Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->GetTriggerManager()->GetTriggerRecords().MultiFind(*GetGameName().ToString(), savedTriggers);

   for(FTriggerData& finishedTriggerActivation : savedTriggers) {
      controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
   }
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}

void ANPC::SetSpecificDialog(FGameplayTag topic, FName newConversationName)
{
   // If our this conversationTopic was a quest giving dialog, decrease the available quest count because its being removed
   FName oldConversationName = GetConversationName(topic);

   if(IsQuestDialog(oldConversationName) && IsTopicLearned(topic))
      --numAvailableQuests;
   // If this conversationTopic is a questGiving dialog, increase the quest count because it's the new conversation for this topic
   if(IsQuestDialog(newConversationName) && IsTopicLearned(topic))
      ++numAvailableQuests;
   conversationTopics[topic] = newConversationName;
}

FName ANPC::GetConversationName(FGameplayTag conversationTopic) const
{
   // does this NPC know anything about the topic?
   if(conversationTopics.Contains(conversationTopic)) {
      return conversationTopics[conversationTopic];
   } else // if no, return some default reply like "I'm not sure what that is sire!"
   {
      check(defaultResponseName != "");
      return defaultResponseName;
   }
}

TArray<FGameplayTag> ANPC::GetConversationTopics() const
{
   TArray<FGameplayTag> keys;
   conversationTopics.GetKeys(keys);
   return keys;
}

void ANPC::Interact_Implementation(ABaseHero* hero)
{
   // Check to see there is a brain component (behavior tree component stored in brain component base class field)
   if(npcAIController->GetBrainComponent())
      npcAIController->GetBrainComponent()->StopLogic("Talking To Hero");

   GetController()->StopMovement();

   // Turn towards our NPC
   FVector currentLocation    = GetActorLocation();
   FVector difference         = hero->GetActorLocation() - currentLocation;
   FVector projectedDirection = FVector(difference.X, difference.Y, 0);
   SetActorRotation(FRotationMatrix::MakeFromX(FVector(projectedDirection)).Rotator());

   // If this npc wants to converse, we go to another screen after the initial conversation where we can interact more
   if(Execute_CanInteract(this)) {
      hudManagerRef->GetIngameHUD()->GetSocialWindow()->SetNPC(this);
      SetupAppropriateView();
      // If they have a conversation starter
      if(conversationStarterName != "") {
         hudManagerRef->ShowDialogWithSource(conversationStarterName, EDialogBoxCloseCase::finishedInitialTalkNPC);
      } else {
         // If there was no entry for conversationStarterName, just display a default one
         hudManagerRef->ShowDialogCustomLines(TArray<FDialogData>{FDialogData({}, NSLOCTEXT("NPCDialog", "Default", "This person is silent..."), FName())},
                                              EDialogBoxCloseCase::finishedInitialTalkNPC);
      }
   }

   // Check to see if any quests wanted us to talk to this NPC
   controllerRef->GetGameMode()->GetQuestManager()->OnTalkNPC(this, FGameplayTag());
   controllerRef->GetBasePlayer()->heroInBlockingInteraction = hero;
   AddConversedDialog(conversationStarterName);
}

void ANPC::OnDoneInitialTalk()
{
   // If the NPC doesn't want to talk after the initial dialog
   if(!bWantsToConverse) {
      if(npcAIController->GetBrainComponent())
         npcAIController->GetBrainComponent()->RestartLogic();
      controllerRef->GetBasePlayer()->heroInBlockingInteraction = nullptr;
   } else {
      // If NPC does want to talk, open up the conversation screen
      hudManagerRef->AddHUD(static_cast<uint8>(EHUDs::HS_Social));
   }
}

void ANPC::OnDoneTalking()
{
   // Resume AI of NPC if it has a behavior tree active
   if(npcAIController->GetBrainComponent())
      npcAIController->GetBrainComponent()->RestartLogic();
   controllerRef->GetBasePlayer()->heroInBlockingInteraction = nullptr;
}

void ANPC::SetupAppropriateView()
{
   hudManagerRef->GetIngameHUD()->GetSocialWindow()->SetConversationView();
}

FVector ANPC::GetInteractableLocation_Implementation() const
{
   return GetActorLocation();
}

bool ANPC::CanInteract_Implementation() const
{
   return !controllerRef->GetBasePlayer()->heroInBlockingInteraction;
}

void ANPC::MakeNPCData(FNPCSaveInfo& npcSaveInfo)
{
   npcSaveInfo.name                    = GetGameName();
   npcSaveInfo.transform               = GetTransform();
   npcSaveInfo.bWantsToConverse        = GetWantsToConverse();
   npcSaveInfo.conversationStarterName = GetStartingConversationName();
   npcSaveInfo.defaultResponseName     = GetDefaultResponseName();

   npcSaveInfo.conversationTopicKeys = GetConversationTopics();
   for(FGameplayTag key : npcSaveInfo.conversationTopicKeys) {
      npcSaveInfo.conversationTopicValues.Add(GetConversationName(key));
   }

   npcSaveInfo.previousConversations = dialogAlreadyConversed;
}

void ANPC::ReloadNPCData(const FNPCSaveInfo& npcSaveInfo)
{
   SetActorTransform(npcSaveInfo.transform);
   bWantsToConverse        = npcSaveInfo.bWantsToConverse;
   conversationStarterName = npcSaveInfo.conversationStarterName;
   defaultResponseName     = npcSaveInfo.defaultResponseName;

   for(int i = 0; i < npcSaveInfo.conversationTopicKeys.Num(); ++i) {
      conversationTopics.Add(npcSaveInfo.conversationTopicKeys[i], npcSaveInfo.conversationTopicValues[i]);
   }

   dialogAlreadyConversed = npcSaveInfo.previousConversations;
}

void ANPC::SaveNPCData(FMapSaveInfo& mapInfo)
{
   FNPCSaveInfo npcSaveInfo;
   MakeNPCData(npcSaveInfo);
   mapInfo.npcsInfo.Add(npcSaveInfo);
}

void ANPC::LoadNPCData(FMapSaveInfo& mapInfo)
{
   auto npcSaveInfo = mapInfo.npcsInfo.FindByHash<ANPC*>(GetTypeHash(*this), this);
   if(npcSaveInfo)
      ReloadNPCData(*npcSaveInfo);
}

void ANPC::CountQuestDialogs()
{
   // Check if default conversation is a quest
   FName conversationName = defaultResponseName;
   if(conversationName.ToString().StartsWith("Quest"))
      ++numAvailableQuests;

   // Loop through all of our dialogTopics we know
   TArray<FGameplayTag> topics;

   // if this NPC wants to converse and we can talk to him/her about more than one topic
   if(bWantsToConverse && conversationTopics.GetKeys(topics) > 0) {
      for(FGameplayTag topic : topics) {
         if(IsTopicLearned(topic)) {
            conversationName = GetConversationName(topic);

            if(IsQuestDialog(conversationName))
               ++numAvailableQuests;
         }
      }
   }
}

bool ANPC::IsQuestDialog(FName conversationName) const
{
   return conversationName.ToString().StartsWith("Quest") ? true : false;
}

bool ANPC::IsTopicLearned(FGameplayTag topic) const
{
   return controllerRef->GetBasePlayer()->HasDialogTopic(topic);
}

void ANPC::OnTopicLearned(FGameplayTag topicLearned)
{
   if(IsQuestDialog(GetConversationName(topicLearned))) {
      ++numAvailableQuests;
   }
}
