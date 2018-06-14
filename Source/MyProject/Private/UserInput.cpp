// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UserInput.h"
#include "MyGameInstance.h"
#include "RTSGameMode.h"
#include "Engine.h"
#include "SceneViewport.h"
#include "Extras/FlyComponent.h"
#include "BasePlayer.h"
#include "WorldObjects/Ally.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/Enemies/Enemy.h"
#include "UI/HUDManager.h"
#include "Quests/QuestManager.h"
#include "UI/UserWidgets/MainWidget.h"
#include "AbilitySystemComponent.h"
#include "SpellSystem/MySpell.h"
#include "ActionbarInterface.h"
#include "MyCheatManager.h"


AUserInput::AUserInput()
{
	//Initialization of variables
	PrimaryActorTick.bCanEverTick = false;
	basePlayer = nullptr;
	gameInstance = nullptr;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::GrabHandClosed;

	//ObjectTypeQueries can be seen in enum list ObjectTypeQuery in the blueprints
	queryableTargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery1); //WorldActor
	queryableTargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery7); //Enemy
	queryableTargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery8); //Interactable
	queryableTargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery9); //NPC
	//queryableTargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery10); don't hit buildings for these click traces
	//queryableTargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery11); //VisionBlocker
	queryableTargetObjects.Add(EObjectTypeQuery::ObjectTypeQuery12); //Friendly

	queryParamVision.AddObjectTypesToQuery(ECC_GameTraceChannel6); //Query vision blockers only
	//HitResultTraceDistance = 100000.f; set in parent
	CheatClass = UMyCheatManager::StaticClass();
}

void AUserInput::BeginPlay()
{
	gameInstance = Cast<UMyGameInstance>(GetGameInstance());

	gameMode = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());

	//There should only be one player
	basePlayer = Cast<ABasePlayer>(PlayerState);

	if (basePlayer)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Input component here"));
		basePlayer->heroes.SetNum(4, false);
		for (TActorIterator<ABaseHero> heroItr(GetWorld()); heroItr; ++heroItr)
		{
			//if this hero 
			if ((*heroItr)->heroIndex >= 0 && (*heroItr)->heroIndex < 4)
			{
				basePlayer->heroes[(*heroItr)->heroIndex] = *heroItr;
			}
		}
	}
	//Super Beginplay calls the blueprint BeginPlay 
	hudManager = GetWorld()->SpawnActor<AHUDManager>(AHUDManager::StaticClass(), FTransform(), FActorSpawnParameters());
	Super::BeginPlay();
}

void AUserInput::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
	CursorHover();
}

void AUserInput::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);
	InputComponent->BindAction("Stop", IE_Pressed, this, &AUserInput::StopSelectedAllyCommands);
	InputComponent->BindAction("PrevFlightPath", IE_Pressed, this, &AUserInput::PrevFlight);
	InputComponent->BindAction("NextFlightPath", IE_Pressed, this, &AUserInput::NextFlight);
	InputComponent->BindAction("OpenInventory", IE_Pressed, this, &AUserInput::Inventory);
	InputComponent->BindAction("OpenCharacterSheet", IE_Pressed, this, &AUserInput::Character);
	InputComponent->BindAction("Equipment", IE_Pressed, this, &AUserInput::Equipment);
	InputComponent->BindAction("Spellbook", IE_Pressed, this, &AUserInput::Spellbook);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AUserInput::RightClick);
	InputComponent->BindAction("QuestKey", IE_Pressed, this, &AUserInput::QuestJournal);
	InputComponent->BindAction("ToggleQuestWidget", IE_Pressed, this, &AUserInput::QuestList);
	InputComponent->BindAction("Break", IE_Pressed, this, &AUserInput::ToggleBreakMenu);
	InputComponent->BindAction("SelectNext", IE_Pressed, this, &AUserInput::TabNextAlly);
}

void AUserInput::StopSelectedAllyCommands()
{
	for (int i = 0; i < basePlayer->selectedAllies.Num(); i++)
	{
		if (basePlayer->selectedAllies[i]->GetSelected())
		{
			basePlayer->selectedAllies[i]->Stop();
			basePlayer->selectedAllies[i]->ClearCommandQueue();
		}
	}

	hasSecondaryCursor = false;
	HideSpellCircle();
}

