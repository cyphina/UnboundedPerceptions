// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "AIController.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Quests/QuestManager.h"
#include "Enemy.h"
#include "BaseCharacter.h"
#include "SpellSystem/MySpell.h"
#include "AIStuff/AIControllers/EnemyAIController.h"
#include "../BaseHero.h"

AEnemy::AEnemy(const FObjectInitializer& oI) : AUnit(oI)
{
	aggroRange = 20;
	isEnemy = true;
	state = TUniquePtr<StateMachine>(new StateMachine(this));
	SetActorHiddenInGame(true); //set hidden by default so won't be revealed by vision
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
	gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
	baseC->GetVital(static_cast<int>(Vitals::Health))->SetBaseValue(health);
	baseC->GetVital(static_cast<int>(Vitals::Health))->SetCurrValue(health);
}

void AEnemy::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void AEnemy::Die()
{
	Super::Die();
	gameModeRef->GetQuestManager()->OnEnemyDie(GetClass());
	controllerRef->GetBasePlayer()->UpdateEXP(expGiven);
	controllerRef->GetBasePlayer()->UpdateGold(moneyGiven);
}

void AEnemy::SetSelected(bool value)
{
	Super::SetSelected(value);
	if(value)
	{
		controllerRef->GetBasePlayer()->focusedUnit = this;
	}
	else
	{
		controllerRef->GetBasePlayer()->focusedUnit = this;
	}
}







