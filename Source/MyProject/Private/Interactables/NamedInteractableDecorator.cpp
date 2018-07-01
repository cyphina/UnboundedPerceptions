// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSConditional.h"
#include "NamedInteractableDecorator.h"
#include "Quests/QuestManager.h"
#include "RTSGameMode.h"

void UNamedInteractableSpawner::OnInteractedQuestManagerWrapper()
{
	Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->GetQuestManager()->OnInteracted(this);
}

void UNamedInteractableSpawner::Init()
{
	UInteractableActorDecoratorBase::Init();
	OnInteracted.AddDynamic(this, &UNamedInteractableSpawner::OnInteractedQuestManagerWrapper);
}

bool UNamedInteractableSpawner::Interact()
{
	if(!decoratedInteractable || decoratedInteractable->Interact())
	{
		OnInteracted.Broadcast();
		return true;
	}
	return false;
}
