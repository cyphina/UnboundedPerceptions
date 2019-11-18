// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameMode.h"
#include "HUDManager.h"
#include "UserInput.h"
#include "RTSPawn.h"
#include "UserWidgets/MainWidget.h"

#include "UserWidgets/ActionbarInterface.h"
#include "Stats/CharacterMenu.h"
#include "Items/EquipmentMenu.h"

#include "Items/Inventory.h"
#include "Items/HeroInventory.h"
#include "Items/StoreInventory.h"
#include "Items/ItemExamineWidget.h"
#include "WorldObjects/ShopNPC.h"

#include "Quests/UI/QuestList.h"
#include "Quests/UI/QuestJournal.h"
#include "Quests/UI/Minimap.h"

#include "DialogSystem/DialogBox.h"
#include "DialogSystem/DialogUI.h"
#include "EventSystem/Trigger.h"

#include "UI/UserWidgets/BreakMenu.h"
#include "UI/UserWidgets/SettingsMenu.h"

#include "UI/UserWidgets/ConfirmationBox.h"
#include "UI/UserWidgets/RTSInputBox.h"

// Sets default values
AHUDManager::AHUDManager()
{
   // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = false;
   widgetReferences.SetNum(HUDCount);
   SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

// Called when the game starts or when spawned
void AHUDManager::BeginPlay()
{
   Super::BeginPlay();
   playerControllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

void AHUDManager::SetWidget(uint8 newState, UMyUserWidget* widgetRef)
{
   widgetReferences[newState] = widgetRef;
}

void AHUDManager::AddHUD(uint8 newState)
{
   if (!bBlocked || currentlyDisplayedWidgetsBitSet[newState] == true) {
      switch (newState) {
         case HUDs::HS_Ingame: ApplyHUD(newState, true, true, true, false); break;
         case HUDs::HS_Inventory: ApplyHUD(newState, true, true, true, false); break;
         case HUDs::HS_Equipment: ApplyHUD(newState, true, true, true, false); break;
         case HUDs::HS_Character: ApplyHUD(newState, true, true, true, false); break;
         case HUDs::HS_QuestJournal: ApplyHUD(newState, true, true, false, false, true); break;
         case HUDs::HS_QuestList: ApplyHUD(newState, true, true, true, true); break;
         case HUDs::HS_Spellbook: ApplyHUD(newState, true, true, true, false); break;
         case HUDs::HS_Dialog:
         case HUDs::HS_Storage:
         case HUDs::HS_Shop_General:
#if UE_EDITOR
            UE_LOG(LogTemp, Warning, TEXT("Don't call AddHUD(uint8) for widgets with parameters.  Call their respective AddHUD"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Don't call AddHUD(uint8) for widgets with parameters.  Call their respective AddHUD"));
#endif
            break;
         case HUDs::HS_Social: ApplyHUD(newState, true, true, false, false); break;
         case HUDs::HS_Break: ApplyHUD(newState, true, true, true, false); break;
         case HUDs::HS_Settings: ApplyHUD(newState, true, true, false, false); break;
         case HUDs::HS_SaveLoad: ApplyHUD(newState, true, true, false, false); break;
         case HUDs::HS_ChatBox: ApplyHUD(newState, true, true, true, false); break;
         default: break;
      }
   }
}

void AHUDManager::AddHUD(FName conversationName, EDialogSource dialogSource)
{
   if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Dialog)] && conversationName != "") // If not on screen
   {
      GetDialogBox()->SetConversation(conversationName);
      GetDialogBox()->SetDialogSource(dialogSource);

      //Social window hides main hud so reopen it
      if(dialogSource == EDialogSource::conversation)
         AddHUD(static_cast<uint8>(HUDs::HS_Ingame));
      ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, true, false);

   } else // if on screen (from being added by a trigger w/o a hero)
   {
      ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, true, false);
   }
}

void AHUDManager::AddHUD(TArray<FDialogData> linesToDisplay, EDialogSource dialogSource)
{
   if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Dialog)] && linesToDisplay.Num() > 0) // If not on screen
   {
      GetDialogBox()->SetDialogLines(linesToDisplay);
      GetDialogBox()->SetDialogSource(dialogSource);
      ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, true, false);
   } else { // If on screen

      ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, true, false);
   }
}

void AHUDManager::AddHUD(UBackpack* backpack)
{
   if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Storage)] && backpack) // If not on screen (impossible state reached if on screen and there's no interactedHero)
   {
      GetStorageHUD()->SetBackPack(backpack);
      ApplyHUD(static_cast<int>(HUDs::HS_Storage), true, true, false, false);
   } else {
      ApplyHUD(static_cast<int>(HUDs::HS_Storage), true, true, false, false);
   }
}

