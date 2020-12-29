// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "NPCSocialMenu.h"

#include "DialogWheel.h"
#include "WorldObjects/NPC.h"
#include "UMG/Public/Components/Button.h"
#include "UI/HUDManager.h"
#include "Algo/RemoveIf.h"

void UNPCSocialMenu::NativeConstruct()
{
   Super::NativeConstruct();
   btnTalk->OnClicked.AddDynamic(this, &UNPCSocialMenu::Talk);
   btnGift->OnClicked.AddDynamic(this, &UNPCSocialMenu::Gift);
   btnSharedPsychosis->OnClicked.AddDynamic(this, &UNPCSocialMenu::Psychosis);
   btnShop->OnClicked.AddDynamic(this, &UNPCSocialMenu::Shop);
   btnLeave->OnClicked.AddDynamic(this, &UNPCSocialMenu::Leave);
}

void UNPCSocialMenu::SetConversationView()
{
   socialHUDState = ESocialHUDState::Conversation;
   HideOrShowAllButtons();
   btnTalk->SetVisibility(ESlateVisibility::Visible);
   btnLeave->SetVisibility(ESlateVisibility::Visible);
}

void UNPCSocialMenu::SetIntimateView()
{
   socialHUDState = ESocialHUDState::Intimate;
   HideOrShowAllButtons(false);
   btnShop->SetVisibility(ESlateVisibility::Collapsed);
}

void UNPCSocialMenu::SetShopView()
{
   socialHUDState = ESocialHUDState::Shop;
   HideOrShowAllButtons(false);

}

void UNPCSocialMenu::SetMainView()
{
   switch(socialHUDState) {
      case ESocialHUDState::Conversation: SetConversationView(); break; // Show social menu and enable talk buttons
      case ESocialHUDState::Intimate: SetIntimateView(); break;         // Show social menu and enable gift buttons
      case ESocialHUDState::Shop: SetShopView(); break;                 //Show social menu and enable shop buttons
   }
   dialogWheel->SetVisibility(ESlateVisibility::Collapsed);
}

void UNPCSocialMenu::HideOrShowAllButtons(bool bHide) const
{
   ESlateVisibility visibilitySettingForButtons = bHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
   // Iterate through properties to find buttons
   btnGift->SetVisibility(visibilitySettingForButtons);
   btnLeave->SetVisibility(visibilitySettingForButtons);
   btnSharedPsychosis->SetVisibility(visibilitySettingForButtons);
   btnShop->SetVisibility(visibilitySettingForButtons);
   btnTalk->SetVisibility(visibilitySettingForButtons);
}

void UNPCSocialMenu::Talk()
{
   HideOrShowAllButtons();
   dialogWheel->SetVisibility(ESlateVisibility::Visible);
}

void UNPCSocialMenu::Gift()
{
   HideOrShowAllButtons();
   //TODO: Open up hero inventory
}

void UNPCSocialMenu::Psychosis()
{
   HideOrShowAllButtons();
   //TODO: Initiate the dream world menu
}

void UNPCSocialMenu::Shop()
{
   hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Shop_General));
}

bool UNPCSocialMenu::OnWidgetAddToViewport_Implementation()
{
   // change view to whatever it is set to prior
   // SetMainView();

   // Hide the ingame widget so we just see social options
   if(hudManagerRef->IsWidgetOnScreen(HUDs::HS_Actionbar))
      hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Actionbar));

   dialogWheel->OnWidgetAddToViewport();

   return true;
}

void UNPCSocialMenu::Leave()
{
   hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Social));
   if(!hudManagerRef->IsWidgetOnScreen(HUDs::HS_Actionbar))
      hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Actionbar));
   // Reactivate patrolling and remove global reference to current blocking interactable
   npcRef->OnDoneTalking();
}