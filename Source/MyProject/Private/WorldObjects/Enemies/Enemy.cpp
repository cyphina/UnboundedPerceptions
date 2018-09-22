// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "AIController.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "Quests/QuestManager.h"
#include "Enemy.h"
#include "BaseCharacter.h"
#include "SpellSystem/MySpell.h"
#include "AIStuff/AIControllers/EnemyAIController.h"
#include "SpellSystem/MyAbilitySystemComponent.h"
#include "NavArea_EnemySpot.h"
#include "../BaseHero.h"

AEnemy::AEnemy(const FObjectInitializer& oI) : AUnit(oI)
{
	aggroRange = 20;
	isEnemy = true;
	state = TUniquePtr<StateMachine>(new StateMachine(this));
	SetActorHiddenInGame(true); //set hidden by default so won't be revealed by vision
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	GetCapsuleComponent()->AreaClass = UNavArea_EnemySpot::StaticClass(); //custom area class so navigation filter for defensive movement will avoid this
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel8, ECollisionResponse::ECR_Block); //traceEnemyOnly
	visionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnVisionSphereOverlap);
	visionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnVisionSphereEndOverlap);
	visionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Overlap); //Friendly
	visionSphere->SetCanEverAffectNavigation(true);
	visionSphere->AreaClass = UNavArea_EnemySpot::StaticClass();
	visionSphere->SetAbsolute(false, false, true);
	visionSphere->bDynamicObstacle = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
	gameModeRef = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
	gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());
	gameStateRef->enemyList.Add(this);

	baseC->GetVital(static_cast<int>(Vitals::Health))->SetBaseValue(health);
	baseC->GetVital(static_cast<int>(Vitals::Health))->SetCurrValue(health);
}

void AEnemy::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void AEnemy::Die()
{
	gameModeRef->GetQuestManager()->OnEnemyDie(this);
	controllerRef->GetBasePlayer()->UpdateEXP(expGiven);
	controllerRef->GetBasePlayer()->UpdateGold(moneyGiven);
	gameStateRef->enemyList.Remove(this);
	Super::Die();
}

void AEnemy::SetSelected(bool value)
{
	Super::SetSelected(value);
	if (value)
	{
		controllerRef->GetBasePlayer()->focusedUnit = this;
	}
	else
	{
		controllerRef->GetBasePlayer()->focusedUnit = this;
	}
}

void AEnemy::OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepRes)
{
	if (otherActor->IsA(AAlly::StaticClass()))
	{
		AAlly* ally = Cast<AAlly>(otherActor);
		//TODO: Check for invisibility
		possibleEnemiesInRadius.Add(ally);
		ally->IncVisionCount();
	}
}

void AEnemy::OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	AAlly* ally = Cast<AAlly>(otherActor);
	if (IsValid(ally))
	{
		possibleEnemiesInRadius.Remove(ally);
		ally->DecVisionCount();
		if (!ally->GetVisionCount())
		{
			gameState->visibleAllies.Remove(ally);
		}
	}
}