void AHUDManager::AddHUD(AShopNPC* shopNPC)
{
   if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Shop_General)] && shopNPC) // if not on screen
   {
      GetShopHUD()->SetBackPack(shopNPC->itemsToSellBackpack);
      GetShopHUD()->shopkeeper = shopNPC;
      ApplyHUD(static_cast<int>(HUDs::HS_Shop_General), true, true, true, false);
   } else { // if on screen
      ApplyHUD(static_cast<int>(HUDs::HS_Shop_General), true, true, true, false);
   }
}

void AHUDManager::AddItemExamineHUD(int itemID)
{
   if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_ExamineMenu)]) // if not on screen
   {
      if (itemID > 0) {
         GetExamineMenu()->itemToDisplayID = itemID;
         ApplyHUD(static_cast<int>(HUDs::HS_ExamineMenu), true, true, true, false, true);
      }
   } else {
      ApplyHUD(static_cast<int>(HUDs::HS_ExamineMenu), true, true, true, false, true);
   }
}

void AHUDManager::AddHUDConfirm(FName funcName, UObject* funcObject, FText newTitle, FText newDesc)
{
   if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Confirmation)]) {
      if (funcObject) {
         GetConfirmationBox()->onConfirmationMade.BindUFunction(funcObject, funcName);
         GetConfirmationBox()->SetTitle(newTitle);
         GetConfirmationBox()->SetDesc(newDesc);
         ApplyHUD(static_cast<int>(HUDs::HS_Confirmation), true, true, true, false, true);
      }
   } else {
      ApplyHUD(static_cast<int>(HUDs::HS_Confirmation), true, true, true, false, true);
   }
}

void AHUDManager::AddHUDInput(FName funcName, UObject* funcObject, FText newTitle, FText newDesc)
{
   if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_InputBox)]) {
      if (funcObject) {
         GetInputBox()->onInputConfirmed.BindUFunction(funcObject, funcName);
         GetInputBox()->SetTitle(newTitle);
         GetInputBox()->SetDesc(newDesc);
         GetInputBox()->AddToViewport(2);
         ApplyHUD(static_cast<int>(HUDs::HS_InputBox), true, true, true, false, true);
      }
   } else {
      ApplyHUD(static_cast<int>(HUDs::HS_InputBox), true, true, false, false, true);
   }
}

bool AHUDManager::ApplyHUD(uint8 newState, bool bShowMouseCursor, bool bEnableClickEvents, bool canOpenCombat, bool hasAnim, bool bBlocking)
{
   check(newState >= 0 && newState < widgetReferences.Num());
   UMyUserWidget* widgetToApply = widgetReferences[newState];

   // if that's not a valid widget or else it isn't set up properly
#if UE_EDITOR
   if (!widgetToApply) {
      UE_LOG(LogTemp, Warning, TEXT("Invalid Widget Reference being added or removed"))
      return false;
   }
#endif

   // Nullcheck whatever widget we're trying to apply
   if (currentlyDisplayedWidgetsBitSet[newState]) // if our widget is already on screen, we probably pressed button to take it off
   {
      if (hasAnim) // does our widget have animation?
      {
         UWidgetAnimation* anim = Cast<UAnimHudWidget>(widgetToApply)->anim;
         if (widgetToApply->IsAnimationPlaying(anim)) // if animation is currently playing, it's opening
         {
            widgetToApply->ReverseAnimation(anim);
         } else {
            widgetToApply->PlayAnimation(anim, 0, 1, EUMGSequencePlayMode::Reverse, 1);
         }
         // animation will be in charge of removing from parent

      } else // no animation
      {
         widgetToApply->SetVisibility(ESlateVisibility::Hidden); // hidden prevents tick updates
      }
   } else // else our widget isn't already on the screen
   {
      if (hasAnim) // does our widget have animation?
      {
         UWidgetAnimation* anim = Cast<UAnimHudWidget>(widgetToApply)->anim;
         if (widgetToApply->IsAnimationPlaying(anim)) {
            widgetToApply->ReverseAnimation(anim);
         } else {
            if (!widgetToApply->OnWidgetAddToViewport()) // if preconditions not set, don't make it visible
               return false;
            widgetToApply->SetVisibility(ESlateVisibility::SelfHitTestInvisible); // since it's offscreen, add visibility first
            widgetToApply->PlayAnimation(anim, 0, 1, EUMGSequencePlayMode::Forward, 1);
         }
      } else // no animation
      {
         if (!widgetToApply->OnWidgetAddToViewport()) // if preconditions not set, don't make it visible
            return false;
         widgetToApply->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
      }
   }

   if(bBlocking)
      bBlocked = !bBlocked;

   UpdateWidgetTracking(newState, bShowMouseCursor, bEnableClickEvents, canOpenCombat);
   return true;
}

