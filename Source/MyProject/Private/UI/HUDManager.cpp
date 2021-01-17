// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameMode.h"
#include "HUDManager.h"

#include "ActionbarInterface.h"
#include "UserInput.h"
#include "RTSGameState.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "SpellBook.h"
#include "StorageContainer.h"
#include "UserWidgets/MainWidget.h"

#include "Actionbar/ActionbarInterface.h"
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
#include "DialogSystem/NPCSocialMenu.h"
#include "DialogSystem/DialogWheel.h"
#include "EventSystem/Trigger.h"

#include "SpellSystem/SpellbookHUD.h"

#include "UI/UserWidgets/BreakMenu.h"
#include "UI/UserWidgets/SettingsMenu.h"

#include "UI/UserWidgets/ConfirmationBox.h"
#include "UI/UserWidgets/RTSInputBox.h"

#include "UI/UserWidgets/StartMenu.h"

#include "Minigames/MinigameManager.h"
#include "Quests/QuestManager.h"

TBitArray<FDefaultBitArrayAllocator> AHUDManager::currentlyDisplayedWidgetsBitSet = TBitArray<FDefaultBitArrayAllocator>(false, HUDCount);

AHUDManager::AHUDManager() : Super()
{
   PrimaryActorTick.bCanEverTick = false;
   widgetReferences.SetNum(HUDCount);
   SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AHUDManager::BeginPlay()
{
   Super::BeginPlay();

   playerControllerRef = Cast<AUserInput>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController());
   if(!ensure(playerControllerRef != nullptr)) return;

   gameMode = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   if(!ensure(gameMode != nullptr)) return;

   CreateMainWidget();

   startMenu = CreateWidget<UStartMenu>(playerControllerRef, startMenuClass, "Start Menu");
   InjectDependency(startMenu);

   // For some reason the hardware cursor doesn't show in a packaged build, so we need to do what was written here:
   // https://forums.unrealengine.com/development-discussion/blueprint-visual-scripting/1700190-custom-hardware-cursor-does-odd-thing
   // In ProjectSettings->Input change Default Viewport Capture Mode to Capture Permanently, and Default Viewport Lock mode to Do not Lock
   // The cursor starts at the top left of the screen even on windowed mode which can be jank, but it fixes the problem!
   FInputModeGameAndUI inputModeData;
   inputModeData.SetWidgetToFocus(mainWidget->TakeWidget());
   inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
   inputModeData.SetHideCursorDuringCapture(false); // This game uses the cursor
   playerControllerRef->SetInputMode(inputModeData);
   playerControllerRef->bShowMouseCursor = true;

   InjectDependentClasses();

   SetupWidgetReferences();
   
   ANPC::SetHUDManagerRef(this);
}

void AHUDManager::SetWidget(UMyUserWidget* widgetRef, EHUDs newState)
{
   widgetReferences[static_cast<uint8>(newState)] = widgetRef;
}

