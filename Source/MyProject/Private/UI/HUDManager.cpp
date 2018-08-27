// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameMode.h"
#include "HUDManager.h"
#include "UserInput.h"
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
	switch (newState)
	{
		case HUDs::HS_Ingame: ApplyHUD(newState, true, true, true, false); break;
		case HUDs::HS_Inventory: ApplyHUD(newState, true, true, true, false); break;
		case HUDs::HS_Equipment: ApplyHUD(newState, true, true, false, false); break;
		case HUDs::HS_Character: ApplyHUD(newState, true, true, true, false); break;
		case HUDs::HS_QuestJournal: ApplyHUD(newState, true, true, false, false); break;
		case HUDs::HS_QuestList: ApplyHUD(newState, true, true, true, true); break;
		case HUDs::HS_Spellbook: ApplyHUD(newState, true, true, false, false); break;
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
		default: break;
	}
}

void AHUDManager::AddHUD(FName conversationName, TArray<FTriggerData> onDialogEndTriggers, ABaseHero* interactingHero)
{
	if (!playerControllerRef->GetBasePlayer()->interactedHero) //Only one hero can do a "waiting" interaction at a time
	{
		if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Dialog)] && conversationName != "") //If not on screen
		{
			playerControllerRef->GetBasePlayer()->interactedHero = interactingHero;
			GetDialogBox()->SetConversation(conversationName);
			GetDialogBox()->SetOnDialogFinishedTrigger(onDialogEndTriggers);
			ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, false, false);
		}
		else //if on screen (from being added by a trigger w/o a hero)
		{
			ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, false, false);
		}
	}
	else
	{		
		if (currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Dialog)]) //If on screen
		{
			playerControllerRef->GetBasePlayer()->interactedHero = nullptr;
			ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, false, false);
		}
	}
}

void AHUDManager::AddHUD(TArray<FDialogData> linesToDisplay, TArray<FTriggerData> onDialogEndTriggers, ABaseHero* interactingHero)
{
	if (!playerControllerRef->GetBasePlayer()->interactedHero)
	{
		if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Dialog)] && linesToDisplay.Num() > 0) //If not on screen
		{
			playerControllerRef->GetBasePlayer()->interactedHero = interactingHero;
			GetDialogBox()->SetDialogLines(linesToDisplay);
			GetDialogBox()->SetOnDialogFinishedTrigger(onDialogEndTriggers);
			ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, false, false);
		}
	}
	else
	{
		if (currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Dialog)]) //If on screen
		{
			playerControllerRef->GetBasePlayer()->interactedHero = nullptr;
			ApplyHUD(static_cast<int>(HUDs::HS_Dialog), true, true, false, false);
		}
	}
}

void AHUDManager::AddHUD(UBackpack* backpack, ABaseHero* interactingHero)
{
	if (!playerControllerRef->GetBasePlayer()->interactedHero)
	{
		if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Storage)] && backpack) //If not on screen (impossible state reached if on screen and there's no interactedHero)
		{
			playerControllerRef->GetBasePlayer()->interactedHero = interactingHero;
			GetStorageHUD()->SetBackPack(backpack);
			ApplyHUD(static_cast<int>(HUDs::HS_Storage), true, true, true, false);
		}
	}
	else
	{
		if (currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Storage)]) //if on screen
		{
			playerControllerRef->GetBasePlayer()->interactedHero = nullptr;
			ApplyHUD(static_cast<int>(HUDs::HS_Storage), true, true, true, false);
		}
	}
}

void AHUDManager::AddHUD(AShopNPC* shopNPC, ABaseHero* interactingHero)
{
	if (!playerControllerRef->GetBasePlayer()->interactedHero)
	{
		if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Shop_General)] && shopNPC) //if not on screen
		{
			playerControllerRef->GetBasePlayer()->interactedHero = interactingHero;
			GetShopHUD()->SetBackPack(shopNPC->itemsToSellBackpack);
			GetShopHUD()->shopkeeper = shopNPC;
			ApplyHUD(static_cast<int>(HUDs::HS_Shop_General), true, true, true, false);
		}

	}
	else
	{
		if (currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_Shop_General)]) //if on screen
		{
			playerControllerRef->GetBasePlayer()->interactedHero = nullptr;
			ApplyHUD(static_cast<int>(HUDs::HS_Shop_General), true, true, true, false);
		}
	}
}

