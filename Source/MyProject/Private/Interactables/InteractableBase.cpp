// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "InteractableBase.h"
#include "Interactables/InteractableActorDecoratorBase.h"
#include "Interactables/NamedInteractableDecorator.h"
#include "TriggerInteractableDecorator.h"
#include "LevelSaveStructs.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	if(decorator)
	{
		//decorator->Rename(0,this);
		decorator->Init();
	}
}

FInteractableSaveInfo AInteractableBase::SaveInteractableData()
{
	FInteractableSaveInfo interactableSaveInfo;
	UInteractableActorDecoratorBase* decor = decorator;
	while(decor)
	{
		if(decor->GetClass() == UTriggerInteractableDecorator::StaticClass())
		{
			UTriggerInteractableDecorator* triggerDecor = Cast<UTriggerInteractableDecorator>(decor);
			for(FTriggerData& trigger : triggerDecor->triggersActivatedOnInteract)
			{
				interactableSaveInfo.triggerDecoratorInfo.Add(FTriggerInteractableDecoratorSaveInfo(trigger.numCalls, trigger.enabled));
			}
			break;
		}
		decor = decor->decoratedInteractable;
	}
	interactableSaveInfo.transform = GetTransform();
	interactableSaveInfo.interactableClass = GetClass();
	return interactableSaveInfo;
}

void AInteractableBase::LoadInteractableData(FInteractableSaveInfo& interactableInfo)
{
	SetActorTransform(interactableInfo.transform);
	UInteractableActorDecoratorBase* decor = decorator;

	while (decor != nullptr)
	{
		if(decor->GetClass() == UTriggerInteractableDecorator::StaticClass())
		{
			UTriggerInteractableDecorator* triggerDecor = Cast<UTriggerInteractableDecorator>(decor);
			check(triggerDecor->triggersActivatedOnInteract.Num() == interactableInfo.triggerDecoratorInfo.Num());
			for(int i = 0; i < triggerDecor->triggersActivatedOnInteract.Num(); ++i)
			{
				triggerDecor->triggersActivatedOnInteract[i].numCalls = interactableInfo.triggerDecoratorInfo[i].numCalls;
				triggerDecor->triggersActivatedOnInteract[i].enabled = interactableInfo.triggerDecoratorInfo[i].enabled;
			}
			break;
		}
		decor = decor->decoratedInteractable;
	}

	for(FTriggerInteractableDecoratorSaveInfo triggerDecorData : interactableInfo.triggerDecoratorInfo)
	{
		UTriggerInteractableDecorator* triggerDecor = NewObject<UTriggerInteractableDecorator>(this);
		decor->decoratedInteractable = triggerDecor;
	}
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FText AInteractableBase::GetGameName() const
{
	if(decorator)
		return decorator->GetName();
	return FText::GetEmpty();
}

void AInteractableBase::SetGameName(FText value)
{
	UInteractableActorDecoratorBase* nextDecorator = decorator;
	while(nextDecorator);
	{
		nextDecorator = nextDecorator->decoratedInteractable;
	}
	nextDecorator->decoratedInteractable = NewObject<UNamedInteractableDecorator>(nextDecorator);
}

void AInteractableBase::Interact_Implementation(ABaseHero* hero)
{
	if(IInteractable::Execute_CanInteract(this)) //calls Interact on decorator in the process
		return;
}

FVector AInteractableBase::GetInteractableLocation_Implementation()
{
	return GetActorLocation();
}

bool AInteractableBase::CanInteract_Implementation()
{
	if(!decorator || decorator->Interact())
		return true;
	return false;
}

void AInteractableBase::SaveInteractable(FMapSaveInfo& mapData)
{
	mapData.interactablesInfo.Add(SaveInteractableData());
}

void AInteractableBase::LoadInteractable(FInteractableSaveInfo& interactableInfo)
{
	SetActorTransform(interactableInfo.transform);
	UInteractableActorDecoratorBase* decor = decorator;

	while (decor != nullptr)
	{
		if(decor->GetClass() == UTriggerInteractableDecorator::StaticClass())
			break;
		decor = decor->decoratedInteractable;
	}

	if(!decor)
	{
		decor = NewObject<UTriggerInteractableDecorator>(this);
	}

	UTriggerInteractableDecorator* triggerDecor = Cast<UTriggerInteractableDecorator>(decor);
	int index = 0;

	for(FTriggerInteractableDecoratorSaveInfo triggerInfo : interactableInfo.triggerDecoratorInfo)
	{
		if(triggerDecor->triggersActivatedOnInteract.Num() > index + 1)
		{
			triggerDecor->triggersActivatedOnInteract[index].numCalls = triggerInfo.numCalls;
			triggerDecor->triggersActivatedOnInteract[index].enabled = triggerInfo.enabled;
		}
	}
}