void AUserInput::PrevFlight()
{
	for (int i = 0; i < basePlayer->selectedAllies.Num(); i++)
	{
		Cast<UFlyComponent>(basePlayer->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->PreviousFlightPathSelected();
	}
}

void AUserInput::NextFlight()
{
	for (int i = 0; i < basePlayer->selectedAllies.Num(); i++)
	{
		Cast<UFlyComponent>(basePlayer->heroes[i]->GetComponentByClass(UFlyComponent::StaticClass()))->NextFlightPathSelected();
	}
}

void AUserInput::Inventory()
{
	int numSelectedHeroes = basePlayer->selectedHeroes.Num();
	if (numSelectedHeroes > 0)
	{
		GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Inventory));
	}
}

void AUserInput::QuestJournal()
{
	GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_QuestJournal));
}

void AUserInput::QuestList()
{
	GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_QuestList));
}

void AUserInput::Character()
{
	int numSelectedHeroes = basePlayer->selectedHeroes.Num();
	if (numSelectedHeroes > 0 || GetHUDManager()->widgetReferences[static_cast<uint8>(HUDs::HS_Character)]->IsVisible())
	{
		GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Character));
	}
}

void AUserInput::Equipment()
{
	int numSelectedHeroes = basePlayer->selectedHeroes.Num();
	if (numSelectedHeroes > 0)
	{
		GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Equipment));
	}
}

void AUserInput::Spellbook()
{
	int numSelectedHeroes = basePlayer->selectedHeroes.Num();
	if (numSelectedHeroes > 0)
	{
		GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Spellbook));
	}
}

void AUserInput::ToggleBreakMenu()
{
	GetHUDManager()->AddHUD(static_cast<uint8>(HUDs::HS_Break));
}

void AUserInput::TabNextAlly()
{
	int selectedHeroIndex = -1;
	if(basePlayer->selectedAllies.Num() > 1)
	{
		basePlayer->unitIndex = (basePlayer->unitIndex + 1) % basePlayer->selectedAllies.Num();
		//Make sure any other selected heroes are actually in hero array.  If a hero is on the map, it better be in our party else spawn the NPC version of it
		hudManager->GetActionHUD()->SingleAllyViewIndexFree(basePlayer->selectedAllies[basePlayer->unitIndex]);
	}
	else
	{
		//tab through heroes if one/zero allies are selected
		if(basePlayer->selectedAllies.Num() > 0)
		{
			selectedHeroIndex = basePlayer->selectedHeroes[0]->heroIndex;
			basePlayer->heroes[selectedHeroIndex]->SetSelected(false);
		}
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, FString::FromInt(basePlayer->heroes.Num()));
		basePlayer->heroes[(selectedHeroIndex+1)%basePlayer->heroes.Num()]->SetSelected(true);
		OnAllySelectedDelegate.Broadcast();
	}
}

void AUserInput::RightClick()
{
	if (cursorState == ECursorStateEnum::Magic)
	{
		StopSelectedAllyCommands();
		return;
	}

	if (cursorState != ECursorStateEnum::UI) {
		if (cursorState != ECursorStateEnum::Attack)
		{
			if (GetHitResultUnderCursorForObjects(queryableTargetObjects, false, hitResult))
			{
				//last time when I didn't make this temporary, we tried making our lambda automatically capture location and it failed miserably
				FVector location = hitResult.Location;

				//create a little decal where we clicked
				CreateClickVisual(location);

				if (IsInputKeyDown(EKeys::LeftShift))
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, hitResult.Location.ToString());
					for (AAlly* ally : basePlayer->selectedAllies)
					{
						ally->QueueAction(TFunction<void()>(
							[ally, location]() {ally->Move(location);}
						));
					}
				}
				else
				{
					StopSelectedAllyCommands();
					for (AAlly* ally : basePlayer->selectedAllies)
					{
						ally->Move(FVector(location));
					}
				}
			}
		}
		else
		{
			if (GetHitResultUnderCursorForObjects(queryableTargetObjects, false, hitResult))
			{
				AEnemy* enemy = Cast<AEnemy>(hitResult.GetActor());
				if (IsInputKeyDown(EKeys::LeftShift))
				{
					for (AAlly* ally : basePlayer->selectedAllies)
					{
						ally->QueueAction(TFunction<void()>(
							[ally, enemy]() {ally->BeginAttack(enemy);}
						));
					}
				}
				else
				{
					StopSelectedAllyCommands();
					for (AAlly* ally : basePlayer->selectedAllies)
					{
						ally->BeginAttack(enemy);
					}
				}
			}
		}
	}
}