void AHUDManager::AddItemExamineHUD(int itemID)
{
	if (!currentlyDisplayedWidgetsBitSet[static_cast<int>(HUDs::HS_ExamineMenu)]) //if not on screen
	{
		if (itemID > 0)
		{
			GetExamineMenu()->itemToDisplayID = itemID;
			ApplyHUD(static_cast<int>(HUDs::HS_ExamineMenu), true, true, true, false);
		}
	}
	else
	{
		ApplyHUD(static_cast<int>(HUDs::HS_ExamineMenu), true, true, true, false);
	}
}

bool AHUDManager::ApplyHUD(uint8 newState, bool bShowMouseCursor, bool bEnableClickEvents, bool canOpenCombat, bool hasAnim)
{
	check(newState >= 0 && newState < widgetReferences.Num());
	UMyUserWidget* widgetToApply = widgetReferences[newState];

	//if that's not a valid widget or else it isn't set up properly
#if UE_EDITOR
	if (!widgetToApply)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Widget Reference being added or removed"))
		return false;
	}
#endif

	//Nullcheck whatever widget we're trying to apply
	if(currentlyDisplayedWidgetsBitSet[newState]) //if our widget is already on screen, we probably pressed button to take it off
	{
		if (hasAnim) //does our widget have animation?
		{
			UWidgetAnimation* anim = Cast<UAnimHudWidget>(widgetToApply)->anim;
			if (widgetToApply->IsAnimationPlaying(anim)) //if animation is currently playing, it's opening 
			{
				widgetToApply->ReverseAnimation(anim);
			}
			else
			{
				widgetToApply->PlayAnimation(anim, 0, 1, EUMGSequencePlayMode::Reverse, 1);
			}
			//animation will be in charge of removing from parent

		}
		else //no animation
		{
			widgetToApply->SetVisibility(ESlateVisibility::Hidden); //hidden prevents tick updates		
		}
	}
	else //else our widget isn't already on the screen 
	{
		if (hasAnim) //does our widget have animation?
		{
			UWidgetAnimation* anim = Cast<UAnimHudWidget>(widgetToApply)->anim;
			if (widgetToApply->IsAnimationPlaying(anim))
			{
				widgetToApply->ReverseAnimation(anim);
			}
			else
			{
				if (!widgetToApply->OnWidgetAddToViewport()) //if preconditions not set, don't make it visible
					return false;
				widgetToApply->SetVisibility(ESlateVisibility::SelfHitTestInvisible); //since it's offscreen, add visibility first
				widgetToApply->PlayAnimation(anim, 0, 1, EUMGSequencePlayMode::Forward, 1);
			}
		}
		else //no animation
		{
			if (!widgetToApply->OnWidgetAddToViewport()) //if preconditions not set, don't make it visible
				return false;
			widgetToApply->SetVisibility(ESlateVisibility::SelfHitTestInvisible);	
		}
	}
	UpdateWidgetTracking(newState, canOpenCombat, bShowMouseCursor, bEnableClickEvents);
	return true;
}

void AHUDManager::UpdateWidgetTracking(int updateIndex, bool canOpenCombat, bool showMouseCursor, bool enableClickEvents)
{
	currentlyDisplayedWidgetsBitSet[updateIndex] = !currentlyDisplayedWidgetsBitSet[updateIndex];
	if(showMouseCursor)
	{
		currentlyDisplayedWidgetsBitSet[updateIndex] ? ++showMouseCursorCount : --showMouseCursorCount;
		showMouseCursorCount > 0 ? playerControllerRef->bShowMouseCursor = true : playerControllerRef->bShowMouseCursor = false;
	}
	if(enableClickEvents)
	{
		currentlyDisplayedWidgetsBitSet[updateIndex] ? ++enableClickEventsCount : --enableClickEventsCount;
		enableClickEventsCount > 0 ? playerControllerRef->bEnableClickEvents = true : playerControllerRef->bEnableClickEvents = false;
	}
	if(!canOpenCombat)
	{
		currentlyDisplayedWidgetsBitSet[updateIndex] ? ++numWidgetsBlocking : --numWidgetsBlocking;
		numWidgetsBlocking > 0 ? playerControllerRef->isCamNavDisabled = true : playerControllerRef->isCamNavDisabled = false;
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
  