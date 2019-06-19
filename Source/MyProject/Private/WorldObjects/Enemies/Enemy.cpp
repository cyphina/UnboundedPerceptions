// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "AIStuff/AIControllers/EnemyAIController.h"

#include "RTSGameMode.h"
#include "RTSGameState.h"

#include "UserInput.h"
#include "BasePlayer.h"

#include "Quests/QuestManager.h"

#include "Enemy.h"

#include "BaseCharacter.h"
#include "SpellSystem/MySpell.h"

#include "SpellSystem/MyAbilitySystemComponent.h"

#include "NavArea_EnemySpot.h"
#include "../BaseHero.h"

#include "UpResourceManager.h"

#include "Interactables/Pickup.h"

AEnemy::AEnemy(const FObjectInitializer& oI) : AUnit(oI)
{
   aggroRange = 20;
   state      = TUniquePtr<StateMachine>(new StateMachine(this));
   SetActorHiddenInGame(true); //Set hidden by default so won't be revealed by vision
   combatParams.isEnemy = true;
   
   GetCapsuleComponent()->AreaClass = UNavArea_EnemySpot::StaticClass(); //Custom area class so navigation filter for defensive movement will avoid this
   GetCapsuleComponent()->SetCollisionProfileName("Enemy");

   visionSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel9);
   visionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnVisionSphereOverlap);
   visionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnVisionSphereEndOverlap);
   visionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Overlap); //Friendly
   visionSphere->SetCanEverAffectNavigation(true);
   visionSphere->AreaClass = UNavArea_EnemySpot::StaticClass();
   visionSphere->SetAbsolute(false, false, true);
   visionSphere->bDynamicObstacle = true;

   GetMesh()->CustomDepthStencilValue = 254;
}

void AEnemy::BeginPlay()
{
   Super::BeginPlay();
   controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());
   gameModeRef   = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   gameStateRef  = Cast<ARTSGameState>(GetWorld()->GetGameState());
   gameStateRef->enemyList.Add(this);

   InitializeStats();
}

void AEnemy::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
}

void AEnemy::Die_Implementation()
{
   Super::Die_Implementation();
   gameModeRef->GetQuestManager()->OnEnemyDie(this);
   controllerRef->GetBasePlayer()->UpdateEXP(expGiven);
   controllerRef->GetBasePlayer()->UpdateGold(moneyGiven);

   //Generate drops
   for(FItemDrop& itemDrop : itemDrops)
   {
      int dropRoll = FMath::FRandRange(0,100);
      if(itemDrop.dropPerc > dropRoll)
      {
         APickup* itemPickup = GetWorld()->SpawnActorDeferred<APickup>(APickup::StaticClass(), GetActorTransform());
         itemPickup->item = itemDrop.itemInfo;
         UGameplayStatics::FinishSpawningActor(itemPickup, GetActorTransform());
      }       
   }

   gameStateRef->enemyList.Remove(this);
   Destroy();
}

void AEnemy::EndPlay(EEndPlayReason::Type e)
{
   Super::EndPlay(e);
   gameStateRef->enemyList.Remove(this);
}

void AEnemy::Destroyed()
{
   Super::Destroyed();
}

void AEnemy::Attack_Implementation()
{
   Super::Attack_Implementation();
   //If they die and the targets get canceled out, then targetUnit can be nulled
   if(IsValid(targetData.targetUnit))
      if(!targetData.targetUnit->IsVisible())
         GetUnitController()->Stop();
}

bool AEnemy::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   bool invis = GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility"));
   if (Super::CastSpell(spellToCast))
   {
      //Cancel AI targetting if enemy turns invisible
      if (IsValid(targetData.targetUnit) && !gameState->visibleAllies.Contains(targetData.targetUnit))
         GetUnitController()->Stop();
      //Reveal self if invisile when spell casted
      //Reveal self if invisile when spell casted.  If we don't check this before spell casted, we could just end up canceling an invisibility spell being cast
         if(invis)
      GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));
      return true;
   }
   return false;
}

void AEnemy::SetSelected(bool value)
{
   Super::SetSelected(value);
   if (value) {
      controllerRef->GetBasePlayer()->focusedUnit = this;
   } else {
      controllerRef->GetBasePlayer()->focusedUnit = nullptr;
   }
}

void AEnemy::SetEnabled(bool bEnabled)
{
   if(bEnabled)
      controllerRef->GetGameState()->enemyList.Add(this);
   else
      controllerRef->GetGameState()->enemyList.Remove(this);
}

bool AEnemy::IsVisible()
{
   return gameState->visibleEnemies.Contains(this);
}

TSet<AUnit*>* AEnemy::GetSeenEnemies()
{
   return &possibleEnemiesInRadius;
}

void AEnemy::OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepRes)
{
   if (otherActor->IsA(AAlly::StaticClass())) {
      AAlly* ally = Cast<AAlly>(otherActor);
      // TODO: Check for invisibility
      possibleEnemiesInRadius.Add(ally);
      ally->IncVisionCount();
   }
}

void AEnemy::OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
   AUnit* ally = Cast<AUnit>(otherActor);
   if (IsValid(ally)) {
      possibleEnemiesInRadius.Remove(ally);
      ally->DecVisionCount();
      if (!ally->GetVisionCount()) { gameState->visibleAllies.Remove(ally); }
   }
}

void AEnemy::InitializeStats()
{
   int index = -1;
   for (Attributes x : initialStats.defaultAttributes) {
      baseC->GetAttribute(static_cast<int>(x))->SetBaseValue(initialStats.defaultValues[++index]);
      baseC->GetAttribute(static_cast<int>(x))->SetCurrentValue(initialStats.defaultValues[index]);
   }

   for (UnitStats x : initialStats.defaultUnitStats) {
      baseC->GetSkill(static_cast<int>(x))->SetBaseValue(initialStats.defaultValues[++index]);
   }

   for (Vitals x : initialStats.defaultVitals) {
      baseC->GetVital(static_cast<int>(x))->SetBaseValue(initialStats.defaultValues[++index]);
      baseC->GetVital(static_cast<int>(x))->SetCurrValue(initialStats.defaultValues[index]);
   }

   for (Mechanics x : initialStats.defaultMechanics) {
      baseC->GetMechanic(static_cast<int>(x))->SetBaseValue(initialStats.defaultValues[++index]);
      baseC->GetMechanic(static_cast<int>(x))->SetCurrentValue(initialStats.defaultValues[index]);
   }
}

float AEnemy::CalculateTargetRisk()
{
   int targetNum = 0;
   for (AUnit* a : controllerRef->GetGameState()->visibleAllies) {
      if (a->GetTarget() == this) targetNum += 1;
   }

   const float targetRiskValue = FMath::Clamp(diminishFunc(targetNum), 0.f, 1.f);
   return targetRiskValue;
}
