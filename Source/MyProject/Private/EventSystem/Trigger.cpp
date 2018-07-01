// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Trigger.h"
#include "RTSGameMode.h"
#include "ResourceManager.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "Quests/Quest.h"
#include "Quests/QuestManager.h"
#include "DialogSystem/DialogManager.h"
#include "WorldObjects/NPC.h"
#include "WorldObjects/Unit.h"
#include "WorldObjects/BaseHero.h"

FTriggerData FTriggerData::defaultTrigger = FTriggerData();

FTriggerData::FTriggerData()
{
	
}

void UTriggerManager::Init()
{
	cpcRef = Cast<AUserInput>(GetOuter()->GetWorld()->GetFirstPlayerController());
	gameModeRef = Cast<ARTSGameMode>(GetOuter());
}

void UTriggerManager::AddTriggerToRecords(FName worldObjectName, const FTriggerData& trigger) 
{
	triggerRecords.Add(worldObjectName, trigger);
}

void UTriggerManager::ActivateTrigger(UPARAM(ref) FTriggerData& triggerData)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("TRIGGER ACTIVATED!"));
	if (triggerData.enabled)
	{
		if (triggerData.numCalls != 0)
		{
			TriggerEffect(triggerData);		
		}
	}
}

void UTriggerManager::ChangeDialog(const FTriggerData& tdata)
{
	check(tdata.triggerObjects.Num() == 1 && tdata.triggerValues.Num() <= 2)
	if (ANPC* npc = ResourceManager::FindTriggerObjectInWorld<ANPC>(tdata.triggerObjects[0], cpcRef->GetWorld()))
	{
		if (tdata.triggerValues.Num() == 1)
			npc->SetCurrentDialog(*tdata.triggerValues[0]);
		else
			npc->SetSpecificDialog(FGameplayTag::RequestGameplayTag(*tdata.triggerValues[0]), *tdata.triggerValues[1]);
	}
	else
	{
		//TODO: Find a way to add to trigger records w/o knowing if there's already an entry in triggerRecords 
		AddTriggerToRecords(*tdata.triggerObjects[0], tdata);
	}
}

void UTriggerManager::ModifyStats(const FTriggerData& tdata)
{
	check(tdata.triggerObjects.Num() == 1 && tdata.triggerValues.Num() <= 2)
	if (AUnit* unit = ResourceManager::FindTriggerObjectInWorld<AUnit>(tdata.triggerObjects[0], cpcRef->GetWorld()))
		return;
}

void UTriggerManager::OpenHUDTrigger(const FTriggerData& tdata)
{
	checkf(tdata.triggerObjects.Num() == 0 && tdata.triggerValues.Num() == 1, TEXT("Incorrect parameters for OPENHUDTRIGGER"))
	checkf(tdata.triggerValues[0].IsNumeric(), TEXT("OPENHUDTRIGGER triggerValue should be numeric"))
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, tdata.triggerValues[0]);
	cpcRef->GetHUDManager()->AddHUD(FCString::Atoi(*tdata.triggerValues[0]));
}

void UTriggerManager::ChangeParty(const FTriggerData& tdata)
{
	checkf(tdata.triggerObjects.Num() <= 4, TEXT("Incorrect number of parameters %d for ChangePartyTrigger"), tdata.triggerObjects.Num());
	TArray<ABaseHero*> newHeroes = TArray<ABaseHero*>();

	for(FString heroName : tdata.triggerObjects)
	{
		//empty string means no hero at that slot
		if(heroName.IsEmpty())
		{
			break;
		}

		ABaseHero* hero = ResourceManager::FindTriggerObjectInWorld<ABaseHero>(heroName, cpcRef->GetWorld());
		if(hero)
		{
			newHeroes.Add(hero);
		}
	}
	//GetBasePlayer will return null if we don't go through persistent level setup first
	cpcRef->GetBasePlayer()->UpdateParty(newHeroes);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::FromInt(newHeroes.Num()));
}

void UTriggerManager::ActivateOtherTrigger(const FTriggerData& tdata)
{
}