void AHUDManager::SetupWidgetReferences()
{
   SetWidget(mainWidget->GetIngameWidget(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetBreakMenu(), EHUDs::HS_Break);
   SetWidget(mainWidget->GetSettingsMenu(), EHUDs::HS_Settings);
   SetWidget(mainWidget->GetIngameWidget()->GetCharacterHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetInventoryHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetEquipHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetQuestList(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetQuestJournal(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetMinimap(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetActionHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetSocialWindow(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetDialogBox(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetSpellBookMenu(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetStorageHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetShopHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetExamineMenu(), EHUDs::HS_Ingame);
   //SetWidget(mainWidget->GetIngameWidget()->Chat(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetShopHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetShopHUD(), EHUDs::HS_Ingame);
   SetWidget(mainWidget->GetIngameWidget()->GetShopHUD(), EHUDs::HS_Ingame);
}

void AHUDManager::AddHUD(uint8 newState)
{
   if(!bBlocked || currentlyDisplayedWidgetsBitSet[newState] == true) {
      switch(newState) {
         case EHUDs::HS_Ingame: ApplyHUD(newState, true, true, false); break;
         case EHUDs::HS_Inventory: ApplyHUD(newState, true, true, false); break;
         case EHUDs::HS_Equipment: ApplyHUD(newState, true, true, false); break;
         case EHUDs::HS_Character: ApplyHUD(newState, true, false, true); break;
         case EHUDs::HS_QuestJournal: ApplyHUD(newState, true, false, true); break;
         case EHUDs::HS_QuestList: ApplyHUD(newState, true, true, false); break;
         case EHUDs::HS_Spellbook: ApplyHUD(newState, true, true, false); break;
         case EHUDs::HS_Shop_General: ApplyHUD(static_cast<int>(EHUDs::HS_Shop_General), true, false, false); break;
         case EHUDs::HS_Storage: ApplyHUD(newState, true, true, false); break;
         case EHUDs::HS_Dialog:
         case EHUDs::HS_Confirmation:
         case EHUDs::HS_InputBox: {
            UE_LOG(LogTemp, Verbose, TEXT("Don't call AddHUD(uint8) for widgets with parameters.  Call their respective AddHUD"));
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Don't call AddHUD(uint8) for widgets with parameters.  Call their respective AddHUD"));
            ensure(false);
            return;
            break;
         }
         case EHUDs::HS_ExamineMenu: ApplyHUD(newState, true, false, false);
         case EHUDs::HS_Social: ApplyHUD(newState, true, false, false); break;
         case EHUDs::HS_Break: ApplyHUD(newState, true, false, false); break;
         case EHUDs::HS_Settings: ApplyHUD(newState, true, false, true); break;
         case EHUDs::HS_SaveLoad: ApplyHUD(newState, true, false, true); break;
         case EHUDs::HS_ChatBox: ApplyHUD(newState, true, true, false); break;
         case EHUDs::HS_KeyMap: ApplyHUD(newState, true, false, false); break;
         default: break;
      }
   }
}

void AHUDManager::ShowDialogWithSource(FName conversationName, EDialogBoxCloseCase dialogSource)
{
   if(!currentlyDisplayedWidgetsBitSet[static_cast<int>(EHUDs::HS_Dialog)] && conversationName != "") {
      GetIngameHUD()->GetDialogBox()->SetConversation(conversationName);
      GetIngameHUD()->GetDialogBox()->SetDialogSource(dialogSource);
   }
   ApplyHUD(static_cast<int>(EHUDs::HS_Dialog), true, true, false);
}

void AHUDManager::ShowDialogCustomLines(TArray<FDialogData> linesToDisplay, EDialogBoxCloseCase dialogSource)
{
   if(!currentlyDisplayedWidgetsBitSet[static_cast<int>(EHUDs::HS_Dialog)] && linesToDisplay.Num() > 0) {
      GetIngameHUD()->GetDialogBox()->SetDialogLines(linesToDisplay);
      GetIngameHUD()->GetDialogBox()->SetDialogSource(dialogSource);
      ApplyHUD(static_cast<int>(EHUDs::HS_Dialog), true, true, false);
   }
}

void AHUDManager::ShowConfirmationBox(FName funcName, UObject* funcObject, FText newTitle, FText newDesc)
{
   if(!currentlyDisplayedWidgetsBitSet[static_cast<int>(EHUDs::HS_Confirmation)]) {
      if(funcObject) {
         GetConfirmationBox()->OnConfirmationMade().BindUFunction(funcObject, funcName);
         GetConfirmationBox()->SetTitle(newTitle);
         GetConfirmationBox()->SetDesc(newDesc);
         ApplyHUD(static_cast<int>(EHUDs::HS_Confirmation), true, false, false);
      }
   } else {
      ApplyHUD(static_cast<int>(EHUDs::HS_Confirmation), true, false, false);
   }
}

void AHUDManager::ShowInputBox(FName funcName, UObject* funcObject, FText newTitle, FText newDesc)
{
   if(!currentlyDisplayedWidgetsBitSet[static_cast<int>(EHUDs::HS_InputBox)]) {
      if(funcObject) {
         GetInputBox()->OnInputConfirmed().BindUFunction(funcObject, funcName);
         GetInputBox()->SetTitle(newTitle);
         GetInputBox()->SetDesc(newDesc);
         ApplyHUD(static_cast<int>(EHUDs::HS_InputBox), true, false, false);
      }
   } else {
      ApplyHUD(static_cast<int>(EHUDs::HS_InputBox), true, false, false);
   }
}

bool AHUDManager::ApplyHUD(uint8 newState, bool bEnableClickEvents, bool canOpenCombat, bool bBlocking)
{
   check(newState >= 0 && newState < widgetReferences.Num());
   UMyUserWidget* widgetToApply = widgetReferences[newState];

   if(!widgetToApply) {
      UE_LOG(LogTemp, Error, TEXT("Invalid Widget Reference being added or removed"))
      return false;
   }

   if(currentlyDisplayedWidgetsBitSet[newState]) // if our widget is already on screen, we probably pressed button to take it off
   {
      HideWidgetOnScreen(widgetToApply);
   } else {
      if(!ShowHiddenWidget(widgetToApply)) return false;
   }

   if(bBlocking) bBlocked = !bBlocked;

   UpdateWidgetTracking(newState, bEnableClickEvents, canOpenCombat);
   return true;
}

void AHUDManager::HideWidgetOnScreen(UMyUserWidget* widgetToApply) const
{
   if(widgetToApply->GetClass()->IsChildOf(UAnimHudWidget::StaticClass())) {
      UWidgetAnimation* anim = Cast<UAnimHudWidget>(widgetToApply)->GetAnim();
      if(widgetToApply->IsAnimationPlaying(anim)) {
         widgetToApply->ReverseAnimation(anim);
      } else {
         widgetToApply->PlayAnimation(anim, 0, 1, EUMGSequencePlayMode::Reverse, 1);
      }
   } else {
      widgetToApply->OnWidgetRemovedFromViewport();
      widgetToApply->SetVisibility(ESlateVisibility::Collapsed);
   }
}

bool AHUDManager::ShowHiddenWidget(UMyUserWidget* widgetToApply) const
{
   if(widgetToApply->GetClass()->IsChildOf(UAnimHudWidget::StaticClass())) {
      UWidgetAnimation* anim = Cast<UAnimHudWidget>(widgetToApply)->GetAnim();
      if(widgetToApply->IsAnimationPlaying(anim)) {
         widgetToApply->ReverseAnimation(anim);
      } else {
         if(!widgetToApply->OnWidgetAddToViewport()) { return false; }
         widgetToApply->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
         widgetToApply->PlayAnimation(anim, 0, 1, EUMGSequencePlayMode::Forward, 1);
      }
   } else {
      if(!widgetToApply->OnWidgetAddToViewport()) { return false; }
      widgetToApply->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
   }
   return true;
}

void AHUDManager::UpdateWidgetTracking(int updateIndex, bool enableClickEvents, bool canOpenCombat)
{
   currentlyDisplayedWidgetsBitSet[updateIndex] = !currentlyDisplayedWidgetsBitSet[updateIndex];

   // If this hud allows for clicking outside of the hud
   if(enableClickEvents) {
      currentlyDisplayedWidgetsBitSet[updateIndex] ? ++enableClickEventsCount : --enableClickEventsCount;
      enableClickEventsCount > 0 ? playerControllerRef->bEnableClickEvents = true : playerControllerRef->bEnableClickEvents = false;
   }
   // If this hud can't be opened during combat, then we should change the cursor to the UI cursor (restricting them from doing much) and prevent camera panning
   if(!canOpenCombat) {
      currentlyDisplayedWidgetsBitSet[updateIndex] ? ++numWidgetsBlocking : --numWidgetsBlocking;
      if(numWidgetsBlocking > 0) {
         playerControllerRef->GetCameraPawn()->isCamNavDisabled = true;
         playerControllerRef->GetCameraPawn()->DisableInput(playerControllerRef);
         playerControllerRef->GetCameraPawn()->SetActorTickEnabled(false);
         playerControllerRef->GetCameraPawn()->ChangeCursor(ECursorStateEnum::UI);
      } else {
         playerControllerRef->GetCameraPawn()->isCamNavDisabled = false;
         playerControllerRef->GetCameraPawn()->EnableInput(playerControllerRef);
         playerControllerRef->GetCameraPawn()->SetActorTickEnabled(true);
      }
   }
}

URTSIngameWidget* AHUDManager::GetIngameHUD() const
{
   return Cast<URTSIngameWidget>(widgetReferences[static_cast<int>(EHUDs::HS_Ingame)]);
}

UBreakMenu* AHUDManager::GetBreakMenu() const
{
   return Cast<UBreakMenu>(widgetReferences[static_cast<int>(EHUDs::HS_Break)]);
}

USettingsMenu* AHUDManager::GetSettingsMenu() const
{
   return Cast<USettingsMenu>(widgetReferences[static_cast<int>(EHUDs::HS_Settings)]);
}

UConfirmationBox* AHUDManager::GetConfirmationBox() const
{
   return Cast<UConfirmationBox>(widgetReferences[static_cast<int>(EHUDs::HS_Confirmation)]);
}

URTSInputBox* AHUDManager::GetInputBox() const
{
   return Cast<URTSInputBox>(widgetReferences[static_cast<int>(EHUDs::HS_InputBox)]);
}

UStartMenu* AHUDManager::GetStartMenu() const
{
   return Cast<UStartMenu>(widgetReferences[static_cast<int>(EHUDs::HS_Start)]);
}

void AHUDManager::InjectDependentClasses()
{
   InjectDependency(GetIngameHUD());
   InjectDependency(GetIngameHUD()->GetDialogBox());
   InjectDependency(GetIngameHUD()->GetSocialWindow());
   InjectDependency(GetIngameHUD()->GetSocialWindow()->dialogWheel);
   InjectDependency(GetIngameHUD()->GetShopHUD());
   InjectDependency(gameMode->GetMinigameManager());
   InjectDependency(gameMode->GetQuestManager());
   InjectDependency(gameMode->GetTriggerManager());
   InjectDependency(GetBreakMenu());
   InjectDependency(GetConfirmationBox());
   InjectDependency(GetInputBox());
   InjectDependency(GetIngameHUD()->GetSpellBookMenu());
   InjectDependency(GetIngameHUD()->GetExamineMenu());
}

void AHUDManager::CreateMainWidget()
{
   if(ensure(mainMenuClass != nullptr)) {
      if(mainWidget = CreateWidget<UMainWidget>(playerControllerRef, mainMenuClass, "Main Menu"); ensure(mainWidget)) {
         mainWidget->bIsFocusable = false;
         InjectDependency(mainWidget);
         InjectDependency(playerControllerRef); // Set this here else we can't use this reference during widget construction
         mainWidget->AddToViewport();
      }
   }
}
