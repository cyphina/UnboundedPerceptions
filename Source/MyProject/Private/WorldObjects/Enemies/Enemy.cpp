// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "AIStuff/AIControllers/EnemyAIController.h"

#include "RTSGameMode.h"
#include "RTSGameState.h"

#include "UserInput.h"
#include "BasePlayer.h"

#include "Quests/QuestManager.h"

#include "Enemy.h"

#include "CombatParameters.h"
#include "RTSVisionComponent.h"
#include "SpellSystem/MySpell.h"

#include "SpellSystem/RTSAbilitySystemComponent.h"
#include "DIRender.h"

#include "NavArea_EnemySpot.h"
#include "PartyDelegateStore.h"
#include "../BaseHero.h"

#include "UpResourceManager.h"

#include "Interactables/Pickup.h"

AEnemy::AEnemy(const FObjectInitializer& oI) : AUnit(oI)
{
   aggroRange = 20;

   GetCapsuleComponent()->AreaClass = UNavArea_EnemySpot::StaticClass(); //Custom area class so navigation filter for defensive movement will avoid this
   GetCapsuleComponent()->SetCollisionProfileName("Enemy");

   visionComponent->SetCollisionProfileName("EnemyVision");
   visionComponent->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Overlap); //Friendly
   visionComponent->SetCanEverAffectNavigation(true);
   visionComponent->AreaClass = UNavArea_EnemySpot::StaticClass();
   visionComponent->SetAbsolute(false, false, true);
   visionComponent->bDynamicObstacle = true;
   visionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnVisionSphereOverlap);
   visionComponent->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnVisionSphereEndOverlap);

   GetMesh()->CustomDepthStencilValue = 254;
}

void AEnemy::BeginPlay()
{
   Super::BeginPlay();
   auto gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState());

   gameStateRef->RegisterEnemyUnit(this);

   // Setup status as customized in level editor
   InitializeStats();
   SetActorHiddenInGame(true); //Set hidden by default so won't be revealed by vision

}

void AEnemy::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
}

void AEnemy::Die_Implementation()
{
   Super::Die_Implementation();

   controllerRef->GetGameMode()->GetQuestManager()->OnEnemyDie(this);
   controllerRef->GetBasePlayer()->UpdateEXP(expGiven);
   controllerRef->GetBasePlayer()->UpdateGold(moneyGiven);

   for(FItemDrop& itemDrop : itemDrops) {
      const int dropRoll = FMath::FRandRange(0, 100);
      if(itemDrop.dropPerc > dropRoll) {
         APickup* itemPickup = GetWorld()->SpawnActorDeferred<APickup>(APickup::StaticClass(), GetActorTransform());
         itemPickup->item    = itemDrop.itemInfo;
         UGameplayStatics::FinishSpawningActor(itemPickup, GetActorTransform());
      }
   }

   // Enemy is hidden in Unit's implementation but reveal damage numbers still
   TArray<UDIRender*> damageComponents;
   GetComponents<UDIRender>(damageComponents);
   for(auto x : damageComponents) {
      x->SetVisibility(true);
   }

   // Set alive to show damage number
   SetLifeSpan(2.f);
}

void AEnemy::EndPlay(EEndPlayReason::Type e)
{
   Super::EndPlay(e);
   visionComponent->visionMutex.WriteLock();
   controllerRef->GetGameState()->UnRegisterEnemyUnit(this);
   visionComponent->visionMutex.WriteUnlock();
}

void AEnemy::Destroyed()
{
   Super::Destroyed();
}

void AEnemy::Attack_Implementation()
{
   Super::Attack_Implementation();
   //If they die (when this current attack kills them) and the targets get canceled out, then targetUnit can be nulled
   if(IsValid(GetTargetUnit()))
      if(!GetTargetUnit()->IsUnitVisible()) GetUnitController()->Stop();
}

bool AEnemy::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   bool invis = GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility"));
   if(Super::CastSpell(spellToCast)) {
      //Cancel AI targetting if enemy target turns invisible
      if(GetTargetUnitValid() && !controllerRef->GetGameState()->visiblePlayerUnits.Contains(GetTargetUnit())) GetUnitController()->Stop();
      //Reveal self if invisible when spell casted.  If we don't check this before spell casted, we could just end up canceling an invisibility spell being cast
      if(invis) GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Combat.Effect.Invisibility")));
      return true;
   }
   return false;
}

void AEnemy::SetSelected(bool value)
{
   if(value) {
      controllerRef->GetBasePlayer()->GetFocusedUnit() = this;
   } else {
      if(controllerRef->GetBasePlayer()->focGetusedUnit == this) controllerRef->GetBasePlayer()->focusedUnit = nullptr;
   }
   // Call on unit selected delegate afterwards
   Super::SetSelected(value);
}

void AEnemy::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   if(UPartyDelegateStore* store = Cast<ULocalPlayer>(controllerRef->Player)->GetSubsystem<UPartyDelegateStore>())
   {
      store->OnEnemyActiveChanged().Broadcast(this, bEnabled);
   }

   if(bEnabled) {
      UGameplayStatics::GetLocal
      controllerRef->GetGameState()->enemyList.Add(this);
   } else {
      controllerRef->GetGameState()->enemyList.Remove(this);
   }
   controllerRef->GetGameState()->enemyList.Shrink();
   controllerRef->GetGameState()->enemyList.Compact();
   }
}

TSet<AUnit*>* AEnemy::GetSeenEnemies()
{
   return &possibleEnemiesInRadius;
}

void AEnemy::OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep,
                                   const FHitResult& sweepRes)
{
   //! Note to self, we rely on these functions being called as we destroy the overlap sphere to not run into data races during vision checks
   if(otherActor->GetClass()->IsChildOf(AAlly::StaticClass())) {
      AAlly* ally = Cast<AAlly>(otherActor);

      possibleEnemiesInRadius.Add(ally);
      ally->IncVisionCount();
   }
}

void AEnemy::OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
   AUnit* ally = Cast<AUnit>(otherActor);
   if(IsValid(ally)) {
      possibleEnemiesInRadius.Remove(ally);
      ally->DecVisionCount();
      if(!ally->GetVisionCount()) {
         controllerRef->GetGameState()->visiblePlayersMutex.WriteLock();
         controllerRef->GetGameState()->visiblePlayerUnits.Remove(ally);
         controllerRef->GetGameState()->visiblePlayersMutex.WriteUnlock();
      }
   }
}

void AEnemy::InitializeStats()
{
   int index = -1;
   for(auto& x : initialStats.defaultAttributes) {
      ModifyStats<true>(x.defaultValue, x.att);
      ModifyStats<true>(x.defaultValue, x.att);
   }

   for(auto& x : initialStats.defaultUnitScalingStats) {
      ModifyStats<true>(x.defaultValue, x.stat);
      ModifyStats<true>(x.defaultValue, x.stat);
   }

   for(auto& x : initialStats.defaultVitals) {
      ModifyStats<true>(x.defaultValue, x.vit);
      ModifyStats<true>(x.defaultValue, x.vit);
   }

   for(auto& x : initialStats.defaultMechanics) {
      ModifyStats<true>(x.defaultValue, x.mech);
      ModifyStats<true>(x.defaultValue, x.mech);
   }
}

float AEnemy::CalculateTargetRisk()
{
   int targetNum = 0;
   for(AUnit* a : controllerRef->GetGameState()->visiblePlayerUnits) {
      if(a->GetTargetUnit() == this) targetNum += 1;
   }

   const float targetRiskValue = FMath::Clamp(diminishFunc(targetNum), 0.f, 1.f);
   return targetRiskValue;
}
