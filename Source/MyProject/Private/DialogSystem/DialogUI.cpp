// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogUI.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/NPC.h"
#include "AIStuff/AIControllers/NPCAIController.h"
#include "UI/HUDManager.h"

void UDialogUI::NativeConstruct()
{
   Super::NativeConstruct();
}

void UDialogUI::SetMainView()
{
   switch (socialHUDState) {
      case ESocialHUDState::Conversation: SetConversationView(); break; // Show social menu and enable talk buttons
      case ESocialHUDState::Intimate: SetIntimateView(); break; // Show social menu and enable gift buttons
      case ESocialHUDState::Shop: SetShopView(); break; //Show social menu and enable shop buttons 
   }
}

bool UDialogUI::OnWidgetAddToViewport_Implementation()
{
   // change view to whatever it is set to prior
   // SetMainView();
   return true;
}

void UDialogUI::Leave()
{
   CPC->GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Social));
   CPC->GetBasePlayer()->interactedHero = nullptr;
   // A movement will reactivate patrolling if there is any
   ANPCAIController* npcControllerRef = Cast<ANPCAIController>(npcRef->GetController());
   if (npcControllerRef) npcControllerRef->MoveToLocation(npcRef->GetActorLocation());
}