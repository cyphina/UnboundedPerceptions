// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "DialogUI.h"

#include "DialogWheel.h"
#include "WorldObjects/NPC.h"
#include "UMG/Public/Components/Button.h"
#include "UI/HUDManager.h"
#include "Algo/RemoveIf.h"

void UDialogUI::NativeConstruct()
{
   Super::NativeConstruct();
   btnTalk->OnClicked.AddDynamic(this, &UDialogUI::Talk);
   btnGift->OnClicked.AddDynamic(this, &UDialogUI::Gift);
   btnSharedPsychosis->OnClicked.AddDynamic(this, &UDialogUI::Psychosis);
   btnShop->OnClicked.AddDynamic(this, &UDialogUI::Shop);
   btnLeave->OnClicked.AddDynamic(this, &UDialogUI::Leave);
}

void UDialogUI::SetConversationView()
{
   socialHUDState = ESocialHUDState::Intimate;
   HideOrShowAllButtons();
   btnTalk->SetVisibility(ESlateVisibility::Visible);
   btnLeave->SetVisibility(ESlateVisibility::Visible);
}

void UDialogUI::SetIntimateView()
{
   socialHUDState = ESocialHUDState::Conversation;
   HideOrShowAllButtons(false);
   btnShop->SetVisibility(ESlateVisibility::Collapsed);
   dialogWheel->SetVisibility(ESlateVisibility::Collapsed);
}

void UDialogUI::SetShopView()
{
   socialHUDState = ESocialHUDState::Shop;
   HideOrShowAllButtons(false);
   dialogWheel->SetVisibility(ESlateVisibility::Collapsed);
}

void UDialogUI::SetMainView()
{
   switch(socialHUDState) {
      case ESocialHUDState::Conversation: SetConversationView(); break; // Show social menu and enable talk buttons
      case ESocialHUDState::Intimate: SetIntimateView(); break;         // Show social menu and enable gift buttons
      case ESocialHUDState::Shop: SetShopView(); break;                 //Show social menu and enable shop buttons
   }
}

void UDialogUI::HideOrShowAllButtons(bool bHide) const
{
   const ESlateVisibility visibilitySettingForButtons = bHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
   // Iterate through properties to find buttons
   TArray<UProperty*, TFixedAllocator<NUM_BUTTONS>> buttons;
   for(TFieldIterator<UProperty> propIt(GetClass()); propIt; ++propIt) {
      UProperty* prop = *propIt;
      if(prop->GetFName().ToString().StartsWith("btn")) {
         buttons.Emplace(prop);
      }
   }

   for(auto& button : buttons) {
      Cast<UButton>(button)->SetVisibility(visibilitySettingForButtons);
   }
}

void UDialogUI::Talk()
{
   HideOrShowAllButtons();
   dialogWheel->SetVisibility(ESlateVisibility::Visible);
}

void UDialogUI::Gift()
{
   HideOrShowAllButtons();
   //TODO: Open up hero inventory
}

void UDialogUI::Psychosis()
{
   HideOrShowAllButtons();
   //TODO: Initiate the dream world menu
}

void UDialogUI::Shop()
{
   hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Shop_General));
}

bool UDialogUI::OnWidgetAddToViewport_Implementation()
{
   // change view to whatever it is set to prior
   // SetMainView();
   return true;
}

void UDialogUI::Leave()
{
   hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Social));
   hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Ingame));
   // Reactivate patrolling and remove global reference to current blocking interactable
   npcRef->OnDoneTalking();
}