void UTriggerManager::DeactivateOtherTrigger(const FTriggerData& tdata)
{

}

void UTriggerManager::ChangeTriggerType(const FTriggerData& tdata)
{
}

bool UTriggerManager::AddQuest(const FTriggerData& tdata)
{
	checkf(tdata.triggerObjects.Num() == 0 && tdata.triggerValues.Num() == 2, TEXT("Incorrect parameters for OPENHUDTRIGGER"))
	checkf(tdata.triggerValues[1].IsNumeric(), TEXT("ADDQUESTTRIGGER triggerValue 2 should be numeric"))
	if(gameModeRef->GetQuestManager()->AddNewQuest(gameModeRef->GetQuestManager()->questMap->questClassList[
		FGameplayTag::RequestGameplayTag(*(FString("QuestName.")+tdata.triggerValues[0]))], 
		FCString::Atoi(*tdata.triggerValues[0]) != 0))
	return true;

	return false;
}

bool UTriggerManager::CompleteQuestGoal(const FTriggerData& tdata)
{
	checkf(tdata.triggerValues.Num() == 2, TEXT("Complete quest goal trigger should have 2 parameters!"))
	checkf(tdata.triggerValues[1].IsNumeric(), TEXT("ADDQUESTTRIGGER triggerValue 2 should be numeric"))
	
	AQuest* quest = *gameModeRef->GetQuestManager()->quests.FindByPredicate([&](AQuest* questToFilter) { return questToFilter->questInfo.id.ToString() == "QuestName." + tdata.triggerValues[0]; });
	int goalIndex = FCString::Atoi(*tdata.triggerValues[1]);
	
	if(quest->questInfo.subgoals[goalIndex].goalState == EGoalState::currentGoal)
	{
		quest->CompleteSubGoal(goalIndex, FCString::Atoi(*tdata.triggerValues[2]));
		return true;
	}
	return false;
}

void UTriggerManager::DisplayDialog(const FTriggerData& tdata)
{
	TArray<FDialogData> dialog;
	int index = 1;
	for(int i = 0; i < tdata.triggerValues.Num() - 1; ++i)
	{
		TArray<int> nextIndices = {i};
		dialog.Emplace(nextIndices, FText::FromString(tdata.triggerValues[i]), "");
		++index;
	}
	dialog.Emplace(TArray<int>(), FText::FromString(tdata.triggerValues[tdata.triggerValues.Num() - 1]), "");
	cpcRef->GetHUDManager()->AddHUD(dialog);
}

void UTriggerManager::DisplayConversation(const FTriggerData& tdata)
{
	cpcRef->GetHUDManager()->AddHUD(*tdata.triggerValues[0]);
}

void UTriggerManager::TriggerEffect(FTriggerData& triggerData)
{
	#if UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(static_cast<uint8>(triggerData.triggerType)) + " Type Trigger Activated");
	#endif
	
	//If the trigger activation fails somehow, the call count will not be decreased.
	switch (triggerData.triggerType)
	{
		case ETriggerType::None: return; break;
		case ETriggerType::OpenHUDTrigger: OpenHUDTrigger(triggerData); break;
		case ETriggerType::ActivateOtherTrigger: ActivateTrigger(UPARAM(ref) triggerData); break;
		case ETriggerType::ChangeDialog: ChangeDialog(triggerData); break;
		case ETriggerType::ChangePartyTrigger: ChangeParty(triggerData); break;
		case ETriggerType::AddQuestTrigger: if(!AddQuest(triggerData)) return; break;
		case ETriggerType::CompleteQuestGoalTrigger: if(!CompleteQuestGoal(triggerData)) return; break;
		case ETriggerType::DisplayConversationTrigger: DisplayConversation(triggerData); break;
		case ETriggerType::DisplayDialogTrigger: DisplayDialog(triggerData); break;
		default: UE_LOG(LogTemp, Warning, TEXT("Unknown Trigger type attempted to be activated!"));
	}

	if (triggerData.numCalls != -1)
		--triggerData.numCalls;
}
