// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "NPC.h"
#include "BaseHero.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogBox.h"
#include "DialogSystem/DialogUI.h"

// Sets default values
ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;
	onDialogEndTriggerData.triggerType = TriggerType::OpenHUDTrigger;
	onDialogEndTriggerData.enabled = true;
	TArray<FString> triggerValues;
	triggerValues.Add(FString("11"));
	onDialogEndTriggerData.triggerValues = triggerValues;
	onDialogEndTriggerData.numCalls = -1;
	GetMesh()->SetCollisionProfileName("NPC");
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName ANPC::GetConversationName(FGameplayTag conversationTopic) const
{
	//does this NPC know anything about the topic?
	if (conversationTopics.Contains(conversationTopic))
	{
		return conversationTopics[conversationTopic];
	}
	else //if no, return some default reply like "I'm not sure what that is sire!"
	{
		return defaultResponseName;
	}
}

void ANPC::Interact_Implementation(ABaseHero* hero)
{
	GetController()->StopMovement();
	FVector currentLocation = GetActorLocation();
	FVector difference = hero->GetActorLocation() - currentLocation;
	FVector projectedDirection = FVector(difference.X, difference.Y, 0);
	SetActorRotation(FRotationMatrix::MakeFromX(FVector(projectedDirection)).Rotator());
	if (bWantsToConverse)
	{
		controllerRef->GetHUDManager()->GetSocialWindow()->SetNPC(this);
		controllerRef->GetHUDManager()->GetSocialWindow()->SetConversationView();
		controllerRef->GetHUDManager()->AddHUDDialog(conversationStarterName, onDialogEndTriggerData);
	}
	else
		controllerRef->GetHUDManager()->AddHUDDialog(conversationStarterName, FTriggerData::defaultTrigger);
}

FVector ANPC::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}






