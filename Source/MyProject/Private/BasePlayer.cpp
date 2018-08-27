// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "BasePlayer.h"
#include "EngineUtils.h"
#include "WorldObjects/BaseHero.h"
#include "Quests/QuestManager.h"
#include "GameplayTagContainer.h"

ABasePlayer::ABasePlayer()
{
	questManager = nullptr;
	money = 1000;
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->SpawnActor<ABaseHero>(FVector::ZeroVector, FRotator::ZeroRotator);
	//for (TActorIterator<ABaseHero> heroItr(GetWorld()); heroItr; ++heroItr)
	//	heroes.Add(*heroItr);
}

void ABasePlayer::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);
}

void ABasePlayer::UpdateParty(TArray<ABaseHero*> newHeroes)
{
	#if UE_EDITOR 
	if(newHeroes.Num() <= 0 && newHeroes.Num() > MAX_NUM_HEROES)
		UE_LOG(LogTemp, Warning, TEXT("Inappropriate size (%d) of hero array"), newHeroes.Num());
		//checkf(newHeroes.Num() > 0 && newHeroes.Num() <= MAX_NUM_HEROES, TEXT("Inappropriate size (%d) of hero array"), newHeroes.Num());
	#endif

	for(ABaseHero* hero : heroes)
	{
		hero->SetEnabled(false);
	}
	heroes = newHeroes;
	for(ABaseHero* hero : heroes)
	{
		hero->SetEnabled(true);
	}
}


void ABasePlayer::UpdateGold(int32 amount)
{
	money = money + amount;
}

void ABasePlayer::UpdateEXP(int32 amount)
{
	for(ABaseHero* hero : heroes)
	{
		if(IsValid(hero))
			hero->SetCurrentExp(amount);
	}
}

void ABasePlayer::LearnDialogTopic(FGameplayTag topic)
{
	dialogTopics.AddTag(topic);
	OnDialogLearned.Broadcast(topic);
}


