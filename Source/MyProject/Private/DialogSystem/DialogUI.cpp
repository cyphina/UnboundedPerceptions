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
   socialHUDState = ESocialHUDState::Conversation;
   HideOrShowAllButtons();
   btnTalk->SetVisibility(ESlateVisibility::Visible);
   btnLeave->SetVisibility(ESlateVisibility::Visible);
}

void UDialogUI::SetIntimateView()
{
   socialHUDState = ESocialHUDState::Intimate;
   HideOrShowAllButtons(false);
   btnShop->SetVisibility(ESlateVisibility::Collapsed);
}

void UDialogUI::SetShopView()
{
   socialHUDState = ESocialHUDState::Shop;
   HideOrShowAllButtons(false);

}

void UDialogUI::SetMainView()
{
   switch(socialHUDState) {
      case ESocialHUDState::Conversation: SetConversationView(); break; // Show social menu and enable talk buttons
      case ESocialHUDState::Intimate: SetIntimateView(); break;         // Show social menu and enable gift buttons
      case ESocialHUDState::Shop: SetShopView(); break;                 //Show social menu and enable shop buttons
   }
   dialogWheel->SetVisibility(ESlateVisibility::Collapsed);
}

void UDialogUI::HideOrShowAllButtons(bool bHide) const
{
   ESlateVisibility visibilitySettingForButtons = bHide ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
   // Iterate through properties to find buttons
   btnGift->SetVisibility(visibilitySettingForButtons);
   btnLeave->SetVisibility(visibilitySettingForButtons);
   btnSharedPsychosis->SetVisibility(visibilitySettingForButtons);
   btnShop->SetVisibility(visibilitySettingForButtons);
   btnTalk->SetVisibility(visibilitySettingForButtons);
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

   // Hide the ingame widget so we just see social options
   if(hudManagerRef->IsWidgetOnScreen(HUDs::HS_Actionbar))
      hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Actionbar));

   dialogWheel->OnWidgetAddToViewport();

   return true;
}

void UDialogUI::Leave()
{
   hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Social));
   if(!hudManagerRef->IsWidgetOnScreen(HUDs::HS_Actionbar))
      hudManagerRef->AddHUD(static_cast<uint8>(HUDs::HS_Actionbar));
   // Reactivate patrolling and remove global reference to current blocking interactable
   npcRef->OnDoneTalking();
}