void AHUDManager::UpdateWidgetTracking(int updateIndex, bool showMouseCursor, bool enableClickEvents, bool canOpenCombat)
{
   currentlyDisplayedWidgetsBitSet[updateIndex] = !currentlyDisplayedWidgetsBitSet[updateIndex];
   // Should this hud remove the cursor?
   if (showMouseCursor) {
      currentlyDisplayedWidgetsBitSet[updateIndex] ? ++showMouseCursorCount : --showMouseCursorCount;
      showMouseCursorCount > 0 ? playerControllerRef->bShowMouseCursor = true : playerControllerRef->bShowMouseCursor = false;
   }
   // If this hud allows for clicking outside of the hud
   if (enableClickEvents) {
      currentlyDisplayedWidgetsBitSet[updateIndex] ? ++enableClickEventsCount : --enableClickEventsCount;
      enableClickEventsCount > 0 ? playerControllerRef->bEnableClickEvents = true : playerControllerRef->bEnableClickEvents = false;
   }
   // If this hud can't be opened during combat, then we should change the cursor to the UI cursor (restricting them from doing much) and prevent camera panning
   if (!canOpenCombat) {
      currentlyDisplayedWidgetsBitSet[updateIndex] ? ++numWidgetsBlocking : --numWidgetsBlocking;
      if(numWidgetsBlocking > 0) {
         playerControllerRef->GetCameraPawn()->isCamNavDisabled = true;
         playerControllerRef->GetCameraPawn()->DisableInput(playerControllerRef);
         playerControllerRef->GetCameraPawn()->SetActorTickEnabled(false);
         playerControllerRef->GetCameraPawn()->ChangeCursor(ECursorStateEnum::UI);
      }
      else {
         playerControllerRef->GetCameraPawn()->isCamNavDisabled = false;
         playerControllerRef->GetCameraPawn()->EnableInput(playerControllerRef);
         playerControllerRef->GetCameraPawn()->SetActorTickEnabled(true);
      }
   }
}

 UMainWidget* AHUDManager::GetMainHUD()
{
   return Cast<UMainWidget>(widgetReferences[static_cast<int>(HUDs::HS_Ingame)]);
}

 UCharacterMenu* AHUDManager::GetCharacterHUD()
{
   return Cast<UCharacterMenu>(widgetReferences[static_cast<int>(HUDs::HS_Character)]);
}

 UEquipmentMenu* AHUDManager::GetEquipHUD() const
{
   return Cast<UEquipmentMenu>(widgetReferences[static_cast<int>(HUDs::HS_Equipment)]);
}

 UHeroInventory* AHUDManager::GetInventoryHUD() const
{
   return Cast<UHeroInventory>(widgetReferences[static_cast<int>(HUDs::HS_Inventory)]);
}

 UStoreInventory* AHUDManager::GetShopHUD() const
{
   return Cast<UStoreInventory>(widgetReferences[static_cast<int>(HUDs::HS_Shop_General)]);
}

 UInventory* AHUDManager::GetStorageHUD() const
{
   return Cast<UInventory>(widgetReferences[static_cast<int>(HUDs::HS_Storage)]);
}

 UActionbarInterface* AHUDManager::GetActionHUD() const
{
   return Cast<UActionbarInterface>(widgetReferences[static_cast<int>(HUDs::HS_Actionbar)]);
}

 UQuestList* AHUDManager::GetQuestList() const
{
   return Cast<UQuestList>(widgetReferences[static_cast<int>(HUDs::HS_QuestList)]);
}

 UQuestJournal* AHUDManager::GetQuestJournal() const
{
   return Cast<UQuestJournal>(widgetReferences[static_cast<int>(HUDs::HS_QuestJournal)]);
}

 UMinimap* AHUDManager::GetMinimap() const
{
   return Cast<UMinimap>(widgetReferences[static_cast<int>(HUDs::HS_Minimap)]);
}

 UDialogBox* AHUDManager::GetDialogBox() const
{
   return Cast<UDialogBox>(widgetReferences[static_cast<int>(HUDs::HS_Dialog)]);
}

 UDialogUI* AHUDManager::GetSocialWindow() const
{
   return Cast<UDialogUI>(widgetReferences[static_cast<int>(HUDs::HS_Social)]);
}

 UBreakMenu* AHUDManager::GetBreakMenu() const
{
   return Cast<UBreakMenu>(widgetReferences[static_cast<int>(HUDs::HS_Break)]);
}

 USettingsMenu* AHUDManager::GetSettingsMenu() const
{
   return Cast<USettingsMenu>(widgetReferences[static_cast<int>(HUDs::HS_Settings)]);
}

 UItemExamineWidget* AHUDManager::GetExamineMenu() const
{
   return Cast<UItemExamineWidget>(widgetReferences[static_cast<int>(HUDs::HS_ExamineMenu)]);
}

 UConfirmationBox* AHUDManager::GetConfirmationBox() const
{
   return Cast<UConfirmationBox>(widgetReferences[static_cast<int>(HUDs::HS_Confirmation)]);
}

 URTSInputBox* AHUDManager::GetInputBox() const
{
   return Cast<URTSInputBox>(widgetReferences[static_cast<int>(HUDs::HS_InputBox)]);
}