void AUserInput::SetSecondaryCursor(ECursorStateEnum cursorType)
{
	if (cursorType == ECursorStateEnum::Magic || cursorType == ECursorStateEnum::Item)
	{
		hasSecondaryCursor = true;
		ChangeCursor(cursorType);
	}
	else
	{
		hasSecondaryCursor = false;
	}
}

void AUserInput::ChangeCursor(ECursorStateEnum newCursorState)
{
	if (cursorState != newCursorState)
	{
		cursorState = newCursorState;
		CurrentMouseCursor = static_cast<EMouseCursor::Type>(newCursorState);
		FSlateApplication::Get().GetPlatformCursor().Get()->SetType(static_cast<EMouseCursor::Type>(newCursorState));
	}
}

void AUserInput::CursorHover()
{
	if (GetHUDManager() && GetHUDManager()->numWidgetsBlocking > 0)
	{
		ChangeCursor(ECursorStateEnum::UI);
		return;
	}

	if (!hasSecondaryCursor)
	{
		if (isEdgeMoving && !isCamNavDisabled)
		{
			ChangeCursor(cursorDirections.Last());
			return;
		}

		if (basePlayer && basePlayer->selectedAllies.Num() > 0)
		{
			GetHitResultUnderCursorForObjects(queryableTargetObjects, true, hitResult);
			if (hitResult.GetActor())
			{
				hitActor = hitResult.GetActor();
				switch (hitResult.GetComponent()->GetCollisionObjectType())
				{
				case ECollisionChannel::ECC_WorldStatic: ChangeCursor(ECursorStateEnum::Moving); break;
				case ECollisionChannel::ECC_GameTraceChannel3: ChangeCursor(ECursorStateEnum::Interact); break;
				case ECollisionChannel::ECC_GameTraceChannel4: ChangeCursor(ECursorStateEnum::Talking); break;
				case ECollisionChannel::ECC_GameTraceChannel2: ChangeCursor(ECursorStateEnum::Attack); break;
				default: ChangeCursor(ECursorStateEnum::Select); break;
				}
			}
		}
		else
		{
			ChangeCursor(ECursorStateEnum::Select);
		}
	}
}

void AUserInput::UpdateVisibleEnemies()
{
#if UE_EDITOR
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, TEXT("Vision Checking!"));
#endif

	/**OnComponentOverlap handles cases when enemy comes in and out of vision range
	*every ally has its own list of enemis it can
	by keeping seperate lists of possiblenemies we don't have to check each hero and see which one line traced through the enemy*/

	checkedEnemies.Empty();

	for (AAlly* ally : basePlayer->allies)
	{
		for (AEnemy* enemy : ally->possibleEnemiesInRadius)
		{
			//if enemy hasn't been checked yet so we don't do it twice
			if (!checkedEnemies.Contains(enemy))
			{
				//If we've already deemed this enemy visible
				if (visibleEnemies.Contains(enemy))
				{
					//check if it's not visible anymore by seeing if it hits a wall
					if (GetWorld()->LineTraceSingleByObjectType(visionHitResult, ally->GetActorLocation(), enemy->GetActorLocation(), queryParamVision))
					{
						//if so then remove it from visible list
						if (enemy->GetCapsuleComponent()->bVisible)
						{
							enemy->GetCapsuleComponent()->SetVisibility(false, true);
							visibleEnemies.Remove(enemy);
						}
					}
				}
				//if we haven't added the enemy to the visible list
				else
				{
					//If we can trace a line and hit the enemy without hitting a walk
					if (!GetWorld()->LineTraceSingleByObjectType(visionHitResult, ally->GetActorLocation(), enemy->GetActorLocation(), queryParamVision))
					{
						//make it visible
						if (!enemy->GetCapsuleComponent()->bVisible)
						{
							enemy->GetCapsuleComponent()->SetVisibility(true, true);
							visibleEnemies.Add(enemy);
						}
					}
				}
				checkedEnemies.Add(enemy);
			}
		}
	}
}


